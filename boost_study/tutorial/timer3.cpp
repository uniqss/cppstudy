#include <asio.hpp>
#include <chrono>
#include <iostream>

void print(std::error_code, asio::steady_timer* t, int* count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + std::chrono::seconds(1));
        t->async_wait(std::bind(print, std::placeholders::_1, t, count));
    }
}

int main() {
    asio::io_context io;
    int count = 0;
    asio::steady_timer t(io, std::chrono::seconds(1));
    t.async_wait(std::bind(print, std::placeholders::_1, &t, &count));
    io.run();
    std::cout << "final count is: " << count << std::endl;
    return 0;
}

