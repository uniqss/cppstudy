
#include "http_session.hpp"
#include "websocket_session.hpp"
#include <boost/config.hpp>
#include <iostream>
#include "ulog.h"

http_session::http_session(tcp::socket &&socket, boost::shared_ptr<shared_state> const &state)
    : stream_(std::move(socket)), state_(state) {}

void http_session::run() {
    do_read();
}

// Report a failure
void http_session::fail(beast::error_code ec, char const *what) {
    // Don't report on canceled operations
    if (ec == net::error::operation_aborted) return;

    std::cerr << what << ": " << ec.message() << "\n";
}

void http_session::do_read() {
    dlog();
    // Construct a new parser for each message
    parser_.emplace();

    // Apply a reasonable limit to the allowed size
    // of the body in bytes to prevent abuse.
    parser_->body_limit(10000);

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request
    http::async_read(stream_, buffer_, parser_->get(),
                     beast::bind_front_handler(&http_session::on_read, shared_from_this()));
}

void http_session::on_read(beast::error_code ec, std::size_t) {
    dlog();
    // This means they closed the connection
    if (ec == http::error::end_of_stream) {
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    }

    // Handle the error, if any
    if (ec) return fail(ec, "read");

    // See if it is a WebSocket Upgrade
    if (websocket::is_upgrade(parser_->get())) {
        // Create a websocket session, transferring ownership
        // of both the socket and the HTTP request.
        boost::make_shared<websocket_session>(stream_.release_socket(), state_)->run(parser_->release());
        return;
    }

    // we support websocket only.
    return;
}
