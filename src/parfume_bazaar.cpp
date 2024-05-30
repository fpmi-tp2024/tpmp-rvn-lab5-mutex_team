//
// Created by User on 4/3/2024.
//
#include "parfume_bazaar/parfume_bazaar.h"
#include "sql/sqlite3.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace parfume_bazaar {

  
    
    void printHelloWorld() {
        sqlite3 *db;
        std::cout << "Hello, everything is ok!" << "\n";
    }

    //здесь будут находится нащи ф-ции для работы с бд (insert, update, delete и ещё пару вынесены в классы

    //Admin& как параметр показвает, что эта ф-ция вызывается тольуо админом. Makler& - что только для маклера

    //ф-ция для создания таблиц при запуске
    bool createTables(const char* db_path) {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }

        //Makler
        const char* createTableMaklerQuery = "CREATE TABLE IF NOT EXISTS Makler (id INTEGER PRIMARY KEY AUTOINCREMENT, surname TEXT UNIQUE, birth_date TEXT, address TEXT, password TEXT);";
        rc = sqlite3_exec(db, createTableMaklerQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "Error while creating table 'Makler': " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        //Item
        const char* createTableItemQuery = "CREATE TABLE IF NOT EXISTS Item (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, type TEXT, price REAL, firm_seller_name TEXT, expiration_date TEXT, amount INTEGER);";
        rc = sqlite3_exec(db, createTableItemQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "Error while creating table 'Item': " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        //Statistic
        const char* createTableStatisticQuery = "CREATE TABLE IF NOT EXISTS Statistic (id INTEGER PRIMARY KEY AUTOINCREMENT, makler_id INTEGER, item_id INTEGER, amount INTEGER, revenue REAL, FOREIGN KEY (makler_id) REFERENCES Makler(id), FOREIGN KEY (item_id) REFERENCES Item(id));";
        rc = sqlite3_exec(db, createTableStatisticQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "Error while creating table 'Statistic': " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        //Bargain
        const char* createTableBargainQuery = "CREATE TABLE IF NOT EXISTS Bargain (id INTEGER PRIMARY KEY AUTOINCREMENT, date TEXT, item_name TEXT, item_type TEXT, amount INTEGER, makler_surname TEXT, firm_buyer_name TEXT, FOREIGN KEY (item_name) REFERENCES Item(name), FOREIGN KEY (makler_surname) REFERENCES Makler(surname));";
        rc = sqlite3_exec(db, createTableBargainQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "Error while creating table 'Bargain': " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }
        
        //Admin
        const char* createTableAdminQuery = "CREATE TABLE IF NOT EXISTS Admin (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE, password TEXT);";
        rc = sqlite3_exec(db, createTableAdminQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cout << "Error while creating table 'Admin': " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        sqlite3_close(db);
        return true;
    }

    // по каждому названию товара – сведения о проданном количестве и общей стоимости
    //        за указанный период;
    //возвращает ответ в виде вектора строк вида "<название товара>:\n\t кол-во проданных: <число>шт., стоимость: <число>$"
    vector<string> getItemInfo(Admin& admin, Date startDate, Date endDate, const char* db_path) {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return std::vector <std::string> ();
        }

        std::string query = "SELECT item_name, SUM(Bargain.amount), SUM(Bargain.amount * Item.price) AS Total FROM Bargain JOIN Item ON Bargain.item_name = Item.name GROUP BY item_name;";
        
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::vector <string>();
        }

        std::vector <string> output;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            output.push_back("Name: " + string((char*)(sqlite3_column_text(stmt, 0))) + "\n\tAmount: " + string((char*)(sqlite3_column_text(stmt, 1))) + ", Total cost: " + string((char*)(sqlite3_column_text(stmt, 2))));
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
        return output;
    }

    // по каждому названию товара – перечень фирм-покупателей с указанием сведений о
    //        количестве единиц и стоимости купленного ими товара по каждой фирме-покупателю ;
    // ответ возвращается в виде массива строк вида
    // "<название товара>:\n\t<название фирмы-покупателя>: <кол-во купленных>шт., <общая стоимость купленных>$\n некст фирма \n некст фирма \n..."
    vector<string> getFirmInfoAboutItem(Admin& admin, Date startDate, Date endDate, const char* db_path) {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return std::vector <std::string>();
        }

        std::string query = "SELECT Item.name, Bargain.firm_buyer_name, SUM(Bargain.amount), SUM(Bargain.amount * Item.price) FROM Bargain JOIN Item ON Bargain.item_name = Item.name WHERE Bargain.date BETWEEN '" + startDate.toSqliteFormat() + "' AND '" + endDate.toSqliteFormat() + "' GROUP BY Item.name, Bargain.firm_buyer_name; ";

        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::vector <string>();
        }

        std::vector <string> output;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            output.push_back("Item name: " + string((char*)(sqlite3_column_text(stmt, 0))) + "\n\tFirm buyer name: " + string((char*)(sqlite3_column_text(stmt, 1))) + ", Amount: " + std::to_string((sqlite3_column_int(stmt, 2))) + ", Total cost: " + std::to_string((sqlite3_column_double(stmt, 3))) + "\n");
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
        return output;
    }

    // по виду товара, пользующемуся наибольшим спросом, – сведения о количестве и
    //        стоимости проданного товара по каждой фирме-покупателю (*);
    // ф-ция для Маклера! Возвращает пару <название типа> и массив строк вида
    // "<название фирмы-покупателя>: кол-во проданных: <число>шт., стоимость: <число>$
    // обращаю внимание, что тип товара не уникальный
    std::pair<std::string, std::vector<std::string>> getPopularItemInfo(Makler& makler, Date startDate, Date endDate, const char* db_path)
    {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return std::pair<std::string, std::vector<std::string>>();
        }

        //most popular type
        std::string query = "SELECT item_type FROM Bargain GROUP BY item_type ORDER BY COUNT(*) DESC LIMIT 1;";

        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::pair<std::string, std::vector<std::string>>();
        }
        std::string mostPopular;
        while (sqlite3_step(stmt) == SQLITE_ROW) {

            mostPopular = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);

        std::pair output = std::make_pair(mostPopular, std::vector <std::string>());

        query = "SELECT firm_buyer_name, SUM(Bargain.amount), SUM(Bargain.amount * Item.price) FROM Bargain JOIN Item ON Bargain.item_type = Item.type WHERE Bargain.item_type = '" + mostPopular + "' GROUP BY Bargain.firm_buyer_name ;";
      
        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::pair<std::string, std::vector<std::string>>();
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            output.second.push_back("Firm name: " + string((char*)(sqlite3_column_text(stmt, 0))) + " Amount: " + std::to_string(sqlite3_column_int(stmt, 1)) + " Total cost: " + std::to_string(sqlite3_column_double(stmt, 2)));
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
        return output;
    }
    // по маклеру, совершившему максимальное количество сделок, – сведения о нем и
    //        фирмах-поставщиках;
    // Если вариантов несколько, вернуть любой
    // возвращает пару <объект типа Маклер> и массив строк с названиями фирм поставщиков, с которыми он заключал сделки

 
    std::pair< Makler, std::vector<std::string> > getSuccessfulMaklerInfo(Makler& makler, Date startDate, Date endDate, const char* db_path) {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return std::pair< Makler, std::vector<std::string> >(Makler(), std::vector <std::string> ());
        }

        //most popular makler
        std::string query = "SELECT Bargain.makler_surname, Makler.birth_date, Makler.address FROM Bargain JOIN Makler ON Bargain.makler_surname = Makler.surname GROUP BY Bargain.makler_surname ORDER BY COUNT(*) DESC LIMIT 1;";

        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::pair< Makler, std::vector<std::string> >(Makler(), std::vector <std::string>());
        }

        std::string mostPopular;
        std::pair< Makler, std::vector<std::string> > output;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            mostPopular = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            output = std::make_pair(Makler(string((char*)(sqlite3_column_text(stmt, 0))), Date(string((char*)(sqlite3_column_text(stmt, 1)))), string((char*)(sqlite3_column_text(stmt, 2)))), std::vector <std::string>());
        }
        sqlite3_finalize(stmt);

        query = "SELECT firm_buyer_name FROM Bargain WHERE makler_surname = '" + mostPopular + "';";

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::pair< Makler, std::vector<std::string> >(Makler(), std::vector <std::string>());
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            output.second.push_back(string((char*)(sqlite3_column_text(stmt, 0))));
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
        return output;
    }

    // по каждой фирме-поставщику – список маклеров с указанием сведений о количестве
    //        и стоимости проданного ими товара по каждому маклеру.
    // возвращает вектор пар вида { <има фирмы-поставщика>, вектор строк вида "<имя маклера>: \n\t<кол-во>шт. <стоимость>$
    std::vector< std::pair <std::string, std::vector<std::string> > > getFirmSellerInfo(Admin& admin, const char* db_path) {
        std::vector<std::pair<std::string, std::vector<std::string>>> firmSellerInfo;

        sqlite3* db;
        int rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return firmSellerInfo;
        }

        const char* selectFirmsQuery = "SELECT DISTINCT firm_seller_name FROM Item;";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, selectFirmsQuery, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cout << "Error while preparing select query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return firmSellerInfo;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string firmName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));

            std::vector<std::string> sellerInfo;
            const char* selectSellerInfoQuery = "SELECT m.surname, s.amount, s.revenue FROM Makler m INNER JOIN Statistic s ON m.id = s.makler_id INNER JOIN Item i ON s.item_id = i.id WHERE i.firm_seller_name = ?;";
            sqlite3_stmt* stmt2;
            rc = sqlite3_prepare_v2(db, selectSellerInfoQuery, -1, &stmt2, nullptr);
            if (rc != SQLITE_OK) {
                std::cout << "Error while preparing select query: " << sqlite3_errmsg(db) << "\n";
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return firmSellerInfo;
            }

            sqlite3_bind_text(stmt2, 1, firmName.c_str(), -1, SQLITE_STATIC);

            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                std::string sellerName(reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 0)));
                int amount = sqlite3_column_int(stmt2, 1);
                double revenue = sqlite3_column_double(stmt2, 2);

                std::string sellerInfoStr = sellerName + ": " + std::to_string(amount) + "items. " + std::to_string(revenue).substr(0, to_string(revenue).size()-4) + "$";
                sellerInfo.push_back(sellerInfoStr);
            }

            sqlite3_finalize(stmt2);

            firmSellerInfo.emplace_back(firmName, sellerInfo);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return firmSellerInfo;
    }

    //3. Обеспечить с помощью операторов Insert, Update, Delete обновление информации в
    //        указанных таблицах.
    // Это всё см. cpp файлы классов

    //4. Создать функцию, которая по совершении очередной сделки обновляет по каждому
    //        маклеру сведения о количестве единиц проданного товара и сумме сделки в специальной
    //        таблице статистики(*).
    // это ф-ция void parfume_bazaar::Bargain::updateMaklersStatistics() {
    //
    //}
    // Она находится в Bargain, потому что вызывается когда мы добавляем новую сделку в таблицу

    //5. Создать функцию, которая на указанную дату выполняет обновление данных таблицы,
    //        содержащей сведения о товаре, вычитая из количества поставленных единиц количество
    //        проданных единиц до указанной даты, и удаляет соответствующие строки из таблицы
    //        заключенных сделок. В качестве параметра передать дату периода.
    bool updateItemsAmount(Admin& admin, Date& date, const char* db_path) {
        sqlite3* db;
        int rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }

        string updateItemsQuery = "UPDATE Item SET amount = COALESCE(amount, 0) - COALESCE((SELECT SUM(amount) FROM Bargain WHERE Bargain.item_name = Item.name AND Bargain.item_type = Item.type AND Bargain.date <= '"+date.toSqliteFormat() + "'), 0);";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, updateItemsQuery.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cout << "Error while preparing update query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_text(stmt, 1, date.toSqliteFormat().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date.toSqliteFormat().c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Error while executing update query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }

        sqlite3_finalize(stmt);

        string deleteBargainQuery = "DELETE FROM Bargain WHERE date <= '" + date.toSqliteFormat() + "';";
        rc = sqlite3_prepare_v2(db, deleteBargainQuery.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cout << "Error while preparing delete query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_text(stmt, 1, date.toSqliteFormat().c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Error while executing delete query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    //6.Создать функцию, которая на указанную дату выводит всю информацию о
    //        совершенных сделках(*)
    // Возвращает массив сделок конкретного малера на конкретную дату
    vector<Bargain> getBargains(Makler& makler, Date& date, const char* db_path) {
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;

        rc = sqlite3_open(db_path, &db);
        if (rc != SQLITE_OK) {
            std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return std::vector <Bargain>();
        }

        std::string query = "SELECT * FROM Bargain WHERE date = '" + date.toSqliteFormat() + "' AND makler_surname = '" + makler.getSurname() + "';";

        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return std::vector <Bargain>();
        }

        std::vector <Bargain> output;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
           
            output.push_back(Bargain(makler, Date(string((char*)(sqlite3_column_text(stmt, 6)))), string((char*)(sqlite3_column_text(stmt, 1))), string((char*)sqlite3_column_text(stmt, 2)), sqlite3_column_int(stmt, 3), string((char*)sqlite3_column_text(stmt, 5))));
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
        return output;
    }
}
