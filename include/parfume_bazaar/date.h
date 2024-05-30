//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_DATE_H
#define PARFUME_BAZAAR_PROJECT_DATE_H

#pragma once
#include <string>

namespace parfume_bazaar {


    class Date {
    private:
        int year;
        int month;
        int day;
    public:
        explicit Date(std::string sql_date_text);

        Date() : year(0), month(0), day(0) {};

        explicit Date(int year, int month, int day);

        

        int getYear() {
            return year;
        }

        int getMonth() {
            return month;
        }

        int getDay() {
            return day;
        }

        //возвращает валидную строку-дату для вставки в таблицу
        std::string toSqliteFormat();
        static bool checkDate(int year, int month, int day);
    };
}

#endif //PARFUME_BAZAAR_PROJECT_DATE_H
