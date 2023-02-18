#pragma once

#include <string>

bool ProcessExistByPidFile(const std::string& pidFilePath, std::string& err);
bool ProcessExistByPathKey(const std::string& processKey, std::string& err);
