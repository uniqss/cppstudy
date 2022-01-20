#ifndef __CONNECTION_S_H__
#define __CONNECTION_S_H__

#include "net.h"

class client_connection : public connection_t {
   public:
    virtual ~client_connection() {}
    virtual void on_establish();
    virtual void on_recv(void* buf, int readbytes);
    virtual void on_close(int error);
};

#endif
