#ifndef __NET_H__
#define __NET_H__

#include<memory>
#include<list>
#include<unordered_set>

typedef struct hio_s hio_t;
typedef struct hloop_s hloop_t;

class listener;
class connection_t
{
    public:
    hio_t*    connio;
    char      addr[64];
    char      is_server_peer; // 0 not set. 1 server peer. 2 client peer
    listener* root_listener_or_connector;
    virtual ~connection_t(){}
    virtual void on_establish();
    virtual void on_recv(void* buf, int readbytes);
    virtual void on_close(int error);
};

class connection_factory
{
    protected:
    std::unordered_set<connection_t*> set_connections;
    public:
    virtual ~connection_factory(){}
    virtual connection_t* create_connection();
    virtual void release_connection(connection_t* conn);
};

class net;
class listener {
    public:
    net*                    root_net;
    hio_t*                  listenio;
    int                     roomid;
    connection_factory*     conn_factory;
    std::list<connection_t*> conn_list;
};

class net {
    public:
    hloop_t*    loop;
    std::unique_ptr<listener> create_listener(const char* ip, unsigned short port, connection_factory* conn_factory);
    int run();
    ~net();
};

std::unique_ptr<net> create_net();

#endif
