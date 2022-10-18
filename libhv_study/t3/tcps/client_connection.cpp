
#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include "client_connection.h"

#include <assert.h>


void broadcast(listener* room, const char* msg, int msglen) {
    printf("> %.*s", msglen, msg);
    for (connection_t* conn : room->conn_list) {
        hio_write(conn->connio, msg, msglen);
    }
}

void join(listener* room, connection_t* conn) {
    room->conn_list.push_back(conn);

    char msg[256] = {0};
    int msglen = 0;

    connection_t* cur;
    msglen = snprintf(msg, sizeof(msg), "room[%06d] clients:\r\n", room->roomid);
    hio_write(conn->connio, msg, msglen);
    for (connection_t* cur : room->conn_list) {
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

void client_connection::on_establish() {
    printf("client_connection::on_establish\n");
}
void client_connection::on_recv(void* buf, int readbytes) {
    printf("client_connection::on_recv readbytes:%d\n", readbytes);
    assert(root_listener_or_connector != NULL);
    char msg[256] = {0};
    int msglen = snprintf(msg, sizeof(msg), "client[%s] say: %.*s", addr, readbytes, (char*)buf);
    broadcast(root_listener_or_connector, msg, msglen);
}
void client_connection::on_close(int error) {
    printf("client_connection::on_recv error:%d\n", error);
    if (root_listener_or_connector) {
        leave(root_listener_or_connector, this);
    }
}
