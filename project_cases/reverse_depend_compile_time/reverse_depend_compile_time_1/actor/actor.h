#pragma once

#include <string>
#include <vector>
#include <string.h>
#include "defs/money_def.h"

class WORLD_ACTOR
{
  public:
    int uid;
    std::string name;
    int money[MONEY_TYPE_MAX];

  public:
    WORLD_ACTOR() : uid(0) { memset(money, 0, sizeof(money)); }
};
