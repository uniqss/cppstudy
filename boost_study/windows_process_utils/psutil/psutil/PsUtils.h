#pragma once

#include <string>

bool ProcessExistByPidFile(const std::string& pidFilePath, std::string& err);
bool ProcessExistByPathKey(const std::string& processKey, std::string& err);

int CreateDetachProcess(const char* exe_path, const char* cmd_line_args, const char* workingDir);
