#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
//#include <memory>
//#include <string>
#include "RequestHandler.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket, RequestHandler& handler);

    void run();

private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    RequestHandler& handler_;

    void do_read();
    void do_write(http::response<http::string_body> res);
};

