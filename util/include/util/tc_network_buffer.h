//
// Created by jarod on 2019-03-01.
//

#ifndef TAF_CPP_TC_NETWORKBUFFER_H
#define TAF_CPP_TC_NETWORKBUFFER_H

#include <list>
#include <vector>
#include <functional>
#include <iostream>
#include <memory>
#include "util/tc_socket.h"

/////////////////////////////////////////////////
/**
 * @file  tc_network_buffer.h
 * @brief  网络buffer缓冲类
 * @brief  Network buffer class
 *
 * @author  ruanshudong@qq.com
 */
/////////////////////////////////////////////////

/**
 * @brief 网络buffer解析, 主要目的是避免buffer的copy, 提速
 * @brief Network buffer resolution, the main purpose is to avoid buffer copy, speed up
 *
 */

namespace tars
{

/**
* @brief
*/
struct TC_NetWorkBuffer_Exception : public TC_Exception
{
	TC_NetWorkBuffer_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
	~TC_NetWorkBuffer_Exception() {};
};

class TC_NetWorkBuffer
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /**
     * 定义协议解析的返回值
	 * Define return values for protocol resolution
     */
    enum PACKET_TYPE
    {
        PACKET_LESS = 0,
        PACKET_FULL = 1,
		PACKET_FULL_CLOSE = 2,  ///< get whole package, and need close connection, for example: http
        PACKET_ERR  = -1,   
    };

    /**
     * 定义协议解析器接口
	 * Define Protocol Resolver Interface
     */
    typedef std::function<PACKET_TYPE(TC_NetWorkBuffer &, vector<char> &)> protocol_functor;

    /**
    * 发送buffer
	* Send buffer
    */
    class Buffer
    {

    public:
	    Buffer() { }
	    Buffer(const vector<char> &sBuffer) : _buffer(sBuffer) {}
	    Buffer(const char *sBuffer, size_t length) : _buffer(sBuffer, sBuffer+length) {}

        void swap(vector<char> &buff, size_t pos = 0)
        {
	    	if(_pos != 0)
		    {
	    	    buff.resize(length());
	    	    memcpy(&buff[0], buffer(), length());
		    }
	        else
            {
			    buff.swap(_buffer);
		    }
	        _pos = pos;
        }

        void clear()
        {
            _buffer.clear();
	        _pos = 0;
        }

        bool empty() const
        {
            return _buffer.size() <= _pos;
        }

        void addBuffer(const vector<char> &buffer)
        {
            _buffer.insert(_buffer.end(), buffer.begin(), buffer.end());
        }

        void assign(const char *buffer, size_t length, size_t pos = 0)
        {
            _buffer.assign(buffer, buffer + length);
	        _pos = pos;
        }

        void setBuffer(const vector<char> &buff, size_t pos = 0)
        {
	        _buffer  = buff;
	        _pos     = pos;
        }

	    char *buffer() { return _buffer.data() + _pos; }

        const char *buffer() const { return _buffer.data() + _pos; }

        size_t length() const { return _buffer.size() - _pos; }

        size_t pos() const { return _pos; }

        char &at(size_t offset)
        {
	    	if(_pos + offset >= _buffer.size() )
	    		throw TC_NetWorkBuffer_Exception("[TC_NetWorkBuffer::Buffer] at '" + TC_Common::tostr(offset) + "' offset overflow");
	    	return _buffer[_pos + offset];
        }

	    char at(size_t offset) const
	    {
		    if(_pos + offset >= _buffer.size() )
			    throw TC_NetWorkBuffer_Exception("[TC_NetWorkBuffer::Buffer] at '" + TC_Common::tostr(offset) + "' offset overflow");
		    return _buffer[_pos + offset];
	    }

	    void add(uint32_t ret)
        {
	        _pos += ret;
            assert(_pos <= _buffer.size());
        }

    protected:
	    vector<char>    _buffer;
	    size_t          _pos = 0;

    };

