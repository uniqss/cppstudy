#include "connection_c.h"
#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"

#include "p.h"

#pragma pack(push, 1)
struct SCommonHead {
    unsigned int len;
};
#pragma pack(pop)
#define HEADLEN (sizeof(SCommonHead))


static void send_heartbeat(hio_t* io) {
    _myprtf("\n");
    static char buf[] = "aaaaaaaaaa This is heartbeat from some client.";
    SCommonHead* head = (SCommonHead*)buf;
    head->len = sizeof(buf) - HEADLEN;

    _myprtf("send heartbeat [%d] %s\n", head->len, buf + HEADLEN);
    hio_write(io, buf, head->len + HEADLEN);
}

void connection_c::on_establish() {
    _myprtf("connection_c::on_establish\n");

    // uncomment to test heartbeat
    hio_set_heartbeat(this->connio, 3000, send_heartbeat);
}

void connection_c::on_recv(void* buf, int readbytes) {
    _myprtf("connection_c::on_recv readbytes=%d\n", readbytes);

    SCommonHead* head = (SCommonHead*)buf;
    _myprtf("%.*s", readbytes, (char*)buf);
    _myprtf("%d %s", head->len, (char*)buf + HEADLEN);

    fflush(stdout);
}

void connection_c::on_close(int error) {
    _myprtf("connection_c::on_close error=%d\n", error);
}
