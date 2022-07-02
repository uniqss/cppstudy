#include "oss_log/oss_api.h"

std::stack<OSS_FRAME> g_frames;

int oss_ctx_top()
{
    if (g_frames.empty()) return -1;
    return g_frames.top().op_type;
}
