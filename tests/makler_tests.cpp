#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>

TEST(InsertMaklerTest, correctInput) {

	parfume_bazaar::Date birthDate(2000, 02, 02);
	parfume_bazaar::Makler testMakler("Surname", birthDate, "address");

	ASSERT_EQ(parfume_bazaar::Makler::insert(testMakler, "password", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Makler WHERE surname = 'Surname' AND password = 'password' AND birth_date = '" + birthDate.toSqliteFormat() + "' AND address = 'address'";

	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	int output = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		output++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	ASSERT_EQ(output, 1);

}

TEST(UpdateMaklerTest, correctInput) {

	parfume_bazaar::Date birthDate(2000, 02, 02);
	parfume_bazaar::Makler testMakler("Surname", birthDate, "address");

	ASSERT_EQ(parfume_bazaar::Makler::update(testMakler, "password", "newPassword", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Makler WHERE surname = 'Surname' AND password = 'newPassword' AND birth_date = '" + birthDate.toSqliteFormat() + "' AND address = 'address'";

	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	int output = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		output++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	ASSERT_EQ(output, 1);

}

TEST(UpdateMaklerTest, incorrectPassword) {

	parfume_bazaar::Date birthDate(2000, 02, 02);
	parfume_bazaar::Makler testMakler("Surname", birthDate, "address");

	ASSERT_EQ(parfume_bazaar::Makler::update(testMakler, "wrongpassword", "newPassword", "../tests/test.db"), false);

}

TEST(UpdateMaklerTest, incorrectSurname) {

	parfume_bazaar::Date birthDate(2000, 02, 02);
	parfume_bazaar::Makler testMakler("WrongSurname", birthDate, "address");

	ASSERT_EQ(parfume_bazaar::Makler::update(testMakler, "password", "newPassword", "../tests/test.db"), false);

}

TEST(RemoveMaklerTest, correctInput) {

	parfume_bazaar::Date birthDate(2000, 02, 02);
	parfume_bazaar::Makler testMakler("Surname", birthDate, "address");

	ASSERT_EQ(parfume_bazaar::Makler::remove(testMakler, "newPassword", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Makler WHERE surname = 'Surname' AND password = 'newPassword' AND birth_date = '" + birthDate.toSqliteFormat() + "' AND address = 'address'";

	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	int output = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		output++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	ASSERT_EQ(output, 0);

}

TEST(RemoveMaklerTest, incorrectPassword) {

	parfume_bazaar::Date birthDate(2000, 1, 1);
	parfume_bazaar::Makler testMakler("Pavlov", birthDate, "Okyabrskaya 10");

	ASSERT_EQ(parfume_bazaar::Makler::remove(testMakler, "wrongPassword", "../tests/test.db"), false);

}

TEST(RemoveMaklerTest, incorrectLogin) {

	parfume_bazaar::Date birthDate(2000, 1, 1);
	parfume_bazaar::Makler testMakler("Fillipov", birthDate, "Okyabrskaya 10");

	ASSERT_EQ(parfume_bazaar::Makler::remove(testMakler, "12321", "../tests/test.db"), false);

}

