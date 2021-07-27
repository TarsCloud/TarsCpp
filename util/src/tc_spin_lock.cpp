
#include "util/tc_spin_lock.h"
#include <thread>
#include <iostream>
#include <cassert>
using namespace std;

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

    for (; _flag.test_and_set(std::memory_order_acquire);) {
        std::this_thread::yield();
//        asm volatile("rep; nop":: : "memory");
    }
}

void TC_SpinLock::unlock() const
{
    _flag.clear(std::memory_order_release);
}
//
bool TC_SpinLock::tryLock() const
{
    int trys = 100;
    for (; trys > 0 && _flag.test_and_set(std::memory_order_acquire); --trys)
    {
        std::this_thread::yield();
//#if TARGET_PLATFORM_LINUX
//        asm volatile("rep; nop" ::: "memory");
//#endif

    }

    if (trys > 0)
        return true;

    return false;
}

}
