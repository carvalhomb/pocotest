#include <Poco/Buffer.h>

#include <gtest/gtest.h>

TEST(BufferTest, all)
{
	Poco::Buffer<int> buf(100);

	int i = 0;
	for (int *p = buf.begin(); p < buf.end(); p++)
		*p = i++;

	EXPECT_EQ(0, buf[0]);
	EXPECT_EQ(1, buf[1]);
	EXPECT_EQ(99, buf[99]);
}