	typedef std::list<std::shared_ptr<Buffer>>::const_iterator buffer_list_iterator;

	class buffer_iterator : public std::iterator<std::random_access_iterator_tag, char>
	{
	public:
		buffer_iterator(const TC_NetWorkBuffer *buffer, size_t offset) : _buffer(buffer)
		{
			parseOffset(offset);
		}

		/**
		 * @brief copy
		 * @param it
		 */
		buffer_iterator(const buffer_iterator &it)
		{
			if(this != &it)
			{
				_buffer = it._buffer;
				_pos    = it._pos;
				_offset = it._offset;
				_it     = it._it;
			}
		}

		/**
		 *
		 * @param mcmi
		 *
		 * @return bool
		 */
		bool operator==(const buffer_iterator& it) const
		{
			if (_buffer == it._buffer && _it == it._it && _offset == it._offset)
			{
				return true;
			}

			return false;
		}

		/**
		 *
		 * @param mv
		 *
		 * @return bool
		 */
		bool operator!=(const buffer_iterator& it) const
		{
			if (_buffer == it._buffer && _it == it._it && _offset == it._offset)
			{
				return false;
			}

			return true;
		}

		char & operator *()
		{
			return (*_it)->buffer()[_pos];
		}

		char * operator ->()
		{
			return &(*_it)->buffer()[_pos];
		}

		char& operator *() const
		{
			return (*_it)->buffer()[_pos];
		}

		const char* operator ->() const
		{
			return &(*_it)->buffer()[_pos];
		}

		buffer_iterator operator +(size_t n) const
		{
			return buffer_iterator(_buffer, _offset + n);
		}

		buffer_iterator operator += (size_t n)
		{
			parseOffset(_offset + n);
			return *this;
		}

		difference_type operator -(const buffer_iterator &n) const
		{
			return _offset - n._offset;
		}

		buffer_iterator operator -(size_t n) const
		{
			return buffer_iterator(_buffer, _offset - n);
		}

		bool  operator < (const buffer_iterator n) const
		{
			return _offset<n._offset;
		}

		bool  operator <= (const buffer_iterator n) const
		{
			return _offset<=n._offset;
		}

		bool  operator > (const buffer_iterator n) const
		{
			return _offset>n._offset;
		}

		bool  operator >= (const buffer_iterator n) const
		{
			return _offset>=n._offset;
		}

		//前置++
		//Pre++.
		buffer_iterator& operator ++()
		{
			if(_offset >= _buffer->getBufferLength())
				return *this;

			assert(_it != _buffer->_bufferList.end());

			if(_pos < (*_it)->length() - 1)
			{
				++_pos;
				++_offset;
			}
			else
			{
				++_it;
				++_offset;
				_pos    = 0;
			}
			return *this;
		}

		//后置++
		//Post++
		buffer_iterator operator ++(int)
		{
			buffer_iterator it(*this);

			if(_it != _buffer->_bufferList.end())
			{
				if (_pos < (*_it)->length() - 1)
				{
					++_pos;
					++_offset;
				}
				else
				{
					++_it;
					++_offset;
					_pos = 0;
				}
			}
			else
			{
				_offset = _buffer->getBufferLength();
				_pos = 0;
			}
			return it;
		}

	protected:
		void parseOffset(size_t offset)
		{
			_offset = offset;

			bool flag = false;
			for(auto it  = _buffer->_bufferList.begin(); it != _buffer->_bufferList.end(); ++it)
			{
				if(offset >= (*it)->length())
				{
					offset -= (*it)->length();
				}
				else
				{
					_it     = it;
					_pos    = offset;
					flag    = true;

					break;
				}
			}

			if(!flag)
			{
				_offset = _buffer->getBufferLength();
				_pos    = 0;
				_it     = _buffer->_bufferList.end();
			}
		}

		friend class TC_NetWorkBuffer;

