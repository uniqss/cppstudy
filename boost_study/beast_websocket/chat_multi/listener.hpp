#pragma once

#include "beast.hpp"
#include "net.hpp"
#include <boost/smart_ptr.hpp>
#include <memory>
#include <string>

// Accepts incoming connections and launches the sessions
class listener : public boost::enable_shared_from_this<listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

    void fail(beast::error_code ec, char const* what);
    void on_accept(beast::error_code ec, tcp::socket socket);

   public:
    listener(net::io_context& ioc, tcp::endpoint endpoint);

    // Start accepting incoming connections
    void run();
};
