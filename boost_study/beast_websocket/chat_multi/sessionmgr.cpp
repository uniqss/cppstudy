
#include "sessionmgr.hpp"
#include "websocket_session.hpp"

#include "ulog.h"

std::unique_ptr<session_mgr> gpSessionMgr = std::make_unique<session_mgr>();

session_mgr::session_mgr() {}

void session_mgr::join2prelogin(websocket_session* session) {
    std::lock_guard<std::mutex> lock(mutex_);
    prelogin_sessions_.insert(session);
}

void session_mgr::switch2loggedin(websocket_session* session, int64_t uid) {
    bool exist_prelogin = true;
    bool exist_login = true;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        exist_prelogin = prelogin_sessions_.count(session) > 0;
        exist_login = uid2sessions_.count(uid) > 0;
        prelogin_sessions_.erase(session);
        uid2sessions_[uid] = session;
    }
    if (!exist_prelogin) {
        // log error. must be pre login
        throw std::logic_error("must be pre login");
    }
    if (exist_login) {
        // log error, should not exist in login
        throw std::logic_error("should not exist in login");
    }
}

void session_mgr::leave(websocket_session* session) {
    std::lock_guard<std::mutex> lock(mutex_);
    prelogin_sessions_.erase(session);
    if (session != nullptr) {
        int64_t uid = session->get_uid();
        uid2sessions_.erase(uid);
    }
}

// Broadcast a message to all websocket client sessions
void session_mgr::broadcast(const std::string& message) {
    // Put the message in a shared pointer so we can re-use it for each client
    auto const ss = boost::make_shared<std::string const>(std::move(message));

    // Make a local list of all the weak pointers representing
    // the sessions, so we can do the actual sending without
    // holding the mutex:
    std::vector<boost::weak_ptr<websocket_session>> v;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        v.reserve(uid2sessions_.size());
        dlog("================ broadcast uid2sessions_.size():%d", uid2sessions_.size());
        for (auto p : uid2sessions_) {
            v.emplace_back(p.second->weak_from_this());
        }
    }

    // For each session in our local list, try to acquire a strong
    // pointer. If successful, then send the message on that session.
    for (auto const& wp : v) {
        if (auto sp = wp.lock()) {
            sp->send(ss);
        }
    }
}
