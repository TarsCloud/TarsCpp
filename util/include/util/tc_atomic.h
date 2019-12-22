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

#ifndef __TC_ATOMIC_H
#define __TC_ATOMIC_H

#include <stdint.h>

namespace tars
{

/////////////////////////////////////////////////
/** 
 * @file  tc_atomic.h 
 * @brief  原子计数类. 
 */           

typedef struct { volatile int counter; } tars_atomic_t;

/**
 * @brief 原子操作类,对int做原子操作
 */
class TC_Atomic
{
public:

    /**
     * 原子类型
     */
    typedef int atomic_type;

    /**
     * @brief 构造函数,初始化为0 
     */
    TC_Atomic(atomic_type at = 0)
    {
        set(at);
    }

    TC_Atomic& operator++()
    {
        inc();
        return *this;
    }

    TC_Atomic& operator--()
    {
        dec();
        return *this;
    }

    operator atomic_type() const
    {
        return get();
    }

    TC_Atomic& operator+=(atomic_type n)
    {
        add(n);
        return *this;
    }

    TC_Atomic& operator-=(atomic_type n)
    {
        sub(n);
        return *this;
    }

    TC_Atomic& operator=(atomic_type n)
    {
        set(n);
        return *this;
    }

    /**
     * @brief 获取值
     *
     * @return int
     */
    atomic_type get() const           { return _value.counter; }

    /**
     * @brief 添加
     * @param i
     *
     * @return int
     */
    atomic_type add(atomic_type i)    { return add_and_return(i); }

    /**
     * @brief 减少
     * @param i
     *
     * @return int
     */
    atomic_type sub(atomic_type i)    { return add_and_return(-i); }

    /**
     * @brief 自加1
     *
     * @return int
     */
    atomic_type inc()               { return add(1); }

    /**
     * @brief 自减1
     */
    atomic_type dec()               { return sub(1); }

    /**
     * @brief 自加1
     *
     * @return void
     */
    void inc_fast()
    {
	__sync_add_and_fetch(&(_value.counter), 1);
    }

    /**
     * @brief 自减1
     * Atomically decrements @_value by 1 and returns true if the
     * result is 0, or false for all other
     */
    bool dec_and_test()
    {
	return 	__sync_sub_and_fetch(&(_value.counter), 1)==0;
    }

    /**
     * @brief 设置值
     */
    atomic_type set(atomic_type i)
    {
        _value.counter = i;

        return i;
    }

protected:

    /**
     * @brief 增加并返回值
     */
    int add_and_return(int i)
    {
	return __sync_add_and_fetch(&(_value.counter), i);
    }

protected:

    /**
     * 值
     */
    tars_atomic_t    _value;
};

}

#endif
