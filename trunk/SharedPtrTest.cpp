#include <Poco/SharedPtr.h>

#include <gtest/gtest.h>

TEST(SharedPtrTest, all)
{
	Poco::SharedPtr<std::string> ptr(new std::string("abc"));
	EXPECT_FALSE(ptr.isNull());

	Poco::SharedPtr<std::string> ptr1 = ptr;

	char *pChar = new char[100];
	Poco::SharedPtr<char, Poco::ReferenceCounter, Poco::ReleaseArrayPolicy<char> > pArray(pChar);
	pArray[0] = 'a';
	EXPECT_EQ(pArray[0], pChar[0]);
	EXPECT_EQ('a', pChar[0]);
}
