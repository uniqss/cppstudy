#include "businessc/business_c.h"

#include "money/money_api.h"
#include "defs/op_type_def.h"

int some_businessc_1(WORLD_ACTOR *actor)
{
    int ret = -1;

    // now the monkey will never forget the op_type, because it will cause a compile-time error.
    ret = actor_add_money(actor, 1, 3333, OP_TYPE_BUSINESSC_1);
    if (ret != 0) return ret;

    // and the monkey can use different op_type to distinguish OP_TYPE_BUSINESSC_1 from OP_TYPE_BUSINESSC_2
    ret = actor_add_money(actor, 1, 6666, OP_TYPE_BUSINESSC_2);
    if (ret != 0) return ret;

    return 0;
}
