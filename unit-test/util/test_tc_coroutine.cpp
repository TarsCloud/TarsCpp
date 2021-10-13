//
// Created by jarod on 2020/2/20.
//

#include "util/tc_coroutine.h"
#include "util/tc_coroutine_queue.h"
#include "util/tc_thread.h"
#include "gtest/gtest.h"

using namespace tars;

class UtilCoroutineTest : public testing::Test
{
public:
    //添加日志
    static void SetUpTestCase()
    {
//        cout<<"SetUpTestCase"<<endl;
    }
    static void TearDownTestCase()
    {
//        cout<<"TearDownTestCase"<<endl;
    }
    virtual void SetUp()   //TEST跑之前会执行SetUp
    {
//        cout<<"SetUp"<<endl;
    }
    virtual void TearDown() //TEST跑完之后会执行TearDown
    {
//        cout<<"TearDown"<<endl;
    }
};

class CoSleepThread : public TC_Thread
{
public:
	CoSleepThread() {}

	virtual void run()
	{
		int64_t at = TC_Common::now2ms();
		int64_t lt = 0;
		_scheduler->sleep(100);
		lt = TC_Common::now2ms();
		ASSERT_TRUE(lt - at >= 100 && lt - at <= 106);
		at = lt;

		_scheduler->sleep(100);
		lt = TC_Common::now2ms();
		ASSERT_TRUE(lt - at >= 100 && lt - at <= 106);
		at = lt;

		_scheduler->sleep(100);
		lt = TC_Common::now2ms();
		ASSERT_TRUE(lt - at >= 100 && lt - at <= 106);
	}
};

class CoYieldThread : public TC_Thread
{
public:
	CoYieldThread() {}

	virtual void run()
	{
		int64_t lt, at;
		int coroId = this->getScheduler()->getCoroutineId();

		{
			std::thread check([&]()
			                  {
				                  TC_Common::msleep(500);

				                  this->_scheduler->put(coroId);
			                  });

			at = TC_Common::now2ms();
			lt = 0;
			_scheduler->yield(false);
			lt = TC_Common::now2ms();

			ASSERT_TRUE(lt - at >= 500 && lt - at <= 506);

			check.join();
		}

		{
			std::thread check([&]()
			                  {
				                  TC_Common::msleep(500);

				                  this->_scheduler->notify();
			                  });
			at = lt;
			_scheduler->yield(true);
			lt = TC_Common::now2ms();
			ASSERT_TRUE(lt - at >= 500 && lt - at <= 506);
			check.join();
		}
	}
};

bool CoThreadDo = false;
class CoThread : public TC_Thread
{
public:
	CoThread() {}

	virtual void run()
	{
		CoThreadDo = true;
		cout << "in coroutine" << endl;
	}
};

TEST_F(UtilCoroutineTest, CoThread)
{
	CoThreadDo = false;

	CoThread* a = new CoThread();

	a->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	ASSERT_TRUE(CoThreadDo);

	delete a;

}

class CoThread1 : public TC_Thread
{
public:
	CoThread1() {}

	virtual void run()
	{
		TC_CoroutineScheduler::scheduler()->go(std::bind(&CoThread1::doCo, this));
		LOG_CONSOLE_DEBUG << "in coroutine" << endl;
	}

	void doCo()
	{
		CoThreadDo = true;
		LOG_CONSOLE_DEBUG << "in coroutine" << endl;
	}
};

TEST_F(UtilCoroutineTest, CoThread1)
{
	CoThreadDo = false;
 
	CoThread1* a = new CoThread1();

	a->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	ASSERT_TRUE(CoThreadDo);

	delete a;
}

class CoAThread : public TC_Thread
{
public:
	CoAThread(TC_CoroutineQueue<int> * data): _queue(data) {}

	virtual void run()
	{
		for(int i = 0; i < 100; i++) {
			_queue->push_back(i);
			_scheduler->sleep(10);
		}
	}

	TC_CoroutineQueue<int> *_queue;
};

class CoBThread : public TC_Thread
{
public:
	CoBThread(TC_CoroutineQueue<int> * data): _queue(data) {}

	virtual void run()
	{
		_queue->exec([&](int i) {

			if(_last != -1) {
				ASSERT_TRUE(_last + 1 == i);
			}

			_last = i;
		});
	}
	int _last = -1;
	TC_CoroutineQueue<int> *_queue;
};

TEST_F(UtilCoroutineTest, coThreadQuit)
{
	TC_CoroutineQueue<int> * data = new TC_CoroutineQueue<int>();

	CoAThread* a = new CoAThread(data);

	a->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	ASSERT_TRUE(data->size() == 100);

	delete data;
	delete a;
}

TEST_F(UtilCoroutineTest, coThread)
{
	TC_CoroutineQueue<int> * data = new TC_CoroutineQueue<int>();

	CoAThread* a = new CoAThread(data);

	a->startCoroutine(10, 128*1024, false);

	std::thread start([=]{
		TC_Common::sleep(2);

		ASSERT_TRUE(data->size() == 100 );

		a->getScheduler()->terminate();
	});

	a->getThreadControl().join();
	start.join();

	delete data;
	delete a;
}


TEST_F(UtilCoroutineTest, coQueue)
{
	TC_CoroutineQueue<int> * data = new TC_CoroutineQueue<int>();
	CoAThread* a = new CoAThread(data);
	CoBThread* b = new CoBThread(data);

	a->startCoroutine(10, 128*1024, true);
	b->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	b->_queue->terminate();
	b->getThreadControl().join();

	delete data;
	delete a;
	delete b;

}

TEST_F(UtilCoroutineTest, sleep)
{
	CoSleepThread* a = new CoSleepThread();

	a->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	delete a;
}

TEST_F(UtilCoroutineTest, yield)
{
	CoYieldThread* a = new CoYieldThread();

	a->startCoroutine(10, 128*1024, true);

	a->getThreadControl().join();

	delete a;
}

TEST_F(UtilCoroutineTest, aliveQuit)
{
	CoSleepThread* a = new CoSleepThread();

	a->startCoroutine(10, 128*1024, true);

	TC_Common::msleep(100);

	ASSERT_TRUE(a->isAlive());

	a->getThreadControl().join();

	delete a;
}

TEST_F(UtilCoroutineTest, alive)
{
	CoSleepThread* a = new CoSleepThread();

	a->startCoroutine(10, 128*1024, false);

	TC_Common::msleep(200);

	ASSERT_TRUE(a->isAlive());

	a->getScheduler()->terminate();
	a->getThreadControl().join();

	delete a;
}


class MyCoroutine : public TC_Coroutine
{
protected:
	void handle()
	{
		++_count;

		this->go(std::bind(&MyCoroutine::co_test, this));
	}

	void co_test()
	{
		++_count;
	}

public:
	static atomic<int> _count;
};

atomic<int> MyCoroutine::_count{0};

TEST_F(UtilCoroutineTest, coCoroutine)
{
	MyCoroutine::_count = 0;

	MyCoroutine co;

	co.setCoroInfo(10, 200, 128*1024);

	co.start();

	co.join();

	ASSERT_TRUE(MyCoroutine::_count == 20);
}

TEST_F(UtilCoroutineTest, coCoroutineQuit)
{
	std::thread cor_call([&]()
	{
		auto scheduler = TC_CoroutineScheduler::create();

		scheduler->go([&]()
		{

			scheduler->setNoCoroutineCallback([=](TC_CoroutineScheduler* s)
			{
				s->terminate();
			});

			TC_Common::msleep(100);
		});

		scheduler->run();
	});
	cor_call.join();
}
