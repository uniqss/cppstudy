#include "sqlite3_helper.h"

#include "sqlite3.h"


int Sqlite3Helper::GetAllData(const std::string& dbFileName, const std::string& dbTableName,
                              std::vector<std::string>* columnNames, std::vector<std::vector<std::string>>* datas,
                              std::string* errMsg) {
    int ret = 0;
    sqlite3* db = nullptr;
    int bytes = 0;
    sqlite3_stmt* stmt = nullptr;

    do {
        ret = sqlite3_open(dbFileName.c_str(), &db);
        if (ret != SQLITE_OK) break;

        std::string sql = "SELECT * FROM " + dbTableName;
        ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (ret != SQLITE_OK) break;

        int cols = sqlite3_column_count(stmt);
        if (columnNames) {
            for (int i = 0; i < cols; ++i) {
                columnNames->push_back(sqlite3_column_name(stmt, i));
            }
        }

        if (datas) {
            int row = 0;

            ret = sqlite3_step(stmt);
            while (ret == SQLITE_ROW) {
                std::vector<std::string> row_data;
                for (int i = 0; i < cols; ++i) {
                    // bytes = sqlite3_column_bytes(stmt, i);
                    char* column = (char*)sqlite3_column_blob(stmt, i);
                    row_data.push_back(column);
                }
                datas->emplace_back(row_data);

                ret = sqlite3_step(stmt);
                if (ret == SQLITE_DONE) break;
            }

            ret = 0;
        }
    } while (false);


    if (ret != 0) {
        if (errMsg) *errMsg = sqlite3_errmsg(db);
    }

    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);

    return ret;
}

int Sqlite3Helper::ExecSql(const std::string& dbFileName, const std::string& sql,
                           std::vector<std::vector<std::string>>& datas, std::string& errMsg) {
    int ret = 0;
    sqlite3* db = nullptr;
    int bytes = 0;
    sqlite3_stmt* stmt = nullptr;

    do {
        ret = sqlite3_open(dbFileName.c_str(), &db);
        if (ret != SQLITE_OK) break;

        ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (ret != SQLITE_OK) break;

        int cols = sqlite3_column_count(stmt);

        int row = 0;

        ret = sqlite3_step(stmt);
        while (ret == SQLITE_ROW) {
            std::vector<std::string> row_data;
            for (int i = 0; i < cols; ++i) {
                // bytes = sqlite3_column_bytes(stmt, i);
                char* column = (char*)sqlite3_column_blob(stmt, i);
                row_data.push_back(column);
            }
            datas.emplace_back(row_data);

            ret = sqlite3_step(stmt);
            if (ret == SQLITE_DONE) break;
        }

        ret = 0;
    } while (false);


    if (ret != 0) {
        errMsg = sqlite3_errmsg(db);
    }

    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);

    return ret;
}
