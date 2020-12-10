#ifndef __TC_XML_H_
#define __TC_XML_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

#include <stdio.h>

#include "util/tc_autoptr.h"
using namespace std;

namespace tars
{
/////////////////////////////////////////////////
// 说明: Xml编解码的公共库
// Author : linfengchen@tencent.com
/////////////////////////////////////////////////

/**
* 编解码抛出的异常
*/
struct TC_Xml_Exception : public TC_Exception
{
    TC_Xml_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Xml_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_Xml_Exception() throw(){};
};

enum eXmlType
{
    eXmlTypeString = 1,
    eXmlTypeArray,
    eXmlTypeObj,
};

/*
 * Xml类型的基类。没有任何意义
 */
class XmlValue : public tars::TC_HandleBase
{
public:
    virtual eXmlType getType()=0;
    virtual ~XmlValue(){}
protected:
    XmlValue*   parent;
};
typedef tars::TC_AutoPtr<XmlValue> XmlValuePtr;


/*
 * Xml类型 string类型
 */
class XmlValueString : public XmlValue
{
public:
    XmlValueString(const string & s, bool _cdata = false):value(s), cdata(_cdata)
    {
    }
    XmlValueString(bool _cdata = false):cdata(_cdata)
    {
    }

    eXmlType getType()
    {
        return eXmlTypeString;
    }
    virtual ~XmlValueString(){}
    string value;
    bool   cdata;
};
typedef tars::TC_AutoPtr<XmlValueString> XmlValueStringPtr;


/*
 * Xml类型 object类型 例如
 */
class XmlValueObj: public XmlValue
{
public:
    eXmlType getType()
    {
        return eXmlTypeObj;
    }
    virtual ~XmlValueObj(){}
public:
    map<string,	XmlValuePtr> value;
};
typedef tars::TC_AutoPtr<XmlValueObj> XmlValueObjPtr;

/*
 * Xml类型 array类型 例如
 */
class XmlValueArray: public XmlValue
{
public:
    eXmlType getType()
    {
        return eXmlTypeArray;
    }
    void push_back(XmlValuePtr & p)
    {
        value.push_back(p);
    }
    virtual ~XmlValueArray(){}
public:
    vector<XmlValuePtr> value;
};
typedef tars::TC_AutoPtr<XmlValueArray> XmlValueArrayPtr;

/*
 * 分析Xml字符串用到的 读字符的类
 */
class BufferXmlReader
{
public:
    const char *        _buf;		///< 缓冲区
    size_t              _len;    	///< 缓冲区长度
    size_t              _pos;		///< 当前位置

public:

    BufferXmlReader () :_buf(NULL),_len(0), _pos(0) {}

    void reset() { _pos = 0;}

    void setBuffer(const char * buf, size_t len)
    {
        _buf = buf;
        _len = len;
        _pos = 0;
    }

    void setBuffer(const std::vector<char> &buf)
    {
        _buf = &buf[0];
        _len = buf.size();
        _pos = 0;
    }

    bool hasEnd()
    {
        return (_pos >= _len || *(_buf + _pos) == 0);
    }

    bool expect(char ch)
    {
        if (get() == ch)
        {
            _pos++;
            return true;
        }
        return false;
    }

    void skip(size_t i = 1)
    {
        _pos += i;
    }

    char read()
    {
        check(++_pos);
        return *(_buf+_pos-1);
    }

    char get(size_t pos = 0)
    {
        check(pos + _pos);
        return *(_buf+_pos + pos);
    }

    size_t pos()
    {
        return _pos;
    }

    void check(size_t pos)
    {
        if (pos > _len)
        {
            char s[64];
            snprintf(s, sizeof(s), "buffer[%u] overflow when peekBuf, over %u.", (uint32_t)pos, (uint32_t)_len);
            throw TC_Xml_Exception(s);
        }
    }
};

/*
 * 分析Xml的类。都是static
 */
class TC_Xml
{
public:
    //Xml类型到字符串的转换
    static string writeValue(const XmlValuePtr& p, bool bHead = true);
	static void writeValue(const XmlValuePtr& p, vector<char>& buf, bool bHead = true);
    //Xml字符串到Xml结构的转换
    static XmlValuePtr getValue(const string& str);
	static XmlValuePtr getValue(const vector<char>& buf);
private:
    static XmlValuePtr getNode(BufferXmlReader& reader, const string& nodename = "");
    static XmlValueStringPtr getValue(BufferXmlReader& reader);
    static XmlValueStringPtr getCdata(BufferXmlReader& reader);

    static void insertArray(const string& name, XmlValuePtr& v, XmlValueObjPtr& p);
    static bool isEndNode(BufferXmlReader& reader, const string& nodename);
    static void ignoreDeclaration(BufferXmlReader& reader);
    static bool ignoreComment(BufferXmlReader& reader);


    static void writeArray(std::ostream& os, const string& name, const XmlValuePtr& p);
    static void writeString(std::ostream& os, const XmlValuePtr& p);
    static void writeEChar(std::ostream& os, const string& data);
    static void writeObj(std::ostream& os, const XmlValuePtr& p);

    //判断一个字符是否符合Xml定义的空白字符
    static bool isspace(char c);
};

}

#endif

