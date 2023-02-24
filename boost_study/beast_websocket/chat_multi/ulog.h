#pragma once

#include <iostream>
#include <thread>
static const char* uniqs_getfilename(const char* file) {
    auto i = strrchr(file, '/');
    if (i != nullptr)
        return i + 1;
    else
        return file;
}

inline void uniqs_dlog(const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);

    std::cout << buf << std::endl;
}

#define dlog(fmt, ...)                                                                                              \
    uniqs_dlog("[%lld %s:%d %s] " fmt, std::this_thread::get_id(), uniqs_getfilename(__FILE__), __LINE__, __func__, \
               ##__VA_ARGS__)
