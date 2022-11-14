#include <array>
#include <iostream>
#include <asio.hpp>
#include <memory>

using asio::awaitable;
using asio::buffer;
using asio::co_spawn;
using asio::detached;
using asio::ip::tcp;
using asio::use_awaitable;

struct proxy_state {
    proxy_state(tcp::socket client) : client(std::move(client)) {}
    tcp::socket client;
    tcp::socket server{client.get_executor()};
};

using proxy_state_ptr = std::shared_ptr<proxy_state>;

awaitable<void> client_to_server(proxy_state_ptr state) {
    try {
        std::array<char, 1024> data;
        for (;;) {
            auto n = co_await state->client.async_read_some(buffer(data), use_awaitable);
            co_await async_write(state->server, buffer(data, n), use_awaitable);
        }
    } catch (const std::exception& e) {
        state->client.close();
        state->server.close();
    }
}
awaitable<void> server_to_client(proxy_state_ptr state) {
    try {
        std::array<char, 1024> data;
        for (;;) {
            auto n = co_await state->server.async_read_some(buffer(data), use_awaitable);
            co_await async_write(state->client, buffer(data, n), use_awaitable);
        }
    } catch (const std::exception& e) {
        state->client.close();
        state->server.close();
    }
}

awaitable<void> proxy(tcp::socket client, tcp::endpoint target) {
    auto state = std::make_shared<proxy_state>(std::move(client));

    co_await state->server.async_connect(target, use_awaitable);

    auto ex = state->client.get_executor();
    co_spawn(ex, client_to_server(state), detached);
    co_await server_to_client(state);
}

awaitable<void> listen(asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::endpoint target) {
    for (;;) {
        auto client = co_await acceptor.async_accept(use_awaitable);

        auto ex = client.get_executor();
        co_spawn(ex, proxy(std::move(client), target), detached);
    }
}
int main(int argc, const char** argv) {
    try {
        if (argc != 5) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> <target_addr> <target_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint host_endpoint =
            *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2], asio::ip::tcp::resolver::passive);
        asio::ip::tcp::endpoint target_endpoint = *asio::ip::tcp::resolver(ctx).resolve(argv[3], argv[4]);
        asio::ip::tcp::acceptor acceptor(ctx, host_endpoint);

        co_spawn(ctx, listen(acceptor, target_endpoint), detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
