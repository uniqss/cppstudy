#ifndef __P_H__
#define __P_H__

#include <iostream>
#include <thread>

#define _myprtf                                                                                      \
    {                                                                                                \
        std::thread::id __t_id = std::this_thread::get_id();                                         \
        std::cout << "[" << __FILE__ << ":" << __LINE__ << " " << __func__ << " " << __t_id << "] "; \
    };                                                                                               \
    printf

#endif
