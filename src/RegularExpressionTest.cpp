#include <Poco/RegularExpression.h>
#include <gtest/gtest.h>

TEST(RegularExpressionTest, match)
{
	Poco::RegularExpression re1("[0-9]+");
	EXPECT_TRUE(re1.match("123"));
	EXPECT_FALSE(re1.match("abc"));
	EXPECT_TRUE(re1.match("abc123", 3));
}

TEST(RegularExpressionTest, matchPos)
{
	Poco::RegularExpression re1("[0-9]+");
	Poco::RegularExpression::Match pos;
	EXPECT_EQ(1, re1.match("123", 0, pos));
	EXPECT_EQ(0, pos.offset);
	EXPECT_EQ(3, pos.length);

	EXPECT_EQ(1, re1.match("ab12de90cd12d", 0, pos));
	EXPECT_EQ(2, pos.offset);
	EXPECT_EQ(2, pos.length);

	EXPECT_EQ(0, re1.match("abcd", 0, pos));
	EXPECT_EQ(std::string::npos, pos.offset);

	Poco::RegularExpression re2("([0-9]+) ([0-9]+)");
	Poco::RegularExpression::MatchVec posVec;
	EXPECT_EQ(3, re2.match("123 456", 0, posVec));
	EXPECT_EQ(0, posVec[0].offset);
	EXPECT_EQ(7, posVec[0].length);
	EXPECT_EQ(0, posVec[1].offset);
	EXPECT_EQ(3, posVec[1].length);
	EXPECT_EQ(4, posVec[2].offset);
	EXPECT_EQ(3, posVec[2].length);
}

TEST(RegularExpressionTest, extract)
{
	std::string s;
	Poco::RegularExpression re1("[0-9]+");
	Poco::RegularExpression re2("([0-9]+) ([0-9]+)");

	EXPECT_EQ(1, re1.extract("123", s));
	EXPECT_EQ("123", s);

	EXPECT_EQ(1, re1.extract("ab12de", 0, s));
	EXPECT_EQ("12", s);

	EXPECT_EQ(0, re1.extract("abcd", 0, s));
	EXPECT_EQ("", s);

	std::vector<std::string> vec;
	re2.split("123 456", 0, vec);
	// FIXME: split?
	EXPECT_EQ("123 456", vec[0]);
	EXPECT_EQ("123", vec[1]);
	EXPECT_EQ("456", vec[2]);
}

TEST(RegularExpressionTest, subst)
{
	std::string s = "123";
	Poco::RegularExpression re1("[0-9]+");
	Poco::RegularExpression re2("([0-9]+) ([0-9]+)");

	re1.subst(s, "ABC");
	EXPECT_EQ("ABC", s);

	s = "123 456";
	re2.subst(s, "$2 $1");
	EXPECT_EQ("456 123", s);
}

TEST(RegularExpressionTest, options)
{
	Poco::RegularExpression re1("ABC");
	EXPECT_FALSE(re1.match("abc", 0));

	Poco::RegularExpression re2("ABC", Poco::RegularExpression::RE_CASELESS);
	EXPECT_TRUE(re2.match("abc", 0));
}
