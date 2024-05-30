#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>

void copyTestDB() {

    std::filesystem::path src_file = "../tests/test.db";
	std::filesystem::path out = "./testcopy.db";

    try {

        std::filesystem::copy_file(src_file, out);
    }
    catch (const std::filesystem::filesystem_error& e) {

		std::cout << "aa";
        return;
    }
}


TEST(updateMaklersStatisticsTests, correctInputMaklerNewBargain) {

    copyTestDB();
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopy.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}
    parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
    parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 11, 11), "bataille", "women parfume", 10, "Gold Apple");
    EXPECT_EQ(parfume_bazaar::Bargain::updateMaklersStatistics(makler, bargain, db),true);

	

	std::string query = "SELECT * FROM Statistic WHERE makler_id = 10 AND item_id = 4 AND amount = 10";

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
	std::filesystem::remove("./testcopy.db");

	EXPECT_EQ(output, 1);
}

TEST(updateMaklersStatisticsTests, correctInputMaklerOldBargain) {

	copyTestDB();
	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopy.db", &db);
	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
	parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 1, 11), "voluptuous seduction", "aromadiffusor", 10, "Gold Apple");
	EXPECT_EQ(parfume_bazaar::Bargain::updateMaklersStatistics(makler, bargain, db), true);

	

	std::string query = "SELECT * FROM Statistic WHERE makler_id = 10 AND item_id = 10 AND amount = 40";

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
	std::filesystem::remove("./testcopy.db");

	EXPECT_EQ(output, 1);
}

TEST(insertTests, correctInputMaklerOldBargain) {

	copyTestDB();
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
	parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 1, 11), "voluptuous seduction", "aromadiffusor", 10, "Gold Apple");
	EXPECT_EQ(parfume_bazaar::Bargain::insert(makler, bargain, "./testcopy.db"), true);

	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopy.db", &db);

	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT * FROM Statistic WHERE makler_id = 10 AND item_id = 10 AND amount = 40";

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
	EXPECT_EQ(output, 1);

	sqlite3_finalize(stmt);

	std::string query1 = "SELECT * FROM Bargain WHERE item_name = 'voluptuous seduction' AND makler_surname = 'Semenov' AND amount = 10";

	sqlite3_stmt* stmt1;
	rc = sqlite3_prepare_v2(db, query1.c_str(), -1, &stmt1, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	output = 0;
	while (sqlite3_step(stmt1) == SQLITE_ROW) {
		output++;
	}
	EXPECT_EQ(output, 1);
	sqlite3_finalize(stmt1);
	sqlite3_close(db);
	
	std::filesystem::remove("./testcopy.db");


}


TEST(insertTests, correctInputMaklerNewBargain) {

	copyTestDB();
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
	parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 11, 11), "bataille", "women parfume", 10, "Gold Apple");
	EXPECT_EQ(parfume_bazaar::Bargain::insert(makler, bargain, "./testcopy.db"), true);

	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopy.db", &db);

	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT * FROM Statistic WHERE makler_id = 10 AND item_id = 4 AND amount = 10";

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
	EXPECT_EQ(output, 1);

	sqlite3_finalize(stmt);

	std::string query1 = "SELECT * FROM Bargain WHERE item_name = 'bataille' AND makler_surname = 'Semenov' AND amount = 10";

	sqlite3_stmt* stmt1;
	rc = sqlite3_prepare_v2(db, query1.c_str(), -1, &stmt1, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	output = 0;
	while (sqlite3_step(stmt1) == SQLITE_ROW) {
		output++;
	}
	EXPECT_EQ(output, 1);
	sqlite3_finalize(stmt1);
	sqlite3_close(db);

	std::filesystem::remove("./testcopy.db");


}

TEST(insertTests, incorrectAmount) {

	copyTestDB();
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
	parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 01, 11), "bataille", "women parfume", 1000, "Gold Apple");
	EXPECT_EQ(parfume_bazaar::Bargain::insert(makler, bargain, "./testcopy.db"), false);
	std::filesystem::remove("./testcopy.db");
}

TEST(insertTests, incorrectItemName) {

	copyTestDB();
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2003, 5, 11), "Levina 13");
	parfume_bazaar::Bargain bargain(makler, parfume_bazaar::Date(2024, 01, 11), "wrongName", "women parfume", 10, "Gold Apple");
	EXPECT_EQ(parfume_bazaar::Bargain::insert(makler, bargain, "./testcopy.db"), false);
	std::filesystem::remove("./testcopy.db");
}

TEST(removeBeforeDateTests, correctInput) {

	copyTestDB();
	parfume_bazaar::Makler makler("Semenov", parfume_bazaar::Date(2024, 5, 11), "Levina 13");
	parfume_bazaar::Date date(2024, 5, 11);
	EXPECT_EQ(parfume_bazaar::Bargain::removeBeforeDate(makler, date, "./testcopy.db"), true);

	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopy.db", &db);

	if (rc != SQLITE_OK) {
		std::cout << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
	}

	std::string query = "SELECT * FROM Bargain WHERE date < '" + date.toSqliteFormat() + "';";

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
	EXPECT_EQ(output, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	std::filesystem::remove("./testcopy.db");



}