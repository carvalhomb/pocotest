#include <Poco/MemoryPool.h>

#include <gtest/gtest.h>

struct Foo
{
	int a;
	double b;
	char c;
	char d[10];
};

TEST(MemoryPoolTest, all)
{
	Poco::MemoryPool pool(sizeof(struct Foo), 3, 4); // 3 preallocate, 4 at most
	struct Foo *buffer = reinterpret_cast<struct Foo *>(pool.get());
	buffer->a = 1;
	buffer->b = 2.5;

	EXPECT_EQ(3, pool.allocated());
	EXPECT_EQ(2, pool.available());

	struct Foo *buf2 = (struct Foo *) pool.get();

	EXPECT_EQ(3, pool.allocated());
	EXPECT_EQ(1, pool.available()); // available return allocated and usable count

	struct Foo *buf3 = (struct Foo *) pool.get();
	EXPECT_EQ(3, pool.allocated());
	EXPECT_EQ(0, pool.available());

	struct Foo *buf4 = (struct Foo *) pool.get();
	EXPECT_EQ(4, pool.allocated());
	EXPECT_EQ(0, pool.available());

	EXPECT_THROW({
		struct Foo *buf5 = (struct Foo *) pool.get();
		pool.release(buf5);
	}, Poco::OutOfMemoryException);

	pool.release(buf2);
	pool.release(buf3);
	pool.release(buf4);
}
