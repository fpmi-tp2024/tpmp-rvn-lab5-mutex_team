//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_BARGAIN_H
#define PARFUME_BAZAAR_PROJECT_BARGAIN_H

#pragma once
#include <string>
#include <vector>
#include "parfume_bazaar/date.h"
#include "parfume_bazaar/makler.h"
#include "sql/sqlite3.h"

namespace parfume_bazaar {
    class Bargain {
    private:
        Date date;
        std::string itemName;
        std::string itemType;
        int amount;
        std::string maklerSurname;
        std::string firmBuyerName;
    public:
        Bargain(Makler& makler, Date date, std::string itemName, std::string itemType, int amount, std::string firmBuyerName)
                : date(date), itemName(itemName), itemType(itemType), amount(amount), maklerSurname(makler.getSurname()), firmBuyerName(firmBuyerName) {
        }

        Date getDate() const {
            return date;
        }

        std::string getItemName() const {
            return itemName;
        }

        std::string getItemType() const {
            return itemType;
        }

        int getAmount() const {
            return amount;
        }

        std::string getMaklerSurname() const {
            return maklerSurname;
        }

        std::string getFirmBuyerName() const {
            return firmBuyerName;
        }

        //используется в ф-ции insert, обновляет таблицу статистики после внесения новой сделки в бд
        static bool updateMaklersStatistics(Makler& makler, Bargain& bargain, sqlite3* db);

        //будет вызываться в UI при проведении маклером новой сделки
        //Перед тем, как добавить в таблицу проверяет, есть ли такой товар с таким типом и хватает ли его по кол-ву
        static bool insert(Makler& makler, Bargain& bargain, const char* db_path = "./parfume.db");

        //обновление сделки не будет, потому что это ломает суть уже обработанной статистики

        //будет вызываться при удалении сделок до некоторой даты для определенного маклера (используется в ф-ции в пункте 5)
        //что-то типо DELETE ... WHERE date<=bargain.date and makler.name == makler_name
        static bool removeBeforeDate(Makler& makler, Date& date, const char* db_path = "./parfume.db");
    };
}

#endif //PARFUME_BAZAAR_PROJECT_BARGAIN_H
