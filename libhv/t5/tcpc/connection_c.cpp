#include "connection_c.h"
#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"

#include "../tcps/p.h"

static void send_heartbeat(hio_t* io) {
    _myprtf("\n");
    static char buf[] = "PING\r\n";
    _myprtf("send heartbeat %s", buf);
    hio_write(io, buf, 6);
}

void connection_c::on_establish(){
    _myprtf("connection_c::on_establish\n");
    
    // uncomment to test heartbeat
    hio_set_heartbeat(this->connio, 3000, send_heartbeat);
}

void connection_c::on_recv(void* buf, int readbytes){
    _myprtf("connection_c::on_recv readbytes=%d\n", readbytes);
    
    _myprtf("%.*s", readbytes, (char*)buf);

    fflush(stdout);
}

void connection_c::on_close(int error){
    _myprtf("connection_c::on_close error=%d\n", error);
}
