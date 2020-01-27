
#ifndef __TC_SPIN_LOCK_H
#define __TC_SPIN_LOCK_H

//#include <mutex>
#include <atomic>
//#include <condition_variable>
#include <memory>

using namespace std;

namespace tars
{

/**
 * 自旋锁
 * 不能阻塞wait, 只能快速加解锁, 适用于锁粒度非常小的情况, 减小线程切换的开销
 * 不支持trylock
 */
class TC_SpinLock
{
public:

	TC_SpinLock();
	virtual ~TC_SpinLock();

	void lock() const;
    bool tryLock() const;
    void unlock() const;
protected:
//	struct ticket_lock
//	{
//		atomic<int> now_serving{0};
//		atomic<int> next_ticket{0};
//	};
//
//	struct node
//	{
//		volatile int isLocked   = true;
//		atomic<node*> next{NULL};
//	};
private:

	TC_SpinLock(const TC_SpinLock&) = delete;
	TC_SpinLock(TC_SpinLock&&) = delete;
	TC_SpinLock& operator=(const TC_SpinLock&) = delete;
	TC_SpinLock& operator=(TC_SpinLock&&) = delete;

private:

//	mutable ticket_lock     _lock;
//	static thread_local shared_ptr<node> _currNode;
//
	mutable std::atomic_flag _flag;
//
//	mutable atomic<node*> _tail{NULL};
};
//
//class TC_SpinMutex
//{
//
//public:
//
//	explicit TC_SpinMutex(int trys = 200) : _tryCounts(trys)
//	{
//		_flag.clear(std::memory_order_release);
//	}
//	virtual ~TC_SpinMutex() = default;
//
//public:
//
//	void lock() const
//	{
//		for (int millis = 5; ;millis = (millis < 100 ? millis << 2 : millis))
//		{
//			int trys = _tryCounts;
//			for (; trys > 0 && _flag.test_and_set(std::memory_order_acquire); --trys);
//			if (trys > 0)
//				return;
//
//			std::unique_lock<std::mutex> ulk(_mutex);
//			_cond.wait_for(ulk, std::chrono::milliseconds(millis));
//		}
//	}
//
//	bool tryLock() const
//	{
//		int trys = _tryCounts;
//		for (; trys > 0 && _flag.test_and_set(std::memory_order_acquire); --trys);
//		if (trys > 0)
//			return true;
//		return false;
//	}
//
//	void unlock() const
//	{
//		_flag.clear(std::memory_order_release);
//		_cond.notify_all();
//	}
//
//private:
//
//	TC_SpinMutex(const TC_SpinMutex&) = delete;
//	TC_SpinMutex(TC_SpinMutex&&) = delete;
//	TC_SpinMutex& operator=(const TC_SpinMutex&) = delete;
//	TC_SpinMutex& operator=(TC_SpinMutex&&) = delete;
//
//private:
//
//	mutable std::condition_variable _cond;
//	mutable std::mutex              _mutex;
//	mutable std::atomic_flag        _flag;
//	int                     _tryCounts{ 200 };
//
//};

}
#endif
