
#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include "connection_s.h"

#include <assert.h>

#include "p.h"

void broadcast(listener* l, const char* msg, int msglen) {
    _myprtf("> %.*s", msglen, msg);
    l->conn_factory->for_each([&](connection_t* conn) {
        hio_write(conn->connio, msg, msglen);
        return 0;
    });
}

void join(listener* l, connection_t* conn) {
    _myprtf("\n");
    char msg[256] = {0};
    int msglen = 0;

    connection_t* cur;
    msglen = snprintf(msg, sizeof(msg), "room[%llu] clients:\r\n", l);
    hio_write(conn->connio, msg, msglen);
    l->conn_factory->for_each([&](connection_t* conn) {
        msglen = snprintf(msg, sizeof(msg), "[%s]\r\n", cur->addr);
        hio_write(conn->connio, msg, msglen);
        return 0;
    });

    hio_write(conn->connio, "\r\n", 2);

    msglen = snprintf(msg, sizeof(msg), "client[%s] join room[%llu]\r\n", conn->addr, l);
    broadcast(l, msg, msglen);
}

void leave(listener* l, connection_t* conn) {
    _myprtf("\n");
    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] leave room[%llu]\r\n", conn->addr, l);
    broadcast(l, msg, msglen);
}

void connection_s::on_establish() {
    _myprtf("connection_s::on_establish\n");
}
void connection_s::on_recv(void* buf, int readbytes) {
    _myprtf("connection_s::on_recv readbytes:%d\n", readbytes);
    assert(root_listener_or_connector != NULL);
    listener* l = (listener*)root_listener_or_connector;

    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] say: %.*s", addr, readbytes, (char*)buf);
    broadcast(l, msg, msglen);
}
void connection_s::on_close(int error) {
    _myprtf("connection_s::on_close error:%d\n", error);
    if (root_listener_or_connector) {
        listener* l = (listener*)root_listener_or_connector;
        leave(l, this);
    }
}
