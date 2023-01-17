#pragma once

#include <boost/beast.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

/** Represents an active WebSocket connection to the server
 */
class websocket_session : public boost::enable_shared_from_this<websocket_session> {
    boost::beast::flat_buffer buffer_;
    boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
    std::vector<boost::shared_ptr<std::string const>> queue_;
    int64_t userId_;

    void fail(boost::beast::error_code ec, char const* what);
    void on_accept(boost::beast::error_code ec);
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);

   public:
    websocket_session(boost::asio::ip::tcp::socket&& socket);

    ~websocket_session();

    int64_t get_uid() { return userId_; }
    void set_uid(int64_t uid) { userId_ = uid; }

    template <class Body, class Allocator>
    void run(boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> req);

    // Send a message
    void send(boost::shared_ptr<std::string const> const& ss);

   private:
    void on_send(boost::shared_ptr<std::string const> const& ss);
};

template <class Body, class Allocator>
void websocket_session::run(boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> req) {
    // Set suggested timeout settings for the websocket
    ws_.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res) { res.set(boost::beast::http::field::server, "uniqs"); }));

    // Accept the websocket handshake
    ws_.async_accept(req, boost::beast::bind_front_handler(&websocket_session::on_accept, shared_from_this()));
}
