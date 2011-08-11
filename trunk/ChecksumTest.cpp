#include <Poco/Checksum.h>
#include <gtest/gtest.h>

TEST(ChecksumTest, Trivial)
{
	Poco::Checksum sum;
	sum.update("abcdefghijklmnopqrstuvwxyz");
	EXPECT_EQ(1277644989, sum.checksum());
}
