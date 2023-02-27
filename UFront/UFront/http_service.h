#pragma once

#include <functional>
#include <string>

bool StartHttpService(std::function<void()> exitCb);
int CreateDetachProcess(const std::string& exe_path, const std::string& cmd_line_args, const std::string& workingDir);
