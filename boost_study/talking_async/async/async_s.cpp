#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <ctime>
#include <string>

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

struct client_data {
    proxy_state(asio::ip::tcp::socket socket) : socket(std::move(socket)) {}
    asio::ip::tcp::socket socket;
};
using client_data_ptr = std::shared_ptr<client_data_ptr>;

asio::awaitable<void> send_time(client_data_ptr state) {
    std::string data = make_daytime_string();
    co_await async_write(state->client, asio::buffer(data), asio::use_awaitable);
}

asio::awaitable<void> process_client(asio::ip::tcp::socket socket) {
    auto cdata = std::make_shared<client_data>(std::move(socket));

    co_await send_time(cdata);

    cdata->client.close();
}

asio::awaitable<void> listen(asio::ip::tcp::acceptor& acceptor) {
    for (;;) {
        auto csocket = co_await acceptor.async_accept(asio::use_awaitable);

        auto ex = client.get_executor();
        asio::co_spawn(ex, process_client(std::move(csocket)), asio::detached);
    }
}
int main(int argc, const char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint endpoint =
            *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2], asio::ip::tcp::resolver::passive);
        asio::ip::tcp::acceptor acceptor(ctx, endpoint);

        asio::co_spawn(ctx, listen(acceptor), asio::detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
