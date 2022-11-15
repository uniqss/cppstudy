#pragma once

#include <unordered_map>
#include <thread>
#include <chrono>

std::atomic_int g_dbg_tid_next = 0;
std::mutex g_dbg_tid_readable_mtx;
std::unordered_map<std::thread::id, int> g_dbg_tid_readable;
#define PPID                                                           \
    int __tid_readable = 0;                                            \
    do {                                                               \
        std::lock_guard<std::mutex> __dbg_tid(g_dbg_tid_readable_mtx); \
        auto tid = std::this_thread::get_id();                         \
        if (g_dbg_tid_readable.count(tid) > 0) {                       \
            __tid_readable = g_dbg_tid_readable[tid];                  \
        } else {                                                       \
            g_dbg_tid_readable[tid] = ++g_dbg_tid_next;                \
            __tid_readable = g_dbg_tid_next;                           \
        }                                                              \
    } while (false);                                                   \
    std::cout << "******* " << __tid_readable << "\t" << __FILE__ << ":" << __LINE__ << " " << __func__ << std::endl;

#define PTIMEMS \
    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

#define PT std::cout << PTIMEMS << " "