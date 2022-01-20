
#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include <list>
#include <memory>

#include "net.h"

// member functions

void connection_t::on_establish() {
    printf("connection_t::on_establish\n");
}
void connection_t::on_recv(void* buf, int readbytes) {
    printf("connection_t::on_recv readbytes:%d\n", readbytes);
}
void connection_t::on_close(int error) {
    printf("connection_t::on_recv error:%d\n", error);
}

connection_t* connection_factory::create_connection() {
    connection_t* conn = new connection_t();
    if (conn == NULL) {
        return NULL;
    }
    set_connections.insert(conn);
    return conn;
}
void connection_factory::release_connection(connection_t* conn) {
    delete (conn);
    set_connections.erase(conn);
}

static void on_accept(hio_t* io);
std::unique_ptr<listener> net::create_listener(const char* ip, unsigned short port, connection_factory* conn_factory) {
    std::unique_ptr<listener> l(new listener);

    hio_t* listenio = hloop_create_tcp_server(this->loop, "0.0.0.0", port, on_accept);
    if (listenio == NULL) {
        return NULL;
    }
    printf("listenfd=%d\n", hio_fd(listenio));

    hevent_set_userdata(listenio, l.get());

    l->root_net = this;
    l->listenio = listenio;
    l->conn_factory = conn_factory;
    srand(time(NULL));
    l->roomid = rand() % 1000000;
    return l;
}

int net::run() {
    return hloop_run(loop);
}
net::~net() {
    hloop_free(&loop);
}


// callbacks
static void on_close(hio_t* io) {
    int fd = hio_fd(io);
    int errorno = hio_error(io);
    printf("on_close fd=%d error=%d\n", fd, errorno);

    connection_t* conn = (connection_t*)hevent_userdata(io);
    if (conn) {
        conn->on_close(errorno);
        hevent_set_userdata(io, NULL);

        HV_FREE(conn);
    }
}

static void on_recv(hio_t* io, void* buf, int readbytes) {
    printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("[%s] <=> [%s]\n", SOCKADDR_STR(hio_localaddr(io), localaddrstr), SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    printf("< %.*s", readbytes, (char*)buf);

    // broadcast
    connection_t* conn = (connection_t*)hevent_userdata(io);
    assert(conn != NULL);
    assert(conn->root_listener_or_connector != NULL);
    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] say: %.*s", conn->addr, readbytes, (char*)buf);
    // broadcast(conn->root_listener_or_connector, msg, msglen);
    conn->on_recv(buf, readbytes);
}

static void on_accept(hio_t* io) {
    printf("on accept connfd=%d\n", hio_fd(io));
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("accept connfd=%d [%s] <= [%s]\n", hio_fd(io), SOCKADDR_STR(hio_localaddr(io), localaddrstr), SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));

    hio_setcb_close(io, on_close);
    hio_setcb_read(io, on_recv);
    hio_read(io);

    listener* l = (listener*)hevent_userdata(io);

    assert(l != NULL);
    connection_factory* cf = l->conn_factory;
    assert(cf != NULL);
    // free on_close
    connection_t* conn = cf->create_connection();
    assert(cf != NULL);
    conn->connio = io;
    conn->is_server_peer = 1;
    conn->root_listener_or_connector = l;
    strcpy(conn->addr, peeraddrstr);
    hevent_set_userdata(io, conn);
    // join(l, conn);
    conn->on_establish();
}

// root
std::unique_ptr<net> create_net() {
    std::unique_ptr<net> n(new net);
    hloop_t* loop = hloop_new(0);
    if (loop == NULL) {
        return NULL;
    }
    n->loop = loop;
    return std::move(n);
}
