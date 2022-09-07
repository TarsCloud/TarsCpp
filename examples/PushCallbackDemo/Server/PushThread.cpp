//
// Created by jarod on 2022/9/7.
//

#include "PushThread.h"
#include "Push.h"

void PushThread::terminate()
{
	std::lock_guard<std::mutex> lock(_mutex);
	_terminate = true;
	_cond.notify_one();

}

void PushThread::addCurrent(CurrentPtr &current)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_currents[current->getUId()] = current;
}

void PushThread::delCurrent(CurrentPtr &current)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_currents.erase(current->getUId());
}

void PushThread::run()
{
	while(!_terminate)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		for(auto it : _currents)
		{
			TestApp::Push::async_response_push_pushMsg(it.second, 0, "push message");
		}

		_cond.wait_for(lock, std::chrono::milliseconds(1000));
	}

}
