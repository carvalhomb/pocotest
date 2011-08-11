#include <Poco/SharedPtr.h>
#include <Poco/DynamicFactory.h>

#include <gtest/gtest.h>

class Base
{
public:
	Base() : mB(0) {}
	Base(int i) : mB(i) {}
	virtual ~Base() {}
	virtual int get() {return mB;}

private:
	int mB;
};

class A : public Base
{
public:
	A() : Base(1) {}
};

class B : public Base
{
public:
	B() : Base(2) {}
};

class C : public Base
{
public:
	C(int a, int b) : Base(a) {}
};

class CInstantiator: public Poco::AbstractInstantiator<Base>
{
public:
	CInstantiator(int a, int b): mA(a), mB(b) {}

	Base* createInstance() const
	{
		return new C(mA, mB);
	}

private:
	int mA, mB;
};

TEST(DynamicFactoryTest, all)
{
	Poco::DynamicFactory<Base> factory;
	factory.registerClass<A>("A");
	factory.registerClass<B>("B");
	factory.registerClass("C", new CInstantiator(99, -1));

	EXPECT_TRUE(factory.isClass("A"));
	EXPECT_TRUE(factory.isClass("B"));
	EXPECT_TRUE(factory.isClass("C"));
	EXPECT_FALSE(factory.isClass("D"));

	Poco::SharedPtr<Base> pA = factory.createInstance("A");
	EXPECT_EQ(1, pA->get());
	Poco::SharedPtr<Base> pB = factory.createInstance("B");
	EXPECT_EQ(2, pB->get());

	Poco::SharedPtr<Base> pC = factory.createInstance("C");
	EXPECT_FALSE(pC.isNull());
	EXPECT_EQ(99, pC->get());

	factory.unregisterClass("B");
	
	EXPECT_TRUE(factory.isClass("A"));
	EXPECT_FALSE(factory.isClass("B"));
	EXPECT_TRUE(factory.isClass("C"));
	EXPECT_FALSE(factory.isClass("D"));
}
