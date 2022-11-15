#include <asio.hpp>
#include "dbg.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
std::atomic_bool g_working = true;

struct redis_query {
    int qid;
    std::string query;
    std::string result;
};
class safe_queue {
   public:
    void pop(redis_query*& e) {
        e = nullptr;
        std::lock_guard<std::mutex> _(mtx);
        if (q.empty()) return;
        *e = q.front();
        q.pop();
    }
    void push(const redis_query& e) {
        std::lock_guard<std::mutex> _(mtx);
        q.push(e);
    }

   private:
    std::mutex mtx;
    std::queue<redis_query> q;
};
safe_queue g_redis_working_q;
safe_queue g_redis_finish_q;

void fake_redis_thread() {
    while (g_working) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        redis_query* query = nullptr;
        g_redis_working_q.pop(query);
        if (query != nullptr) {
            g_redis_finish_q.push(*query);
        }
    }
}

int g_redis_id = 0;
asio::awaitable<void> call_redis() {
    redis_query query{++g_redis_id, ""};
    g_redis_working_q.push(query);

    co_return;
}

asio::awaitable<void> awaitable_test(asio::io_context& ctx) {
    for (int i = 0; i < 20; ++i) {
        asio::co_spawn(ctx, call_redis(), asio::detached);
    }
    co_return;
}

int main() {
    std::thread t(fake_redis_thread);
    t.detach();
    asio::io_context ctx;
    asio::co_spawn(ctx, awaitable_test(ctx), asio::detached);
    ctx.run();
    return 0;
}