#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>

TEST(InsertItemTest, correctInput) {

	parfume_bazaar::Admin admin("admin");
	parfume_bazaar::Item testItem("name","type",100.00,"seller",parfume_bazaar::Date(2000,1,1),1);

	ASSERT_EQ(parfume_bazaar::Item::insert(admin, testItem, "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Item WHERE name = 'name'";

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

TEST(UpdateItemTest, correctInput) {

	parfume_bazaar::Admin admin("admin");

	ASSERT_EQ(parfume_bazaar::Item::update(admin, "name", 2, "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Item WHERE name = 'name' AND amount = 2";

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

TEST(UpdateItemTest, incorrectName) {

	parfume_bazaar::Admin admin("admin");

	ASSERT_EQ(parfume_bazaar::Item::update(admin, "wrongName", 2, "../tests/test.db"), false);
}

TEST(RemoveItemTest, correctInput) {

	parfume_bazaar::Admin admin("admin");

	ASSERT_EQ(parfume_bazaar::Item::remove(admin, "name", "../tests/test.db"), true);
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("../tests/test.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT *  FROM Item WHERE name = 'name'";

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

TEST(RemoveItemTest, incorrectName) {

	parfume_bazaar::Admin admin("admin");

	ASSERT_EQ(parfume_bazaar::Item::remove(admin, "wrongName", "../tests/test.db"), false);
	
}
