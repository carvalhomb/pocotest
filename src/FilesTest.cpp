#include <Poco/Platform.h>
#include <Poco/Path.h>
#include <Poco/Environment.h>

#include <gtest/gtest.h>

TEST(FilesTest, PathTest)
{
	Poco::Path p(true); // path will be absolute

#if defined(POCO_OS_FAMILY_WINDOWS)
	p.setDevice("C");
#endif
	p.pushDirectory("Dir1");
	p.pushDirectory("Dir2");
	p.setFileName("test.ext");

#if defined(POCO_OS_FAMILY_WINDOWS)
	EXPECT_EQ("C:\\Dir1\\Dir2\\test.ext", p.toString(Poco::Path::PATH_WINDOWS));
#elif defined(POCO_OS_FAMILY_UNIX)
	EXPECT_EQ("/Dir1/Dir2/test.ext", p.toString(Poco::Path::PATH_UNIX));
#endif

	// "VMS001::DSK001:[POCO.INCLUDE.POCO]POCO.H"
	p.clear(); // start over with a clean state
	p.pushDirectory("projects");
	p.pushDirectory("poco");
	EXPECT_EQ("projects\\poco\\", p.toString(Poco::Path::PATH_WINDOWS));
	EXPECT_EQ("projects/poco/", p.toString(Poco::Path::PATH_UNIX));

#if defined(POCO_OS_FAMILY_WINDOWS)
	EXPECT_EQ("projects\\poco\\", p.toString());
#elif defined(POCO_OS_FAMILY_UNIX)
	EXPECT_EQ("projects/poco/", p.toString());
#endif
}

TEST(FilesTest, PathFromStringTest)
{
	Poco::Path p("C:\\Windows\\system32\\cmd.exe");
	Poco::Path p1("/bin/sh");
	p = "projects\\poco";
	p1 = "projects/poco";

	EXPECT_NO_THROW({
		p.parse("/usr/include/stdio.h", Poco::Path::PATH_UNIX);
	});

	EXPECT_TRUE(p.tryParse("/usr/*/stdio.h"));
	EXPECT_TRUE(p.tryParse("/usr/include/stdio.h", Poco::Path::PATH_UNIX));
	EXPECT_TRUE(p.tryParse("/usr/include/stdio.h", Poco::Path::PATH_WINDOWS));
}

TEST(FilesTest, PathPartsTest)
{
	Poco::Path p("c:\\projects\\poco\\build_vs80.cmd", Poco::Path::PATH_WINDOWS);
	EXPECT_EQ("c", p.getDevice()); // "c"
	EXPECT_EQ(2, p.depth()); // 2
	EXPECT_EQ("projects", p.directory(0)); // "projects"
	EXPECT_EQ("poco", p[1]); // "poco"
	EXPECT_EQ("build_vs80.cmd", p[2]); // "build_vs80.cmd"
	EXPECT_EQ("build_vs80.cmd", p.getFileName());
	EXPECT_EQ("build_vs80", p.getBaseName()); // "build_vs80"
	EXPECT_EQ("cmd", p.getExtension()); // "cmd"

	p.setBaseName("build_vs71");
	EXPECT_EQ("build_vs71.cmd", p.getFileName()); // "build_vs71.cmd"
}

TEST(FilesTest, PathOperationTest)
{
	Poco::Path p("/usr/include/stdio.h", Poco::Path::PATH_UNIX);
	Poco::Path parent(p.parent());
	EXPECT_EQ("/usr/include/", parent.toString(Poco::Path::PATH_UNIX));

	Poco::Path p1("stdlib.h");
	Poco::Path p2("/opt/Poco/include/Poco.h", Poco::Path::PATH_UNIX);
	p.resolve(p1);
	EXPECT_EQ("/usr/include/stdlib.h", p.toString(Poco::Path::PATH_UNIX));

	p.resolve(p2);
	EXPECT_EQ("/opt/Poco/include/Poco.h", p.toString(Poco::Path::PATH_UNIX));

	EXPECT_TRUE(p.isAbsolute());
	EXPECT_FALSE(p.isRelative());
	EXPECT_FALSE(p.isDirectory());
	EXPECT_TRUE(p.isFile());
}

TEST(FilesTest, PathSystemSpecialTest)
{
	Poco::Path::current();
	Poco::Path::home();
	Poco::Path::temp();
	Poco::Path::null();
}

TEST(FilesTest, PathFindTest)
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	std::string shellName("cmd.exe");
#elif defined(POCO_OS_FAMILY_UNIX)
	std::string shellName("sh");
#endif
	std::string path(Poco::Environment::get("PATH"));
	Poco::Path shellPath;
	EXPECT_TRUE(Poco::Path::find(path, shellName, shellPath));

#if defined(POCO_OS_FAMILY_WINDOWS)
	EXPECT_EQ("C:\\Windows\\system32\\cmd.exe", shellPath.toString());
#elif defined(POCO_OS_FAMILY_UNIX)
	EXPECT_EQ("/usr/bin/sh", shellPath.toString());
#endif
}
