//
// Created by jarod on 2019-03-01.
//
#include "util/tc_network_buffer.h"
#include "util/tc_http.h"

using namespace std;

namespace tars
{

//void TC_NetWorkBuffer::addSwapBuffer(vector<char>& buff)
//{
//	_length += buff.size();
//
//	_bufferList.push_back(std::make_shared<Buffervector<char>());
//	_bufferList.back().swap(buff);
//}

void TC_NetWorkBuffer::addBuffer(const shared_ptr<TC_NetWorkBuffer::Buffer> & buff)
{
	_bufferList.push_back(buff);
	_length += buff->length();
}

void TC_NetWorkBuffer::addBuffer(const vector<char>& buff)
{
    _bufferList.push_back(std::make_shared<Buffer>(buff));

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

    return make_pair((*it)->buffer(), (*it)->length());
}

const char * TC_NetWorkBuffer::mergeBuffers()
{
    //merge to one buffer
    if(_bufferList.size() > 1)
    {
	    std::list<std::shared_ptr<Buffer>> bufferList;

	    bufferList.push_back(std::make_shared<Buffer>(getBuffers()));

        _bufferList.swap(bufferList);
    }

    assert(_bufferList.size() <= 1);

    if(!_bufferList.empty())
    {
    	return (*_bufferList.begin())->buffer();
    }

    return NULL;
}

size_t TC_NetWorkBuffer::getBuffers(char *buffer, size_t length) const
{
	assert(length <= getBufferLength());

	auto it = _bufferList.begin();

	size_t left = length;
	size_t pos = 0;

	while(it != _bufferList.end() && left != 0)
	{
		size_t len = std::min(left, (*it)->length());

		memcpy(buffer + pos, (*it)->buffer(), len);

		left -= len;
		pos += len;

		++it;
	}

	return pos;
}

string TC_NetWorkBuffer::getBuffersString() const
{
	string buffer;
	buffer.resize(_length);

	getBuffers(&buffer[0], _length);

	return buffer;
}

vector<char> TC_NetWorkBuffer::getBuffers() const
{
    vector<char> buffer;

    buffer.resize(_length);

	getBuffers(&buffer[0], _length);

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

    buffer.resize(len);

	getBuffers(&buffer[0], len);
//
//    auto it = _bufferList.begin();
//
//    size_t left = len;
//
//    while(it != _bufferList.end())
//    {
//        if((*it)->length() >= left)
//        {
//            //当前buffer足够
//            buffer.append((*it)->buffer(), left);
//            return true;
//        }
//        else
//        {
//            //当前buffer不够
//            buffer.append((*it)->buffer(), (*it)->length());
//            left = left - (*it)->length();
//        }
//
//        ++it;
//    }

//    assert(buffer.length() == len);

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

    buffer.resize(len);

	getBuffers(&buffer[0], len);
//
//	auto it = _bufferList.begin();
//
//    size_t left = len;
//
//    while(it != _bufferList.end())
//    {
//	    if((*it)->length() >= left)
//        {
//            //当前buffer足够
//            buffer.insert(buffer.end(), (*it)->buffer(), (*it)->buffer() + left);
//            return true;
//        }
//        else
//        {
//            //当前buffer不够
//            buffer.insert(buffer.end(), (*it)->buffer(), (*it)->buffer() + (*it)->length());
//	        left = left - (*it)->length();
//        }
//
//        ++it;
//    }
//
//    assert(buffer.size() == len);

    return true;
}

bool TC_NetWorkBuffer::moveHeader(size_t len)
{
    if(getBufferLength() < len)
        return false;

    if(len == 0)
        return true;

    auto it = _bufferList.begin();

//    assert(it->size() >= _pos);

    size_t left = (*it)->length();

    if(left > len)
    {
	    (*it)->add(len);
        _length -= len;
    }
    else if(left == len)
    {
        _length -= len;
        _bufferList.erase(it);
    }
    else
    {
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

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::checkHttp()
{
    try
    {
        mergeBuffers();

        pair<const char*, size_t> buffer = getBufferPointer();

        if(buffer.first == NULL || buffer.second == 0)
        {
            return PACKET_LESS;
        }

        bool b = TC_HttpRequest::checkRequest(buffer.first, buffer.second);

        return b ? PACKET_FULL : PACKET_LESS;
    }
    catch (exception &ex)
    {
        return PACKET_ERR;
    }

    return PACKET_LESS;
}

TC_NetWorkBuffer::PACKET_TYPE TC_NetWorkBuffer::parseHttp(TC_NetWorkBuffer&in, vector<char> &out)
{
    TC_NetWorkBuffer::PACKET_TYPE b = in.checkHttp();

    if (b == PACKET_FULL)
    {
        out = in.getBuffers();

        in.clearBuffers();
    }

    return b;
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
