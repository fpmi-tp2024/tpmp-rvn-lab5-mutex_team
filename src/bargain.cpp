//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/bargain.h"


// теперь принимает базу а не путь, иначе крашится при вызове из insert
bool parfume_bazaar::Bargain::updateMaklersStatistics(parfume_bazaar::Makler& makler, parfume_bazaar::Bargain& bargain, sqlite3* mydb) {
    sqlite3* db = mydb;
    char* errMsg = nullptr;
    int rc;


    std::string sql = "SELECT Statistic.* FROM Statistic JOIN Makler ON makler_id = Makler.id JOIN Item ON item_id = Item.id WHERE Makler.surname = '" + makler.getSurname() + "' AND  Item.name = '" + bargain.getItemName() + "';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
     
        return false;
    }
    else {

        int rowCount = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            rowCount++;
        }
        
        if (rowCount > 0) {
            // запрос не работал, переписала на новый
            sql = "UPDATE Statistic SET amount = amount + " + std::to_string(bargain.getAmount())  + " WHERE makler_id = (SELECT id FROM Makler WHERE surname = '" + makler.getSurname() + "'); " +
            "UPDATE Statistic SET  revenue = revenue + " + std::to_string(bargain.getAmount()) + " * (SELECT price FROM Item WHERE Item.name = '" + bargain.getItemName() + "') WHERE makler_id = (SELECT id FROM Makler WHERE surname = '" + makler.getSurname() + "'); ";


        }
        else {
          
           sql =  "INSERT INTO Statistic (makler_id, item_id, amount, revenue) VALUES ((SELECT id from Makler WHERE Makler.surname = '" + bargain.getMaklerSurname() + "'), (SELECT id from Item WHERE Item.name = '" + bargain.getItemName() + "'), '" + std::to_string(bargain.getAmount()) + "', (" + std::to_string(bargain.getAmount()) + " * (SELECT price FROM Item WHERE Item.name = '" + bargain.getItemName() + "')));";
         
        }

        sqlite3_exec(db, sql.c_str(), 0, 0, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Error with UPDATE query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_finalize(stmt);
           
            return false;
        }
        else if (sqlite3_changes(db) == 0) {
            std::cout << "No such makler or item." << std::endl;
           
            return false;
        }
        else {
            std::cout << "Statistic was updated successfully!" << "\n";
            sqlite3_finalize(stmt);
           
        }
    }

    return true;
}

bool parfume_bazaar::Bargain::insert(parfume_bazaar::Makler &makler, parfume_bazaar::Bargain &bargain, const char* db_path) {
   
	std::string name = bargain.getItemName();
	int amount = bargain.getAmount();

    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }
  


    std::string query = "SELECT amount FROM Item WHERE Item.name='"  + name + "';";

    sqlite3_stmt* stmt;


    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
   
    if (rc != SQLITE_OK) {
        std::cout << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    while (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        std::cout << "No such item" << "\n";
        sqlite3_close(db);
        return false;
    }

    int availableAmount = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        availableAmount = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (availableAmount < amount) {
        std::cout << "Insufficient quantity of item: " << name << "\n";
        sqlite3_close(db);
        return false;
    }

   
    query = "INSERT INTO Bargain (date, item_name, item_type, amount, makler_surname, firm_buyer_name) VALUES ('" + bargain.getDate().toSqliteFormat() + "', '" + bargain.getItemName() + "', '" + bargain.getItemType() + "', '" + std::to_string(bargain.getAmount()) + "', '" + bargain.getMaklerSurname() + "', '" + bargain.getFirmBuyerName() + "');";


    //раньше было sqlite3_prepare_v2 но с ним летел файл с тестами
    sqlite3_exec(db, query.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cout << "Error with INSERT query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

     


    bool ret = updateMaklersStatistics(makler, bargain, db);   
   
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

bool parfume_bazaar::Bargain::removeBeforeDate(parfume_bazaar::Makler &makler, parfume_bazaar::Date &date, const char* db_path) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cout << "Unable to open or create database 'Bargain': " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    std::string sql = "DELETE FROM Bargain WHERE date < '" + date.toSqliteFormat() + "';";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Error with DELETE query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    else {
        std::cout << "Entries successfully deleted." << std::endl;
    }

    sqlite3_close(db);
    return true;
}