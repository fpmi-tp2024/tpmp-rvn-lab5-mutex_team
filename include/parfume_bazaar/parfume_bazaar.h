//
// Created by User on 4/3/2024.
//

#ifndef TPMP_RVN_LAB5_MUTEX_TEAM_PARFUME_BAZAAR_H
#define TPMP_RVN_LAB5_MUTEX_TEAM_PARFUME_BAZAAR_H

#pragma once
#include "parfume_bazaar/admin.h"
#include "parfume_bazaar/bargain.h"
#include "parfume_bazaar/item.h"
#include "sql/sqlite3.h"

namespace parfume_bazaar {

    void printHelloWorld();
    bool createTables(const char* db_path = "./parfume.db");

    std::vector<std::string> getItemInfo(Admin& admin, Date startDate, Date endDate, const char* db_path = "./parfume.db");
    std::vector<std::string> getFirmInfoAboutItem(Admin& admin, Date startDate, Date endDate, const char* db_path = "./parfume.db");
    std::vector< std::pair <std::string, std::vector<std::string> > > getFirmSellerInfo(Admin& admin, const char* db_path = "./parfume.db");
    bool updateItemsAmount(Admin& admin, Date& date, const char* db_path = "./parfume.db");

    std::pair< std::string, std::vector<std::string> > getPopularItemInfo(Makler& makler, Date startDate, Date endDate, const char* db_path = "./parfume.db");
    std::pair< Makler, std::vector<std::string> > getSuccessfulMaklerInfo(Makler& makler, Date startDate, Date endDate, const char* db_path = "./parfume.db");
    std::vector<Bargain> getBargains(Makler& makler, Date& date, const char* db_path = "./parfume.db");
}

#endif //TPMP_RVN_LAB5_MUTEX_TEAM_PARFUME_BAZAAR_H
