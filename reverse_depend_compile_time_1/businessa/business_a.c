#include "businessa/business_a.h"

#include "money/money_api.h"
#include "defs/op_type_def.h"

int some_businessa_1(WORLD_ACTOR *actor)
{
    int ret = -1;
    ret = actor_add_money(actor, 1, 1234, OP_TYPE_BUSINESSA_1);
    if (ret != 0) return ret;
    ret = actor_add_money(actor, 1, 5678, OP_TYPE_BUSINESSA_1);
    if (ret != 0) return ret;
    return 0;
}
