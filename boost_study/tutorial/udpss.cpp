#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <asio.hpp>

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

int main() {
    try {
        asio::io_context ctx;
        asio::ip::udp::socket socket(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), 8686));
        for (;;) {
            std::array<char, 256> recv_buf;
            asio::ip::udp::endpoint remote_endpoint;
            std::size_t n = socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

            std::cout << "recv n:" << n << " data:" << recv_buf.data() << std::endl;

            std::string msg = make_daytime_string();

            asio::error_code error;
            socket.send_to(asio::buffer(msg), remote_endpoint, 0, error);
            if (error) {
                std::cerr << "error:" << error << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "exception. e.what():" << e.what() << std::endl;
    }
    return 0;
}

