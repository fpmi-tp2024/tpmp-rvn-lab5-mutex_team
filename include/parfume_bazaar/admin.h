//
// Created by User on 4/5/2024.
//

#ifndef PARFUME_BAZAAR_PROJECT_ADMIN_H
#define PARFUME_BAZAAR_PROJECT_ADMIN_H

#pragma once
#include <string>
#include <vector>

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
        static void insert(Admin& admin, std::string password);

        //будет вызываться в UI при обновлении пароля (требует пароль, чтобы подтвердить, что ты редачишь свой аккаунт)
        //что-то типо SET ... WHERE login==admin.login and password==admin.password
        static void update(Admin& admin, std::string old_password, std::string new_password);

        //будет вызываться в UI при удалении аккаунта (требует пароль, чтобы подтвердить, что ты удаляешь свой аккаунт)
        //что-то типо DELETE ... WHERE login==admin.login and password==admin.password
        static void remove(Admin& admin, std::string password);
    };
}

#endif //PARFUME_BAZAAR_PROJECT_ADMIN_H
