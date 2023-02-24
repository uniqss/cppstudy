#pragma once

#include <boost/beast.hpp>
#include <boost/optional.hpp>
#include <boost/smart_ptr.hpp>
#include <cstdlib>
#include <memory>

class http_session : public boost::enable_shared_from_this<http_session> {
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_;

    // The parser is stored in an optional container so we can
    // construct it from scratch it at the beginning of each new message.
    boost::optional<boost::beast::http::request_parser<boost::beast::http::string_body>> parser_;

    void fail(boost::beast::error_code ec, char const* what);
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t);

   public:
    http_session(boost::asio::ip::tcp::socket&& socket);

    void run();
};
