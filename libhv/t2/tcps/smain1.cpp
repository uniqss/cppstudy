#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include <list>
#include <memory>

static void on_accept(hio_t* io);

class listener;
class connection_t
{
    public:
    hio_t*    connio;
    char      addr[SOCKADDR_STRLEN];
    listener* root_listener_or_connector;
};

class net;
class listener {
    public:
    net*                     root_net;
    hio_t*                   listenio;
    int                      roomid;
    std::list<connection_t*> conn_list;
};

class net {
    public:
    hloop_t*    loop;
    std::unique_ptr<listener> create_listener(const char* ip, unsigned short port){
        std::unique_ptr<listener> l(new listener);

        hio_t* listenio = hloop_create_tcp_server(loop, "0.0.0.0", port, on_accept);
        if (listenio == NULL) {
            return NULL;
        }
        printf("listenfd=%d\n", hio_fd(listenio));

        hevent_set_userdata(listenio, l.get());

        l->root_net = this;
        l->listenio = listenio;
        srand(time(NULL));
        l->roomid = rand() % 1000000;
        return l;
    }
    void run(){
        hloop_run(loop);
    }
    ~net(){
        hloop_free(&loop);
    }
};

static std::unique_ptr<net> s_net;

class listener;
class connection_t;
static void join(listener* room, connection_t* conn);
static void leave(listener* room, connection_t* conn);
static void broadcast(listener* room, const char* msg, int msglen);

static void on_close(hio_t* io) {
    printf("on_close fd=%d error=%d\n", hio_fd(io), hio_error(io));

    connection_t* conn = (connection_t*)hevent_userdata(io);
    if (conn) {
        if (conn->root_listener_or_connector){
            leave(conn->root_listener_or_connector, conn);
        }
        hevent_set_userdata(io, NULL);

        HV_FREE(conn);
    }
}

static void on_recv(hio_t* io, void* buf, int readbytes) {
    printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("[%s] <=> [%s]\n",
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    printf("< %.*s", readbytes, (char*)buf);

    // broadcast
    connection_t* conn = (connection_t*)hevent_userdata(io);
    assert(conn != NULL);
    assert(conn->root_listener_or_connector != NULL);
    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] say: %.*s", conn->addr, readbytes, (char*)buf);
    broadcast(conn->root_listener_or_connector, msg, msglen);
}

static void on_accept(hio_t* io) {
    printf("on accept connfd=%d\n", hio_fd(io));
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("accept connfd=%d [%s] <= [%s]\n", hio_fd(io),
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));

    hio_setcb_close(io, on_close);
    hio_setcb_read(io, on_recv);
    hio_read(io);

    listener* l = (listener*)hevent_userdata(io);

    // free on_close
    connection_t* conn = NULL;
    HV_ALLOC_SIZEOF(conn);
    conn->connio = io;
    conn->root_listener_or_connector = l;
    strcpy(conn->addr, peeraddrstr);
    hevent_set_userdata(io, conn);
    join(l, conn);
}


std::unique_ptr<net> create_net(){
    std::unique_ptr<net> n(new net);
    hloop_t* loop = hloop_new(0);
    if (loop == NULL) {
        return NULL;
    }
    n->loop = loop;
    return std::move(n);
}

void join(listener* room, connection_t* conn) {
    room->conn_list.push_back(conn);

    char msg[256] = {0};
    int msglen = 0;

    connection_t* cur;
    msglen = snprintf(msg, sizeof(msg), "room[%06d] clients:\r\n", room->roomid);
    hio_write(conn->connio, msg, msglen);
    for (connection_t* cur : room->conn_list)
    {
        msglen = snprintf(msg, sizeof(msg), "[%s]\r\n", cur->addr);
        hio_write(conn->connio, msg, msglen);
    }
    
    hio_write(conn->connio, "\r\n", 2);

    msglen = snprintf(msg, sizeof(msg), "client[%s] join room[%06d]\r\n", conn->addr, room->roomid);
    broadcast(room, msg, msglen);
}

void leave(listener* room, connection_t* conn) {
    room->conn_list.remove(conn);

    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] leave room[%d]\r\n", conn->addr, room->roomid);
    broadcast(room, msg, msglen);
}

void broadcast(listener* room, const char* msg, int msglen) {
    printf("> %.*s", msglen, msg);
    for (connection_t* conn : room->conn_list)
    {
        hio_write(conn->connio, msg, msglen);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -10;
    }
    int port = atoi(argv[1]);

    s_net = create_net();
    if (s_net == NULL) {
        printf("s_net == NULL\n");
        return -100;
    }

    auto l = s_net->create_listener("0.0.0.0", port);

    s_net->run();

    return 0;
}
