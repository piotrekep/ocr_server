#include "http_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <vector>
#include "datastore.hpp"
#include "ocrService.hpp"

int main()
{
    DataStore<cv::Mat> rxBuffer; 
    DataStore<std::string> txBuffer; 

    ocrService ocr;

    ocr.initTesseract();
    ocr.setMode(tesseract::PSM_AUTO_ONLY);

std::thread acceptorThread([&rxBuffer,&txBuffer]() {
    try
    {
        auto const address = net::ip::make_address("192.168.1.147");
        auto const port = static_cast<unsigned short>(std::atoi("8080"));
        auto const doc_root = std::make_shared<std::string>(".");
        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            // Launch the session in a separate thread
            std::thread{std::bind(&do_session, std::move(socket), doc_root, &rxBuffer, &txBuffer)}.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    });
acceptorThread.detach();  

std::thread workerThread([&ocr,&rxBuffer,&txBuffer]() {
    
   while(1){
    if(!rxBuffer.checkEmpty()){
         std::cout << "elo\n";
         std::pair<int, cv::Mat> elem =rxBuffer.get();
        std::cout << elem.first << "\n";;
        if(elem.first!=-1)
            ocr.loadImage(elem.second);
         //std::cout << ocr.returnText();
         std::cout << ocr.returnTSVText();
        }
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
  
    });
   workerThread.detach();
   std::cin.get(); 
  ocr.deInit(); 
}
