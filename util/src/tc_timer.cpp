/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "util/tc_timer.h"
#include "util/tc_logger.h"

namespace tars
{

TC_TimerBase::~TC_TimerBase()
{
}

void TC_TimerBase::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);

	_mapEvent.clear();
	_mapTimer.clear();
	_repeatIds.clear();
	_nextTimer = -1;
}

size_t TC_TimerBase::count()
{
	std::lock_guard<std::mutex> lock(_mutex);

	return _mapEvent.size();
}

size_t TC_TimerBase::repeatCount()
{
	std::lock_guard<std::mutex> lock(_mutex);

	return _repeatIds.size();
}

uint32_t TC_TimerBase::genUniqueId()
{
	uint32_t i = ++_increaseId;
	if (i == 0)
	{
		i = ++_increaseId;
	}

	return i;
}


bool TC_TimerBase::exist(int64_t uniqId,bool repeat )
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (repeat)
	{
		return  _repeatIds.find(uniqId) != _repeatIds.end();
	}
	else
	{
		return _mapEvent.find(uniqId) != _mapEvent.end();
	}
}

void TC_TimerBase::erase(int64_t uniqId)
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _mapEvent.find(uniqId);

	//LOG_CONSOLE_DEBUG << "before erase event!" << ",uniqId=" << uniqId << "|event size :" << _mapEvent.size() << "|timer size:" << _mapTimer.size() << endl;
	if (it != _mapEvent.end())
	{
		auto itEvent = _mapTimer.find(it->second->_fireMillseconds);
		if (itEvent != _mapTimer.end())
		{
			itEvent->second.erase(uniqId);
			if (itEvent->second.empty())
			{
				_mapTimer.erase(itEvent);
			}
		}
		it->second->_func = nullptr;
		_mapEvent.erase(it);
	}
	_repeatIds.erase(uniqId);
}

int64_t TC_TimerBase::post(const shared_ptr<TC_TimerBase::Func> & event, bool repeat)
{
	std::unique_lock <std::mutex> lock(_mutex);

	int64_t uniqId = event->_uniqueId;
	if (repeat) {
		_repeatIds.insert(uniqId);
	}

	if(_mapEvent.find(uniqId) == _mapEvent.end()) {
		_mapEvent[uniqId] = event;
	}

	_mapTimer[event->_fireMillseconds].insert(uniqId);

	// LOG_CONSOLE_DEBUG << "fireMillseconds:" << event->_fireMillseconds << ", " << TNOWMS << ", " << event->_fireMillseconds - TNOWMS << endl;

	if (_nextTimer < 0)
	{
		//无下一个事件了, 且还没有wait, 不需要onAddTimer去唤醒wait(在epoll wait醒来后,把定时器处理完, 如果是重复定时器, 就会进入这种状态)
		_nextTimer = event->_fireMillseconds;
		onAddTimer();
	}
	else if(event->_fireMillseconds < (uint64_t)_nextTimer)
	{
		//新时间更近, 需要唤醒wait, 等到最新时间上
		_nextTimer = (uint64_t)event->_fireMillseconds;
		onAddTimer();
	}

	// LOG_CONSOLE_DEBUG << _nextTimer << ", " << TNOWMS - _nextTimer << endl;
	
	return uniqId;
}

int64_t TC_TimerBase::fireEvents(int64_t ms)
{
	//get timer event
	EVENT_SET el;

	//获得事件, 返回下一次时间
	getEvents(el);

	auto itList = el.begin();

	while (itList != el.end())
	{
		shared_ptr<Func> func ;

		{
			std::lock_guard<std::mutex> lock(_mutex);

			auto it = _mapEvent.find(*itList);

			if (it != _mapEvent.end()) {

				func = it->second;

				_tmpEvent[it->first] = it->second;

				_mapEvent.erase(it);
			}
		}

		if(func) {
			//执行具体事件对象
			onFireEvent(func->_func);
		}

		++itList;
	}

	std::lock_guard <std::mutex> lock(_mutex);

	int64_t waitTime = ms;

	if(_nextTimer > 0)
	{
		waitTime = _nextTimer - TNOWMS;
	}

	if(waitTime < 0)
	{
		waitTime = 0;
	}

	return waitTime;
}

int64_t TC_TimerBase::getEvents(TC_TimerBase::EVENT_SET &el)
{
	std::unique_lock <std::mutex> lock(_mutex);

	_nextTimer = -1;

	for (auto it = _mapTimer.begin(); it != _mapTimer.end();)
	{
		if ((uint64_t)it->first <= TNOWMS)
		{
			//时间过了, 有事件需要触发了
			el.insert(it->second.begin(), it->second.end());
			_mapTimer.erase(it++);
		}
		else
		{
			//时间还没到
			_nextTimer = it->first;
			break;
		}
	}

	return _nextTimer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
TC_Timer::~TC_Timer()
{
	stopTimer();
}

void TC_Timer::startTimer(int numThread)
{
	if (numThread <= 0)
	{
		numThread = 1;
	}

	_terminate = false;

	//多个线程, 其中一个给TC_Timer::run使用
	_tpool.init(numThread + 1);
	_tpool.start();
	_tpool.exec(std::bind(&TC_Timer::run, this));
}

void TC_Timer::stopTimer()
{
	if (_terminate)
	{
		return;
	}

	{
		std::unique_lock<std::mutex> lck(_mutex);
		_terminate = true;
		_cond.notify_all();
	}

	_tpool.stop();
}

void TC_Timer::onAddTimer()
{
	_cond.notify_one();
}

void TC_Timer::onFireEvent(std::function<void()> func)
{
	//执行具体事件对象
	_tpool.exec(func);
}


tuple<int64_t, int64_t, int64_t> TC_Timer::status()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return make_tuple(_tpool.getJobNum(), _mapEvent.size(), _repeatIds.size());
}

void TC_Timer::run()
{
	while (!_terminate)
	{
		try
		{
			fireEvents(1000);

			std::unique_lock <std::mutex> lock(_mutex);

			uint64_t ms = 1000;
			
			if (_nextTimer > 0)
			{
				ms = _nextTimer - TNOWMS;
			}
			_cond.wait_for(lock, std::chrono::milliseconds(ms));
		}
		catch (exception & ex) {
			cerr << ex.what() << endl;
		}
	}
}

}


