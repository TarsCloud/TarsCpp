

#ifndef __TC_JSON_H
#define __TC_JSON_H

#include <string>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <stdio.h>

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
	eJsonTypeBoolean
};

/*
 * json类型的基类。没有任何意义
 */
class JsonValue : public TC_HandleBase
{
public:
	virtual eJsonType getType()=0;
	virtual ~JsonValue()
	{
	}
};
typedef TC_AutoPtr<JsonValue> JsonValuePtr;

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

	eJsonType getType()
	{
		return eJsonTypeString;
	}
	virtual ~JsonValueString()
	{
	}
	string value;
};
typedef TC_AutoPtr<JsonValueString> JsonValueStringPtr;

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
	eJsonType getType()
	{
		return eJsonTypeNum;
	}
	virtual ~JsonValueNum(){}
public:
	double value;
	bool isInt;
};
typedef TC_AutoPtr<JsonValueNum> JsonValueNumPtr;

/*
 * json类型 object类型 例如 {"aa","bb"} 
 */
class JsonValueObj: public JsonValue
{
public:
	eJsonType getType()
	{
		return eJsonTypeObj;
	}
	const JsonValuePtr &get(const char *name)
	{
		auto it = value.find(name);

		if(it == value.end())
		{
			char s[64];
			snprintf(s, sizeof(s), "get obj error(key is not exists)[key:%s]", name);
			throw TC_Json_Exception(s);
		}

		return it->second;
	}

	virtual ~JsonValueObj(){}
public:
	unordered_map<string,JsonValuePtr> value;
};
typedef TC_AutoPtr<JsonValueObj> JsonValueObjPtr;

/*
 * json类型 array类型 例如 ["aa","bb"] 
 */
class JsonValueArray: public JsonValue
{
public:
	eJsonType getType()
	{
		return eJsonTypeArray;
	}
	void push_back(const JsonValuePtr & p)
	{
		value.push_back(p);
	}
	virtual ~JsonValueArray(){}
public:
	vector<JsonValuePtr> value;
};
typedef TC_AutoPtr<JsonValueArray> JsonValueArrayPtr;

/*
 * json类型 boolean类型 例如 true 
 */
class JsonValueBoolean : public JsonValue
{
public:
	eJsonType getType()
	{
		return eJsonTypeBoolean;
	}
	bool getValue()
	{
		return value;
	}
	virtual ~JsonValueBoolean(){}
public:
	bool value;
};
typedef TC_AutoPtr<JsonValueBoolean> JsonValueBooleanPtr;

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

	size_t getCur()
	{
		return _cur;
	}

	const char * getPoint()
	{
		return _buf+_cur;
	}

	char read()
	{
		check();
		_cur ++;
		return *(_buf+_cur-1);
	}

	char get()
	{
		check();
		return *(_buf+_cur);
	}

	char getBack()
	{
		assert(_cur>0);
		return *(_buf+_cur-1);
	}

	void back()
	{
		assert(_cur>0);
		_cur--;
	}

	void check()
	{
		if (_cur + 1 > _buf_len)
		{
			char s[64];
			snprintf(s, sizeof(s), "buffer overflow when peekBuf, over %u.", (uint32_t)_buf_len);
			throw TC_Json_Exception(s);
		}
	}

	bool hasEnd()
	{
		return _cur >= _buf_len;
	}
};

/*
 * 分析json的类。都是static
 */
class TC_Json
{
public:
	//json类型到字符串的转换
	static string writeValue(const JsonValuePtr & p);
	static void writeValue(const JsonValuePtr & p, string& ostr);
	static void writeValue(const JsonValuePtr & p, vector<char>& buf);

	//json字符串到json结构的转换
	static JsonValuePtr getValue(const string & str);
    static JsonValuePtr getValue(const vector<char>& buf);
private:
	//string 类型到json字符串
	static void writeString(const JsonValueStringPtr & p, string& ostr);
	static void writeString(const string & s, string& ostr);

	//num 类型到json字符串
	static void writeNum(const JsonValueNumPtr & p, string& ostr);

	//obj 类型到json字符串
	static void writeObj(const JsonValueObjPtr & p, string& ostr);

	//array 类型到json字符串
	static void writeArray(const JsonValueArrayPtr & p, string& ostr);

	//boolean 类型到json字符串
	static void writeBoolean(const JsonValueBooleanPtr & p, string& ostr);

	//读取json的value类型 也就是所有的json类型 如果不符合规范会抛异常
	static JsonValuePtr getValue(BufferJsonReader & reader);
	//读取json的object 如果不符合规范会抛异常
	static JsonValueObjPtr getObj(BufferJsonReader & reader);
	//读取json的array(数组) 如果不符合规范会抛异常
	static JsonValueArrayPtr getArray(BufferJsonReader & reader);
	//读取json的string 如 "dfdf" 如果不符合规范会抛异常
	static JsonValueStringPtr getString(BufferJsonReader & reader,char head='\"');
	//读取json的数字 如 -213.56 如果不符合规范会抛异常
	static JsonValueNumPtr getNum(BufferJsonReader & reader,char head);
	//读取json的boolean值  如 true false 如果不符合规范会抛异常
	static JsonValueBooleanPtr getBoolean(BufferJsonReader & reader,char c);
	//读取json的 null 如果不符合规范会抛异常
	static JsonValuePtr getNull(BufferJsonReader & reader,char c);
	//获取16进制形式的值 如\u003f 如果不符合规范会抛异常
	static uint32_t getHex(BufferJsonReader & reader);
	//判断一个字符是否符合json定义的空白字符
	static bool isspace(char c);
};


class TC_JsonWriteOstream
{
public:
	static void writeValue(const JsonValuePtr & p, ostream& ostr);
private:
	//string 类型到json字符串
	static void writeString(const JsonValueStringPtr & p, ostream& ostr);
	static void writeString(const string & s, ostream& ostr);

	//num 类型到json字符串
	static void writeNum(const JsonValueNumPtr & p, ostream& ostr);

	//obj 类型到json字符串
	static void writeObj(const JsonValueObjPtr & p, ostream& ostr);

	//array 类型到json字符串
	static void writeArray(const JsonValueArrayPtr & p, ostream& ostr);

	//boolean 类型到json字符串
	static void writeBoolean(const JsonValueBooleanPtr & p, ostream& ostr);
};
}

#endif
