#include "http_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <fstream>
#include <vector>


#include "datastore.hpp"
#include "ocrService.hpp"
#include "setWorkDir.hpp"
#include "logger.hpp"

int main()
{
    DataStore<cv::Mat> rxBuffer; 

    ocrService ocr;

    setWorkdir();
    logFile()<< "start log"<<std::endl;
    //initLogger();

    ocr.initTesseract();
    ocr.setMode(tesseract::PSM_AUTO_ONLY);

std::thread acceptorThread([&rxBuffer]() {
    try
    {
        auto const address = net::ip::make_address("192.168.1.147");
        auto const port = static_cast<unsigned short>(std::atoi("8080"));
        auto const doc_root = std::make_shared<std::string>(".");
        // The io_context is required for all I/O
        net::io_context ioc{1};

       
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            // Launch the session in a separate thread
            std::thread{std::bind(&do_session, std::move(socket), doc_root, &rxBuffer)}.detach();
        }
    }
    catch (const std::exception& e)
    {
        logFile() << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    });
acceptorThread.detach();  

std::thread workerThread([&ocr,&rxBuffer]() {
    
   while(1){
    if(!rxBuffer.checkEmpty()){
        std::pair<int, cv::Mat> elem =rxBuffer.get();
        if(elem.first!=-1){
            ocr.loadImage(elem.second);
        
            std::string data = ocr.returnHOCRText();
            std::string filename = std::to_string(elem.first)+".txt"; 
            std::ofstream outFile("data/"+filename); 
            if (!outFile) {
                logFile() << "Error: Could not open " + filename + " for writing." << std::endl;
                }
            else{
                outFile << data;
                outFile.close();
            }
            }
        }
    else
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }
  
    });
  workerThread.detach();
  
  std::cin.get(); 
  
  ocr.deInit();
  deinitLogger(); 
}
