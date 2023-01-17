
#include "listener.hpp"
#include "http_session.hpp"
#include <iostream>

#include "ulog.h"

listener::listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint) : ioc_(ioc), acceptor_(ioc) {
    boost::beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "acceptor_.open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "acceptor_.set_option reuse_address");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        fail(ec, "acceptor_.bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "acceptor_.listen");
        return;
    }
}

void listener::run() {
    dlog();
    // The new connection gets its own strand
    acceptor_.async_accept(boost::asio::make_strand(ioc_),
                           boost::beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}

// Report a failure
void listener::fail(boost::beast::error_code ec, char const* what) {
    // Don't report on canceled operations
    if (ec == boost::asio::error::operation_aborted) return;
    dlog("what:[%s] ec.message():[%s]", what, ec.message().c_str());
    // std::cerr << what << ": " << ec.message() << "\n";
}

// Handle a connection
void listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
    dlog();
    if (ec)
        return fail(ec, "on_accept");
    else
        // Launch a new session for this connection
        boost::make_shared<http_session>(std::move(socket))->run();

    // The new connection gets its own strand
    acceptor_.async_accept(boost::asio::make_strand(ioc_),
                           boost::beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}
