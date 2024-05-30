//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_MAKLER_H
#define PARFUME_BAZAAR_PROJECT_MAKLER_H

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "parfume_bazaar/date.h"
#include "sql/sqlite3.h"

namespace parfume_bazaar {
    class Makler {
    private:
        std::string surname; //аля логин (униакльный)
        Date birthDate;
        std::string address;

    public:
        explicit Makler(std::string surname, Date birthDate, std::string address)
                : surname(surname), birthDate(birthDate), address(address) {
        }
        explicit Makler() = default;

        std::string getSurname() const {
            return surname;
        }

        Date getBirthDate() const {
            return birthDate;
        }

        std::string getAddress() const {
            return address;
        }

        //будет вызываться в UI при регистрации нового маклера (пароль сохраняется в таблице)
        static bool insert(Makler& makler, std::string password, const char* db_path = "./parfume.db");

        //будет вызываться в UI при обновлении данных (требует пароль, чтобы подтвердить, что ты редачишь свой аккаунт)
        //делает что-то типо SET ... WHERE surname==makler.surname and password==makler.password
        static bool update(Makler& makler, std::string old_password, std::string new_password, const char* db_path = "./parfume.db");

        //будет вызываться в UI при удалении аккаунта (требует пароль, чтобы подтвердить, что ты удаляешь свой аккаунт)
        //соответсвенно удалить все записи из других таблиц, связанные с этим маклером
        //что-то типо DELETE ... WHERE surname==makler.surname and password==makler.password
        static bool remove(Makler& makler, std::string password, const char* db_path = "./parfume.db");
    };
}

#endif //PARFUME_BAZAAR_PROJECT_MAKLER_H
