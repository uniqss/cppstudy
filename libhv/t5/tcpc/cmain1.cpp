#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"

#include "net.h"

#include "connection_c.h"

#include "../tcps/p.h"

int main(int argc, char** argv) {
    _myprtf("\n");
    if (argc < 3)
    {
        _myprtf("arg not enough.\n");
        return -1;
    }

    int index = 1;
    const char* host = argv[index++];
    int port = atoi(argv[index++]);

    HV_MEMCHECK;

    auto net = create_net();

    connection_c conn;
    auto c = net->create_connector(&conn);
    if (c == NULL){
        return -300;
    }
    bool ok = false;
    ok = c->connect(host, (unsigned short)port);
    if (!ok){
        _myprtf("c->connect failed.host:%s, port:%d\n", host, (int)(unsigned short)port);
        return -300;
    }
    
    net->run();

    return 0;
}
