//
// Created by jarod on 2019-03-01.
//
#include "util/tc_network_buffer.h"
#include "util/tc_http.h"

using namespace std;

namespace tars
{

void TC_NetWorkBuffer::addSwapBuffer(vector<char>& buff)
{
	_length += buff.size();

	_bufferList.push_back(vector<char>());
	_bufferList.back().swap(buff);
}

void TC_NetWorkBuffer::addBuffer(const vector<char>& buff)
{
    _bufferList.push_back(buff);

    _length += buff.size();
}

void TC_NetWorkBuffer::addBuffer(const char* buff, size_t length)
{
    addBuffer(vector<char>(buff, buff + length));
}

void TC_NetWorkBuffer::clearBuffers()
{
    _bufferList.clear();
    _length = 0;
    _pos    = 0;
}

bool TC_NetWorkBuffer::empty() const
{
    return _length == 0;
}

size_t TC_NetWorkBuffer::getBufferLength() const
{
    return _length;
}

pair<const char*, size_t> TC_NetWorkBuffer::getBufferPointer() const
{
    if(empty())
    {
        return make_pair((const char*)NULL, 0);
    }

    auto it = _bufferList.begin();

    return make_pair(it->data() + _pos, it->size() - _pos);
}

vector<char> TC_NetWorkBuffer::getBuffers() const
{
	vector<char> buffer;
	buffer.resize(_length);

    auto it = _bufferList.begin();

    size_t pos = 0;
    while(it != _bufferList.end())
    {
        if(it == _bufferList.begin())
        {
        	memcpy(&buffer[pos], it->data() + _pos, it->size() - _pos);
        	pos += it->size() - _pos;
        }
        else
        {
	        memcpy(&buffer[pos], it->data(), it->size());
	        pos += it->size();
        }
        ++it;
    }

    return buffer;
}

bool TC_NetWorkBuffer::getHeader(size_t len, std::string &buffer) const
{
    if(getBufferLength() < len)
        return false;

    buffer.clear();

    if(len == 0)
    {
        return true;
    }

    buffer.reserve(len);
    auto it = _bufferList.begin();

    size_t left = len;
    size_t cur  = _pos;

    while(it != _bufferList.end())
    {
        if(it->size() - cur >= left)
        {
            //当前buffer足够
            buffer.append(it->data() + cur, left);
            return true;
        }
        else
        {
            //当前buffer不够
            buffer.append(it->data() + cur, it->size() - cur);
            left = left - (it->size() - cur);
            cur     = 0;
        }

        ++it;
    }

    assert(buffer.length() == len);

    return true;
}

bool TC_NetWorkBuffer::getHeader(size_t len, std::vector<char> &buffer) const
{
    if(getBufferLength() < len)
        return false;

    buffer.clear();

    if(len == 0)
    {
        return true;
    }

    buffer.reserve(len);

    auto it = _bufferList.begin();

    size_t left = len;
    size_t cur  = _pos;

    while(it != _bufferList.end())
    {
        if(it->size() - cur >= left)
        {
            //当前buffer足够
            buffer.insert(buffer.end(), it->data() + cur, it->data() + cur + left);
            return true;
        }
        else
        {
            //当前buffer不够
            buffer.insert(buffer.end(), it->data() + cur, it->data() + it->size());
            left = left - (it->size() - cur);
            cur     = 0;
        }

        ++it;
    }

    assert(buffer.size() == len);

    return true;
}
bool TC_NetWorkBuffer::moveHeader(size_t len)
{
    if(getBufferLength() < len)
        return false;

    if(len == 0)
        return true;

    auto it = _bufferList.begin();

    assert(it->size() >= _pos);

    size_t left = it->size() - _pos;

    if(left > len)
    {
        _pos    += len;
        _length -= len;
    }
    else if(left == len)
    {
        _pos    = 0;
        _length -= len;
        _bufferList.erase(it);
    }
    else
    {
        _pos    = 0;
        _length -= left;

        _bufferList.erase(it);

        return moveHeader(len - left);
    }
    return true;
}

uint8_t TC_NetWorkBuffer::getValueOf1() const
{
    return getValue<uint8_t>();
}

uint16_t TC_NetWorkBuffer::getValueOf2() const
{
    return getValue<uint16_t>();
}

uint32_t TC_NetWorkBuffer::getValueOf4() const
{
    return getValue<uint32_t>();
}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseBufferOf1(vector<char> &buffer, uint8_t minLength, uint8_t maxLength)
{
    return parseBuffer<uint8_t>(buffer, minLength, maxLength);
}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseBufferOf2(vector<char> &buffer, uint16_t minLength, uint16_t maxLength)
{
    return parseBuffer<uint16_t>(buffer, minLength, maxLength);
}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseBufferOf4(vector<char> &buffer, uint32_t minLength, uint32_t maxLength)
{
    return parseBuffer<uint32_t>(buffer, minLength, maxLength);
}

//bool TC_NetWorkBuffer::checkHttp(string &buffer) const
//{
//    //这样处理性能是有问题的, 有提升的空间
//    buffer = getBuffers();
//
//    return TC_HttpRequest::checkRequest(buffer.c_str(), buffer.length());
//}
//
//TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseHttp(TC_NetWorkBuffer&in, string &out)
//{
//    try
//    {
//        string buffer;
//        bool b = in.checkHttp(buffer);
//        if (b)
//        {
//            out.swap(buffer);
//            in.clearBuffers();;
//            return PACKET_FULL;
//        }
//        else
//        {
//            return PACKET_LESS;
//        }
//    }
//    catch (exception &ex)
//    {
//        return PACKET_ERR;
//    }
//
//    return PACKET_LESS;
//}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseHttp(TC_NetWorkBuffer&in, vector<char> &out)
{
    try
    {
        if(in._bufferList.empty())
        {
            return PACKET_LESS;
        }

	    vector<char> buffer;

        bool b;

        if(in._bufferList.empty())
        {
            b = false;
        }
        else
        {
            //不用size来判读只有一个元素, list的size会比较慢!!!

            auto it = in._bufferList.begin();
            auto pre = it;

            ++it;

            if (it == in._bufferList.end())
            {
                //只有一个buffer
                b = TC_HttpRequest::checkRequest(pre->data(), pre->size());
				if (b)
				{
					buffer = in.getBuffers();
				}
            }
            else
            {
            	//todo 性能还需要优化
                buffer = in.getBuffers();
                b = TC_HttpRequest::checkRequest(buffer.data(), buffer.size());
            }
        }

        if (b)
        {
            out.swap(buffer);
            in.clearBuffers();
            return PACKET_FULL;
        }
        else
        {
            return PACKET_LESS;
        }
    }
    catch (exception &ex)
    {
        return PACKET_ERR;
    }

    return PACKET_LESS;
}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseEcho(TC_NetWorkBuffer&in, vector<char> &out)
{
    try
    {
        out = in.getBuffers();
        in.clearBuffers();
        return TC_NetWorkBuffer::PACKET_FULL;
    }
    catch (exception &ex)
    {
        return TC_NetWorkBuffer::PACKET_ERR;
    }

    return TC_NetWorkBuffer::PACKET_LESS;             //表示收到的包不完全
}

}
