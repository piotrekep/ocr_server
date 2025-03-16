#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include <vector>
#include "datastore.hpp"


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

// Returns a reasonable mime type based on the extension of a file.
beast::string_view mime_type(beast::string_view path);

// Appends an HTTP rel-path to a local filesystem path.
std::string path_cat(beast::string_view base, beast::string_view path);

//---------------------------------------------------------------------
// The request handler is a template so it must be defined in the header.

template<class Body, class Allocator, class Send>
void handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send, DataStore<cv::Mat>* rxBuffer, DataStore<std::string>* txBuffer)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        std::cout << req;
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head &&
        req.method() != http::verb::post)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
        path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle file not found
    if(ec == beast::errc::no_such_file_or_directory)
        return send(not_found(req.target()));

    // Handle any other error
    if(ec)
        return send(server_error(ec.message()));

    // Cache the size
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == http::verb::head)
    {
        http::response<http::empty_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }
    // Respond to POST request
    if(req.method() == http::verb::post)
    {

        http::response<http::string_body> res{http::status::ok, req.version()};

        auto nazwa_pliku = req.find("nazwa");
        if(nazwa_pliku != req.end())
        {
          auto customValue = nazwa_pliku->value();
          std::cout << customValue << "\n";
          //std::cout << req.body() << "\n";
          std::vector<uchar> data(req.body().begin(), req.body().end());
          cv::Mat img = cv::imdecode(data, cv::IMREAD_ANYCOLOR);
          if(!img.empty()){
            res.body() = std::string("jeeest! ") + std::to_string(rxBuffer->store(img));
          }
          else {
            res.body() = "niema:(";
          }

        }
       
        
        res.set(http::field::server, "Boost.Beast");
        res.set(http::field::content_type, "text/plain");
        //res.body() = "File uploaded successfully";
        res.content_length(size);
        res.prepare_payload();
        return send(std::move(res));
    }
    // Respond to GET request
    http::response<http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
}

// Report a failure
void fail(beast::error_code ec, char const* what);

//---------------------------------------------------------------------
// A helper functor to send an HTTP message. As a template it must be fully
// defined in the header.

template<class Stream>
struct send_lambda
{
    Stream& stream_;
    bool& close_;
    beast::error_code& ec_;

    send_lambda(Stream& stream, bool& close, beast::error_code& ec)
        : stream_(stream), close_(close), ec_(ec)
    {}

    template<bool isRequest, class Body, class Fields>
    void operator()(http::message<isRequest, Body, Fields>&& msg) const
    {
        // Determine if we should close the connection after
        close_ = msg.need_eof();

        // Serialize and write the message
        http::serializer<isRequest, Body, Fields> sr{msg};
        http::write(stream_, sr, ec_);
    }
};

// Handles an HTTP server connection.
void do_session(tcp::socket& socket, std::shared_ptr<std::string const> const& doc_root,DataStore<cv::Mat>* rxBuffer, DataStore<std::string>* txBuffer);

#endif // HTTP_SERVER_H
