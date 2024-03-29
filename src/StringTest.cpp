﻿#include <Poco/String.h>
#include <gtest/gtest.h>

// Test Fixtures: Using the Same Data Configuration for Multiple Tests
class StringTest : public ::testing::Test
{
protected:
	virtual void SetUp() {
		str1 = "The quick brown fox jumps over the lazy dog";
		str2 = " abc\t ";
		numStr = " 9919 ";
		wstr1 = L"Hello World!  ";
		wstr2 = L"  张岐文";
	}

	virtual void TearDown() {

	}

	std::string str1;
	std::string str2;
	std::string numStr;
	std::wstring wstr1;
	std::wstring wstr2;
};

// Test with Fixtures
TEST_F(StringTest, trim)
{
	EXPECT_EQ("abc\t ", Poco::trimLeft(str2));
	EXPECT_EQ(" abc\t ", str2); // test str2 NOT changed

	EXPECT_EQ(" abc", Poco::trimRight(str2));
	EXPECT_EQ(" abc\t ", str2); // test str2 NOT changed

	EXPECT_EQ("abc", Poco::trim(str2));
	EXPECT_EQ(" abc\t ", str2); // test str2 NOT changed

	std::string &trimedStr = Poco::trimInPlace(str2);
	EXPECT_EQ("abc", trimedStr);
	EXPECT_EQ(trimedStr, str2);
	EXPECT_EQ(&trimedStr, &str2);

	std::wstring expected = L"Hello World!";
	EXPECT_EQ(expected, Poco::trim(wstr1));
	EXPECT_EQ(L"Hello World!  ", wstr1); // NOT changed

	std::wstring &trimedWStr = Poco::trimInPlace(wstr1);
	EXPECT_EQ(expected, trimedWStr);
	EXPECT_EQ(trimedWStr, wstr1);
	EXPECT_EQ(&trimedWStr, &wstr1);

	// FIXED: Debug Assertion failed!
	// Poco 1.4.1p1 fixed this bug
	EXPECT_EQ(L"张岐文", Poco::trim(wstr2));
}

TEST_F(StringTest, caseConversion)
{
	EXPECT_EQ("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", Poco::toUpper(str1));
	EXPECT_EQ("the quick brown fox jumps over the lazy dog", Poco::toLower(str1));

	Poco::toUpperInPlace(wstr1);
	EXPECT_EQ(L"HELLO WORLD!  ", wstr1);
}

TEST_F(StringTest, CaseInsensitiveComparison)
{
	std::string iStr1 = Poco::toUpper(str1);
	EXPECT_NE(iStr1, str1);
	EXPECT_TRUE(Poco::icompare(str1, iStr1) == 0);
}

TEST_F(StringTest, Translation)
{
	//               "The quick brown fox jumps over the lazy dog"
	std::string rs = "The quicb brevj fex jumps ever the lzxy deg";
	EXPECT_EQ(rs, Poco::translate(str1, "aknowz", "zbjevx"));

	Poco::translateInPlace(str1, "aknowz", "zbjevx");
	EXPECT_EQ(rs, str1);
}

TEST_F(StringTest, SubstringReplace)
{
	// str1 = "The quick brown fox jumps over the lazy dog";
	std::string retStr = Poco::replace(str1, "fox", "men");
	EXPECT_EQ("The quick brown men jumps over the lazy dog", retStr);
	EXPECT_EQ("The quick brown fox jumps over the lazy dog", str1);

	// wstr1 = L"Hello World!  ";
	Poco::replaceInPlace(wstr1, std::wstring(L"World"), std::wstring(L"China"));
	EXPECT_EQ(L"Hello China!  ", wstr1);
}

TEST_F(StringTest, Concatenation)
{
	std::string catStr = Poco::cat(str1, str2, numStr);
	EXPECT_EQ(str1 + str2 + numStr, catStr);

	std::wstring wcatStr = Poco::cat(wstr1, wstr2);
	EXPECT_EQ(wstr1 + wstr2, wcatStr);

	std::vector<std::string> strVec;
	strVec.push_back("kevin");
	strVec.push_back("ning");
	strVec.push_back("yeat");
	strVec.push_back("duoyi");
	std::string delimCatStr = Poco::cat(std::string(", "), strVec.begin(), strVec.end());
	EXPECT_EQ("kevin, ning, yeat, duoyi", delimCatStr);
}
