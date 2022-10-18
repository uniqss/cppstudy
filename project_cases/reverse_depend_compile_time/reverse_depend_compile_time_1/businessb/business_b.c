#include "businessb/business_b.h"

#include "money/money_api.h"
#include "defs/op_type_def.h"

int some_businessb_1(WORLD_ACTOR *actor)
{
    int ret = -1;
    ret = actor_add_money(actor, 1, 3333, OP_TYPE_BUSINESSB_1);
    if (ret != 0) return ret;
    ret = actor_add_money(actor, 1, 6666, OP_TYPE_BUSINESSB_1);
    if (ret != 0) return ret;
    return 0;
}
