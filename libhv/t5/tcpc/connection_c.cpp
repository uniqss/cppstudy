#include "connection_c.h"
#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"

static void send_heartbeat(hio_t* io) {
    static char buf[] = "PING\r\n";
    printf("send heartbeat %s", buf);
    hio_write(io, buf, 6);
}

void connection_c::on_establish(){
    printf("connection_c::on_establish\n");
    
    // uncomment to test heartbeat
    hio_set_heartbeat(this->connio, 3000, send_heartbeat);
}

void connection_c::on_recv(void* buf, int readbytes){
    printf("connection_c::on_recv readbytes=%d\n", readbytes);
    
    printf("%.*s", readbytes, (char*)buf);

    fflush(stdout);
}

void connection_c::on_close(int error){
    printf("connection_c::on_close error=%d\n", error);
}
