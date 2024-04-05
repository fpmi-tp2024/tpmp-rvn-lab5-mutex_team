//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/date.h"

std::string parfume_bazaar::Date::toSqliteFormat() {
    return std::to_string(year) + '-' + std::to_string(month) + '-' + std::to_string(day);
}