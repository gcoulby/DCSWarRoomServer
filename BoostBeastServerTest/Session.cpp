
#include "Session.h"
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <memory>
//#include <string>
//#include <functional>
//#include <iostream>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

Session::Session(tcp::socket socket, RequestHandler& handler)
    : socket_(std::move(socket)), handler_(handler) {}

void Session::run() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());
    http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t) {
        if (!ec) {
            do_write(handler_.handle_request(req_));
        }
        });
}

void Session::do_write(http::response<http::string_body> res) {
    auto self(shared_from_this());
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        });
}
