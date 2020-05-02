
#ifndef __TC_SPIN_LOCK_H
#define __TC_SPIN_LOCK_H

#include "util/tc_platform.h"
#include <atomic>
#include <memory>

using namespace std;

namespace tars
{

/**
 * 自旋锁
 * 不能阻塞wait, 只能快速加解锁, 适用于锁粒度非常小的情况, 减小线程切换的开销
 * 不支持trylock
 */
class UTIL_DLL_API TC_SpinLock
{
public:

	TC_SpinLock();
	virtual ~TC_SpinLock();

	void lock() const;
    bool tryLock() const;
    void unlock() const;

private:

	TC_SpinLock(const TC_SpinLock&) = delete;
	TC_SpinLock(TC_SpinLock&&) = delete;
	TC_SpinLock& operator=(const TC_SpinLock&) = delete;
	TC_SpinLock& operator=(TC_SpinLock&&) = delete;

private:

	mutable std::atomic_flag _flag;
};

}
#endif
