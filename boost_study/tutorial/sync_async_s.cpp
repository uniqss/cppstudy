#include <ctime>
#include <iostream>
#include <asio.hpp>
#include <string>

std::string make_daytime_string() {
    std::time_t now(std::time(0));
    return std::ctime(&now);
}

asio::awaitable<void> echo_time(asio::ip::tcp::socket socket) {
    auto data = make_daytime_string();
    co_await asio::async_write(socket, asio::buffer(data), asio::use_awaitable);
    co_return;
}

asio::awaitable<void> listen(asio::ip::tcp::acceptor& acceptor) {
    for (;;) {
        auto client = co_await acceptor.async_accept(asio::use_awaitable);
        auto ex = client.get_executor();
        co_spawn(ex, echo_time(std::move(client)), asio::detached);
    }
}

int main() {
    try {
        asio::io_context ctx;
        asio::ip::tcp::endpoint ep =
            *asio::ip::tcp::resolver(ctx).resolve("127.0.0.1", "8181", asio::ip::tcp::resolver::passive);
        asio::ip::tcp::acceptor acceptor(ctx, ep);
        asio::co_spawn(ctx, listen(acceptor), asio::detached);
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

