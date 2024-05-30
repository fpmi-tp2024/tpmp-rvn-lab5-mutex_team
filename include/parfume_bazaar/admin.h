//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_ADMIN_H
#define PARFUME_BAZAAR_PROJECT_ADMIN_H

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "sql/sqlite3.h"

namespace parfume_bazaar {
    class Admin {
    private:
        std::string login; //униакльный

    public:
        explicit Admin(std::string login)
                : login(login) {
        }

        std::string getLogin() const {
            return login;
        }

        //будет вызываться в UI при регистрации нового админа (пароль сохраняется в таблице)
        static bool insert(Admin& admin, std::string password, const char* db_path = "./parfume.db");

        //будет вызываться в UI при обновлении пароля (требует пароль, чтобы подтвердить, что ты редачишь свой аккаунт)
        //что-то типо SET ... WHERE login==admin.login and password==admin.password
        static bool update(Admin& admin, std::string old_password, std::string new_password, const char* db_path = "./parfume.db");

        //будет вызываться в UI при удалении аккаунта (требует пароль, чтобы подтвердить, что ты удаляешь свой аккаунт)
        //что-то типо DELETE ... WHERE login==admin.login and password==admin.password
        static bool remove(Admin& admin, std::string password, const char* db_path = "./parfume.db");
    };
}

#endif //PARFUME_BAZAAR_PROJECT_ADMIN_H
