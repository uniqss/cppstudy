#include <iostream>

#include "businessa/business_a.h"
#include "businessb/business_b.h"
#include "businessc/business_c.h"
#include "actor/actor.h"

int main()
{
    WORLD_ACTOR actor;
    actor.uid = 1111;
    some_businessa_1(&actor);
    some_businessb_1(&actor);
    some_businessc_1(&actor);
    return 0;
}