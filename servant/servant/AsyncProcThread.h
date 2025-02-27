﻿/**
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
#pragma once

#include "servant/Message.h"
#include "servant/Global.h"
#include "util/tc_cas_queue.h"
#include "util/tc_thread_queue.h"

namespace tars
{

//////////////////////////////////////////////////////////
/**
 * 异步回调后的处理线程
 */
class SVT_DLL_API AsyncProcThread : public TC_Thread, public TC_ThreadLock
{
public:
    /**
     * 构造函数
     */
    AsyncProcThread(size_t iQueueCap, bool merge);

    /**
     * 析构函数
     */
    virtual ~AsyncProcThread();

    /**
     * 结束处理线程
     */
    void terminate();

    /**
     * 插入
     */
    void push_back(ReqMessage * msg);

    /**
     * 从队列中取消息后执行回调逻辑
     */
    void run();

    /**
     * 获取异步队列的大小
     */
    size_t getSize()
    {
        return _msgQueue->size();
    }

protected:
	void callback(ReqMessage * msg);

private:
    /**
     * 是否需要退出
     */
    bool            _terminate;

    /**
     * 异步队列
     */
    TC_CasQueue<ReqMessage*> * _msgQueue;

    /**
     * 队列流量控制
     */
    size_t _iQueueCap;

    /**
     * 合并网络线程和回调线程
     */
    bool    _merge;
};
///////////////////////////////////////////////////////
}
