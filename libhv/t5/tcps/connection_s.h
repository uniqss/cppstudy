#ifndef __CONNECTION_S_H__
#define __CONNECTION_S_H__

#include "net.h"

class connection_s : public connection_t {
   public:
    virtual void on_establish();
    virtual void on_recv(void* buf, int readbytes);
    virtual void on_close(int error);
};

#endif
