#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

std::string gsHost;
std::string gsPort;
std::string gsText;
using std::cout;
using std::endl;

std::atomic<int> gnWaitGroup = 0;

int proc(int i) {
    try {
        int sendidx = 1;
        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        std::string host = gsHost;

        // Look up the domain name
        auto const results = resolver.resolve(host, gsPort);

        // Make the connection on the IP address we get from a lookup
        auto ep = net::connect(ws.next_layer(), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        host += ':' + std::to_string(ep.port());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req) { req.set(http::field::user_agent, "uniqs"); }));

        // Perform the websocket handshake
        cout << "host:" << host << endl;
        ws.handshake(host, "/");

        std::string login_msg = "login";
        ws.write(net::buffer(login_msg));

        std::string text = gsText + std::to_string(i);
        cout << "text:" << text << endl;

        // Send the message
        ws.write(net::buffer(text));

        // This buffer will hold the incoming message
        beast::flat_buffer buffer;

        // Read a message into our buffer
        ws.read(buffer);

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << beast::make_printable(buffer.data()) << std::endl;
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        --gnWaitGroup;
        return EXIT_FAILURE;
    }
    --gnWaitGroup;
    return EXIT_SUCCESS;
}

// Sends a WebSocket message and prints the response
int main(int argc, char** argv) {
    try {
        // Check command line arguments.
        if (argc < 5) {
            std::cerr << "Usage: websocket-client-sync <host> <port> <text> <thread_count>\n"
                      << "Example:\n"
                      << "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\" 100\n";
            return EXIT_FAILURE;
        }
        gsHost = argv[1];
        gsPort = argv[2];
        gsText = argv[3];
        int threads_count = std::max(1, std::atoi(argv[4]));

        gnWaitGroup = threads_count;
        cout << "threads_count:" << threads_count << endl;

        std::vector<std::thread> threads;
        for (int i = 0; i < threads_count; ++i) {
            std::thread t(proc, i + 1);
            threads.push_back(std::move(t));
            cout << "i:" << i << endl;
            threads[i].detach();
        }

        int loop_count = 0;
        while (gnWaitGroup > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (++loop_count >= 500) {
                loop_count = 0;
                cout << "gnWaitGroup: " << gnWaitGroup << endl;
            }
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}