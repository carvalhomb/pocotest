#include <Poco/Format.h>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>
#include <Poco/String.h>

#include <gtest/gtest.h>

// TODO: later
TEST(FormattingTest, NumberFormatter)
{
	EXPECT_EQ("123", Poco::NumberFormatter::format(123));
	EXPECT_EQ("  123", Poco::NumberFormatter::format(123, 5));
	EXPECT_EQ("00123", Poco::NumberFormatter::format0(123, 5));
	EXPECT_EQ("FE", Poco::NumberFormatter::formatHex(254));
	EXPECT_EQ("00000000FE", Poco::NumberFormatter::formatHex(254, 10));

	EXPECT_EQ("1234ABCD", Poco::NumberFormatter::format((void*)0x1234abcd));

	EXPECT_EQ("1234.56789", Poco::NumberFormatter::format(1234.56789));
	EXPECT_EQ("1234.57", Poco::NumberFormatter::format(1234.56789, 2));
	EXPECT_EQ("  1234.568", Poco::NumberFormatter::format(1234.56789, 10, 3));

	// Use of append() can greatly improve performance
	// all format() have append() counterparts
	std::string str = "abcd";
	Poco::NumberFormatter::appendHex(str, 254, 10);
	EXPECT_EQ("abcd00000000FE", str);
}

TEST(FormattingTest, sprintfFormat)
{
	std::string s;
	std::string temp("abc");
	Poco::format(s, "%d %s", 45, temp);
	EXPECT_EQ("45 abc", s);

	EXPECT_EQ("  79", Poco::format("%4d", 79));
	EXPECT_EQ("79  ", Poco::format("%-4d", 79));

	EXPECT_THROW({
		Poco::format("%s", 12);
	}, Poco::BadCastException);
}

TEST(FormattingTest, NumberParser)
{
	EXPECT_EQ(123, Poco::NumberParser::parse("123"));

	EXPECT_THROW({
		Poco::NumberParser::parse("123d");
	}, Poco::SyntaxException);

	int value;
	EXPECT_TRUE(Poco::NumberParser::tryParse("123", value));
	EXPECT_EQ(123, value);
	EXPECT_FALSE(Poco::NumberParser::tryParse("123d", value));

	EXPECT_EQ((unsigned)-100, Poco::NumberParser::parseUnsigned("-100"));

	EXPECT_EQ(0xefab1207, Poco::NumberParser::parseHex("0xefab1207"));
	EXPECT_EQ(0xefab1207, Poco::NumberParser::parseHex("EFAB1207"));

	Poco::Int64 temp64 = Poco::NumberParser::parse64("91234567891234567");
	EXPECT_EQ(91234567891234567ll, temp64);
}

TEST(FormattingTest, StringTokenizer)
{
	Poco::StringTokenizer t1("red, green, blue", ",");
	EXPECT_EQ("red", t1[0]);
	EXPECT_EQ(" green", t1[1]);
	EXPECT_EQ(" blue", t1[2]);

	Poco::StringTokenizer t2("red,green,,blue", ",");
	EXPECT_EQ("red", t2[0]);
	EXPECT_EQ("green", t2[1]);
	EXPECT_EQ("", t2[2]);
	EXPECT_EQ("blue", t2[3]);

	Poco::StringTokenizer t3("red; green, blue", ",;",
			Poco::StringTokenizer::TOK_TRIM);
	EXPECT_EQ("red", t3[0]);
	EXPECT_EQ("green", t3[1]);
	EXPECT_EQ("blue", t3[2]);

	Poco::StringTokenizer t4(" black; green ,, blue   ", ",;",
			Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	EXPECT_EQ("black", t4[0]);
	EXPECT_EQ("green", t4[1]);
	EXPECT_EQ("blue", t4[2]);

	std::string s(Poco::cat(std::string("; "), t4.begin(), t4.end()));
	EXPECT_EQ("black; green; blue", s);
}
