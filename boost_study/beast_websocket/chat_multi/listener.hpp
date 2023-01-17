#pragma once

#include <boost/beast.hpp>
#include <boost/smart_ptr.hpp>
#include <memory>
#include <string>
#include <boost/asio.hpp>

// Accepts incoming connections and launches the sessions
class listener : public boost::enable_shared_from_this<listener> {
    boost::asio::io_context& ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void fail(boost::beast::error_code ec, char const* what);
    void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

   public:
    listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint);

    // Start accepting incoming connections
    void run();
};
