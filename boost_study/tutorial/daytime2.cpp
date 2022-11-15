#include <ctime>
#include <asio.hpp>
#include <string>
#include <iostream>

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

int main() {
    try {
        asio::io_context ctx;
        asio::ip::tcp::acceptor acceptor(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
        for (;;) {
            asio::ip::tcp::socket socket(ctx);
            acceptor.accept(socket);
            std::string message = make_daytime_string();
            asio::error_code ignored_error;
            asio::write(socket, asio::buffer(message), ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

