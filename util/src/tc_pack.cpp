#include "util/tc_platform.h"
#include "util/tc_pack.h"

#include <iostream>
#include <string.h>

#if TARGET_PLATFORM_LINUX
#include <arpa/inet.h>
#endif
namespace tars
{

#if TARGET_PLATFORM_WINDOWS
#pragma comment(lib,"ws2_32.lib")
#endif

#if TARGET_PLATFORM_IOS || TARGET_PLATFORM_WINDOWS||TARGET_PLATFORM_LINUX
#   ifndef __LITTLE_ENDIAN
#       define __LITTLE_ENDIAN 1234
#   endif
#   ifndef __BIG_ENDIAN
#       define __BIG_ENDIAN    4321
#   endif
#   ifndef __BYTE_ORDER
#       define __BYTE_ORDER __LITTLE_ENDIAN
#   endif
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
#   define pack_ntohll(x)    (x)
#   define pack_htonll(x)    (x)
#   define pack_ntohf(x)     (x)
#   define pack_htonf(x)     (x)
#   define pack_ntohd(x)     (x)
#   define pack_htond(x)     (x)
#else
#   if __BYTE_ORDER == __LITTLE_ENDIAN
#       define pack_ntohll(x)    pack_htonll(x)
union pack_bswap_helper
{
    int64_t   i64;
    int32_t   i32[2];
};
inline int64_t pack_htonll(int64_t x)
{
        pack_bswap_helper h;
        h.i64 = x;
        int64_t tmp = htonl(h.i32[1]);
        h.i32[1] = htonl(h.i32[0]);
        h.i32[0] = tmp;
        return h.i64;
}
inline float pack_ntohf(float x)
{
    union {
        float f;
        int32_t i32;
    } helper;

    helper.f = x;
    helper.i32 = htonl( helper.i32 );

    return helper.f;
}
#       define pack_htonf(x)     pack_ntohf(x)
inline double pack_ntohd(double x)
{
    union {
        double d;
        int64_t i64;
    } helper;

    helper.d = x;
    helper.i64 = pack_htonll( helper.i64 );

    return helper.d;
}
#       define pack_htond(x)     pack_ntohd(x)
#   endif
#endif

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (bool t)
{
    _buffer.append(sizeof(bool), (char)t);
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (char t)
{
    _buffer.append(sizeof(char), (char)t);
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (unsigned char t)
{
    (*this) << (char)t;
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (short t)
{
    t = htons(t);

    _buffer.append((const char *)&t, sizeof(t));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (unsigned short t)
{
    (*this) << (short)t;
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (int t)
{
    t = htonl(t);

    _buffer.append((const char *)&t, sizeof(int));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (unsigned int t)
{
    (*this) << (int)t;
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (long t)
{
    if(sizeof(long) == 8)
    {
        t = pack_htonll(t);
    }
    else
    {
        t = htonl(t);
    }

    _buffer.append((const char *)&t, sizeof(long));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (unsigned long t)
{
    if(sizeof(unsigned long) == 8)
    {
        t = pack_htonll(t);
    }
    else
    {
        t = htonl(t);
    }

    _buffer.append((const char *)&t, sizeof(unsigned long));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (long long t)
{
    t = pack_htonll(t);

    _buffer.append((const char *)&t, sizeof(long long));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (unsigned long long t)
{
    t = pack_htonll(t);

    _buffer.append((const char *)&t, sizeof(unsigned long long));

    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (float t)
{
    t = pack_htonf(t);

    _buffer.append((const char *)&t, sizeof(float));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (double t)
{
    t = pack_htond(t);

    _buffer.append((const char *)&t, sizeof(double));
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (const char *sBuffer)
{
    _buffer.append(sBuffer, strlen(sBuffer) + 1);
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (const string& sBuffer)
{
    uint32_t len = (uint32_t)sBuffer.length();
    if(len < 255)
    {
        unsigned char c = (unsigned char)len;
        (*this) << c;
    }
    else
    {
        unsigned char c = 255;
        (*this) << c;
        (*this) << len;
    }
    _buffer.append(sBuffer);
    return *this;
}

TC_PackIn::TC_PackInInner& TC_PackIn::TC_PackInInner::operator << (const TC_PackIn& pi)
{
    _buffer.append(pi.topacket());
    return (*this);
}

/************************************************************************/

bool TC_PackOut::isEnd()
{
    if(_pos >= _length)
    {
        return true;
    }
    return false;
}

TC_PackOut& TC_PackOut::operator >> (bool &t)
{
    size_t len = sizeof(bool);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read bool error.");
    }

    memcpy(&t, _pbuffer + _pos, len);
    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (char &t)
{
    size_t len = sizeof(char);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read char error.");
    }

    memcpy(&t, _pbuffer + _pos, len);
    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (unsigned char &t)
{
    size_t len = sizeof(unsigned char);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read unsigned char error.");
    }

    memcpy(&t, _pbuffer + _pos, len);
    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (short &t)
{
    size_t len = sizeof(short);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read short error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = ntohs(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (unsigned short &t)
{
    size_t len = sizeof(unsigned short);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read unsigned short error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = ntohs(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (int &t)
{
    size_t len = sizeof(int);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read int error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = ntohl(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (unsigned int &t)
{
    size_t len = sizeof(unsigned int);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read unsigned int error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = ntohl(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (long &t)
{
    size_t len = sizeof(long);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read long error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    if(sizeof(unsigned long) == 8)
    {
        t = pack_ntohll(t);
    }
    else
    {
        t = ntohl(t);
    }

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (unsigned long &t)
{
    size_t len = sizeof(unsigned long);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read long error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    if(sizeof(unsigned long) == 8)
    {
        t = pack_ntohll(t);
    }
    else
    {
        t = ntohl(t);
    }

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (long long &t)
{
    size_t len = sizeof(long long);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read long long error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = pack_ntohll(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (unsigned long long &t)
{
    size_t len = sizeof(unsigned long long);
    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read long long error.");
    }

    memcpy(&t, _pbuffer + _pos, len);

    t = pack_ntohll(t);

    _pos += len;

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (float &f)
{
    if(_pos + sizeof(float) > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read float error.");
    }

    memcpy(&f, _pbuffer + _pos, sizeof(float));

    f = pack_ntohf(f);

    _pos += sizeof(float);

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (double &f)
{
    if(_pos + sizeof(double) > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read double error.");
    }

    memcpy(&f, _pbuffer + _pos, sizeof(double));

    f = pack_ntohd(f);

    _pos += sizeof(double);

    return *this;
}

TC_PackOut& TC_PackOut::operator >> (char *sBuffer)
{
    strcpy(sBuffer, _pbuffer + _pos);
    _pos += strlen(sBuffer) + 1;

    if(_pos > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read zero string error.");
    }
    return *this;
}

TC_PackOut& TC_PackOut::operator >> (string& sBuffer)
{
    uint32_t len = 0;
    unsigned char c;
    (*this) >> c;
    if(c == 255)
    {
        (*this) >> len;
    }
    else
    {
        len = c;
    }

    if(_pos + len > _length)
    {
        throw TC_PackOut_Exception("TC_PackOut read string error.");
    }

    sBuffer.assign((const char*)(_pbuffer + _pos), len);

    _pos += len;

    return *this;
}

}

