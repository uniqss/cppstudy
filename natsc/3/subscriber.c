
#include "utils.h"

#include "examples.h"

bool async = true;

void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure) {
    if (print)
        printf("%lu Received msg: %s - %.*s\n",
               getThreadTid(), natsMsg_GetSubject(msg), natsMsg_GetDataLength(msg), natsMsg_GetData(msg));

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
    natsMsg *msg = NULL;
    natsStatus s;

    s = constructOptions(opts);
    if (s != NATS_OK) {
        return -1;
    }

    printf("%lu Listening %ssynchronously on '%s'.\n",
           getThreadTid(), (async ? "a" : ""), subj);

    s = natsOptions_SetErrorHandler(opts, asyncCb, NULL);

    if (s == NATS_OK)
        s = natsConnection_Connect(&conn, opts);

    if (s == NATS_OK) {
        if (async)
            s = natsConnection_Subscribe(&sub, conn, subj, onMsg, NULL);
        else
            s = natsConnection_SubscribeSync(&sub, conn, subj);
    }

    // For maximum performance, set no limit on the number of pending messages.
    if (s == NATS_OK)
        s = natsSubscription_SetPendingLimits(sub, -1, -1);

    if (s == NATS_OK)
        s = natsSubscription_AutoUnsubscribe(sub, (int)total);

    if (s == NATS_OK)
        s = natsStatistics_Create(&stats);

    if ((s == NATS_OK) && async) {
        while (s == NATS_OK) {
            s = printStats(STATS_IN | STATS_COUNT, conn, sub, stats);

            if (count + dropped == total)
                break;

            if (s == NATS_OK)
                nats_Sleep(1000);
        }
    } else if (s == NATS_OK) {
        int64_t last = 0;

        for (count = 0; (s == NATS_OK) && (count < total); count++) {
            s = natsSubscription_NextMsg(&msg, sub, 10000);
            if (s != NATS_OK)
                break;

            if (start == 0)
                start = nats_Now();

            if (nats_Now() - last >= 1000) {
                s = printStats(STATS_IN | STATS_COUNT, conn, sub, stats);
                last = nats_Now();
            }

            natsMsg_Destroy(msg);
        }
    }

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
