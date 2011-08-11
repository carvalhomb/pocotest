#include <stdio.h>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	int testResult = RUN_ALL_TESTS();

	// pause the screen
	printf("\nPress Enter key to exit... ");
	fgetc(stdin);

	return testResult;
}
