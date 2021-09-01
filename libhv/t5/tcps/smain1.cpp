#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include <list>
#include <memory>

#include "net.h"
#include "connection_s.h"
#include "client_conn_factory.h"

#include "p.h"

int main(int argc, char** argv) {
    _myprtf("\n");
    if (argc < 2) {
        _myprtf("Usage: %s port\n", argv[0]);
        return -10;
    }
    int port = atoi(argv[1]);

    auto net = create_net();
    if (net == NULL) {
        _myprtf("net == NULL\n");
        return -100;
    }

    client_conn_factory cf;
    auto l = net->create_listener(&cf);
    bool ok = false;
    unsigned short usport = (unsigned short)port;
    ok = l->listen("0.0.0.0", usport);
    if (!ok) {
        _myprtf("listen failed.%d\n", usport);
        return -200;
    }

    net->run();

    return 0;
}
