#include "businessb/business_b.h"

#include "money/money_api.h"
#include "oss_log/oss_api.h"

int some_businessb_1(WORLD_ACTOR *actor)
{
    int ret = -1;
    OSS_CTX ctx(OSS_OP_TYPE_BUSINESSB_1);
    ret = actor_add_money(actor, 1, 3333);
    if (ret != 0) return ret;
    ret = actor_add_money(actor, 1, 6666);
    if (ret != 0) return ret;
    return 0;
}
