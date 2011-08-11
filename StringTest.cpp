#include <Poco/String.h>
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

	// FIXME: Debug Assertion failed!
	// EXPECT_EQ(L"张岐文", Poco::trim(wstr2));
}
