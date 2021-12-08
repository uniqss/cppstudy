
#include "nats_helper.h"

bool async = true;

NetMsg recvMsg;
char recv_buff[NET_SEND_RECV_BODY_MAX_SIZE] = {0};
void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure) {
    // printf("onMsg\n");
    if (print) {
        printf("%lu Received subject:%s len:%d \n",
               getThreadTid(), natsMsg_GetSubject(msg), natsMsg_GetDataLength(msg));
    }

    NetMsg *nmsg = (NetMsg *)natsMsg_GetData(msg);
    int bodyLen = nmsg->head.bodyWithoutHeadLen;
    memcpy(&recvMsg, nmsg, nmsg->head.bodyWithoutHeadLen + NET_MSG_HEAD_SIZE);
    recv_buff[bodyLen] = '\0';
    if (print) {
        printf("%lu bodyWithoutHeadLen:%d from:%d to:%d body:%s \n",
               getThreadTid(), bodyLen, (int)nmsg->head.from, (int)nmsg->head.to, recv_buff);
    }

    if (start == 0)
        start = nats_Now();

    // We should be using a mutex to protect those variables since
    // they are used from the subscription's delivery and the main
    // threads. For demo purposes, this is fine.
    if (++count == total)
        elapsed = nats_Now() - start;

    natsMsg_Destroy(msg);
}

void asyncCb(natsConnection *nc, natsSubscription *sub, natsStatus err, void *closure) {
    if (print)
        printf("%lu Async error: %d - %s\n", getThreadTid(), err, natsStatus_GetText(err));

    natsSubscription_GetDropped(sub, (int64_t *)&dropped);
}

int main() {
    natsConnection *conn = NULL;
    natsOptions *opts = NULL;
    natsSubscription *sub = NULL;
    natsStatistics *stats = NULL;
    natsStatus s;

    s = constructOptions(opts, {NATS_DEFAULT_URL});
    if (s != NATS_OK) {
        return -1;
    }

    printf("%lu Listening %ssynchronously on '%s'.\n",
           getThreadTid(), (async ? "a" : ""), subj);

    do {
        s = natsOptions_SetErrorHandler(opts, asyncCb, NULL);
        if (s != NATS_OK) {
            break;
        }

        s = natsConnection_Connect(&conn, opts);
        if (s != NATS_OK) {
            break;
        }

        s = natsConnection_Subscribe(&sub, conn, subj, onMsg, NULL);
        if (s != NATS_OK) {
            break;
        }

        // For maximum performance, set no limit on the number of pending messages.
        s = natsSubscription_SetPendingLimits(sub, -1, -1);
        if (s != NATS_OK) {
            break;
        }

        s = natsSubscription_AutoUnsubscribe(sub, (int)total);
        if (s != NATS_OK) {
            break;
        }

        s = natsStatistics_Create(&stats);
        if (s != NATS_OK) {
            break;
        }

        while (s == NATS_OK) {
            s = printStats(STATS_IN | STATS_COUNT, conn, sub, stats);

            if (count + dropped == total)
                break;

            if (s == NATS_OK)
                nats_Sleep(1000);
        }
    } while (false);

    if (s == NATS_OK) {
        printStats(STATS_IN | STATS_COUNT, conn, sub, stats);
        printPerf("Received");
    } else {
        printf("Error: %d - %s\n", s, natsStatus_GetText(s));
        nats_PrintLastErrorStack(stderr);
    }

    // Destroy all our objects to avoid report of memory leak
    natsStatistics_Destroy(stats);
    natsSubscription_Destroy(sub);
    natsConnection_Destroy(conn);
    natsOptions_Destroy(opts);

    // To silence reports of memory still in used with valgrind
    nats_Close();

    return 0;
}
