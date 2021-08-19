
#include "util/tc_spin_lock.h"
#include "util/tc_common.h"

#include <thread>
#include <iostream>
#include <cassert>
using namespace std;

#define TRYS_COUNT 10
#define TRYS_SLEEP 1
namespace tars
{

TC_SpinLock::TC_SpinLock()
{
    _flag.clear(std::memory_order_release);
}

TC_SpinLock::~TC_SpinLock()
{
}

void TC_SpinLock::lock() const
{
    for (size_t i = 1; _flag.test_and_set(std::memory_order_acquire); i++)
    {
    	if(i % TRYS_COUNT == 0)
		{
    		TC_Common::msleep(TRYS_SLEEP);
		}
    	else
		{
			std::this_thread::yield();
		}
    }
}

void TC_SpinLock::unlock() const
{
    _flag.clear(std::memory_order_release);
}

bool TC_SpinLock::tryLock() const
{
    int trys = TRYS_COUNT;
    for (; trys > 0 && _flag.test_and_set(std::memory_order_acquire); --trys)
    {
		std::this_thread::yield();
    }

    if (trys > 0)
        return true;

    return false;
}

}
