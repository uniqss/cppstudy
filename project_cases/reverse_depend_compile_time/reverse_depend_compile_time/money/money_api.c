#include <assert.h>

#include <iostream>

#include "money/money_api.h"

#include "oss_log/oss_api.h"
#include "actor/actor.h"

int actor_add_money(WORLD_ACTOR *actor, int money_type, int add_num)
{
    int op_type = oss_ctx_top();
    if (op_type == -1)
    {
        assert(false);
        return -1;
    }

    // do oss log
    std::cout << "oss log. uid:" << actor->uid << " money_type:" << money_type << " add_num:" << add_num << std::endl;

    actor->money[money_type] += add_num;

    return 0;
}
