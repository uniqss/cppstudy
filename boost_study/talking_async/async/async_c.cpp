#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>

using asio::awaitable;
using asio::buffer;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;

struct proxy_state {
    proxy_state(asio::io_context& ctx) : server(ctx, asio::ip::tcp::v4()) {}
    tcp::socket server;
};

using proxy_state_ptr = std::shared_ptr<proxy_state>;

awaitable<void> client_to_server(proxy_state_ptr state) {
    try {
        std::array<char, 1024> data;
        for (;;) {
            std::array<char, 1> data_send = {0};
            co_await async_write(state->server, buffer(data_send), use_awaitable);

            auto n = co_await state->server.async_read_some(buffer(data), use_awaitable);
            std::cout << n << " " << data.data() << std::endl;
        }
    } catch (const std::exception& e) {
        state->server.close();
    }
}

awaitable<void> proxy(asio::io_context& ctx, tcp::endpoint target) {
    auto state = std::make_shared<proxy_state>(ctx);

    co_await state->server.async_connect(target, use_awaitable);

    co_await client_to_server(state);
}

int main(int argc, const char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint target_endpoint = *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2]);

        co_spawn(ctx, proxy(ctx, target_endpoint), detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
