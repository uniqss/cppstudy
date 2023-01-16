#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include <unordered_map>

// Forward declaration
class websocket_session;

// Represents the shared server state
class session_mgr {
    // This mutex synchronizes all access to prelogin_sessions_
    std::mutex mutex_;

    // Keep a list of all the connected clients
    std::unordered_set<websocket_session*> prelogin_sessions_;
    std::unordered_map<int64_t, websocket_session*> uid2sessions_;

   public:
    explicit session_mgr();

    void join2prelogin(websocket_session* session);
    void switch2loggedin(websocket_session* session, int64_t uid);
    void leave(websocket_session* session);
    void broadcast(const std::string& message);
};

extern std::unique_ptr<session_mgr> gpSessionMgr;
