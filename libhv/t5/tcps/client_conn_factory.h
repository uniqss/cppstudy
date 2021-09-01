#ifndef __CLIENT_CONN_FACTORY_H__
#define __CLIENT_CONN_FACTORY_H__

#include "net.h"

class client_conn_factory : public connection_factory
{
    public:
    virtual ~client_conn_factory(){}
    virtual connection_t* create_connection();
    virtual void release_connection(connection_t* conn);
};

#endif
