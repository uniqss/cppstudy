#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <vector>

/*
 * execution serial显示，相同的时间的回调，顺序是随机的。这里会有问题执行多次的结果并不一致的问题。
 * 用传统的timer时间分片思路来做这个事情，是能保证顺序的。不确定是不是我没学到保证顺序的用法还是怎么回事。或者用多个io_context每个跑在一个线程里？
 * 出问题的场景：moba里面各客户端根据相同的随机种子，用这种同步写异步代码的思路写技能系统，当两个技能同时触发时，就会有问题。A是加个魔免的护盾，B是某单位魔法攻击A
 * 				如果A事件先触发，B事件无效，如果B事件先触发，单位血量就不一致了。
 * 				事实上纯逻辑上这个时候AB事件的顺序先后都正确，只是要保证各客户端的一致性。
 * 实测如果只有一个线程，是能保证顺序的
 * 继续学习吧，学无止境
 */

class printer {
   public:
    printer(asio::io_context& io, int timer_count) : strand_(asio::make_strand(io)), count_(0) {
        for (int i = 0; i < timer_count; ++i) {
            asio::steady_timer timer(io, std::chrono::seconds(1));
            timer.async_wait(asio::bind_executor(strand_, std::bind(&printer::print, this, i)));
            timers_.emplace_back(std::move(timer));
        }
    }
    ~printer() {
        std::cout << "count:" << count_ << std::endl;
        std::cout << "execution serial:" << std::endl;
        std::for_each(execution_serial_.begin(), execution_serial_.end(), [](int i) { std::cout << i << " "; });
        std::cout << std::endl;
    }
    void print(int timer_idx) {
        if (count_ < (int)timers_.size() * 5) {
            std::cout << std::this_thread::get_id() << " timer " << timer_idx << ": " << count_ << std::endl;
            ++count_;

            auto& timer = timers_[timer_idx];
            timer.expires_at(timer.expiry() + std::chrono::seconds(1));

            timer.async_wait(asio::bind_executor(strand_, std::bind(&printer::print, this, timer_idx)));
            execution_serial_.push_back(timer_idx);
        }
    }

   private:
    asio::strand<asio::io_context::executor_type> strand_;
    std::vector<asio::steady_timer> timers_;
    std::vector<int> execution_serial_;
    int count_;
};

int main() {
    asio::io_context io;
    printer p(io, 10);
    std::vector<std::thread> threads;
    for (int i = 0; i < 64; ++i) {
        std::thread t([&io]() { io.run(); });
        threads.emplace_back(std::move(t));
    }
    io.run();
    for_each(threads.begin(), threads.end(), [](std::thread& t) { t.detach(); });
    return 0;
}

