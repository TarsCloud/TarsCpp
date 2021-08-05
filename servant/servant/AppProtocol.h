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

#ifndef __TARS_PROTOCOL_H_
#define __TARS_PROTOCOL_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "tup/RequestF.h"
#include "tup/tup.h"
#include "servant/BaseF.h"
#include "util/tc_network_buffer.h"

using namespace std;
using namespace tup;

namespace tars
{

class Transceiver;

#define TARS_NET_MIN_PACKAGE_SIZE 5
#define TARS_NET_MAX_PACKAGE_SIZE 1024*1024*10

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

class Transceiver;

//////////////////////////////////////////////////////////////////////
/**
 * 协议解析
 */
class AppProtocol
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

typedef std::function<vector<char>(RequestPacket&, Transceiver *)> request_protocol;
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

    static vector<char> http1Request(tars::RequestPacket& request, Transceiver *);
    static TC_NetWorkBuffer::PACKET_TYPE http1Response(TC_NetWorkBuffer &in, ResponsePacket& done);

	// static vector<char> httpJceRequest(taf::RequestPacket& request, Transceiver *);
	// static TC_NetWorkBuffer::PACKET_TYPE httpJceResponse(TC_NetWorkBuffer &in, ResponsePacket& done);
#if TARS_HTTP2

    // ENCODE function, called by network thread
    static vector<char> http2Request(tars::RequestPacket& request, Transceiver *);

    // DECODE function, called by network thread
    static TC_NetWorkBuffer::PACKET_TYPE http2Response(TC_NetWorkBuffer &in, ResponsePacket& done);

        // ENCODE function, called by network thread
    static vector<char> grpcRequest(tars::RequestPacket& request, Transceiver *);

    // DECODE function, called by network thread
    static TC_NetWorkBuffer::PACKET_TYPE grpcResponse(TC_NetWorkBuffer &in, ResponsePacket& done);
#endif

    /**
     * 普通二进制请求包
     * @param request
     * @param buff
     */
    static vector<char> streamRequest(RequestPacket& request, Transceiver *)
    {
	    return request.sBuffer;
    }

    /**
     * 普通二进制包普taf请求包
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
        return tupResponseLen<TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE>(in, done);
    }

    template<uint32_t iMinLength, uint32_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE tupResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp)
    {
        uint32_t len = (uint32_t)in.getBufferLength();

        //收到的字节数太少, 还需要继续接收
        if(len < sizeof(uint32_t))
            return TC_NetWorkBuffer::PACKET_LESS;

        //获取包总体长度
        uint32_t iHeaderLen = in.getValueOf4();

        //做一下保护,长度大于10M
        if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
        {
            throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
        }

            //包没有接收全
        if (len < iHeaderLen)
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            if (len >= head)
            {
                string buffer;
                in.getHeader(head, buffer);

                TarsInputStream<BufferReader> is;

                is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

                //tup回来是requestpackage
                RequestPacket rsp;

                is.read(rsp.iVersion, 1, true);

                if (rsp.iVersion != TUPVERSION)
                {
                    throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
                }

                is.read(rsp.cPacketType, 2, true);

                if (rsp.cPacketType != TARSNORMAL)
                {
                    throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                is.read(rsp.iMessageType, 3, true);
                is.read(rsp.iRequestId, 4, true);
            }
            return TC_NetWorkBuffer::PACKET_LESS;
        }
        else
        {
            //buffer包括4个字节长度
            vector<char> buffer;
            buffer.resize(iHeaderLen);

            in.getHeader(iHeaderLen, buffer);
            TarsInputStream<BufferReader> is;

            is.setBuffer(buffer.data() + sizeof(tars::Int32), buffer.size() - sizeof(tars::Int32));

            //TUP的响应包其实也是返回包
            RequestPacket req;
            req.readFrom(is);

            if (req.iVersion != TUPVERSION )
            {
                throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(req.iVersion));
            }

            if (req.cPacketType != TARSNORMAL)
            {
                throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)req.cPacketType));
            }

            rsp.cPacketType     = req.cPacketType;
            rsp.iMessageType    = req.iMessageType;
            rsp.iRequestId      = req.iRequestId;
            rsp.iVersion        = req.iVersion;
            rsp.context         = req.context;
            //tup的响应包直接放入到sBuffer里面
            rsp.sBuffer         = buffer;

            in.moveHeader(iHeaderLen);
        }

        return TC_NetWorkBuffer::PACKET_FULL;
    }
	/**
	* wup响应包(wup的响应会放在ResponsePacket的buffer中)
	* @param request
	* @param buff
	*/
	static TC_NetWorkBuffer::PACKET_TYPE jsonResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
	{
		return jsonResponseLen<TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE>(in, done);
	}

