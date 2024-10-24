#pragma once

#include <string>
#include "util/tc_platform.h"

using namespace std;

namespace tars
{
/**
* @brief  由于解析带分隔符的字符串，类似于csv文件的格式
* 
* example:
    string a = "abcedddfffgkf,200890055,13.95,58399.88";
    TC_Split sp(a, ',');
    string s = sp.getNextAsString();
    int64_t i64 = sp.getNextAsLong();
    double dvalue1 = sp.getNextAsDouble();
    double dvalue2 = sp.getNextAsDouble();
*/
class UTIL_DLL_API TC_Split
{

public:
    TC_Split(const string &data, char sep = ','):
        _p (data.c_str() ), _sep(sep)
    {

    }
    //注意 ，确保data以\0结束
    TC_Split(const char * data, char sep = ',') :
        _p(data), _sep(sep)
    {

    }
    ///////////////////////////////////////////////////////////////
    //getNext 系列函数，用于获取下一个解析出来的值，hasNext用于判断是否还有值；函数本身不会抛出异常，只会返回空或者0
    string getNextAsString();
    double getNextAsDouble();
    int64_t getNextAsLong();
    int getNextAsInt();
    //是否有下一个元素
    bool hasNext() const;

    ///////////////////////////////////////////////////////////////
    //getValue 系列函数，基于位置获取值不能与getNext相关函数混用，因为getNext相关函数会挪动指针的位置；函数本身不会抛出异常，只会返回空或者0
    //取某个位置的值
    string getValueAsString(size_t pos);
    double getValueAsDouble(size_t pos);
    int64_t getValueAsLong(size_t pos);
    int getValueAsInt(size_t pos);

protected:
    //是否分割符号
    bool isSepFlag(char c) const
    {
        return c == _sep;
    }
    string getNextSection();
    string getValue(size_t pos) const;

protected:
    const char * _p = NULL;
    char _sep = ',';
};

}

