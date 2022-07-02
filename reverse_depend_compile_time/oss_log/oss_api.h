#pragma once

#include <stack>

#include "defs/oss_def.h"

class OSS_FRAME
{
  public:
    OSS_OP_TYPES op_type;
    OSS_FRAME(OSS_OP_TYPES optype) : op_type(optype) {}
};

extern std::stack<OSS_FRAME> g_frames;

class OSS_CTX
{

  public:
    OSS_CTX(OSS_OP_TYPES op_type)
    {
        OSS_FRAME frame(op_type);
        g_frames.push(frame);
    }
    ~OSS_CTX()
    {
        g_frames.pop();
    }
};

int oss_ctx_top();
