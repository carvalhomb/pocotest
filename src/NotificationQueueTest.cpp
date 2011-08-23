#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"

using Poco::Notification;
using Poco::NotificationQueue;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;
using Poco::FastMutex;
using Poco::AutoPtr;

#include <gtest/gtest.h>

// The notification sent to worker threads.
class ClacNotification: public Notification
{
public:
	typedef AutoPtr<ClacNotification> Ptr;

	ClacNotification(int oprnd1, int oprnd2, char optor)
			: mOprnd1(oprnd1), mOprnd2(oprnd2), mOptor(optor) {
	}

	int calcResult() const {
		int result;

		switch (mOptor) {
			case '+':
				result = mOprnd1 + mOprnd2;
				break;
			case '-':
				result = mOprnd1 - mOprnd2;
				break;
			case '*':
				result = mOprnd1 * mOprnd2;
				break;
			case '/':
				if (mOprnd2 != 0)
					result = mOprnd1 / mOprnd2;
				else
					result = -1;
				break;
			default:
				result = 0;
				break;
		}
		return result;
	}

private:
	int mOprnd1;
	int mOprnd2;
	char mOptor;
};

// quit notification send to worker thread
class QuitNotification: public Notification
{
public:
	typedef AutoPtr<QuitNotification> Ptr;
};

// A worker thread that gets work items
// from a NotificationQueue.
class Worker: public Runnable
{
public:
	Worker(const std::string& name, NotificationQueue& queue):
		_name(name),
		_queue(queue)
	{
	}

	void run()
	{
		Poco::Random rnd;
		for (;;)
		{
			Notification::Ptr pNf(_queue.waitDequeueNotification());
			if (pNf)
			{
				ClacNotification::Ptr pWorkNf = pNf.cast<ClacNotification>();
				if (pWorkNf)
				{
					{
						FastMutex::ScopedLock lock(_mutex); // used for std::cout
						// std::cout << _name << " got work notification " << pWorkNf->calcResult() << std::endl;
						pWorkNf->calcResult();
					}
					Thread::sleep(rnd.next(200));
					continue;
				}

				QuitNotification::Ptr pQuitNf = pNf.cast<QuitNotification>();
				if (pQuitNf)
					break;
			}
			else
			{
				break;
			}
		}
	}

private:
	std::string        _name;
	NotificationQueue& _queue;
	static FastMutex   _mutex;
};
FastMutex Worker::_mutex; // status member


TEST(NotificationQueueTest, all)
{
	NotificationQueue queue;

	// create some worker threads
	Worker worker1("Worker 1", queue);
	Worker worker2("Worker 2", queue);
	Worker worker3("Worker 3", queue);

	// start worker threads
	ThreadPool::defaultPool().start(worker1);
	ThreadPool::defaultPool().start(worker2);
	ThreadPool::defaultPool().start(worker3);

	Poco::Random rnd;
	char optors[] = "+-*/";
	// distribute some work
	for (int i = 0; i < 30; ++i)
	{
		queue.enqueueNotification(new ClacNotification(rnd.next(100), rnd.next(100), optors[i%4]));
	}

	// three worker, three quit notification
	queue.enqueueNotification(new QuitNotification);
	queue.enqueueNotification(new QuitNotification);
	queue.enqueueNotification(new QuitNotification);

	// wait until queue is empty and all threads are
	// waiting for new work.
	while (!queue.empty()) Thread::sleep(200);
	Thread::sleep(500);

	ThreadPool::defaultPool().joinAll();
}
