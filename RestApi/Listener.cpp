#include "pch.h"
#include "Listener.h"
#include "Session.h"
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/asio/strand.hpp>
//#include <boost/config.hpp>


//#include <memory>
//#include <string>
//#include <thread>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint, RequestHandler& handler)
    : ioc_(ioc), acceptor_(net::make_strand(ioc)), handler_(handler) {
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "Open error: " << ec.message() << std::endl;
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "Set option error: " << ec.message() << std::endl;
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr << "Bind error: " << ec.message() << std::endl;
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "Listen error: " << ec.message() << std::endl;
        return;
    }
}

void Listener::run() {
    do_accept();
}

void Listener::do_accept() {
    acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket), handler_)->run();
        }
        do_accept();
        });
}
