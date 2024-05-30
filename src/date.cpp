//
// Created by User on 4/5/2024.
//
#include "parfume_bazaar/date.h"
#include <iostream>

bool parfume_bazaar::Date::checkDate(int year, int month, int day) {
    if (year > 2024 || year < 1900) return false;
    if (month > 12 || month < 1) return false;
    bool isLeap = (year % 100 == 0 && (year / 100) % 4 == 0) || (year % 100 != 0 && year % 4 == 0);
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (day < 0 || day>31) return false;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day < 0 || day>30) return false;
        break;
    case 2:


        if (!isLeap && (day < 0 || day>28)) return false;
        if (isLeap && (day < 0 || day>29)) return false;
        break;

    default:
        return false;
        break;
    }
    return true;
}

parfume_bazaar::Date::Date(int year, int month, int day):Date() {
    this->year = year;
    this->month = month;
    this->day = day;
}

parfume_bazaar::Date::Date(std::string sqliteFormat) :Date() {
   int year = std::stoi(sqliteFormat.substr(0, 4)); 
   int month = std::stoi(sqliteFormat.substr(5, 2)); 
   int day = std::stoi(sqliteFormat.substr(8, 2));
   this->year = year;
   this->month = month;
   this->day = day;
}

std::string parfume_bazaar::Date::toSqliteFormat() {
    std::string date = std::to_string(year) + '-';
    if (month / 10 == 0) date += "0";
    date += std::to_string(month);
    date += "-";
    if (day / 10 == 0) date += "0";
    return date + std::to_string(day);
}
