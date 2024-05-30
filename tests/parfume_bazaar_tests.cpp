#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>

void copyTestDBMain() {

	std::filesystem::path src_file = "../tests/test.db";

	std::filesystem::path out_file = "./testcopymain.db";
	try {

		std::filesystem::copy_file(src_file, out_file);
	}
	catch (const std::filesystem::filesystem_error& e) {

		std::cout << "Error file copy";
		return;
	}
}

bool containsAllFirms(std::vector< std::pair <std::string, std::vector<std::string>>>  firmSellersInfo, std::vector<std::string> firms) {

	for (std::string firm : firms) {
		bool contains = false;
		for (std::pair <std::string, std::vector<std::string>> sellerInfo : firmSellersInfo) {

			if (sellerInfo.first == firm) {
				contains = true;
				break;
			}
		}
		if (!contains) return false;
	}

	return true;
}

bool containsCorrectMaklers(std::vector< std::pair <std::string, std::vector<std::string>>>  firmSellersInfo) {

	for (std::pair <std::string, std::vector<std::string>> sellerInfo : firmSellersInfo) {

		if (sellerInfo.first == "ESSENTIAL PARFUMS PARIS" &&
			(sellerInfo.second.size() != 1 || sellerInfo.second[0] != "Aralov: 65items. 19732.05$")) return false;

		if (sellerInfo.first == "NEYDO" &&
			sellerInfo.second.size() != 0) return false;

		if (sellerInfo.first == "MARK BUXTON" &&
			(sellerInfo.second.size() != 1 || sellerInfo.second[0] != "Aralov: 30items. 9769.80$")) return false;

		if (sellerInfo.first == "BASTILLE" &&
			(sellerInfo.second.size() != 1 || sellerInfo.second[0] != "Tyshkevich: 50items. 11188.50$")) return false;

		if (sellerInfo.first == "TOM FORD" &&
			(sellerInfo.second.size() != 1 || sellerInfo.second[0] != "Korotkevich: 70items. 53935.00$")) return false;

		if (sellerInfo.first == "BIRKHOLZ" &&
			(sellerInfo.second.size() != 1 || sellerInfo.second[0] != "Nikiforov: 35items. 25317.60$")) return false;

		if (sellerInfo.first == "KILIAN PARIS" &&
			(sellerInfo.second.size() != 2 || sellerInfo.second[0] != "Streltsov: 10items. 13400.00$"
				|| sellerInfo.second[1] != "Vorobyov: 20items. 32985.00$")) return false;

		if (sellerInfo.first == "AROMA HARMONY" &&
			(sellerInfo.second.size() != 2 || sellerInfo.second[0] != "Pavlov: 200items. 11280.00$"
				|| sellerInfo.second[1] != "Kuznetsov: 150items. 8460.00$")) return false;

		if (sellerInfo.first == "STATE OF MIND" &&
			(sellerInfo.second.size() != 2 || sellerInfo.second[0] != "Pashkevich: 150items. 39505.50$"
				|| sellerInfo.second[1] != "Semenov: 30items. 7901.10$")) return false;
	
	}
	return true;
}

bool containsCorrectItemInfo(std::vector<std::string> info) {

	if (info[0] != "Name: #28 white oak\n\tAmount: 350, Total cost: 19740.0") return false;
	if (info[1] != "Name: bataille\n\tAmount: 50, Total cost: 11188.5") return false;
	if (info[2] != "Name: costa azzurra\n\tAmount: 70, Total cost: 53935.0") 
	if (info[3] != "Name: divine vanille\n\tAmount: 65, Total cost: 19732.05") return false;
	if (info[4] != "Name: dreaming with ghosts\n\tAmount: 30, Total cost: 9769.8") return false;
	if (info[5] != "Name: love, don't be shy\n\tAmount: 20, Total cost: 32985.4") return false;
	if (info[6] != "Name: sir santal\n\tAmount: 35, Total cost: 25317.6") return false;
	if (info[7] != "Name: straight to heaven\n\tAmount: 10, Total cost: 13400.0") return false;
	if (info[8] != "Name: voluptuous seduction\n\tAmount: 180, Total cost: 47406.6") return false;
	return true;
}


TEST(createTablesTests, DefaultPathCase) {

   EXPECT_EQ(parfume_bazaar::createTables(), true) << "Tables must be opened or created in the database ./parfume.db";
}

TEST(createTablesTests, ValidPathCase) {

	EXPECT_EQ(parfume_bazaar::createTables("./testDB.db"), true) << "Tables must be opened or created in the database ./testDB.db";
	std::filesystem::remove("./testDB.db");
}

TEST(createTablesTests, InvalidPathCase) {

	EXPECT_EQ(parfume_bazaar::createTables("./wrongPath/wrongDB.db"), false) << "There is no directory ./wrongPath/wrongDB.db";
}

TEST(getFirmSellerInfoTests, validInput) {

	parfume_bazaar::Admin admin("anna21");

	std::vector< std::pair <std::string, std::vector<std::string>>>  firmSellersInfo = getFirmSellerInfo(admin,"../tests/test.db");
	ASSERT_EQ(firmSellersInfo.size(), 9) << "Wrong number of firm sellers (should be 9)";

	std::vector<std::string> firms{"ESSENTIAL PARFUMS PARIS","NEYDO","MARK BUXTON","BASTILLE","TOM FORD","BIRKHOLZ","KILIAN PARIS","AROMA HARMONY","STATE OF MIND"};
	ASSERT_EQ(containsAllFirms(firmSellersInfo, firms), true)<< "Should contain 9 following firms";
	ASSERT_EQ(containsCorrectMaklers(firmSellersInfo), true)<< "Should return correct list of marklers";
	
}

