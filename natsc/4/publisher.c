
#include <iostream>
#include <string>

#include "examples.h"
#include "utils.h"

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
    while (true) {
        std::cin >> input;
        if (input == "e" || input == "exit") break;
        dataLen = input.size();
        s = natsConnection_Publish(conn, subj, (const void *)input.c_str(), dataLen);
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