	template<uint32_t iMinLength, uint32_t iMaxLength>
	static TC_NetWorkBuffer::PACKET_TYPE jsonResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp)
	{
		uint32_t len = (uint32_t)in.getBufferLength();

		//收到的字节数太少, 还需要继续接收
		if (len < sizeof(uint32_t))
			return TC_NetWorkBuffer::PACKET_LESS;

		//获取包总体长度
		uint32_t iHeaderLen = in.getValueOf4();

		//做一下保护,长度大于10M
		if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
		{
			throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
		}

		//包没有接收全
		if (len < iHeaderLen)
		{
			//看看包头是否正确
			static const uint32_t head = 20;

			if (len >= head)
			{
				string buffer;
				in.getHeader(head, buffer);

				TarsInputStream<BufferReader> is;
				is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

				is.read(rsp.iVersion, 1, false);

				if (rsp.iVersion != JSONVERSION)
				{
					throw TarsDecodeException("json version not correct, version:" + TC_Common::tostr(rsp.iVersion));
				}

				is.read(rsp.cPacketType, 2, false);

				if (rsp.cPacketType != TARSNORMAL)
				{
					throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
				}

				is.read(rsp.iMessageType, 3, false);
				is.read(rsp.iRequestId, 4, false);
				is.read(rsp.iRet, 5, false);

				if (rsp.iRet < TARSSERVERUNKNOWNERR)
				{
					throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
				}
			}

			return TC_NetWorkBuffer::PACKET_LESS;
		}
		else
		{
			vector<char> buffer;
			bool ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
			if (!ret)
			{
				throw TarsDecodeException("parse buffer exception");
			}

			TarsInputStream<BufferReader> is;
			is.setBuffer(buffer.data(), buffer.size());

			rsp.readFrom(is);

			if (rsp.iVersion != JSONVERSION)
			{
				throw TarsDecodeException("json version not correct, version:" + TC_Common::tostr(rsp.iVersion));
			}

			if (rsp.cPacketType != TARSNORMAL)
			{
				throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
			}

			if (rsp.iRet < TARSSERVERUNKNOWNERR)
			{
				throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
			}

		}

		return TC_NetWorkBuffer::PACKET_FULL;
	}
	
