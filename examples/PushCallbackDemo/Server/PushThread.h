//
// Created by jarod on 2022/9/7.
//

#ifndef FRAMEWORK_PUSHTHREAD_H
#define FRAMEWORK_PUSHTHREAD_H

#include <mutex>
#include "util/tc_thread.h"
#include "servant/Application.h"
using namespace tars;

class PushThread : public TC_Thread
{
public:
	void addCurrent(CurrentPtr &current);
	void delCurrent(CurrentPtr &current);

	void terminate();
protected:
	virtual void run();

protected:
	bool _terminate = false;
	std::mutex _mutex;
	std::condition_variable _cond;
	map<int, CurrentPtr> _currents;
};



#endif //FRAMEWORK_PUSHTHREAD_H
