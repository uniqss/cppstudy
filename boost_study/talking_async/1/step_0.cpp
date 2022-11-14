#include <array>
#include <iostream>
#include <asio.hpp>
#include <memory>
class proxy : public std::enable_shared_from_this<proxy> {
   public:
    proxy(asio::ip::tcp::socket client) : client_(std::move(client)), server_(client.get_executor()) {}
    void connect_to_server(asio::ip::tcp::endpoint target) {
        auto self = shared_from_this();
        server_.async_connect(target, [self](std::error_code ec) {
            if (!ec) {
                self->read_from_client();
                self->read_from_server();
            }
        });
    }

   private:
    void stop() {
        client_.close();
        server_.close();
    }
    void read_from_client() {
        auto self = shared_from_this();
        client_.async_read_some(asio::buffer(data_from_client_), [self](std::error_code ec, std::size_t n) {
            if (!ec) {
                self->write_to_server(n);
            } else {
                self->stop();
            }
        });
    }
    void write_to_server(std::size_t n) {
        auto self = shared_from_this();
        asio::async_write(server_, asio::buffer(data_from_client_, n), [self](std::error_code ec, std::size_t) {
            if (!ec) {
                self->read_from_client();
            } else {
                self->stop();
            }
        });
    }
    void read_from_server() {
        auto self = shared_from_this();
        server_.async_read_some(asio::buffer(data_from_server_), [self](std::error_code ec, std::size_t n) {
            if (!ec) {
                self->write_to_client(n);
            } else {
                self->stop();
            }
        });
    }
    void write_to_client(std::size_t n) {
        auto self = shared_from_this();
        asio::async_write(client_, asio::buffer(data_from_server_, n), [self](std::error_code ec, std::size_t) {
            if (!ec) {
                self->read_from_server();
            } else {
                self->stop();
            }
        });
    }
    asio::ip::tcp::socket client_;
    asio::ip::tcp::socket server_;
    std::array<char, 1024> data_from_client_;
    std::array<char, 1024> data_from_server_;
};
void listen(asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::endpoint target) {
    acceptor.async_accept([&acceptor, target](std::error_code ec, asio::ip::tcp::socket client) {
        if (!ec) {
            std::make_shared<proxy>(std::move(client))->connect_to_server(target);
        }
        listen(acceptor, target);
    });
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
        listen(acceptor, target_endpoint);
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}