//
// Created by jarod on 2019-03-01.
//

#ifndef TAF_CPP_TC_NETWORKBUFFER_H
#define TAF_CPP_TC_NETWORKBUFFER_H

#include <list>
#include <vector>
#include <functional>
#include <iostream>
#include "util/tc_socket.h"

/////////////////////////////////////////////////
/**
 * @file  tc_network_buffer.h
 * @brief  网络buffer缓冲类
 *
 * @author  jarodruan@upchina.com
 */
/////////////////////////////////////////////////

/**
 * @brief 网络buffer解析, 主要目的是避免buffer的copy, 提速
 *
 */

namespace tars
{

class TC_NetWorkBuffer
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /**
     * 定义协议解析的返回值
     */
    enum PACKET_TYPE
    {
        PACKET_LESS = 0,
        PACKET_FULL = 1,
        PACKET_ERR  = -1,
    };

    /**
     * 定义协议解析器接口
     */
    typedef std::function<PACKET_TYPE(TC_NetWorkBuffer &, vector<char> &)> protocol_functor;

    /**
    * 发送buffer
    */
    class SendBuffer
    {
    protected:
        vector<char> sendBuffer;
        uint32_t sendPos = 0;

    public:
	    SendBuffer() { }
	    SendBuffer(const vector<char> &sBuffer) : sendBuffer(sBuffer) {}
	    SendBuffer(const char *sBuffer, size_t length) : sendBuffer(sBuffer, sBuffer+length) {}

        void swap(vector<char> &buff)
        {
	        sendPos = 0;
        	buff.swap(sendBuffer);
        }

        void clear()
        {
            sendBuffer.clear();
            sendPos = 0;
        }

        bool empty() const
        {
            return sendBuffer.size() <= sendPos;
        }

        void addBuffer(const vector<char> &buffer)
        {
            sendBuffer.insert(sendBuffer.end(), buffer.begin(), buffer.end());// += buffer;
        }

        void assign(const char *buffer, size_t length)
        {
            sendBuffer.assign(buffer, buffer + length);
            sendPos = 0;
        }

        vector<char> &getBuffer()
        {
        	return sendBuffer;
        }

        void setBuffer(const vector<char> &buff, int pos = 0)
        {
	        sendBuffer  = buff;
	        sendPos     = pos;
        }

	    char *buffer()
	    {
		    return sendBuffer.data() + sendPos;
	    }

        const char *buffer() const
        {
            return sendBuffer.data() + sendPos;
        }

        uint32_t length() const
        {
            return (uint32_t)(sendBuffer.size() - sendPos);
        }

        void add(uint32_t ret)
        {
            sendPos += ret;
            assert(sendPos <= sendBuffer.size());
        }
    };

    /**
     * 必须以connection来构造(不同服务模型中获取的对象不一样, 需要自己强制转换)
     * @param buff
     */
    TC_NetWorkBuffer(void *connection) { _connection = connection; }

    /**
     * deconstruct
     * @param buff
     */    
    ~TC_NetWorkBuffer()
    {
        if(_deconstruct)
        {
            _deconstruct();
        }
    }

    /**
     * 获取connection, 不同服务模型中获取的对象不一样, 需要自己强制转换
     * @param buff
     */
    void* getConnection() { return _connection; }

    /**
     * 设置上下文数据, 可以业务存放数据
     * @param buff
     */
    void setContextData(void *contextData, std::function<void()> deconstruct = std::function<void()>() ) { _contextData = contextData; _deconstruct = deconstruct; }

    /**
     * 获取上下文数据,  给业务存放数据
     * @param buff
     */
    void *getContextData() { return _contextData; }

    /**
     * 增加buffer
     * @param buff
     */
    void addBuffer(const vector<char>& buff);

    /**
     * add & swap, 避免一次数据copy
     * @param buff
     */
	void addSwapBuffer(vector<char>& buff);

	/**
     * 增加buffer
     * @param buff
     * @param length
     */
    void addBuffer(const char* buff, size_t length);

    /**
     * 清空所有buffer
     */
    void clearBuffers();

    /**
     * 是否为空的
     */
    bool empty() const;

    /**
     * 返回所有buffer累计的字节数
     * @return size_t
     */
    size_t getBufferLength() const;

    /**
     * 获取第一块有效数据buffer的指针, 可以用来发送数据
     * @return
     */
    pair<const char*, size_t> getBufferPointer() const;

    /**
     * 将链表上的所有buffer拼接起来
     * @return string
     */
    void mergeBuffers();

    /**
     * 返回所有buffer(将所有buffer拼接起来, 注意性能)
     * @return string
     */
    vector<char> getBuffers() const;

    /**
     * 返回所有buffer(将所有buffer拼接起来, 注意性能)
     * @return string
     */
    string getBuffersString() const;

    /**
     * 读取len字节的buffer(避免len个字节被分割到多个buffer的情况)(注意: 不往后移动)
     * @param len
     * @return
     */
    bool getHeader(size_t len, std::string &buffer) const;

    /**
     * 读取len字节的buffer(避免len个字节被分割到多个buffer的情况)(注意: 不往后移动)
     * @param len
     * @return
     */
    bool getHeader(size_t len, std::vector<char> &buffer) const;

    /**
     * 往后移动len个字节
     * @param len
     */
    bool moveHeader(size_t len);

