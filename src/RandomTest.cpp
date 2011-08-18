#include <Poco/Random.h>
#include <Poco/RandomStream.h>
#include <gtest/gtest.h>

TEST(RandomTest, all)
{
	Poco::Random rnd;
	rnd.seed();

	Poco::UInt32 rInt = 0;
	rInt = rnd.next();
	EXPECT_NE(0, rInt); // I got lucky!

	for (int i = 0; i < 10; i++) {
		rInt = rnd.next(50);
		EXPECT_TRUE(0 <= rInt && rInt < 50);
	}

	rnd.nextChar();
	rnd.nextBool();
	rnd.nextDouble();
}

TEST(RandomTest, RandomInputStream)
{
	Poco::RandomInputStream ris;

	std::string temp;
	ris >> temp;
	// don't know how to test this
}
