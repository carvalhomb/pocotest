#include <iostream>

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
using Poco::BasicEvent;
using Poco::Delegate;

#include <gtest/gtest.h>

class Source
{
public:
	BasicEvent<int> theEvent;

	void fileEvent(int n)
	{
		theEvent.notify(this, n);
		// theEvent(this, n); // alternative syntax
	}
};

class Target
{
public:
	int eventArgs;

	void handleEvent(const void *sender, int &evArgs)
	{
		eventArgs = evArgs;
	}
};

TEST(EventTest, all)
{
	Source source;
	Target target;

	source.theEvent += Poco::delegate(&target, &Target::handleEvent);

	source.fileEvent(99);
	EXPECT_EQ(99, target.eventArgs);

	source.theEvent -= Poco::delegate(&target, &Target::handleEvent);

	source.fileEvent(0);
	EXPECT_EQ(99, target.eventArgs);
}
