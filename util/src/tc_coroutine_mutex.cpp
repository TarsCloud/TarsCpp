#include "util/tc_coroutine_mutex.h"

namespace tars
{
	TC_CoMutex::TC_CoMutex()
	{
	}

	TC_CoMutex::~TC_CoMutex()
	{
	}

	bool TC_CoMutex::try_lock() 
	{
		return _slck.tryLock();
	}

	void TC_CoMutex::lock()
	{
		if(_slck.tryLock())
		{
			return ;
		}
		else
		{
			std::shared_ptr<TC_CoroutineScheduler> sched = TC_CoroutineScheduler::scheduler();
			assert(sched && sched->getCoroutineId() != 0);
			{
				std::lock_guard<std::mutex> lock(_mutex);
				//加锁后再检查一次, 如果获取到执行条件则直接退出
				if(_slck.tryLock())
				{
					return ;
				}

				_list.emplace_back(CoMutexInfo(sched, sched->getCoroutineId()));
			}

			sched->yield(false);
		}
	}

	void TC_CoMutex::unlock()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if(!_list.empty())
		{
			CoMutexInfo &info= _list.front();
			info._sched->put(info._coroId);
			_list.pop_front();
		}
		else
		{
			_slck.unlock();
		}
	}
}
