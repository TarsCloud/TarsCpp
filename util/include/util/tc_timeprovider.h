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

#ifndef __TC_TIME_PROVIDER_H_
#define __TC_TIME_PROVIDER_H_

#include <string.h>
#include "util/tc_platform.h"
#include "util/tc_common.h"
#include "util/tc_thread.h"
#include "util/tc_autoptr.h"


namespace tars
{
#define TNOW     TC_TimeProvider::getInstance()->getNow()
#define TNOWMS   TC_TimeProvider::getInstance()->getNowMs()

/////////////////////////////////////////////////
/**
 * @file tc_timeprovider.h
 * @brief 秒级、毫秒级时间提供类(linux有独立线程来获取时间, windows直接使用获取时间函数TC_Common::gettimeofday())
 *
 * @author jarodruan@upchina.com
 */
/////////////////////////////////////////////////
class TC_TimeProvider;

/**
 * @brief 提供秒级别的时间
 */
class UTIL_DLL_API TC_TimeProvider
{
public:

    /**
     * @brief 获取实例.
     *
     * @return TimeProvider&
     */
    static TC_TimeProvider* getInstance();

    /**
     * @brief 构造函数
     */
    TC_TimeProvider() : _use_tsc(true), _cpu_cycle(0), _buf_idx(0)
    {
        memset(_t, 0, sizeof(_t));
        memset(_tsc, 0, sizeof(_tsc));

        struct timeval tv;
        TC_Common::gettimeofday(tv);
        _t[0] = tv;
        _t[1] = tv;
    }

    /**
     * @brief 析构，停止线程
     */
    ~TC_TimeProvider();

    /**
     * @brief 获取时间.
     *
     * @return time_t 当前时间
     */
    time_t getNow()     {  return _t[_buf_idx].tv_sec; }

    /**
     * @brief 获取时间.
     *
     * @para timeval
     * @return void
     */
    void getNow(timeval * tv);

    /**
     * @brief 获取ms时间.
     *
     * @para timeval
     * @return void
     */
    int64_t getNowMs();

protected:
	static TC_TimeProvider      *g_tp;

protected:

	/**
	* @brief 运行
	*/
	virtual void run();

    uint64_t GetCycleCount();

	/**
	* @brief 获取cpu主频.
	*
	* @return float cpu主频
	*/
	// double cpuMHz();

	void setTsc(timeval& tt);

	void addTimeOffset(timeval& tt, const int &idx);

private:
    bool            _use_tsc;

    double          _cpu_cycle;

    volatile int    _buf_idx;

    timeval         _t[2];

    uint64_t       _tsc[2];
};

}

#endif

