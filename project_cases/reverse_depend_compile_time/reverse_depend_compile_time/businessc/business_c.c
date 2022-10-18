#include "businessc/business_c.h"

#include "money/money_api.h"
#include "oss_log/oss_api.h"

int some_businessc_1(WORLD_ACTOR *actor)
{
    int ret = -1;

    // some monkey forget to write this line. and we want to detect this at compile time, not runtime.
    // OSS_CTX ctx(OSS_OP_TYPE_BUSINESSC_1);

    ret = actor_add_money(actor, 1, 3333);
    if (ret != 0) return ret;

    ret = actor_add_money(actor, 1, 6666);
    if (ret != 0) return ret;

    return 0;
}
