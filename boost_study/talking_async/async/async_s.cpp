#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <ctime>
#include <string>

using asio::awaitable;
using asio::buffer;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

struct proxy_state {
    proxy_state(tcp::socket client) : client(std::move(client)) {}
    tcp::socket client;
};
using proxy_state_ptr = std::shared_ptr<proxy_state>;

awaitable<void> send_time(proxy_state_ptr state) {
    std::string data = make_daytime_string();
    co_await async_write(state->client, buffer(data), use_awaitable);
}

awaitable<void> proxy(tcp::socket client) {
    auto state = std::make_shared<proxy_state>(std::move(client));

    co_await send_time(state);

    state->client.close();
}

awaitable<void> listen(asio::ip::tcp::acceptor& acceptor) {
    for (;;) {
        auto client = co_await acceptor.async_accept(use_awaitable);

        auto ex = client.get_executor();
        co_spawn(ex, proxy(std::move(client)), detached);
    }
}
int main(int argc, const char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint host_endpoint =
            *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2], asio::ip::tcp::resolver::passive);
        asio::ip::tcp::acceptor acceptor(ctx, host_endpoint);

        co_spawn(ctx, listen(acceptor), detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