	protected:
		const TC_NetWorkBuffer* _buffer;
		buffer_list_iterator    _it;
		size_t                  _offset = 0;
		size_t                  _pos = 0;
	};

    /**
     * 必须以connection来构造(不同服务模型中获取的对象不一样, 需要自己强制转换)
	 * Must be constructed as a connection (different service models get different objects and need to cast themselves)
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
            _deconstruct(this);
        }
    }

	/**
	 * 获取connection, 不同服务模型中获取的对象不一样, 需要自己强制转换
	 * Get connection, get different objects in different service models, need to force conversion yourself
	 * @param buff
	 */
	void* getConnection() { return _connection; }

	/**
	 * 获取connection, 不同服务模型中获取的对象不一样, 需要自己强制转换
	 * Get connection, get different objects in different service models, need to force conversion yourself
	 * @param buff
	 */
	void setConnection(void *connection) { _connection = connection; }

    /**
     * 设置上下文数据, 可以业务存放数据
	 * Set up context data to allow business to store data
     * @param buff
     */
    void setContextData(void *contextData, std::function<void(TC_NetWorkBuffer*)> deconstruct = std::function<void(TC_NetWorkBuffer*)>() ) { _contextData = contextData; _deconstruct = deconstruct; }

    /**
     * 获取上下文数据,  给业务存放数据
	 * Get context data, store data for business
     * @param buff
     */
    void *getContextData() { return _contextData; }

	/**
	 * 增加buffer
	 * Add buffer
	 * @param buff
	 */
	void addBuffer(const std::shared_ptr<Buffer> & buff);

    /**
     * 增加buffer
	 * Add buffer
     * @param buff
     */
    void addBuffer(const std::vector<char>& buff);

	/**
	 * 增加buffer
	 * Add buffer
	 * @param buff
	 */
	void addBuffer(const std::string& buff);

	/**
     * 增加buffer
	 * Add buffer
     * @param buff
     * @param length
     */
    void addBuffer(const char* buff, size_t length);

    /**
     * begin
     * @return
     */
	buffer_iterator begin() const;

	/**
	 * end
	 * @return
	 */
	buffer_iterator end() const;

	/**
	 * 查找
	 * Search
	 * @param str
	 * @param length
	 * @return
	 */
	buffer_iterator find(const char *str, size_t length);

	/**
	 * 获取从开头到迭代器(前一个, 不包括迭代器本身)所有buffer
	 * Gets all buffers from the beginning to the iterator (the previous one excluding the iterator itself)
	 * T: string or vector<char>
	 * @param it
	 * @return
	 */
	template<typename T>
	T iteratorToIterator(const buffer_iterator &sit, const buffer_iterator &eit)
	{
		T buff;

		if(sit == end() || sit == eit)
		{
			return buff;
		}

		if(sit > eit)
		{
			throw TC_NetWorkBuffer_Exception("[TC_NetWorkBuffer::iteratorToIterator] sit > eit error");
		}

		buff.resize(eit - sit);

		std::copy(sit, eit, buff.begin());

		return buff;
	}

	/**
	 * 匹配到对应buff, 获取之前的buffer
	 * Match to the corresponding buffer, get the previous buffer
	 * T: string or vector<char>
	 * @param sep
	 * @param length
	 * @return
	 */
	template<typename T>
	T getPrefixBuffer(const char *sep, size_t length)
	{
		T buff;
		auto sit = std::search(begin(), end(), sep, sep + length);

		if(sit == end())
		{
			return buff;
		}

		return iteratorToIterator<T>(begin(), sit);
	}

	/**
     * 清空所有buffer
	 * Empty all buffers
     */
    void clearBuffers();

    /**
     * 是否为空的
	 * Is it empty
     */
    bool empty() const;

    /**
     * 返回所有buffer累计的字节数
	 * Returns the cumulative number of bytes for all buffers
     * @return size_t
     */
    size_t getBufferLength() const;

    /**
     * buffer list length
     * @return
     */
    size_t size() const { return _bufferList.size(); }

