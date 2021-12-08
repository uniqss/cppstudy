
#include "examples.h"

int main() {
    natsConnection *conn = NULL;
    natsStatistics *stats = NULL;
    natsOptions *opts = NULL;
    natsStatus s;
    int dataLen = 0;

    s = constructOptions(opts);
    if (s != NATS_OK) {
        return -1;
    }

    printf("Sending %" PRId64 " messages to subject '%s'\n", total, subj);

    s = natsConnection_Connect(&conn, opts);

    if (s == NATS_OK)
        s = natsStatistics_Create(&stats);

    if (s == NATS_OK)
        start = nats_Now();

    dataLen = (int)strlen(payload);
    for (count = 0; (s == NATS_OK) && (count < total); count++)
        s = natsConnection_Publish(conn, subj, (const void *)payload, dataLen);

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
