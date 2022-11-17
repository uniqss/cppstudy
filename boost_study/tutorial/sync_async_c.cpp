#include <iostream>
#include <asio.hpp>
#include <vector>
#include <array>


class tcp_client : public std::enable_shared_from_this<tcp_client> {
   public:
    tcp_client(int index, asio::io_context& ctx) : index_(index), ctx_(ctx), socket_(ctx, asio::ip::tcp::v4()) {}

    asio::awaitable<void> work() {
        std::cout << "********** " << index_ << " 11111" << std::endl;
        target_ = *asio::ip::tcp::resolver(ctx_).resolve("127.0.0.1", "8181");
        co_await socket_.async_connect(target_, asio::use_awaitable);
        for (;;) {
            std::cout << "********** " << index_ << " 22222" << std::endl;
            std::array<char, 1> data = {0};
            co_await asio::async_write(socket_, asio::buffer(data), asio::use_awaitable);
            std::array<char, 1024> data_recv = {0};
            auto n = co_await socket_.async_read_some(asio::buffer(data_recv), asio::use_awaitable);
            std::cout << index_ << "\t" << n << "\t" << data_recv.data() << std::endl;
            asio::steady_timer t(socket_.get_executor(), std::chrono::seconds(1));
            co_await t.async_wait(asio::use_awaitable);
        }
    }

   private:
    int index_;
    asio::io_context& ctx_;
    asio::ip::tcp::socket socket_;
    asio::ip::tcp::endpoint target_;
};


asio::awaitable<void> create_client(int idx, asio::io_context& ctx) {
    auto client = std::make_shared<tcp_client>(idx, ctx);
    co_await client->work();
}

int main() {
    try {
        asio::io_context ctx;
        for (int i = 0; i < 10; ++i) {
            asio::co_spawn(ctx, create_client(i, ctx), asio::detached);
        }
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "##### error:" << e.what() << std::endl;
    }
    return 0;
}
