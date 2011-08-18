#include <Poco/DigestEngine.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>

#include <gtest/gtest.h>

TEST(DigestEngineTest, MD5EngineTest)
{
	std::string msg1 = "Kevin Zhang";
	std::string msg2 = "go fuck yourself!";

	Poco::MD5Engine md5;

	md5.update(msg1);
	std::string digestString(Poco::DigestEngine::digestToHex(md5.digest()));
	EXPECT_EQ("a239723c44d3265ffa739522c5e0182e", digestString);

	md5.reset();
	md5.update(msg2);
	std::string digestString2(Poco::DigestEngine::digestToHex(md5.digest()));
	EXPECT_EQ("2f9a2d20c03c1adb6877d8e57ae3ed00", digestString2);
}

TEST(DigestEngineTest, DigestOutputStreamTest)
{
	Poco::MD5Engine md5;
	Poco::DigestOutputStream ostr(md5);
	ostr << "This is some text";
	ostr.flush(); // Ensure everything gets passed to the digest engine
	std::string result = Poco::DigestEngine::digestToHex(md5.digest());
	EXPECT_EQ("97214f63224bc1e9cc4da377aadce7c7", result);
}