public:
    /**
     * tars请求包
     * @param request
     * @param buff
     */
    static vector<char> tarsRequest(RequestPacket& request, Transceiver *);

    /**
     * tars响应包解析
     * @param recvBuffer
     * @param done
     */
    static TC_NetWorkBuffer::PACKET_TYPE tarsResponse(TC_NetWorkBuffer &in, ResponsePacket& done)
    {
        return tarsResponseLen<TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE>(in, done);
    }

    template<uint32_t iMinLength, uint32_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE tarsResponseLen(TC_NetWorkBuffer &in, ResponsePacket& rsp)
    {
        uint32_t len = (uint32_t)in.getBufferLength();

        //收到的字节数太少, 还需要继续接收
        if(len < sizeof(uint32_t))
            return TC_NetWorkBuffer::PACKET_LESS;

        //获取包总体长度
        uint32_t iHeaderLen = in.getValueOf4();

        //做一下保护,长度大于10M
        if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
        {
            throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (len < iHeaderLen)
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            if (len >= head)
            {
                string buffer;
                in.getHeader(head, buffer);

                TarsInputStream<BufferReader> is;
                is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

                // ResponsePacket rsp;
                is.read(rsp.iVersion, 1, false);

                if (rsp.iVersion != TARSVERSION)
                {
                    throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
                }

                is.read(rsp.cPacketType, 2, false);

                if (rsp.cPacketType != TARSNORMAL)
                {
                    throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                is.read(rsp.iRequestId, 3, false);
                is.read(rsp.iMessageType, 4, false);
                is.read(rsp.iRet, 5, false);

                if (rsp.iRet < TARSSERVERUNKNOWNERR)
                {
                    throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
                }
            }

            return TC_NetWorkBuffer::PACKET_LESS;
        }
        else
        {
	        vector<char> buffer;
            bool ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
            if(!ret)
            {
	            throw TarsDecodeException("parse buffer exception");
            }

            TarsInputStream<BufferReader> is;
            is.setBuffer(buffer.data(), buffer.size());

            rsp.readFrom(is);

            if (rsp.iVersion != TARSVERSION)
            {
                throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
            }

            if (rsp.cPacketType != TARSNORMAL)
            {
                throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
            }

            if (rsp.iRet < TARSSERVERUNKNOWNERR)
            {
                throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
            }
        }

        return TC_NetWorkBuffer::PACKET_FULL;
    }

	 /**
	  * tars各种协议响应包解析
	  * @param recvBuffer
	  * @param done
	  */
	static TC_NetWorkBuffer::PACKET_TYPE totalResponse(TC_NetWorkBuffer &in, ResponsePacket &done)
	{
		return totalResponseLen<TARS_NET_MIN_PACKAGE_SIZE, TARS_NET_MAX_PACKAGE_SIZE>(in, done);
	}

    template <uint32_t iMinLength, uint32_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE totalResponseLen(TC_NetWorkBuffer &in, ResponsePacket &rsp)
    {
        uint32_t len = (uint32_t)in.getBufferLength();

        //收到的字节数太少, 还需要继续接收
        if (len < sizeof(uint32_t))
            return TC_NetWorkBuffer::PACKET_LESS;

        //获取包总体长度
        uint32_t iHeaderLen = in.getValueOf4();

        //做一下保护,长度大于10M
        if (iHeaderLen < iMinLength || iHeaderLen > iMaxLength)
        {
            throw TarsDecodeException("packet length too long or too short,len:" + TC_Common::tostr(iHeaderLen));
        }

        //包没有接收全
        if (len < iHeaderLen)
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            if (len >= head)
            {
                string buffer;
                in.getHeader(head, buffer);

                TarsInputStream<BufferReader> is;
                is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

                // ResponsePacket rsp;
                is.read(rsp.iVersion, 1, false);

                if (rsp.iVersion != TARSVERSION && rsp.iVersion != TUPVERSION && rsp.iVersion != JSONVERSION)
                {
                    throw TarsDecodeException("version not correct, version:" + TC_Common::tostr(rsp.iVersion));
                }

                is.read(rsp.cPacketType, 2, false);

                if (rsp.cPacketType != TARSNORMAL)
                {
                    throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                is.read(rsp.iRequestId, 3, false);
                is.read(rsp.iMessageType, 4, false);
                is.read(rsp.iRet, 5, false);

                if (rsp.iRet < TARSSERVERUNKNOWNERR)
                {
                    throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
                }
            }

            return TC_NetWorkBuffer::PACKET_LESS;
        }
        else
        {
            //看看包头是否正确
            static const uint32_t head = 20;

            string buffer;
            in.getHeader(head, buffer);

            TarsInputStream<BufferReader> is;
            is.setBuffer(buffer.c_str() + sizeof(tars::Int32), head);

            is.read(rsp.iVersion, 1, false);

            if (rsp.iVersion == TUPVERSION)
            {
                vector<char> buffer;
                buffer.resize(iHeaderLen);

                in.getHeader(iHeaderLen, buffer);
                TarsInputStream<BufferReader> is;

                is.setBuffer(buffer.data() + sizeof(tars::Int32), buffer.size() - sizeof(tars::Int32));

                //TUP的响应包其实也是返回包
                RequestPacket req;
                req.readFrom(is);

                if (req.cPacketType != TARSNORMAL)
                {
                    throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)req.cPacketType));
                }

                rsp.cPacketType = req.cPacketType;
                rsp.iMessageType = req.iMessageType;
                rsp.iRequestId = req.iRequestId;
                rsp.iVersion = req.iVersion;
                rsp.context = req.context;
                //tup的响应包直接放入到sBuffer里面
                rsp.sBuffer = buffer;

                in.moveHeader(iHeaderLen);
            }
            else if (rsp.iVersion == TARSVERSION || rsp.iVersion == JSONVERSION)
            {
                vector<char> buffer;
                bool ret = in.parseBufferOf4(buffer, iMinLength, iMaxLength);
                if (!ret)
                {
                    throw TarsDecodeException("parse buffer exception");
                }

                TarsInputStream<BufferReader> is;
                is.setBuffer(buffer.data(), buffer.size());

                rsp.readFrom(is);

                if (rsp.cPacketType != TARSNORMAL)
                {
                    throw TarsDecodeException("packettype not correct, packettype:" + TC_Common::tostr((int)rsp.cPacketType));
                }

                if (rsp.iRet < TARSSERVERUNKNOWNERR)
                {
                    throw TarsDecodeException("response value not correct, value:" + TC_Common::tostr(rsp.iRet));
                }
            }
            else
            {
                throw TarsDecodeException("===>version not correct, version:" + TC_Common::tostr(rsp.iVersion));
            }
        }

        return TC_NetWorkBuffer::PACKET_FULL;
    }

public:
    request_protocol  requestFunc;

    response_protocol responseFunc;
};


//////////////////////////////////////////////////////////////////////
}
#endif
