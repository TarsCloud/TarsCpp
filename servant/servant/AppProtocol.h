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

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "tup/RequestF.h"
#include "tup/tup.h"
#include "servant/BaseF.h"
#include "util/tc_network_buffer.h"
#include "servant/Global.h"

using namespace std;
using namespace tup;

namespace tars
{

class TC_Transceiver;

#define TARS_NET_MIN_PACKAGE_SIZE 5
#define TARS_NET_MAX_PACKAGE_SIZE 1024*1024*100

template<typename T>
T net2host(T len)
{
    switch (sizeof(T))
    {
    case sizeof(uint8_t): return len;
    case sizeof(uint16_t): return ntohs(len);
    case sizeof(uint32_t): return ntohl(len);
    }
    assert(true);
    return 0;
}

class TC_Transceiver;

//////////////////////////////////////////////////////////////////////
/**
 * 协议解析
 */
class SVT_DLL_API AppProtocol
{
public:
    /**
     * 解析协议
     * @param in, 目前的buffer
     * @param out, 一个完整的包
     *
     * @return int, 0表示没有接收完全, 1表示收到一个完整包
     */
    static TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer &in, vector<char> &out)
    {
        return TC_NetWorkBuffer::parseBinary4<TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE>(in, out);
    }

    /**
     *
     * @param T
     * @param offset
     * @param netorder
     * @param in
     * @param out
     * @return int
     */
    template<size_t offset, typename T, bool netorder>
    static TC_NetWorkBuffer::PACKET_TYPE parseStream(TC_NetWorkBuffer& in, vector<char>& out)
    {
        size_t len = offset + sizeof(T);

        if (in.getBufferLength() < len)
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }

        string header;
        in.getHeader(len, header);

        assert(header.size() == len);

        T iHeaderLen = 0;

        ::memcpy(&iHeaderLen, header.c_str() + offset, sizeof(T));

        if (netorder)
        {
            iHeaderLen = net2host<T>(iHeaderLen);
        }

        //长度保护一下
        if (iHeaderLen < (T)(len) || (uint32_t)iHeaderLen > TARS_NET_MAX_PACKAGE_SIZE)
        {
            return TC_NetWorkBuffer::PACKET_ERR;
        }

        if (in.getBufferLength() < (uint32_t)iHeaderLen)
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }

        in.getHeader(iHeaderLen, out);

        assert(out.size() == iHeaderLen);

        in.moveHeader(iHeaderLen);

        return TC_NetWorkBuffer::PACKET_FULL;
    }
};

typedef std::function<shared_ptr<TC_NetWorkBuffer::Buffer>(RequestPacket&, TC_Transceiver*)> request_protocol;

typedef std::function<TC_NetWorkBuffer::PACKET_TYPE(TC_NetWorkBuffer&, ResponsePacket&)> response_protocol;

//////////////////////////////////////////////////////////////////////
/**
 * 客户端自定义协议设置
 */
class ProxyProtocol
{
public:
    /**
     * 构造
     */
    ProxyProtocol() : requestFunc(streamRequest) {}

    /**
     * 将TarsOutputStream<BufferWriter>换成shared_ptr<TC_NetWorkBuffer::Buffer>, 且中间没有内存copy
     * 注意: 转换后os无效了, 数据被置换到shared_ptr<TC_NetWorkBuffer::Buffer>
     */ 
    static shared_ptr<TC_NetWorkBuffer::Buffer> toBuffer(TarsOutputStream<BufferWriter> &os);
    static shared_ptr<TC_NetWorkBuffer::Buffer> http1Request(tars::RequestPacket& request, TC_Transceiver *);
    static TC_NetWorkBuffer::PACKET_TYPE http1Response(TC_NetWorkBuffer &in, ResponsePacket& done);

#if TARS_HTTP2

    // ENCODE function, called by network thread
    static shared_ptr<TC_NetWorkBuffer::Buffer> http2Request(tars::RequestPacket& request, TC_Transceiver *);

    // DECODE function, called by network thread
    static TC_NetWorkBuffer::PACKET_TYPE http2Response(TC_NetWorkBuffer &in, ResponsePacket& done);

    // ENCODE function, called by network thread
    static shared_ptr<TC_NetWorkBuffer::Buffer> grpcRequest(tars::RequestPacket& request, TC_Transceiver *);

    // DECODE function, called by network thread
    static TC_NetWorkBuffer::PACKET_TYPE grpcResponse(TC_NetWorkBuffer &in, ResponsePacket& done);
#endif

