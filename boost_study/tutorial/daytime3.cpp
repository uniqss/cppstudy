#include <ctime>
#include <iostream>
#include <asio.hpp>
#include <array>
#include <memory>

std::string make_daytime_string() {
    std::time_t now(std::time(0));
    return std::ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
   public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(asio::io_context& ctx) { return pointer(new tcp_connection(ctx)); }

    asio::ip::tcp::socket& socket() { return socket_; }

    void start() {
        message_ = make_daytime_string();
        asio::async_write(
            socket_, asio::buffer(message_),
            std::bind(&tcp_connection::handle_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

   private:
    tcp_connection(asio::io_context& ctx) : socket_(ctx) {}

    void handle_write(const asio::error_code&, std::size_t) {}

    asio::ip::tcp::socket socket_;
    std::string message_;
};

class tcp_server {
   public:
    tcp_server(asio::io_context& ctx)
        : io_context_(ctx), acceptor_(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13)) {
        start_accept();
    }
    void start_accept() {
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_);
        acceptor_.async_accept(new_connection->socket(),
                               std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1));
    }

    void handle_accept(tcp_connection::pointer new_connection, const asio::error_code& error) {
        if (!error) {
            new_connection->start();
        }
        start_accept();
    }

   private:
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context ctx;
        tcp_server server(ctx);
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
