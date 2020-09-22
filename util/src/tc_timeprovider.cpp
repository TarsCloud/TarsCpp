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

#include "util/tc_timeprovider.h"
#include <cmath>

namespace tars
{

TC_TimeProvider *TC_TimeProvider::g_tp = NULL;

TC_TimeProvider* TC_TimeProvider::getInstance()
{
    if (!g_tp)
    {
        static std::mutex m;
       	std::lock_guard<std::mutex> lock(m);

        // static TC_ThreadMutex mutex;

        // TC_LockT<TC_ThreadMutex> lock(mutex);

        if (!g_tp)
        {
            g_tp = new TC_TimeProvider();
			std::thread t(&TC_TimeProvider::run, g_tp);
			t.detach();
        }
    }
    return g_tp;
}

TC_TimeProvider::~TC_TimeProvider()
{
}

#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX

//直接从寄存器获取开机上电以来的周期(和主频有关系)
//如果微处理器的主频是1MHZ的话，那么tsc就会在1秒内增加1000000
uint64_t TC_TimeProvider::GetCycleCount()
{
//__inline __attribute__((always_inline)) uint64_t rdtsc() {
#if defined(__i386__)
    int64_t ret;
    __asm__ volatile ("rdtsc" : "=A" (ret) );
    return ret;
#elif defined(__x86_64__) || defined(__amd64__)
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
#elif defined(__aarch64__)
uint64_t cntvct;
    asm volatile ("isb; mrs %0, cntvct_el0; isb; " : "=r" (cntvct) :: "memory");
    return cntvct;
#else
#warning No high-precision counter available for your OS/arch
    return 0;
#endif
/*
    uint32_t high, low;
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));

    uint64_t current_tsc    = ((uint64_t)high << 32) | low;
    return current_tsc;
*/
}

/*
 #define rdtsc(low,high) \
      __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))
          uint64_t current_tsc    = ((uint64_t)high << 32) | low;
*/

#else
uint64_t TC_TimeProvider::GetCycleCount()
{
	LARGE_INTEGER tmp;
	QueryPerformanceCounter(&tmp);
	return tmp.QuadPart;
}

#endif

void TC_TimeProvider::getNow(timeval *tv)
{
#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_LINUX

    int idx = _buf_idx;
    *tv = _t[idx];
    if(fabs(_cpu_cycle - 0) < 0.0001 && _use_tsc)
    {
        addTimeOffset(*tv, idx);
    }
    else
    {
        TC_Common::gettimeofday(*tv); 
    }
#else
	TC_Common::gettimeofday(*tv);
#endif
}

int64_t TC_TimeProvider::getNowMs()
{
    struct timeval tv;
    getNow(&tv);

    return tv.tv_sec * (int64_t)1000 + tv.tv_usec / 1000;
}

void TC_TimeProvider::run()
{
    memset(_tsc, 0x00, sizeof(_tsc));

    while (true)
    {
        timeval& tt = _t[!_buf_idx];

        TC_Common::gettimeofday(tt);

        setTsc(tt);

        _buf_idx = !_buf_idx;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// double TC_TimeProvider::cpuMHz()
// {
//     if (_cpu_cycle != 0)
//         return 1.0 / _cpu_cycle;

//     return 0;
// }

void TC_TimeProvider::setTsc(timeval& tt)
{
    uint64_t current_tsc = GetCycleCount();

    uint64_t& last_tsc   = _tsc[!_buf_idx];
    timeval& last_tt     = _t[_buf_idx];

    static bool first = true;
    if (first)
    {
        //第一次进来
        first = false;
        _cpu_cycle      = 0;
        last_tsc        = current_tsc;
    }
    else
    {
        //计算两次之间的间隔(us)
        time_t sptime   = (tt.tv_sec -  last_tt.tv_sec) * 1000 * 1000 + (tt.tv_usec - last_tt.tv_usec);

        //时间间隔/cpu计数次数, 得到cpu一次的周期时间
        _cpu_cycle      = (double)sptime / (current_tsc - _tsc[_buf_idx]);

        last_tsc        = current_tsc;
    }
}

void TC_TimeProvider::addTimeOffset(timeval& tt, const int &idx)
{
    int64_t current_tsc = (int64_t)GetCycleCount();
    int64_t t = (int64_t)(current_tsc - (int64_t)_tsc[idx]);

    //根据cpu的技术次数*周期时间得到时间间隔(us)
    time_t offset =  (t * _cpu_cycle);
    //偏差超过1s就认为是有问题的!
    if (t < -1000 || offset > 1000000) 
    {
        cerr<< "TC_TimeProvider add_time_offset error,correct it by use gettimeofday. offset:" << offset <<"|current_tsc:"<<current_tsc<<"|last_tsc:"<<_tsc[idx]<<"|t:" << t <<endl;
        _use_tsc = false;
        TC_Common::gettimeofday(tt);
        return;
    }
//	cout << "addTimeOffset:" << offset << ", " << _cpu_cycle << ", t:" << t << endl;
    tt.tv_usec += offset;
    while (tt.tv_usec >= 1000000) { tt.tv_usec -= 1000000; tt.tv_sec++;}
}

}


