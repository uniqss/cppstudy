#pragma once

#include <thread>
#include <algorithm>

std::size_t getThreadTid() {
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}