    /**
     * 普通二进制请求包
     * @param request
     * @param buff
     */
    static shared_ptr<TC_NetWorkBuffer::Buffer> streamRequest(RequestPacket& request, TC_Transceiver *)
    {
        shared_ptr<TC_NetWorkBuffer::Buffer> buff = std::make_shared<TC_NetWorkBuffer::Buffer>();
        buff->addBuffer(request.sBuffer);
	    return buff;
    }

    /**
     * 普通二进制包普tars请求包
     * @param request
     * @param buff
     */
    template
        <
            size_t offset,
            typename T,
            bool netorder,
            size_t idOffset,
            typename K,
            bool idNetorder,
            size_t packetMaxSize
        >
    static size_t streamResponse(TC_NetWorkBuffer &in, ResponsePacket& rsp)
    {
        size_t len = offset + sizeof(T);

        if (in.getBufferLength() < len)
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }

        string header;
        in.getHeader(len, header);

        assert(header.size() == len);

        T iHeaderLen = 0;

        ::memcpy(&iHeaderLen, header.c_str() + offset, sizeof(T));

        if (netorder)
        {
            iHeaderLen = net2host<T>(iHeaderLen);
        }

        //做一下保护
        size_t sizeHeaderLen = static_cast<size_t>(iHeaderLen);
        if (sizeHeaderLen > packetMaxSize || iHeaderLen == 0)
        {
            throw TarsDecodeException("packet length too long or zero,len:(" + TC_Common::tostr(packetMaxSize) + ")" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (in.getBufferLength() < (uint32_t)iHeaderLen)
        {
            return TC_NetWorkBuffer::PACKET_LESS;
        }

        //把buffer读取到
        in.getHeader(iHeaderLen, rsp.sBuffer);

        K requestId;

        vector<char> tmp;
        in.moveHeader(idOffset);
        in.getHeader(sizeof(K), tmp);

        if (idNetorder)
        {
            requestId = net2host<K>(*(K*)tmp.data());
        }

        rsp.iRequestId = static_cast<uint32_t>(requestId);

        return TC_NetWorkBuffer::PACKET_FULL;
    }

    template
        <
            size_t offset,
            typename T,
            bool netorder,
            size_t idOffset,
            typename K,
            bool idNetorder
        >
    static size_t streamResponse(TC_NetWorkBuffer &in, ResponsePacket &done)
    {
        return streamResponse<offset, T, netorder, idOffset, K, idNetorder, TARS_NET_MAX_PACKAGE_SIZE>(in, done);
    }

    /**
     * wup响应包(wup的响应会放在ResponsePacket的buffer中)
     * @param request
     * @param buff
     */
    static TC_NetWorkBuffer::PACKET_TYPE tupResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
    {
        return tupResponseLen(in, done, TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE);
    }

    static TC_NetWorkBuffer::PACKET_TYPE tupResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength);

	/**
	* wup响应包(wup的响应会放在ResponsePacket的buffer中)
	* @param request
	* @param buff
	*/
	static TC_NetWorkBuffer::PACKET_TYPE jsonResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
	{
		return jsonResponseLen(in, done, TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE);
	}

	static TC_NetWorkBuffer::PACKET_TYPE jsonResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength);

    /**
     * tars请求包
     * @param request
     * @param buff
     */
    static shared_ptr<TC_NetWorkBuffer::Buffer> tarsRequest(RequestPacket& request, TC_Transceiver *);

    /**
     * tars响应包解析
     * @param recvBuffer
     * @param done
     */
    static TC_NetWorkBuffer::PACKET_TYPE tarsResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
    {
        return tarsResponseLen(in, done, TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE);
    }

    static TC_NetWorkBuffer::PACKET_TYPE tarsResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp, uint32_t iMinLength, uint32_t iMaxLength);

	 /**
	  * tars各种协议响应包解析
	  * @param recvBuffer
	  * @param done
	  */
	static TC_NetWorkBuffer::PACKET_TYPE totalResponse(TC_NetWorkBuffer &in, ResponsePacket &done)
	{
		return totalResponseLen(in, done, TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE);
	}

    static TC_NetWorkBuffer::PACKET_TYPE totalResponseLen(TC_NetWorkBuffer &in, ResponsePacket &rsp, uint32_t iMinLength, uint32_t iMaxLength);

public:
    request_protocol requestFunc;

    response_protocol responseFunc;

    /**
     * 开启openalive, 如果keepAliveCallback未设置则直接发包, 如果keepAliveCallback设置, 则回调(不再发tars_ping)
     * 如果是非tars协议如果开启openalive, 则需要设置keepAliveCallback, 自己发包
     */
    std::function<void(ServantPrx)>  keepAliveCallback;
};

//////////////////////////////////////////////////////////////////////
}
