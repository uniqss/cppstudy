#pragma once

#include "net.h"

class connection_c : public connection_t {
   public:
    virtual void on_establish();
    virtual void on_recv(void *buf, int readbytes);
    virtual void on_close(int error);
};
