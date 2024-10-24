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
#pragma once

namespace tars
{
class ReqTime
{
public:
    inline void setStartTimeStamp(int64_t t) { _startTimeStamp = t; }
    inline void setDispatchTimeStamp(int64_t t) { _dispatchTimeStamp = t; }
    inline void setFinishTimeStamp(int64_t t) { _finishTimeStamp = t; }

    inline int64_t startTimeStamp() const { return _startTimeStamp; }
    inline int64_t dispatchTimeStamp() const { return _dispatchTimeStamp; }
    inline int64_t finishTimeStamp() const { return _finishTimeStamp; }

    inline int64_t queueWaitTime() const { return _dispatchTimeStamp > 0 ? _dispatchTimeStamp-_startTimeStamp : 0; }
    inline int64_t servantHandleTime() const { return _finishTimeStamp > 0 ? _finishTimeStamp-_dispatchTimeStamp : 0; }
    inline int64_t totalTime() const { return _finishTimeStamp > 0 ? _finishTimeStamp-_startTimeStamp : 0;}

protected:
    int64_t _startTimeStamp = TNOWUS;     // 接收到数据的时间,微秒
    int64_t _dispatchTimeStamp = 0; // 投递给业务线程的时间,微秒
    int64_t _finishTimeStamp = 0;   // 业务线程处理完成时间,微秒
};
}

