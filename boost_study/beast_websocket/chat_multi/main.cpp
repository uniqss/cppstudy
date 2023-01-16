
#include "listener.hpp"

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>

#include "ulog.h"

int main(int argc, char* argv[]) {
    dlog();
    // Check command line arguments.
    if (argc < 4) {
        std::cerr << "Usage: websocket-chat-multi <address> <port> <threads>\n"
                  << "Example:\n"
                  << "    websocket-chat-server 0.0.0.0 8080 5\n";
        return EXIT_FAILURE;
    }
    std::string addrStr = argv[1];
    if (addrStr == "0") {
        addrStr = "0.0.0.0";  // to simplify input in command line.
    }

    auto address = boost::asio::ip::make_address(addrStr);
    auto port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    // The io_context is required for all I/O
    boost::asio::io_context ioc;

    // Create and launch a listening port
    boost::make_shared<listener>(ioc, boost::asio::ip::tcp::endpoint{address, port})->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](boost::system::error_code const&, int) {
        // Stop the io_context. This will cause run()
        // to return immediately, eventually destroying the
        // io_context and any remaining handlers in it.
        ioc.stop();
    });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) v.emplace_back([&ioc] { ioc.run(); });
    ioc.run();

    // (If we get here, it means we got a SIGINT or SIGTERM)

    // Block until all the threads exit
    for (auto& t : v) t.join();

    return EXIT_SUCCESS;
}
