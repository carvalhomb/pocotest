#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

#include <gtest/gtest.h>

class Foo
{
public:
	Foo() : rc(1) {}
	~Foo() {}
	void duplicate() {++rc;}
	void release() {if (--rc == 0) delete this;}
	int count() {return rc;}

private:
	int rc;
};

TEST(AutoPtrTest, Foo)
{
	Poco::AutoPtr<Foo> ptr1(new Foo);
	EXPECT_EQ(1, ptr1->count());

	Poco::AutoPtr<Foo> ptr2(ptr1);
	EXPECT_EQ(2, ptr2->count());

	Poco::AutoPtr<Foo> ptr3(ptr2);
	EXPECT_EQ(3, ptr3->count());

	Poco::AutoPtr<Foo> ptr4(ptr1, true);
	EXPECT_EQ(4, ptr4->count());
}

class Bar : public Poco::RefCountedObject
{
public:
	int count() {return referenceCount();}
};

TEST(AutoPtrTest, Bar)
{
	Poco::AutoPtr<Bar> ptr1(new Bar);
	EXPECT_EQ(1, ptr1->count());

	Poco::AutoPtr<Bar> ptr2(ptr1);
	EXPECT_EQ(2, ptr2->count());
}
