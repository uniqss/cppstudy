#ifndef __NET_H__
#define __NET_H__

#include <memory>
#include <unordered_set>
#include <functional>

typedef struct hio_s hio_t;
typedef struct hloop_s hloop_t;

class listener;
class connection_t {
   public:
    hio_t* connio;
    char addr[64];
    char is_server_peer;  // 0 not set. 1 server peer. 2 client peer
    void* root_listener_or_connector;
    virtual ~connection_t() {}
    virtual void on_establish();
    virtual void on_recv(void* buf, int readbytes);
    virtual void on_close(int error);
};

class connection_factory {
   protected:
    std::unordered_set<connection_t*> set_connections;

   public:
    virtual ~connection_factory() {}
    virtual connection_t* create_connection();
    virtual void release_connection(connection_t* conn);

    connection_t* for_each(std::function<int(const connection_t*)> fn) const;
    connection_t* for_each(std::function<int(connection_t*)> fn);
};

class net;
class listener {
   public:
    net* root_net;
    hio_t* listenio;
    connection_factory* conn_factory;

    virtual bool listen(const char* ip, unsigned short port);
};

class connector {
   public:
    net* root_net;
    char addr[64];
    unsigned short port;
    connection_t* conn;
    virtual bool connect(const char* ip, unsigned short port);
    virtual void close();
};

class net {
   public:
    hloop_t* loop;
    std::unique_ptr<listener> create_listener(connection_factory* conn_factory);
    std::unique_ptr<connector> create_connector(connection_t* conn);
    int run();
    ~net();
};

std::unique_ptr<net> create_net();

#endif
