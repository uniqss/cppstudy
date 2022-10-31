#pragma once

#include <vector>
#include <string>

class Sqlite3Helper {
   public:
    int GetAllData(const std::string& dbFileName, const std::string& dbTableName, std::vector<std::string>* columnNames,
                   std::vector<std::vector<std::string>>* datas, std::string* errMsg);
    int ExecSql(const std::string& dbFileName, const std::string& sql, std::vector<std::vector<std::string>>& datas,
                std::string& errMsg);
};
