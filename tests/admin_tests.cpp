#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>

TEST(InsertAdminTest, correctInput) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::insert(testAdmin, "password", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Admin WHERE login = 'admin' AND password = 'password'";

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

TEST(InsertAdminTest, wrongPath) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::insert(testAdmin, "password", "./wrongPath/test.db"), false);

}

TEST(UpdateAdminTest, incorrectPassword) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::update(testAdmin, "wrongPassword", "newPassword", "../tests/test.db"), false);

}

TEST(UpdateAdminTest, incorrectLogin) {

	parfume_bazaar::Admin testAdmin("sidorov_a");
	ASSERT_EQ(parfume_bazaar::Admin::update(testAdmin, "1111", "newPassword", "../tests/test.db"), false);
}

TEST(UpdateAdminTest, incorrectPath) {

	parfume_bazaar::Admin testAdmin("sidorov_c");
	ASSERT_EQ(parfume_bazaar::Admin::update(testAdmin, "1111", "newPassword", "./wrongPath/test.db"), false);
}

TEST(UpdateAdminTest, correctInput) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::update(testAdmin, "password", "newPassword", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Admin WHERE login = 'admin' AND password = 'newPassword'";

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

TEST(RemoveAdminTest, incorrectPassword) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::remove(testAdmin, "wrongPassword", "../tests/test.db"), false);
}

TEST(RemoveAdminTest, incorrectLogin) {

	parfume_bazaar::Admin testAdmin("sidorov_a");
	ASSERT_EQ(parfume_bazaar::Admin::remove(testAdmin, "1111", "../tests/test.db"), false);
}

TEST(RemoveAdminTest, incorrectPath) {

	parfume_bazaar::Admin testAdmin("sidorov_c");
	ASSERT_EQ(parfume_bazaar::Admin::remove(testAdmin, "1111", "./wrongPath/test.db"), false);
}

TEST(RemoveAdminTest, correctInput) {

	parfume_bazaar::Admin testAdmin("admin");
	ASSERT_EQ(parfume_bazaar::Admin::remove(testAdmin, "newPassword", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Admin WHERE login = 'admin' AND password = 'newPassword'";

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