    /**
    * 取二个字节(字节序)的整型值, 如果长度<1, 返回0
    * @return int8_t
    */
    uint8_t getValueOf1() const;

    /**
    * 取二个字节(字节序)的整型值, 如果长度<2, 返回0
    * @return int16_t
    */
    uint16_t getValueOf2() const;

    /**
     * 取四个字节(字节序)的整型值, 如果长度<4, 返回0
     * @return int32_t
     */
    uint32_t getValueOf4() const;

    /**
     * http协议判读
     * @return
     */
    TC_NetWorkBuffer::PACKET_TYPE checkHttp();

    /**
    * 解析一个包头是1字节的包, 把包体解析出来(解析后, 往后移动)
    * 注意: buffer只返回包体, 不包括头部的1个字节的长度
    * @param buffer, 输出的buffer
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
    * @return PACKET_TYPE
    */
    PACKET_TYPE parseBufferOf1(vector<char> &buffer, uint8_t minLength, uint8_t maxLength);

    /**
    * 解析一个包头是2字节(字节序)的包, 把包体解析出来(解析后, 往后移动)
    * 注意: buffer只返回包体, 不包括头部的2个字节的长度
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
    * @return PACKET_TYPE
    */
    PACKET_TYPE parseBufferOf2(vector<char> &buffer, uint16_t minLength, uint16_t maxLength);

    /**
    * 解析一个包头是4字节(字节序)的包, 把包体解析出来(解析后, 往后移动)
    * 注意: buffer只返回包体, 不包括头部的4个字节的长度
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
    * @return PACKET_TYPE
     */
    PACKET_TYPE parseBufferOf4(vector<char> &buffer, uint32_t minLength, uint32_t maxLength);

    /**
     * 解析二进制包, 1字节长度+包体(iMinLength<包长<iMaxLength, 否则返回PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的1个字节的长度
     * @param in
     * @param out
     * @return
     */
    template<uint8_t iMinLength, uint8_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE parseBinary1(TC_NetWorkBuffer&in, vector<char> &out)
    {
        return in.parseBufferOf1(out, iMinLength, iMaxLength);
    }

    /**
     * 解析二进制包, 2字节长度(字节序)+包体(iMinLength<包长<iMaxLength, 否则返回PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的2个字节的长度
     * @param in
     * @param out
     * @return
     */
    template<uint16_t iMinLength, uint16_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE parseBinary2(TC_NetWorkBuffer&in, vector<char> &out)
    {
        return in.parseBufferOf2(out, iMinLength, iMaxLength);
    }

    /**
     * 解析二进制包, 4字节长度(字节序)+包体(iMinLength<包长<iMaxLength, 否则返回PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的4个字节的长度
     * @param in
     * @param out
     * @return
     */
    template<uint32_t iMinLength, uint32_t iMaxLength>
    static TC_NetWorkBuffer::PACKET_TYPE parseBinary4(TC_NetWorkBuffer&in, vector<char> &out)
    {
        return in.parseBufferOf4(out, iMinLength, iMaxLength);
    }

    /**
     * http1
     * @param in
     * @param out
     * @return
     */
    static TC_NetWorkBuffer::PACKET_TYPE parseHttp(TC_NetWorkBuffer&in, vector<char> &out);

    /**
     * echo
     * @param in
     * @param out
     * @return
     */
    static TC_NetWorkBuffer::PACKET_TYPE parseEcho(TC_NetWorkBuffer&in, vector<char> &out);

protected:
    template<typename T>
    T getValue() const
    {
	    vector<char> buffer;

        if(getHeader(sizeof(T), buffer))
        {
            if(sizeof(T) == 2)
            {
                return ntohs(*(uint16_t*)buffer.data());
            }
            else if(sizeof(T) == 4)
            {
                return ntohl(*(uint32_t*)buffer.data());
            }
            return *((T*)buffer.data());
        }
        return 0;
    }

    template<typename T>
    TC_NetWorkBuffer::PACKET_TYPE parseBuffer(vector<char> &buffer, T minLength, T maxLength)
    {
        if(getBufferLength() < sizeof(T))
        {
            return PACKET_LESS;
        }

        if(minLength < sizeof(T))
            minLength = sizeof(T);

        T length = getValue<T>();

        if(length < minLength || length > maxLength)
        {
            return PACKET_ERR;
        }

        if(getBufferLength() < length)
        {
            return PACKET_LESS;
        }

        //往后移动
        moveHeader(sizeof(T));

        //读取length长度的buffer
        if(!getHeader(length - sizeof(T), buffer))
        {
            return PACKET_LESS;
        }

        moveHeader(length - sizeof(T));
        return PACKET_FULL;
    }

protected:
    /**
     * 连接信息(不同的类里面不一样)
     */
    void*   _connection = NULL;

    /**
     * contextData for use
     */
    void*   _contextData = NULL;

    /**
     * deconstruct contextData
     */
    std::function<void()> _deconstruct;

    /**
     * buffer list
     */
	std::list<std::vector<char>> _bufferList;

	/**
	 * buffer剩余没解析的字节总数
	 */
    size_t _length = 0;

    /**
     * 当前buffer的位置
     */
    size_t _pos = 0;
};

}

#endif //TAF_CPP_TC_NETWORKBUFFER_H
