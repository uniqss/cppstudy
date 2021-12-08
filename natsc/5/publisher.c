
#include <iostream>
#include <string>

#include "nats_helper.h"

int main() {
    natsConnection *conn = NULL;
    natsStatistics *stats = NULL;
    natsOptions *opts = NULL;
    natsStatus s;
    int dataLen = 0;

    s = constructOptions(opts, {NATS_DEFAULT_URL});
    if (s != NATS_OK) {
        return -1;
    }

    printf("%lu Sending %" PRId64 " messages to subject '%s'\n", getThreadTid(), total, subj);

    s = natsConnection_Connect(&conn, opts);

    if (s == NATS_OK)
        s = natsStatistics_Create(&stats);

    if (s == NATS_OK)
        start = nats_Now();

    std::string input;

    NetMsg msg;
    msg.head.bodyWithoutHeadLen = 0;
    msg.head.from = EServerType_Gate;
    msg.head.to = EServerType_Game;

    char send_buff[NET_SEND_RECV_BODY_MAX_SIZE] = {0};
    while (true) {
        fgets(send_buff, NET_SEND_RECV_BODY_MAX_SIZE - 1, stdin);

        if (strncasecmp(send_buff, "e", 1) == 0 || strncasecmp(send_buff, "exit", 4) == 0) break;
        dataLen = strlen(send_buff);
        if (dataLen >= NET_SEND_RECV_BODY_MAX_SIZE) {
            printf("send msg to large. dataLen:%d \n", dataLen);
            break;
        }
        msg.head.bodyWithoutHeadLen = dataLen;
        int totalLen = dataLen + sizeof(NetMsgHead);
        memcpy(msg.body, send_buff, dataLen);
        s = natsConnection_Publish(conn, subj, (const void *)&msg, totalLen);
        if (s != NATS_OK) {
            break;
        }
        // if (s == NATS_OK)
        //     s = natsConnection_FlushTimeout(conn, 1000);
    }

    if (s == NATS_OK)
        s = natsConnection_FlushTimeout(conn, 1000);

    if (s == NATS_OK) {
        printStats(STATS_OUT, conn, NULL, stats);
        printPerf("Sent");
    } else {
        printf("Error: %d - %s\n", s, natsStatus_GetText(s));
        nats_PrintLastErrorStack(stderr);
    }

    // Destroy all our objects to avoid report of memory leak
    natsStatistics_Destroy(stats);
    natsConnection_Destroy(conn);
    natsOptions_Destroy(opts);

    // To silence reports of memory still in used with valgrind
    nats_Close();

    return 0;
}
