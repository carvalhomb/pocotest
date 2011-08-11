#include <Poco/SingletonHolder.h>

#include <gtest/gtest.h>

class China
{
public:
	China() {}
	~China() {}

	static China& instance()
	{
		// thread safe, we don't test here
		static Poco::SingletonHolder<China> sh;
		return *sh.get();
	}
};

TEST(SingletonTest, all)
{
	EXPECT_EQ(&China::instance(), &China::instance());

	China &c1 = China::instance();
	China &c2 = China::instance();
	EXPECT_EQ(&c1, &c2);
}
