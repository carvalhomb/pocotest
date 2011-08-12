#include <Poco/Exception.h>
#include <Poco/Platform.h>
#include <Poco/Environment.h>
#include <gtest/gtest.h>

// Macro: Operating System, Hardware Architecture, Byte Order
TEST(PlatformEnvironmentTest, Platform)
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	// do the Windows thing
#elif POCO_OS == POCO_OS_LINUX
	// do the Linux thing
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
	// do the Windows thing
#elif defined(POCO_OS_FAMILY_UNIX)
	// do the Unix thing
#endif

#if POCO_ARCH == POCO_ARCH_IA32
	// do the Intel 32-bit thing
#elif POCO_ARCH == ARM
	// do the ARM thing
#endif

#if defined(POCO_ARCH_LITTLE_ENDIAN)
	// little endian
#elif defined(POCO_ARCH_BIG_ENDIAN)
	// big endian
#endif

	SUCCEED();
}

TEST(PlatformEnvironmentTest, Environment)
{
	EXPECT_NO_THROW({
#if defined(POCO_OS_FAMILY_WINDOWS)
		std::string path = Poco::Environment::get("path");
#elif defined(POCO_OS_FAMILY_UNIX)
		std::string path = Poco::Environment::get("PATH");
#endif
		EXPECT_TRUE(path.length() != 0);
	});

	EXPECT_THROW({
		Poco::Environment::get("NotExpectedExistVariable");
	}, Poco::NotFoundException);

	EXPECT_FALSE(Poco::Environment::has("NotExpectedExistVariable"));

	std::string setValue = "justTest";
	Poco::Environment::set("MustExistVariable", setValue);
	EXPECT_NO_THROW({
		EXPECT_EQ(setValue, Poco::Environment::get("MustExistVariable"));
	});
	
	EXPECT_FALSE(Poco::Environment::osName().empty());
	EXPECT_FALSE(Poco::Environment::osVersion().empty());
	EXPECT_FALSE(Poco::Environment::osArchitecture().empty());
	EXPECT_FALSE(Poco::Environment::nodeName().empty());
	EXPECT_FALSE(Poco::Environment::nodeId().empty());
}
