#include <asio.hpp>
#include <chrono>
#include <iostream>

void print(const asio::error_code& e) {
    std::cout << "Hello world! e:" << e << std::endl;
}
void print2(std::error_code e) {
    std::cout << "Hello world! e:" << e << std::endl;
}

int main() {
    asio::io_context io;
    asio::steady_timer t(io, asio::chrono::seconds(3));
    t.async_wait(&print);
    asio::steady_timer t2(io, std::chrono::seconds(4));
    t2.async_wait(&print2);
    io.run();
    return 0;
}

