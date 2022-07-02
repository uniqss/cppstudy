#include <assert.h>

#include <iostream>

#include "money/money_api.h"

#include "oss_log/oss_api.h"
#include "actor/actor.h"

int actor_add_money(WORLD_ACTOR *actor, int money_type, int add_num, int op_type)
{
    if (op_type < 0 || op_type > OP_TYPE_MAX)
    {
        assert(false);
        return -1;
    }

    // do oss log
    std::cout << "oss log. uid:" << actor->uid << " money_type:" << money_type << " add_num:" << add_num << " op_type:" << op_type << std::endl;

    actor->money[money_type] += add_num;

    return 0;
}
