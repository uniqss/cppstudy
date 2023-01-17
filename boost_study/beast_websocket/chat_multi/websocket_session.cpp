
#include "websocket_session.hpp"
#include <iostream>
#include "ulog.h"
#include "sessionmgr.hpp"

websocket_session::websocket_session(boost::asio::ip::tcp::socket&& socket)
    : ws_(std::move(socket)), userId_(0) {}

websocket_session::~websocket_session() {
    dlog();
    // Remove this session from the list of active sessions
    gpSessionMgr->leave(this);
}

void websocket_session::fail(boost::beast::error_code ec, char const* what) {
    // Don't report these
    if (ec == boost::asio::error::operation_aborted || ec == boost::beast::websocket::error::closed) return;

    std::cerr << what << ": " << ec.message() << "\n";
}

void websocket_session::on_accept(boost::beast::error_code ec) {
    dlog();
    // Handle the error, if any
    if (ec) return fail(ec, "accept");

    // Add this session to the list of active sessions
    gpSessionMgr->join2prelogin(this);

    // Read a message
    ws_.async_read(buffer_, boost::beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));
}

static int64_t get_uid_from_msg(const std::string& msg) {
    static std::atomic<int64_t> uid_fake = 1024;
    ++uid_fake;

    return uid_fake;
}

void websocket_session::on_read(boost::beast::error_code ec, std::size_t) {
    dlog();
    // Handle the error, if any
    if (ec) return fail(ec, "read");

    std::string msg = boost::beast::buffers_to_string(buffer_.data());

    if (userId_ == 0) {
        dlog("userId_ == 0");
        // must be first login msg. otherwise close connection.

        // first msg, get userId, and save to member uid, and save to sessionmgr
        int64_t uid = get_uid_from_msg(msg);
        userId_ = uid;

        gpSessionMgr->switch2loggedin(this, uid);

        // Clear the buffer
        buffer_.consume(buffer_.size());

        // Read another message
        ws_.async_read(buffer_, boost::beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));

        return;
    }
    dlog("userId_ != 0, userId_:%lld", userId_);

    // Send to all connections
    gpSessionMgr->broadcast(msg);

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Read another message
    ws_.async_read(buffer_, boost::beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));
}

void websocket_session::send(boost::shared_ptr<std::string const> const& ss) {
    dlog();
    // Post our work to the strand, this ensures
    // that the members of `this` will not be
    // accessed concurrently.

    boost::asio::post(ws_.get_executor(), boost::beast::bind_front_handler(&websocket_session::on_send, shared_from_this(), ss));
}

void websocket_session::on_send(boost::shared_ptr<std::string const> const& ss) {
    dlog();
    // Always add to queue
    queue_.push_back(ss);

    // Are we already writing?
    if (queue_.size() > 1) return;

    // We are not currently writing, so send this immediately
    ws_.async_write(boost::asio::buffer(*queue_.front()),
                    boost::beast::bind_front_handler(&websocket_session::on_write, shared_from_this()));
}

void websocket_session::on_write(boost::beast::error_code ec, std::size_t) {
    dlog();
    // Handle the error, if any
    if (ec) return fail(ec, "write");

    // Remove the string from the queue
    queue_.erase(queue_.begin());

    // Send the next message if any
    if (!queue_.empty())
        ws_.async_write(boost::asio::buffer(*queue_.front()),
                        boost::beast::bind_front_handler(&websocket_session::on_write, shared_from_this()));
}
