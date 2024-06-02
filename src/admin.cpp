//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/admin.h"

bool parfume_bazaar::Admin::insert(parfume_bazaar::Admin &admin, std::string password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Admin': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string query = "INSERT INTO Admin (login, password) VALUES ('" + admin.getLogin() + "', '" + password + "');";

    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with INSERT query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

bool parfume_bazaar::Admin::update(parfume_bazaar::Admin &admin, std::string old_password, std::string new_password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Admin': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "UPDATE Admin SET password = '" + new_password + "' WHERE login = '" + admin.getLogin() + "' AND password = '" + old_password + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with UPDATE query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }
    else {
        int rowsChanged = sqlite3_changes(db);
        if (rowsChanged > 0) {
            std::cout << "Password was updated successfully!" << "\n";
        }
        else {
            std::cout << "Incorrect login or password." << "\n";
            sqlite3_close(db);
            return false;
        }
    }

    sqlite3_close(db);
    return true;

}

bool parfume_bazaar::Admin::remove(parfume_bazaar::Admin &admin, std::string password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Admin': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "DELETE FROM Admin WHERE login = '" + admin.getLogin() + "' AND password = '" + password + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }
    else {
        int rowsChanged = sqlite3_changes(db);
        if (rowsChanged > 0) {
            std::cout << "Account was deleted successfully! " << "\n";
        }
        else {
            std::cout << "Incorrect login or password." << "\n";
            sqlite3_close(db);
            return false;
        }
    }

    sqlite3_close(db);
    return true;
}