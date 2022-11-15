#include <iostream>
#include <asio.hpp>
#include <array>

int main(int argc, const char** argv) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <host>" << std::endl;
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::resolver resolver(ctx);
        asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(argv[1], "daytime");
        asio::ip::tcp::socket socket(ctx);
        asio::connect(socket, endpoint);
        for (;;) {
            std::array<char, 128> buf;
            std::error_code ec;
            std::size_t len = socket.read_some(asio::buffer(buf), ec);
            if (ec == asio::error::eof) {
                break;
            } else if (ec) {
                throw std::system_error(ec);
            }
            std::cout.write(buf.data(), len);
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
