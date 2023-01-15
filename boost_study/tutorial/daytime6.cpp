#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <asio.hpp>

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class udp_server {
   public:
    udp_server(asio::io_context& ctx) : socket_(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), 13)) {
        start_receive();
    }

   private:
    void start_receive() {
        socket_.async_receive_from(
            asio::buffer(recv_buff_), remote_endpoint_,
            std::bind(&udp_server::handle_receive, this, std::placeholders::_1, std::placeholders::_2));
    }
    void handle_receive(const std::error_code& error, std::size_t) {
        if (!error) {
            std::shared_ptr<std::string> message(new std::string(make_daytime_string()));

            socket_.async_send_to(
                asio::buffer(*message), remote_endpoint_,
                std::bind(&udp_server::handle_send, this, message, std::placeholders::_1, std::placeholders::_2));
            start_receive();
        }
    }
    void handle_send(std::shared_ptr<std::string>, const std::error_code&, std::size_t) {}
    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buff_;
};

int main() {
    try {
        asio::io_context ctx;
        udp_server server(ctx);
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
