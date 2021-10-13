#include "util/tc_common.h"
#include "util/tc_cas_queue.h"
#include "util/tc_thread.h"
#include "util/tc_autoptr.h"
#include "util/tc_spin_lock.h"
#include "util/tc_thread_queue.h"
#include "gtest/gtest.h"

#include <mutex>
#include <iostream>

using namespace tars;

class UtilCasTest : public testing::Test
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

TEST_F(UtilCasTest, casLock)
{
	size_t i=0;
	size_t count = 1000000;

	TC_SpinLock mutex;
	std::thread add([&]{
		int j = count;
		while(j--) {
			TC_LockT<TC_SpinLock> lock(mutex);
			++i;
		}
	});

	std::thread del([&]{
		int j = count;
		while(j--) {
			TC_LockT<TC_SpinLock> lock(mutex);
			--i;
		}
	});

	add.join();
	del.join();
	ASSERT_TRUE(i == 0);
}

bool g_terminate = false;
TC_CasQueue<int64_t> data_cas;
TC_ThreadQueue<int64_t> data_queue;

class WriteThread : public TC_Thread
{
protected:
	virtual void run()
	{
		for(int64_t i = 0; i < 100000; i++) {
			data_cas.push_back(i);
		}
	}
};

class ReadThread : public TC_Thread, public TC_HandleBase
{
public:
	ReadThread() {}

	virtual void run()
	{
		int64_t last = -1;
		while(!g_terminate) {
			int64_t i;
			if(data_cas.pop_front(i))
			{
				ASSERT_TRUE(i - last == 1);
				last = i;

				if(i == 100000-1)
				{
					g_terminate = true;
				}
			}
		}
	}
};

TEST_F(UtilCasTest, casQueue)
{
	WriteThread w;
	ReadThread r;

	w.start();
	r.start();

	w.join();
	r.join();
}

template<typename Q>
void start(int w, int r, int sleeps, Q &queue_data)
{
	g_terminate = false;

	atomic<int64_t> writeIndex {0};
	vector<std::thread*> wthreads;
	for(int i = 0 ; i < w; i++) {
		wthreads.push_back(new std::thread([&]
		                                   {
			                                   while(!g_terminate)
			                                   {
				                                   queue_data.push_back(++writeIndex);
			                                   }
		                                   }));
	}

	int64_t readIndex = 0;

	vector<std::thread*> rthreads;
	for(int i = 0 ; i < r; i++)
	{
		rthreads.push_back(new std::thread([&]{
			do{
				int64_t j;
				if(queue_data.pop_front(j))
				{
					readIndex = j;
				}
				else
				{
					if(sleeps > 0)
					{
						TC_Common::msleep(sleeps);
					}
				}

			}while(!g_terminate);
		}));
	}

	std::thread print([&]{
		int64_t lastReadIndex = 0;
		int64_t lastWriteIndex = 0;
		while(!g_terminate) {
			cout << "size:" << data_queue.size() << ", write/read index:" << writeIndex/10000. << "/" << readIndex/10000. << ", " << (writeIndex-lastWriteIndex)/10000. << ", " << (readIndex - lastReadIndex)/10000. << endl;
			lastReadIndex = readIndex;
			lastWriteIndex = writeIndex;
			TC_Common::sleep(1);
		}
	});

	std::thread t([&]{
		TC_Common::sleep(10);
		g_terminate = true;
	});
	t.join();
	print.join();
	for(size_t i = 0; i < wthreads.size(); i++)
	{
		wthreads[i]->join();
		delete wthreads[i];
	}
	for(size_t i = 0; i < rthreads.size(); i++)
	{
		rthreads[i]->join();
		delete rthreads[i];
	}
}

int wThread = 1;
int rThread = 4;
int sleepms = 0;

TEST_F(UtilCasTest, queueBatch)
{
	cout << "threadQueueBatch-------------------------------------------" << endl;
	start<TC_ThreadQueue<int64_t>>(wThread, rThread, sleepms, data_queue);

	cout << "casQueueBatch-------------------------------------------" << endl;
	start<TC_CasQueue<int64_t>>(wThread, rThread, sleepms, data_cas);
}