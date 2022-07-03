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

#ifndef __TC_CUSTOM_PROTOCOL_H_
#define __TC_CUSTOM_PROTOCOL_H_

#include "util/tc_ex.h"
#include "util/tc_port.h"
#include "util/tc_common.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread.h"
#include "util/tc_socket.h"
#include "util/tc_epoller.h"
#include "util/tc_timeout_queue.h"
#include "util/tc_network_buffer.h"
#include <map>
#include <sstream>
#include <cassert>
#include <vector>

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_custom_protocol.h 
 * @brief  custom protocol类.
 * @brief  custom protocol class
 *  
 * 包括TC_CustomProtoReq、TC_CustomProtoRsp两个类；
 * Including TC_CustomProtoReq、TC_CustomProtoRsp Two classes of custom protocol;
 *  
 */             
/////////////////////////////////////////////////

class TC_NetWorkBuffer;

/**
* @brief  custom protocol 协议解析异常类
* @brief  custom protocol protocol resolution exception class
*/
struct TC_CustomProto_Exception : public TC_Exception
{
    TC_CustomProto_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
    ~TC_CustomProto_Exception() {};
};

/**
 * @brief  custom 应答协议解析异常类
 * @brief  custom response Protocol Resolution Exception Class
 */
struct TC_CustomProtoReq_Exception : public TC_CustomProto_Exception
{
    TC_CustomProtoReq_Exception(const string &sBuffer) : TC_CustomProto_Exception(sBuffer){};
    ~TC_CustomProtoReq_Exception() {};
};

/**
 * @brief  custom 请求协议解析异常类
 * @brief  custom request protocol resolution exception class
 */
struct TC_CustomProtoRsp_Exception : public TC_CustomProto_Exception
{
    TC_CustomProtoRsp_Exception(const string &sBuffer) : TC_CustomProto_Exception(sBuffer){};
    ~TC_CustomProtoRsp_Exception() {};
};

class TC_CustomProtoReq
{
public:
    void encode(shared_ptr<TC_NetWorkBuffer::Buffer>& buff)
	{
		buff->addBuffer(std::move(_buffer));
	}

	void sendBuffer(const string& buffer)
	{
		_buffer = buffer;
        // LOG_CONSOLE_DEBUG << "_buffer:" << _buffer << endl;
	}

private:
	string _buffer;
};

class TC_CustomProtoRsp
{
public:
	typedef std::function<bool(TC_NetWorkBuffer::Buffer&)> IncrementDecodeFunc;

	virtual bool decode(TC_NetWorkBuffer::Buffer &data)
    {
		return true;
    }

    bool incrementDecode(TC_NetWorkBuffer &buff)
	{
		if(buff.empty())
			return false;

		buff.mergeBuffers();

		size_t length = buff.getBufferLength();

		auto sBuf = buff.getBuffer();

		bool flag = decode(*sBuf.get());

		buff.subLength(length - sBuf->length());

		return flag;
	}

    string &getBuffer() 
	{
		 return _buffer; 
	}

	void set_protocol(const IncrementDecodeFunc& incrementDecodeFunc)
	{
		_incrementDecodeFunc = incrementDecodeFunc;
	}
	
protected:
	string _buffer;

	IncrementDecodeFunc _incrementDecodeFunc;
};


}
#endif

