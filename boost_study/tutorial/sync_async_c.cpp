#include <iostream>
#include <asio.hpp>
#include <vector>
#include <array>

class tcp_client {
   public:
    tcp_client(int index, asio::io_context& ctx) : index_(index), ctx_(ctx), socket_(ctx, asio::ip::tcp::v4()) {
        auto ex = socket_.get_executor();
        asio::co_spawn(ex, std::bind(&tcp_client::send_get_time, this), asio::detached);
    }

    asio::awaitable<void> send_get_time() {
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
        }
    }

   private:
    int index_;
    asio::io_context& ctx_;
    asio::ip::tcp::socket socket_;
    asio::ip::tcp::endpoint target_;
};

int main() {
    try {
        asio::io_context ctx;
        std::vector<tcp_client> clients;
        for (int i = 0; i < 10; ++i) {
            clients.emplace_back(tcp_client(i + 1, ctx));
        }
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "##### error:" << e.what() << std::endl;
    }
    return 0;
}
