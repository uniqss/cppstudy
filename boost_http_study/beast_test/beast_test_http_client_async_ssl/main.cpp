#include "../example/common/root_certificates.hpp"


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << std::endl;
}

class session : public std::enable_shared_from_this<session> {
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;  // must persist between reads.
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;

   public:
    explicit session(net::any_io_executor ex, ssl::context& ctx) : resolver_(ex), stream_(ex, ctx) {}

    // start the asynchronous operation
    void run(char const* host, char const* port, char const* target, int version) {
        if (!SSL_set_tlsext_host_name(stream_.native_handle(), host)) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            std::cerr << ec.message() << std::endl;
            return;
        }

        // set up an http get request message
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        resolver_.async_resolve(host, port, beast::bind_front_handler(&session::on_resolve, shared_from_this()));
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
        if (ec) return fail(ec, "resolve");

        // set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(stream_).async_connect(
            results, beast::bind_front_handler(&session::on_connect, shared_from_this()));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
        if (ec) return fail(ec, "connect");

        // perform the ssl shake
        stream_.async_handshake(ssl::stream_base::client,
                                beast::bind_front_handler(&session::on_handshake, shared_from_this()));
    }

    void on_handshake(beast::error_code ec) {
        if (ec) return fail(ec, "handshake");
        // Set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // send the http request to the remote host
        http::async_write(stream_, req_, beast::bind_front_handler(&session::on_write, shared_from_this()));
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec) return fail(ec, "write");

        http::async_read(stream_, buffer_, res_, beast::bind_front_handler(&session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec) return fail(ec, "read");

        // write the msg to standard out
        std::cout << res_ << std::endl;

        // set a timeout on the  operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        stream_.async_shutdown(beast::bind_front_handler(&session::on_shutdown, shared_from_this()));
    }

    void on_shutdown(beast::error_code ec) {
        if (ec == net::error::eof) {
            ec = {};
        }
        if (ec) return fail(ec, "shutdown");
    }
};

int main(int argc, const char** argv) {
    if (argc != 4 && argc != 5) {
        std::cerr << "Usage: http-client-async-ssl <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n"
                  << "Example:\n"
                  << "  http-client-async-ssl www.example.com 443 /\n"
                  << "  http-client-async-ssl www.example.com 443 / 1.0\n";
        return EXIT_FAILURE;
    }

    auto const host = argv[1];
    auto const port = argv[2];
    auto const target = argv[3];
    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

    // the io_context is required for all I/O
    net::io_context ioc;

    // the SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12_client};

    // this holds the root certificate used for verification
    load_root_certificates(ctx);

    // verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // Launch the asynchronous operation
    // The session is constructed with a strand to ensure that handlers do not execute concurrently.
    std::make_shared<session>(net::make_strand(ioc), ctx)->run(host, port, target, version);

    // Run the I/O service, the call will return when the get operation is complete
    ioc.run();

    return EXIT_SUCCESS;
}
