#include "hloop.h"
#include "hsocket.h"
#include "hbase.h"

#include <list>
#include <memory>

#include "net.h"
#include "client_connection.h"
#include "client_conn_factory.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s port\n", argv[0]);
        return -10;
    }
    int port = atoi(argv[1]);

    auto net = create_net();
    if (net == NULL) {
        printf("net == NULL\n");
        return -100;
    }

    client_conn_factory cf;
    auto l = net->create_listener("0.0.0.0", port, &cf);

    net->run();

    return 0;
}
