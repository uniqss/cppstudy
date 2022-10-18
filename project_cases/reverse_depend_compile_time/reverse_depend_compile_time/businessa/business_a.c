#include "businessa/business_a.h"

#include "money/money_api.h"
#include "oss_log/oss_api.h"

int some_businessa_1(WORLD_ACTOR *actor)
{
    int ret = -1;
    OSS_CTX ctx(OSS_OP_TYPE_BUSINESSA_1);
    ret = actor_add_money(actor, 1, 1234);
    if (ret != 0) return ret;
    ret = actor_add_money(actor, 1, 5678);
    if (ret != 0) return ret;
    return 0;
}