    /**
     * 获取第一块有效数据buffer的指针, 可以用来发送数据
	 * A pointer to get the first valid data buffer that can be used to send data
     * @return
     */
    pair<const char*, size_t> getBufferPointer() const;

    /**
     * 将链表上的所有buffer拼接起来
	 * Stitch together all buffers on the list
     * @return const char *, 返回第一个数据buffer的指针, 为空则返回NULL
	 * @return const char *, Returns a pointer to the first data buffer, or NULL if empty
     */
    const char * mergeBuffers();

    /**
     * 返回所有buffer(将所有buffer拼接起来, 注意性能)
	 * Return all buffers (stitch all buffers together, pay attention to performance)
     * @return string
     */
    vector<char> getBuffers() const;

    /**
     * 返回所有buffer(将所有buffer拼接起来, 注意性能)
	 * Return all buffers (stitch all buffers together, pay attention to performance)
     * @return string
     */
    string getBuffersString() const;

    /**
     * 读取len字节的buffer(避免len个字节被分割到多个buffer的情况)(注意: 不往后移动)
	 * Read buffer of len bytes (to avoid splitting len bytes into multiple buffers) (Note: Do not move backwards)
     * @param len
     * @return
     */
    bool getHeader(size_t len, std::string &buffer) const;

    /**
     * 读取len字节的buffer(避免len个字节被分割到多个buffer的情况)(注意: 不往后移动)
	 * Read buffer of len bytes (to avoid splitting len bytes into multiple buffers) (Note: Do not move backwards)
     * @param len
     * @return
     */
    bool getHeader(size_t len, std::vector<char> &buffer) const;

	/**
	 * 读取len字节的buffer(避免len个字节被分割到多个buffer的情况)(注意: 不往后移动)
	 * Read buffer of len bytes (to avoid splitting len bytes into multiple buffers) (Note: Do not move backwards)
	 * getHeader<string>(10), getHeader<vector<char>>(10);
	 * @param len
	 * @return 不够会抛异常TC_NetWorkBuffer_Exception
	 */
    template<typename T>
    T getHeader(size_t len) const
	{
		if(getBufferLength() < len)
		{
			throw TC_NetWorkBuffer_Exception("[TC_NetWorkBuffer::getHeader] no enough buff(" + TC_Common::tostr(getBufferLength()) + ") to get(" + TC_Common::tostr(len) + ")");
		}

		T buffer;

		if(len == 0)
		{
			return buffer;
		}

		buffer.resize(len);

		getBuffers(&buffer[0], len);

		return buffer;
	}

    /**
     * 往后移动len个字节
	 * Move len bytes backward
     * @param len
     */
    bool moveHeader(size_t len);

    /**
    * 取二个字节(字节序)的整型值, 如果长度<1, 返回0
	* Take an integer value of two bytes (byte order), and return 0 if the length is less than 1
    * @return int8_t
    */
    uint8_t getValueOf1() const;

    /**
    * 取二个字节(字节序)的整型值, 如果长度<2, 返回0
	* Take an integer value of two bytes (byte order), and return 0 if the length is less than 2
    * @return int16_t
    */
    uint16_t getValueOf2() const;

    /**
     * 取四个字节(字节序)的整型值, 如果长度<4, 返回0
	 * Take an integer value of four bytes (byte order), and return 0 if the length is less than 4
     * @return int32_t
     */
    uint32_t getValueOf4() const;

    /**
     * http协议判读
	 * HTTP protocol interpretation
     * @return
     */
    TC_NetWorkBuffer::PACKET_TYPE checkHttp();

