#include "hloop.h"
#include "hbase.h"
#include "hsocket.h"

#define RECV_BUFSIZE 8192
static char recvbuf[RECV_BUFSIZE];

// for stdin
hio_t* stdinio = NULL;
// for socket
hio_t* sockio = NULL;

int verbose = 0;

static void send_heartbeat(hio_t* io) {
    static char buf[] = "PING\r\n";
    printf("send heartbeat %s", buf);
    hio_write(io, buf, 6);
}

static void on_recv(hio_t* io, void* buf, int readbytes) {
    // printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    if (verbose) {
        char localaddrstr[SOCKADDR_STRLEN] = {0};
        char peeraddrstr[SOCKADDR_STRLEN] = {0};
        printf("[%s] <=> [%s]\n", SOCKADDR_STR(hio_localaddr(io), localaddrstr), SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    }
    printf("%.*s", readbytes, (char*)buf);

    // test hio_set_readbuf in hread_cb
#if 0
    static int total_readbytes = 0;
    total_readbytes += readbytes;
    if (total_readbytes >= RECV_BUFSIZE) {
        total_readbytes = 0;
    }
    hio_set_readbuf(io, recvbuf + total_readbytes, RECV_BUFSIZE - total_readbytes);
    printf("%.*s", total_readbytes, recvbuf);
#endif

    fflush(stdout);
}

static void on_close(hio_t* io) {
    printf("on_close fd=%d error=%d\n", hio_fd(io), hio_error(io));
    hio_del(stdinio, HV_READ);
}

static void on_connect(hio_t* io) {
    printf("on_connect fd=%d\n", hio_fd(io));
    if (verbose) {
        char localaddrstr[SOCKADDR_STRLEN] = {0};
        char peeraddrstr[SOCKADDR_STRLEN] = {0};
        printf("connect connfd=%d [%s] => [%s]\n", hio_fd(io), SOCKADDR_STR(hio_localaddr(io), localaddrstr), SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    }

    hio_read_start(io);
    // uncomment to test heartbeat
    // hio_set_heartbeat(sockio, 3000, send_heartbeat);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("arg not enough.\n");
        return -1;
    }

    int index = 1;
    const char* host = argv[index++];
    int port = atoi(argv[index++]);
    if (verbose) {
        printf("%s %d\n", host, port);
    }

    HV_MEMCHECK;

    hloop_t* loop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);

    // tcp
    sockio = hloop_create_tcp_client(loop, host, port, on_connect);
    if (sockio == NULL) {
        return -20;
    }

    printf("sockfd=%d\n", hio_fd(sockio));
    hio_setcb_close(sockio, on_close);
    hio_setcb_read(sockio, on_recv);
    hio_set_readbuf(sockio, recvbuf, RECV_BUFSIZE);

    hloop_run(loop);
    hloop_free(&loop);

    return 0;
}
