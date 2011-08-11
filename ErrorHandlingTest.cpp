#include <Poco/Exception.h>

#include <gtest/gtest.h>

TEST(ErrorHandlingTest, Exception)
{
	EXPECT_THROW({
		throw Poco::ApplicationException("just testing");
	}, Poco::ApplicationException);

	Poco::Exception *pExc = NULL;
	EXPECT_NO_THROW({
		try {
			throw Poco::ApplicationException("again testing");
		} catch (Poco::Exception& exc) {
			pExc = exc.clone();

			EXPECT_STREQ(pExc->name(), exc.name());
			EXPECT_STREQ("Application exception", pExc->name());

			EXPECT_EQ(pExc->message(), exc.message());
			EXPECT_EQ("again testing", pExc->message());
		}
	});

	EXPECT_THROW({
		pExc->rethrow();
	}, Poco::ApplicationException);

	delete pExc;
}
