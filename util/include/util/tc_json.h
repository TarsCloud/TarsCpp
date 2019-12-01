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

#ifndef __TC_JSON_H
#define __TC_JSON_H

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <assert.h>
#include <stdio.h>
#include <sstream>

#include "util/tc_autoptr.h"

using namespace std;

namespace tars
{

/**
* 编解码抛出的异常
*/
struct TC_Json_Exception : public TC_Exception
{
    TC_Json_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_Json_Exception() throw(){};
};

enum eJsonType
{
    eJsonTypeString,
    eJsonTypeNum,
    eJsonTypeObj,
    eJsonTypeArray,
    eJsonTypeBoolean,
    eJsonTypeNull,
};

/*
 * 分析json字符串用到的 读字符的类
 */
class BufferJsonReader
{
    const char *        _buf;		///< 缓冲区
    size_t              _buf_len;	///< 缓冲区长度
    size_t              _cur;		///< 当前位置

public:

    BufferJsonReader () :_buf(NULL),_buf_len(0), _cur(0) {}

    void reset() { _cur = 0;}

    void setBuffer(const char * buf, size_t len)
    {
        _buf = buf;
        _buf_len = len;
        _cur = 0;
    }

    void setBuffer(const std::vector<char> &buf)
    {
        _buf = buf.data();
        _buf_len = buf.size();
        _cur = 0;
    }

    const size_t &getCur() const { return _cur; }

    const char * getPoint() const { return _buf+_cur; }

    const char &read() { check(); _cur ++; return *(_buf+_cur-1); }

    const char &get() const { check(); return *(_buf+_cur); }

    const char &getBack() const { assert(_cur>0); return *(_buf+_cur-1); }

    void back() { assert(_cur>0); _cur--; }

    void check() const
    {
        if (_cur + 1 > _buf_len)
        {
            char s[64];
            snprintf(s, sizeof(s), "buffer overflow when peekBuf, over %u.", (uint32_t)_buf_len);
            throw TC_Json_Exception(s);
        }
    }

    bool hasEnd() const { return _cur >= _buf_len; }
};

class JsonValue;
class JsonValueObj;
class JsonValueArray;
class JsonValueString;
class JsonValueBoolean;
class JsonValueNum;
class JsonValueNull;

typedef TC_AutoPtr<JsonValue> JsonValuePtr;
typedef TC_AutoPtr<JsonValueObj> JsonValueObjPtr;
typedef TC_AutoPtr<JsonValueNum> JsonValueNumPtr;
typedef TC_AutoPtr<JsonValueArray> JsonValueArrayPtr;
typedef TC_AutoPtr<JsonValueString> JsonValueStringPtr;
typedef TC_AutoPtr<JsonValueBoolean> JsonValueBooleanPtr;
typedef TC_AutoPtr<JsonValueNull> JsonValueNullPtr;

/*
 * json类型的基类。没有任何意义
 */
class JsonValue : public TC_HandleBase
{
public:
    virtual eJsonType getType() const = 0;
    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c) = 0;
    virtual ~JsonValue() { }

public:
    static JsonValuePtr createJsonValue(BufferJsonReader & reader);
    static uint32_t getHex(BufferJsonReader & reader);
    static bool isspace(char c);
    static void writeString(const string &str, ostream& ostr);

protected:
    JsonValue(){};
};

/*
 * json类型 string类型 例如"dd\ndfd"
 */
class JsonValueString : public JsonValue
{
public:
    JsonValueString(const string & s):value(s)
    {
    }
    JsonValueString()
    {
    }

    eJsonType getType() const { return eJsonTypeString; }

    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c);

    virtual ~JsonValueString() { }
    string value;
};

/*
 * json类型 number类型 例如 1.5e8
 */
class JsonValueNum : public JsonValue
{
public:
    JsonValueNum(double d,bool b=false):value(d),isInt(b)
    {
    }
    JsonValueNum()
    {
        isInt=false;
		value=0.0f;
    }
    eJsonType getType() const { return eJsonTypeNum; }
    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c);

    virtual ~JsonValueNum(){}
public:
    double value;
    bool isInt;
};

/*
 * json类型 object类型 例如 {"aa","bb"} 
 */
class JsonValueObj: public JsonValue
{
public:
    eJsonType getType() const { return eJsonTypeObj; }
    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c);
    const JsonValuePtr &get(const char *name);
    virtual ~JsonValueObj();
public:
    unordered_map<string, JsonValuePtr> value;
};

/*
 * json类型 array类型 例如 ["aa","bb"] 
 */
class JsonValueArray: public JsonValue
{
public:
    eJsonType getType() const { return eJsonTypeArray; }
    void push_back(const JsonValuePtr & p) { value.push_back(p); }
    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c = 0);
    virtual ~JsonValueArray();
public:
    vector<JsonValuePtr> value;
};

/*
 * json类型 boolean类型 例如 true 
 */
class JsonValueBoolean : public JsonValue
{
public:
    eJsonType getType() const { return eJsonTypeBoolean; }
    bool getValue() const { return value; }
    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader,char c);
    virtual ~JsonValueBoolean(){}
public:
    bool value;
};

/*
 * json类型 null类型 例如"dd\ndfd"
 */
class JsonValueNull : public JsonValue
{
public:
    JsonValueNull() { }

    eJsonType getType() const { return eJsonTypeNull; }

    virtual void write(ostream& ostr) const;
    virtual void read(BufferJsonReader & reader, char c);

    virtual ~JsonValueNull() { }
};

/*
 * 分析json的类。都是static
 */
class TC_Json
{
public:
    //json类型到字符串的转换
    static string writeValue(const JsonValuePtr &p);
    
    //解析成智能指针
    static JsonValuePtr getValue(const string & str);

};

}

#endif