    /**
    * 解析一个包头是1字节的包, 把包体解析出来(解析后, 往后移动)
	* Parse a package with a 1-byte header and parse out the package body (move back after parsing)
    * 注意: buffer只返回包体, 不包括头部的1个字节的长度
	* Note: Buffer only returns the package, not including the length of one byte of the head
    * @param buffer, 输出的buffer
	* @param buffer, Output buffer
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
	* @param minLength, minimum buffer length, if less than, is considered an error package and returns PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
	* @param maxLength, maximum buffer length, if exceeded, is considered an error package and returns PACKET_ERR
    * @return PACKET_TYPE
    */
    PACKET_TYPE parseBufferOf1(vector<char> &buffer, uint8_t minLength, uint8_t maxLength);

    /**
    * 解析一个包头是2字节(字节序)的包, 把包体解析出来(解析后, 往后移动)
	* Parse a 2-byte (byte order) packet header and parse the package body (move back after parsing)
    * 注意: buffer只返回包体, 不包括头部的2个字节的长度
	* Note: Buffer only returns the package, not including the length of 2 bytes of the header
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
	* @param minLength, minimum buffer length, if less than, is considered an error package and returns PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
	* @param maxLength, maximum buffer length, if exceeded, is considered an error package and returns PACKET_ERR
    * @return PACKET_TYPE
    */
    PACKET_TYPE parseBufferOf2(vector<char> &buffer, uint16_t minLength, uint16_t maxLength);

    /**
    * 解析一个包头是4字节(字节序)的包, 把包体解析出来(解析后, 往后移动)
	* Parse a package whose header is a 4-byte (byte order) package and parse out the package body (move back after parsing)
    * 注意: buffer只返回包体, 不包括头部的4个字节的长度
	* Note: Buffer only returns the package, not including the length of 4 bytes of the header
    * @param minLength, buffer最小长度, 如果小于, 则认为是错误包, 会返回PACKET_ERR
	* @param minLength, minimum buffer length, if less than, is considered an error package and returns PACKET_ERR
    * @param maxLength, buffer最大长度, 如果超过, 则认为是错误包, 会返回PACKET_ERR
	* @param maxLength, maximum buffer length, if exceeded, is considered an error package and returns PACKET_ERR
    * @return PACKET_TYPE
     */
    PACKET_TYPE parseBufferOf4(vector<char> &buffer, uint32_t minLength, uint32_t maxLength);

    /**
     * 解析二进制包, 1字节长度+包体(iMinLength<包长<iMaxLength, 否则返回PACKET_ERR)
	 * Parse binary package, 1 byte length + package (iMinLength<package length<iMaxLength, otherwise return PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的1个字节的长度
	 * Note: out only returns the package, not including the length of one byte of the head
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
	 * Parse binary package, 2 byte length (byte order) + package (iMinLength<package length<iMaxLength, otherwise return PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的2个字节的长度
	 * Note: out returns only the package, not the length of 2 bytes of the header
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
	 * Parse binary package, 4 byte length (byte order) + package (iMinLength<package length<iMaxLength, otherwise return PACKET_ERR)
     * 注意: out只返回包体, 不包括头部的4个字节的长度
	 * Note: out only returns the package, not including the length of 4 bytes of the head
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

    /**
    * echo
    * @param in
    * @param out
    * @return
    */
    static TC_NetWorkBuffer::PACKET_TYPE parseJson(TC_NetWorkBuffer&in, vector<char> &out);

protected:

	size_t getBuffers(char *buffer, size_t length) const;

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
		//move backward
        moveHeader(sizeof(T));

        //读取length长度的buffer
		//Read buffer of length length
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
	 * Connection information (different within different classes)
     */
    void*   _connection = NULL;

    /**
     * contextData for use
     */
    void*   _contextData = NULL;

    /**
     * deconstruct contextData
     */
    std::function<void(TC_NetWorkBuffer*)> _deconstruct;

    /**
     * buffer list
     */
	std::list<std::shared_ptr<Buffer>> _bufferList;

	/**
	 * buffer剩余没解析的字节总数
	 * Total number of bytes left unresolved by buffer
	 */
    size_t _length = 0;

};

}

#endif //TAF_CPP_TC_NETWORKBUFFER_H