TEST(getFirmSellerInfoTests, invalidDatabasePath) {

	parfume_bazaar::Admin admin("anna21");

	std::vector< std::pair <std::string, std::vector<std::string>>>  firmSellersInfo = getFirmSellerInfo(admin, "./wrongPath/wrongDB.db");
	ASSERT_EQ(firmSellersInfo.size(), 0) << "Should be an empty structure";

}

TEST(getBargainsTests, BargainsExist) {

	parfume_bazaar::Makler makler("Pavlov", parfume_bazaar::Date(2001, 1, 1), "Okyabrskaya 10");
	parfume_bazaar::Date date(2024, 4, 11);
	std::vector<parfume_bazaar::Bargain> bargains = getBargains(makler, date, "../tests/test.db");
	ASSERT_EQ(bargains.size(), 1);
	ASSERT_EQ(bargains[0].getItemName(), "#28 white oak");
	ASSERT_EQ(bargains[0].getAmount(), 200);
	ASSERT_EQ(bargains[0].getMaklerSurname(), "Pavlov");
	ASSERT_EQ(bargains[0].getDate().toSqliteFormat(), "2024-04-11");
}

TEST(getBargainsTests, BargainDoesNotExistIncorrectDate) {

	parfume_bazaar::Makler makler("Pavlov", parfume_bazaar::Date(2001, 1, 1), "Okyabrskaya 10");
	parfume_bazaar::Date date(2024, 4, 12);
	std::vector<parfume_bazaar::Bargain> bargains = getBargains(makler, date, "../tests/test.db");
	ASSERT_EQ(bargains.size(), 0);
}

TEST(getBargainsTests, BargainDoesNotExistIncorrectMakler) {

	parfume_bazaar::Makler makler("WrongSurname", parfume_bazaar::Date(2001, 1, 1), "Okyabrskaya 10");
	parfume_bazaar::Date date(2024, 4, 11);
	std::vector<parfume_bazaar::Bargain> bargains = getBargains(makler, date, "../tests/test.db");
	ASSERT_EQ(bargains.size(), 0);
}

TEST(updateItemsAmountTests, CorrectInput) {

	parfume_bazaar::Admin admin("anna21");
	parfume_bazaar::Date date(2023, 11, 28);
	copyTestDBMain();
    EXPECT_EQ(parfume_bazaar::updateItemsAmount(admin, date, "./testcopymain.db"), true);

	sqlite3* db;
	char* errMsg = nullptr;
	int rc;

	rc = sqlite3_open("./testcopymain.db", &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Unable to open or create database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
		return;
	}

	std::string query = "SELECT * FROM Item WHERE name = 'voluptuous seduction';";

	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		EXPECT_EQ(sqlite3_column_int(stmt, 6), 100);
	}
	sqlite3_finalize(stmt);

	sqlite3_stmt* stmt1;

	 query = "SELECT * FROM Item WHERE name = 'sir santal';";

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt1, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error with SELECT query: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt1);
		sqlite3_close(db);
		return;
	}

	while (sqlite3_step(stmt1) == SQLITE_ROW) {
		EXPECT_EQ(sqlite3_column_int(stmt1, 6), 70);
	}

	
	sqlite3_finalize(stmt1);
	sqlite3_close(db);


    std::filesystem::remove("./testcopymain.db");

}

TEST(getItemInfoTests, correctInput) {
	parfume_bazaar::Admin admin("anna21");
	std::vector<std::string> itemInfo = parfume_bazaar::getItemInfo(admin, parfume_bazaar::Date(2022, 11, 11), parfume_bazaar::Date(2024, 5, 30), "../tests/test.db");
	ASSERT_EQ(itemInfo.size(), 9);
	ASSERT_EQ(containsCorrectItemInfo(itemInfo), true);
}

TEST(getSuccessfulMaklerInfoTests, correctInput) {

	parfume_bazaar::Makler makler("makler", parfume_bazaar::Date(2000, 5, 12), "address");
	std::pair< parfume_bazaar::Makler, std::vector<std::string> > successfulMaklerInfo = parfume_bazaar::getSuccessfulMaklerInfo(makler, parfume_bazaar::Date(2022, 11, 11), parfume_bazaar::Date(2024, 5, 30), "../tests/test.db");
	ASSERT_EQ(successfulMaklerInfo.first.getSurname(), "Aralov");
	ASSERT_EQ(successfulMaklerInfo.second[0], "Gold Apple");
	ASSERT_EQ(successfulMaklerInfo.second[1], "Kravt");

}

TEST(getPopularItemInfoTests, correctInput) {

	parfume_bazaar::Makler makler("makler", parfume_bazaar::Date(2000, 5, 12), "address");
	std::pair<std::string, std::vector<std::string>> successfulMaklerInfo = parfume_bazaar::getPopularItemInfo(makler, parfume_bazaar::Date(2022, 11, 11), parfume_bazaar::Date(2024, 5, 30), "../tests/test.db");
	ASSERT_EQ(successfulMaklerInfo.first, "men parfume");
	ASSERT_EQ(successfulMaklerInfo.second[0], "Firm name: Gold Apple Amount: 260 Total cost: 205368.800000");
	ASSERT_EQ(successfulMaklerInfo.second[1], "Firm name: Letoile Amount: 320 Total cost: 252761.600000");
}

TEST(getFirmInfoAboutItemTests, validInput) {

	parfume_bazaar::Admin admin("anna21");

	std::vector<std::string>  firmInfo = getFirmInfoAboutItem(admin, parfume_bazaar::Date(2022, 11, 11), parfume_bazaar::Date(2024, 5, 30), "../tests/test.db");
	for (std::string s : firmInfo) std::cout << s << "\n";
	ASSERT_EQ(firmInfo.size(), 11);

}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}