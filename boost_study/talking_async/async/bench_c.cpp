#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>

using asio::awaitable;
using asio::buffer;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;

/*
 * 这个程序的意义是非常重大的，这里变向证明了【同步写异步代码】的可行性。
 * 传统编程中，我们为了规避死锁等问题，多数游戏公司总结出一套服务器逻辑单线程多进程平行扩展的思路，大大降低了低级程序员犯错误的机率。但是问题也非常明显：
 *     例如，大多数公司不愿意用redis，原因是：同步用redis会卡逻辑帧，异步用redis还不如异步用数据库。比如redis的平均延迟是1ms，如果访问redis的消息的频率是
 *     1秒1个，那么同时在线的玩家数量就不能超过1000，否则逻辑帧的消息队列就会堆起来，cpu不高，内存不高，但是逻辑帧在等redis返回。
 * 这个示例中我们模拟redis的延迟是1秒钟，那么理论上，单线程情况下，qps就应该是1，但是这里无论设置成2/3/5/10/1000都是能回来的，也就是qps是能突破传统单线程
 *     卡逻辑帧的瓶颈的，可以同步写异步访问redis的代码，而不用担心会卡逻辑帧会卡其他玩家，是一件多么舒坦的事情！
 * 可以同步访问数据库等数据库返回，可以同步访问http接口，等http返回结果，可以同步访问redis拿数据，果然后出来的东西站在前人的肩膀上确实是有优势，asio的协程
 *     胜go的协程一手。
 */

struct conn_data {
    conn_data(int idx, asio::io_context& ctx) : idx(idx), socket(ctx, asio::ip::tcp::v4()) {}
    int idx;
    tcp::socket socket;
};

using conn_data_ptr = std::shared_ptr<conn_data>;

awaitable<void> get_server_time(conn_data_ptr cdata) {
    std::array<char, 1024> data;
    std::array<char, 1> data_send = {0};
    co_await async_write(cdata->socket, buffer(data_send), use_awaitable);

    auto n = co_await cdata->socket.async_read_some(buffer(data), use_awaitable);
    std::cout << " idx:" << cdata->idx << " n:" << n << " data:" << data.data() << std::endl;

    cdata->socket.close();
}

awaitable<void> client_operation(int idx, asio::io_context& ctx, tcp::endpoint target) {
    auto cdata = std::make_shared<conn_data>(idx, ctx);

    for (;;) {
        co_await cdata->socket.async_connect(target, use_awaitable);

        co_await get_server_time(cdata);

        asio::steady_timer timer(cdata->socket.get_executor(), std::chrono::milliseconds(100));

        co_await timer.async_wait(asio::use_awaitable);
    }
}

int main(int argc, const char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage:" << argv[0] << " <host_addr> <host_port> \n";
            return 1;
        }
        asio::io_context ctx;
        asio::ip::tcp::endpoint target_endpoint = *asio::ip::tcp::resolver(ctx).resolve(argv[1], argv[2]);

        for (int i = 0; i < 5; ++i) {
            co_spawn(ctx, client_operation(i, ctx, target_endpoint), detached);
        }

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << e.what() << std::endl;
    }
    return 0;
}
