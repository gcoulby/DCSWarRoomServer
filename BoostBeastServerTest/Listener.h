#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <iostream>
#include "RequestHandler.h"

namespace net = boost::asio;
using tcp = net::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener> {
public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint, RequestHandler& handler);

    void run();

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    RequestHandler& handler_;

    void do_accept();
};


