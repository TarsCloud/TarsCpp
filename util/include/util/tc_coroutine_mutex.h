#pragma once

#include <memory>
#include <list>
#include <mutex>
#include "util/tc_coroutine.h"
#include "util/tc_spin_lock.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_coroutine_mutex.h 
 * @brief 协程锁互斥类
 *  
 * @author edisenwang
 */
/////////////////////////////////////////////////

/**
 * @brief 协程锁 . 
 *
 *  开启协程后使用
 */
class UTIL_DLL_API TC_CoMutex 
{
protected:
	struct CoMutexInfo
	{
		const std::shared_ptr<TC_CoroutineScheduler> _sched;
		uint32_t _coroId;

		CoMutexInfo(const std::shared_ptr<TC_CoroutineScheduler> sched, uint32_t coroId):_sched(sched), _coroId(coroId){}
	};

public:
	TC_CoMutex();

	virtual ~TC_CoMutex();

	/**
	 * @brief 尝试锁
	 * 
	 * @return bool
	 */
	bool try_lock();

	/**
	 * @brief 加锁
	 */
	void lock();

	/**
	 * @brief 解锁
	 */
	void unlock();

protected:

	// noncopyable
	TC_CoMutex(const TC_CoMutex&) = delete;
	void operator=(const TC_CoMutex&) = delete;

protected:

	TC_SpinLock _slck;

	mutable std::mutex _mutex;

	std::list<CoMutexInfo> _list;
};
}

