#include "http_server.h"
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>

int main()
{
    try
    {
        auto const address = net::ip::make_address("192.168.1.155");
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
            std::thread{std::bind(&do_session, std::move(socket), doc_root)}.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
