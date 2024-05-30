#include <iostream>
#include <gtest/gtest.h>
#include "../ext/sql/sqlite3.h"
#include <parfume_bazaar/parfume_bazaar.h>
#include <filesystem>


TEST(DateCreationTests, Date11_9_2022) {

	parfume_bazaar::Date date(2022, 9, 11);
	ASSERT_EQ(date.getDay(), 11);
	ASSERT_EQ(date.getMonth(), 9);
	ASSERT_EQ(date.getYear(), 2022);

}

TEST(DateCreationTests, Date31_10_2022) {

	parfume_bazaar::Date date(2022, 10, 31);
	ASSERT_EQ(date.getDay(), 31);
	ASSERT_EQ(date.getMonth(), 10);
	ASSERT_EQ(date.getYear(), 2022);

}


TEST(DateCreationTests, InvalidFebruaryDate) {

	parfume_bazaar::Date date(2022, 2, 29);
	std::cout << date.getDay() << "\n";
	ASSERT_EQ(date.getDay(), 0);
	ASSERT_EQ(date.getMonth(), 0);
	ASSERT_EQ(date.getYear(), 0);

}

TEST(DateCreationTests, ValidFebruaryDate) {

	parfume_bazaar::Date date(2022, 2, 28);
	std::cout << date.getDay() << "\n";
	ASSERT_EQ(date.getDay(), 28);
	ASSERT_EQ(date.getMonth(), 2);
	ASSERT_EQ(date.getYear(), 2022);

}

TEST(DateCreationTests, ValidLeapYearFebruaryDate) {

	parfume_bazaar::Date date(2024, 2, 29);
	std::cout << date.getDay() << "\n";
	ASSERT_EQ(date.getDay(), 29);
	ASSERT_EQ(date.getMonth(), 2);
	ASSERT_EQ(date.getYear(), 2024);

}

TEST(DateCreationTests, InvalidDate31_04) {

	parfume_bazaar::Date date(2024, 4, 31);
	std::cout << date.getDay() << "\n";
	ASSERT_EQ(date.getDay(), 0);
	ASSERT_EQ(date.getMonth(), 0);
	ASSERT_EQ(date.getYear(), 0);

}

TEST(DateCreationTests, InvalidDate32_05) {

	parfume_bazaar::Date date(2024, 5, 32);
	std::cout << date.getDay() << "\n";
	ASSERT_EQ(date.getDay(), 0);
	ASSERT_EQ(date.getMonth(), 0);
	ASSERT_EQ(date.getYear(), 0);

}

TEST(toSqliteTests, Date11_9_2022) {

	parfume_bazaar::Date date(2022, 9, 11);
	EXPECT_EQ(date.toSqliteFormat(), "2022-09-11") << "String format must be 2022-09-11";
}

TEST(toSqliteTests, Date21_11_2005) {

	parfume_bazaar::Date date(2005, 11, 21);
	EXPECT_EQ(date.toSqliteFormat(), "2005-11-21") << "String format must be 2005-11-21";
}