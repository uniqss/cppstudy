
#include <iostream>
#include <string>

#include "nats_helper.h"

void AddPressureTestMsg(std::vector<NetMsg> &pressureTestMsgs, const char *body) {
    pressureTestMsgs.resize(pressureTestMsgs.size() + 1);
    NetMsg& msg = pressureTestMsgs[pressureTestMsgs.size() - 1];
    
    // msg.head.bodyWithoutHeadLen = 0;
    msg.head.from = EServerType_Gate;
    msg.head.to = EServerType_Game;
    
    int dataLen = strlen(body);
    msg.head.bodyWithoutHeadLen = dataLen;
    memcpy(msg.body, body, dataLen);
}

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

    std::vector<NetMsg> pressureTestMsgs;
    AddPressureTestMsg(pressureTestMsgs, "On an evening in the latter part of May a middle-aged man was walking homeward from Shaston to the village of Marlott, in the adjoining Vale");
    AddPressureTestMsg(pressureTestMsgs, "of Blakemore, or Blackmoor. The pair of legs that carried him were rickety, and there was a bias in his gait which inclined him somewhat");
    AddPressureTestMsg(pressureTestMsgs, "to the left of a straight line. He occasionally gave a smart nod, as if in confirmation of some opinion, though he was not thinking of anything");
    AddPressureTestMsg(pressureTestMsgs, "in particular. An empty egg-basket was slung upon his arm, the nap of his hat was ruffled, a patch being quite worn away at its brim where");
    AddPressureTestMsg(pressureTestMsgs, "his thumb came in taking it off. Presently he was met by an elderly");
    AddPressureTestMsg(pressureTestMsgs, "parson astride on a gray mare, who, as he rode, hummed a wandering");
    AddPressureTestMsg(pressureTestMsgs, "tune.");
    AddPressureTestMsg(pressureTestMsgs, "“Good night t’ee,” said the man with the basket.");
    AddPressureTestMsg(pressureTestMsgs, "“Good night, Sir John,” said the parson.");
    AddPressureTestMsg(pressureTestMsgs, "The pedestrian, after another pace or two, halted, and turned round.");
    AddPressureTestMsg(pressureTestMsgs, "“Now, sir, begging your pardon; we met last market-day on this road");
    AddPressureTestMsg(pressureTestMsgs, "about this time, and I said ‘Good night,’ and you made reply ‘_Good");
    AddPressureTestMsg(pressureTestMsgs, "night, Sir John_,’ as now.”");
    AddPressureTestMsg(pressureTestMsgs, "“I did,” said the parson.");
    AddPressureTestMsg(pressureTestMsgs, "“It was only my whim,” he said; and, after a moment’s hesitation: “It was on account of a discovery I made some little time ago, whilst I was hunting up pedigrees for the new county history. I am Parson Tringham,");

    char send_buff[NET_SEND_RECV_BODY_MAX_SIZE] = {0};
    int pmsgLen = pressureTestMsgs.size();
    while (true) {
        NetMsg& msg = pressureTestMsgs[(++count)%pmsgLen];
        int totalLen = msg.head.bodyWithoutHeadLen + sizeof(NetMsgHead);
        memcpy(msg.body, send_buff, dataLen);
        s = natsConnection_Publish(conn, subj, (const void *)&msg, totalLen);
        if (s != NATS_OK) {
            break;
        }
        // if (s == NATS_OK)
        //     s = natsConnection_FlushTimeout(conn, 1000);
        if (count >= total) {
            break;
        }
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
