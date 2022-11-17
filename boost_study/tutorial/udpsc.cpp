#include <iostream>
#include <asio.hpp>
#include <array>
int main(int argc, const char** argv) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[1] << " <host> " << std::endl;
            return 1;
        }

        asio::io_context ctx;
        asio::ip::udp::resolver resolver(ctx);
        asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(asio::ip::udp::v4(), argv[1], "8686").begin();
        asio::ip::udp::socket socket(ctx);
        socket.open(asio::ip::udp::v4());
        std::string send_buff = "hello world!";
        socket.send_to(asio::buffer(send_buff), receiver_endpoint);

        std::array<char, 128> recv_buff;
        asio::ip::udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(asio::buffer(recv_buff), sender_endpoint);
        std::cout.write(recv_buff.data(), len);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
