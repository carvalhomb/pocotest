#include <Poco/NotificationCenter.h>
#include <Poco/Notification.h>
#include <Poco/NObserver.h>
#include <Poco/Observer.h>
#include <Poco/AutoPtr.h>

#include <gtest/gtest.h>

using Poco::NotificationCenter;
using Poco::Notification;
using Poco::Observer;
using Poco::NObserver;
using Poco::AutoPtr;

class BaseNotification: public Notification {
};
class SubNotification: public BaseNotification {
};

class Target {
public:
	void handleBase(BaseNotification* pNf) {
		EXPECT_FALSE(pNf->name().empty());
		pNf->release(); // we got ownership, so we must release
	}
	void handleSub(const AutoPtr<SubNotification>& pNf) {
		EXPECT_FALSE(pNf->name().empty());
	}
};

TEST(NotificationCenterTest, all)
{
	NotificationCenter nc;
	Target target;
	nc.addObserver(
			Observer<Target, BaseNotification> (target, &Target::handleBase));
	nc.addObserver(
			NObserver<Target, SubNotification> (target, &Target::handleSub));
	nc.postNotification(new BaseNotification);
	nc.postNotification(new SubNotification); // handleBase and handleSub both got this
	nc.removeObserver(
			Observer<Target, BaseNotification> (target, &Target::handleBase));
	nc.removeObserver(
			NObserver<Target, SubNotification> (target, &Target::handleSub));
}
