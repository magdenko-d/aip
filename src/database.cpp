
#include "database.h"
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <utility>

Database::Database(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }


    const char* createTablesSQL = 
        "CREATE TABLE IF NOT EXISTS caesar_cipher ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "word TEXT NOT NULL UNIQUE);"
        
        "CREATE TABLE IF NOT EXISTS affine_cipher ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "word TEXT NOT NULL UNIQUE);"
        
        "CREATE TABLE IF NOT EXISTS vigenere_cipher ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "word TEXT NOT NULL UNIQUE);";
    
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTablesSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = "Failed to create tables: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }

    fillTestDataIfEmpty();
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::checkError(int rc, const char* error_msg) {
    if (rc != SQLITE_OK) {
        std::string msg = error_msg + std::string(": ") + sqlite3_errmsg(db);
        throw std::runtime_error(msg);
    }
}

void Database::fillTestDataIfEmpty() {
    const std::vector<std::pair<std::string, std::vector<std::string>>> tables = {
        {"caesar_cipher", {"codinginc++makesyouaversatiledevelopertoday.",
             "the quick brown fox jumps", "give five", "programming", "consequences"}},
        {"affine_cipher", {"i love to eat pizza and movie", "mymomcooksgoodfoodforuseveryday",
             "he runs fast", "еhesunshinesbright", "fig", "education"}},
        {"vigenere_cipher", {"iliveinasmalltownwithmyfamily",
             "thesunrisesearlybirdsstartsingingpeoplewakeup",
             "every morning I wake up early, drink fresh coffee"}}
    };

    char* errMsg = nullptr;
    for (const auto& [tableName, words] : tables) {
        std::string checkSql = "SELECT COUNT(*) FROM " + tableName + ";";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) continue;

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            std::string insertSql = "INSERT INTO " + tableName + " (word) VALUES ";
            for (size_t i = 0; i < words.size(); ++i) {
                if (i > 0) insertSql += ", ";
                insertSql += "('" + words[i] + "')";
            }
            insertSql += ";";

            rc = sqlite3_exec(db, insertSql.c_str(), nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                sqlite3_free(errMsg);
            }
        }
        sqlite3_finalize(stmt);
    }
}

std::string Database::getRandomWord(const std::string& table_name) {
    std::string sql = "SELECT word FROM " + table_name + " ORDER BY RANDOM() LIMIT 1;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    checkError(rc, "Failed to prepare statement");
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("No words found in table " + table_name);
    }
    
    std::string word = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    
    return word;
}