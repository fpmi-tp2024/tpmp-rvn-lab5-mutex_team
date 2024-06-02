//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/item.h"

bool parfume_bazaar::Item::insert(parfume_bazaar::Admin &admin, parfume_bazaar::Item &item, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Admin': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string query = "INSERT INTO Item (name, type, price, firm_seller_name, expiration_date, amount) VALUES ('" + item.getName() + "', '" + item.getType() + "', '" + std::to_string(item.getPrice()) + "', '" + item.getFirmSellerName() + "', '" + item.getExpirationDate().toSqliteFormat() + "', '" + std::to_string(item.getAmount()) + "');";

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

bool parfume_bazaar::Item::update(parfume_bazaar::Admin &admin, std::string itemName, int amount, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Item': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "SELECT * FROM Item WHERE name = '" + itemName + "';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    else {
     

        int rowCount = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            rowCount++;
        }

        if (rowCount > 0) {
            sql = "UPDATE Item SET amount = '" + std::to_string(amount) + "' WHERE name = '" + itemName + "';";

        }
        else {
            std::cout << "No such item. Check item name" << "\n";
            return false;
        }

        sqlite3_exec(db, sql.c_str(), 0, 0, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Error with UPDATE query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }
        else {
            std::cout << "Item was updated successfully!" << "\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
    }

    return true;
}

bool parfume_bazaar::Item::remove(parfume_bazaar::Admin &admin, std::string itemName, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Item': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "DELETE FROM Item WHERE name = '" + itemName + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else if (sqlite3_changes(db) == 0) {
        std::cout << "No such item. Check item name." << std::endl;
        sqlite3_close(db);
        return false;
    }
    else {
        std::cout << "Entries successfully deleted." << std::endl;
    }

    return true;

}