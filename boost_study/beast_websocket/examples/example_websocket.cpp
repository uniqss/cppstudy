#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>

using tcp = boost::asio::ip::tcp;

int main() {
    auto const address = boost::asio::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(std::atoi("8083"));

    boost::asio::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};

    while (1) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::cout << "socket accepted" << std::endl;

        std::thread t([q = std::move(socket)]() mutable {
            boost::beast::websocket::stream<tcp::socket> ws{std::move(q)};
            ws.set_option(
                boost::beast::websocket::stream_base::decorator([](boost::beast::websocket::response_type& res) {
                    res.set(boost::beast::http::field::server, "uniqs");
                }));
            ws.accept();

            while (1) {
                try {
                    boost::beast::flat_buffer buffer;

                    ws.read(buffer);

                    auto bufferStr = boost::beast::buffers_to_string(buffer.cdata());
                    std::cout << bufferStr << std::endl;

                    ws.write(buffer.data());
                } catch (boost::beast::system_error const& e) {
                    if (e.code() != boost::beast::websocket::error::closed) {
                        std::cerr << e.code().message() << std::endl;
                        break;
                    }
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        });
        t.detach();
    }

    return 0;
}
