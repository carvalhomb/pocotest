#include <Poco/Types.h>
#include <Poco/ByteOrder.h>
#include <Poco/Any.h>
#include <Poco/DynamicAny.h>

#include <gtest/gtest.h>

TEST(TypesTest, Macro)
{
#ifdef POCO_PTR_IS_64_BIT
	ADD_FAILURE();
#endif

#ifdef POCO_LONG_IS_64_BIT
	ADD_FAILURE();
#endif

#ifdef POCO_ARCH_LITTLE_ENDIAN
	SUCCEED();
#endif

#ifdef POCO_ARCH_BIG_ENDIAN
	SUCCEED();
#endif
}

TEST(TypesTest, ByteOrder)
{
	Poco::UInt32 data = 0x12ab78ef;
	Poco::UInt32 big = Poco::ByteOrder::flipBytes(data);
	EXPECT_EQ(0xef78ab12, big);

	Poco::UInt32 rs = Poco::ByteOrder::toBigEndian(data);
	EXPECT_EQ(0xef78ab12, rs);
	rs = Poco::ByteOrder::toLittleEndian(data);
	EXPECT_EQ(0x12ab78ef, rs);
	rs = Poco::ByteOrder::toNetwork(data);
	EXPECT_EQ(0xef78ab12, rs);

	rs = Poco::ByteOrder::fromBigEndian(data);
	EXPECT_EQ(0xef78ab12, rs);
	rs = Poco::ByteOrder::fromLittleEndian(data);
	EXPECT_EQ(0x12ab78ef, rs);
	rs = Poco::ByteOrder::fromNetwork(data);
	EXPECT_EQ(0xef78ab12, rs);
}

TEST(TypesTest, AnyNumeric)
{
	Poco::Any numeric(19820528L);

	EXPECT_NO_THROW({
		EXPECT_EQ(19820528L, Poco::AnyCast<long>(numeric));
	});

	EXPECT_THROW({
		Poco::AnyCast<int>(numeric);
	}, Poco::BadCastException);
}

TEST(TypesTest, AnyString)
{
	std::string str = "abc";
	Poco::Any anyStr(str);

	EXPECT_NO_THROW({
		std::string castStr = Poco::AnyCast<std::string>(anyStr);
		EXPECT_EQ(str, castStr);
	});

	EXPECT_THROW({
		Poco::AnyCast<int>(anyStr);
	}, Poco::BadCastException);
}

TEST(TypesTest, DynamicAny)
{
	Poco::DynamicAny any(-1234);
	EXPECT_NO_THROW({
		std::string s = any.convert<std::string>();
		EXPECT_EQ("-1234", s);
	});

	EXPECT_NO_THROW({
		Poco::Int16 con = any.convert<Poco::Int16>();
		EXPECT_EQ(-1234, con);
	});

	EXPECT_THROW({
		any.convert<Poco::UInt32>();
	}, Poco::RangeException);
}
