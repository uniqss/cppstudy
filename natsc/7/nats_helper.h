#pragma once

#include <nats.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#define strcasecmp _stricmp
#define strdup _strdup
#else
#include <signal.h>
#include <strings.h>
#endif

#define STATS_IN 0x1
#define STATS_OUT 0x2
#define STATS_COUNT 0x4

#define MAX_SERVERS (10)

const char *subj = "foo";
const char *payload = "hello";
const char *name = "worker";
int64_t total = 100000000;

volatile int64_t count = 0;
volatile int64_t dropped = 0;
int64_t start = 0;
volatile int64_t elapsed = 0;
bool print = false;
int64_t timeout = 10000;  // 10 seconds.

const char *certFile = NULL;
const char *keyFile = NULL;

const char *cluster = "test-cluster";
const char *clientID = "client";
const char *qgroup = NULL;
const char *durable = NULL;
bool deliverAll = false;
bool deliverLast = true;
uint64_t deliverSeq = 0;
bool unsubscribe = false;

const char *stream = NULL;
bool pull = false;
bool flowctrl = false;

std::size_t getThreadTid() {
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

#pragma pack(1)

enum EServerType {
    EServerType_Invalid = 0,
    EServerType_Client,
    EServerType_Gate,
    EServerType_Game
};

#define NET_SEND_RECV_BODY_MAX_SIZE 65536
#define NET_MSG_HEAD_SIZE 6

struct NetMsgHead {
    unsigned short bodyWithoutHeadLen;
    unsigned char from;
    unsigned char to;
};
struct NetMsg {
    NetMsgHead head;
    char body[NET_SEND_RECV_BODY_MAX_SIZE];
};


#pragma pack()

static natsStatus printStats(int mode, natsConnection *conn, natsSubscription *sub,
                             natsStatistics *stats) {
    natsStatus s = NATS_OK;
    uint64_t inMsgs = 0;
    uint64_t inBytes = 0;
    uint64_t outMsgs = 0;
    uint64_t outBytes = 0;
    uint64_t reconnected = 0;
    int pending = 0;
    int64_t delivered = 0;
    int64_t sdropped = 0;

    s = natsConnection_GetStats(conn, stats);
    if (s == NATS_OK)
        s = natsStatistics_GetCounts(stats, &inMsgs, &inBytes,
                                     &outMsgs, &outBytes, &reconnected);
    if ((s == NATS_OK) && (sub != NULL)) {
        s = natsSubscription_GetStats(sub, &pending, NULL, NULL, NULL,
                                      &delivered, &sdropped);

        // Since we use AutoUnsubscribe(), when the max has been reached,
        // the subscription is automatically closed, so this call would
        // return "Invalid Subscription". Ignore this error.
        if (s == NATS_INVALID_SUBSCRIPTION) {
            s = NATS_OK;
            pending = 0;
        }
    }

    if (s == NATS_OK) {
        if (mode & STATS_IN) {
            printf("%lu In Msgs: %9" PRIu64 " - In Bytes: %9" PRIu64 " - ",
                   getThreadTid(), inMsgs, inBytes);
        }
        if (mode & STATS_OUT) {
            printf("%lu Out Msgs: %9" PRIu64 " - Out Bytes: %9" PRIu64 " - ",
                   getThreadTid(), outMsgs, outBytes);
        }
        if (mode & STATS_COUNT) {
            printf("Delivered: %9" PRId64 " - ", delivered);
            printf("Pending: %5d - ", pending);
            printf("Dropped: %5" PRId64 " - ", sdropped);
        }
        printf("Reconnected: %3" PRIu64 "\n", reconnected);
    }

    return s;
}

static void printPerf(const char *perfTxt) {
    if ((start > 0) && (elapsed == 0))
        elapsed = nats_Now() - start;

    if (elapsed <= 0)
        printf("\n %lu Not enough messages or too fast to report performance!\n", getThreadTid());
    else
        printf("\n %lu %s %" PRId64 " messages in %" PRId64 " milliseconds (%d msgs/sec) (%d msgs/millisecond)\n",
               getThreadTid(), perfTxt, count, elapsed, (int)((count * 1000) / elapsed), (int)(count / elapsed));
}

static natsStatus constructOptions(natsOptions *&opts, const std::vector<std::string> &urls) {
    natsStatus s = NATS_OK;
    do {
        if (natsOptions_Create(&opts) != NATS_OK) {
            s = NATS_NO_MEMORY;
            break;
        }

        if (certFile != NULL || keyFile != NULL) {
            s = natsOptions_LoadCertificatesChain(opts, certFile, keyFile);
            if (s != NATS_OK) {
                break;
            }
        }

        // parse urls
        const char *serverUrls[MAX_SERVERS];
        int i = 0;
        for (const auto &url : urls) {
            serverUrls[i++] = url.c_str();
        }
        s = natsOptions_SetServers(opts, (const char **)serverUrls, urls.size());
        if (s != NATS_OK) {
            break;
        }

    } while (false);

    if (s != NATS_OK) {
        printf("%lu Error parsing arguments: %d - %s\n", getThreadTid(), s, natsStatus_GetText(s));

        nats_PrintLastErrorStack(stderr);

        natsOptions_Destroy(opts);
        nats_Close();
    }

    return s;
}
