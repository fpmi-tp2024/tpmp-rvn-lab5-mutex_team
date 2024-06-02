//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_ITEM_H
#define PARFUME_BAZAAR_PROJECT_ITEM_H

#pragma once
#include <string>
#include <vector>
#include "parfume_bazaar/date.h"
#include "parfume_bazaar/admin.h"
#include "sql/sqlite3.h"

namespace parfume_bazaar {
    class Item {
    private:
        std::string name; //уникальное в таблице
        std::string type;
        double price;
        std::string firmSellerName;
        Date expirationDate;
        int amount;
    public:
        explicit Item(std::string name, std::string type, double price, std::string firmSellerName, Date expirationDate, int amount)
                : name(name), type(type), price(price), firmSellerName(firmSellerName), expirationDate(expirationDate), amount(amount) {
        }

        std::string getName() const {
            return name;
        }

        std::string getType() const {
            return type;
        }

        double getPrice() const {
            return price;
        }

        std::string getFirmSellerName() const {
            return firmSellerName;
        }

        Date getExpirationDate() const {
            return expirationDate;
        }

        int getAmount() const {
            return amount;
        }

        //будет вызываться в UI при регистрации админом нового товара
        static bool insert(Admin& admin, Item& item, const char* db_path = "./parfume.db");

        //будет вызываться в UI при обновлении админом товара (используется в пункте 5)
        //что-то типо SET ... WHERE itemName==item.name
        static bool update(Admin& admin, std::string itemName, int amount, const char* db_path = "./parfume.db");

        //будет вызываться в UI при удалении админом товара (соответственно удалить все записи из
        // других таблиц, в которых есть ключ на этот товар)
        //что-то типо DELETE ... WHERE name==item.name
        static bool remove(Admin& admin, std::string itemName, const char* db_path = "./parfume.db");
    };
}

#endif //PARFUME_BAZAAR_PROJECT_ITEM_H
