//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/makler.h"

bool parfume_bazaar::Makler::insert(parfume_bazaar::Makler &makler, std::string password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Admin': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string query = "INSERT INTO Makler (surname, birth_date, address, password) VALUES ('" + makler.getSurname() + "', '" + makler.getBirthDate().toSqliteFormat() + "', '" + makler.getAddress() + "', '" + password + "');";

    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error with INSERT query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool parfume_bazaar::Makler::update(parfume_bazaar::Makler& makler, std::string old_password, std::string new_password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Makler': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "UPDATE Makler SET password = '" + new_password + "' WHERE surname = '" + makler.getSurname() + "' AND password = '" + old_password + "';";
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
            std::cout << "Incorrect surname or password." << "\n";
            sqlite3_close(db);
            return false;
        }
    }
    sqlite3_close(db);
    return true;
}

bool parfume_bazaar::Makler::remove(Makler& makler, std::string password, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Makler': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "SELECT id FROM Makler WHERE surname = '" + makler.getSurname() + "';";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    int makler_id = -1;
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        makler_id = sqlite3_column_int(stmt, 0);
    }
    if (makler_id < 0) {
        std::cout << "No such makler" << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);

    sql = "DELETE FROM Makler WHERE surname = '" + makler.getSurname() + "' AND password = '" + password + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else {
        int rowsChanged = sqlite3_changes(db);
        if (rowsChanged > 0) {
            std::cout << "Makler was deleted successfully! " << "\n";
        }
        else {
            std::cout << "Incorrect surname or password." << "\n";
            return false;
        }
    }

    sql = "DELETE FROM Statistic WHERE makler_id = '" + std::to_string(makler_id) + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << "\n";
    }
    else {
        int rowsChanged = sqlite3_changes(db);
        if (rowsChanged > 0) {
            std::cout << "Statistic updated successfully! " << "\n";
        }
        else {
            std::cout << "No such makler in Statistic" << "\n";
        }
    }

    sql = "DELETE FROM Bargain WHERE makler_surname = '" + makler.getSurname() + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << "\n";
    }
    else {
        int rowsChanged = sqlite3_changes(db);
        if (rowsChanged > 0) {
            std::cout << "Bargain updated successfully! " << "\n";
        }
        else {
            std::cout << "No such makler in Bargain" << "\n";
        }
    }

    sqlite3_close(db);
    return true;
}
