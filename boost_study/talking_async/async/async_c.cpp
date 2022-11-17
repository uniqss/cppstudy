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

struct conn_data {
    conn_data(asio::io_context& ctx) : socket(ctx, asio::ip::tcp::v4()) {}
    tcp::socket socket;
};

using conn_data_ptr = std::shared_ptr<conn_data>;

awaitable<void> get_server_time(conn_data_ptr cdata) {
    std::array<char, 1024> data;
    std::array<char, 1> data_send = {0};
    co_await async_write(cdata->socket, buffer(data_send), use_awaitable);

    auto n = co_await cdata->socket.async_read_some(buffer(data), use_awaitable);
    std::cout << n << " " << data.data() << std::endl;

    cdata->socket.close();
}

awaitable<void> client_operation(asio::io_context& ctx, tcp::endpoint target) {
    auto cdata = std::make_shared<conn_data>(ctx);

    co_await cdata->socket.async_connect(target, use_awaitable);

    co_await get_server_time(cdata);
}

int main(int argc, const char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint target_endpoint = *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2]);

        co_spawn(ctx, client_operation(ctx, target_endpoint), detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
