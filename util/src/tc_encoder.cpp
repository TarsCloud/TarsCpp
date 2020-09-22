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
#include "util/tc_platform.h"
#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <iconv.h>
#endif

#include <string.h>
#include "util/tc_encoder.h"
#include <iostream>

namespace tars
{

#if TARGET_PLATFORM_WINDOWS
#include <windows.h>

void TC_Encoder::gbk2utf8(const string &sIn, vector<string> &vtStr,int mode)
{
    string sOut;

    for(string::size_type pos = 0; pos < sIn.length(); ++pos)
    {
		if((unsigned char)sIn[pos] < 0x80)
        {
            //单字节(GBK: 0x00-0x7F)
			sOut += sIn[pos];
		}
        else
        {
            //双字节
            char pIn[128] = "\0";

            strncpy(pIn, sIn.c_str() + pos, 2);

            sOut = gbk2utf8(pIn);

            ++pos;
		}

        vtStr.push_back(sOut);
    }
}

std::string TC_Encoder::gbk2utf8(const std::string &strGbk,int mode)
{
    string outUtf8 = "";
    int n = MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, NULL, 0);
    WCHAR *str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1,str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char *str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    outUtf8 = str2;
    delete[]str1;
    str1 = NULL;
    delete[]str2;
    str2 = NULL;
    return outUtf8;
}

std::string TC_Encoder::utf82gbk(const std::string &strUtf8,int mode)
{
    string outGBK = "";
    int n = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
    WCHAR *str1 = new WCHAR[n];
    MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, str1, n);
    n = WideCharToMultiByte(CP_ACP, 0, str1, -1, NULL, 0, NULL, NULL);
    char *str2 = new char[n];
    WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, n, NULL, NULL);
    outGBK = str2;
    delete[] str1;
    str1 = NULL;
    delete[] str2;
    str2 = NULL;
    return outGBK;
}

#else

string TC_Encoder::gbk2utf8(const string &sIn,int mode)
{
	iconv_t cd;

    switch(mode){
    case TC_Encoder::ICONV_TRANSLIT:
        cd = iconv_open("UTF-8//TRANSLIT", "GBK");
        break;
    case TC_Encoder::ICONV_IGNORE:
        cd = iconv_open("UTF-8//IGNORE", "GBK");
        break;
    default:
        cd = iconv_open("UTF-8", "GBK");
        break;
    }
	if (cd == (iconv_t)-1){
        THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] iconv_open error");
    }

	string sOut;
	size_t bufsize = sIn.size()*2+1;
	char* buf = new char[bufsize];
	char* pOut = buf;
	size_t isize = sIn.length();
	size_t osize = bufsize;
	char* pIn = (char*)sIn.c_str();

	size_t ret = iconv(cd, &pIn, &isize, &pOut, &osize);
	if(-1 == ret && TC_Encoder::ICONV_NORMAL == mode){
		iconv_close(cd);
		delete []buf;
		THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] iconv error");
		return sOut;
	}

	iconv_close(cd);
	buf[bufsize-osize]=0;
	sOut.assign(buf);
	delete []buf;
	return sOut;
}

void TC_Encoder::gbk2utf8(const string &sIn, vector<string> &vtStr,int mode){
	string out = TC_Encoder::gbk2utf8(sIn,mode);
	for (size_t i = 0; i < out.size();) {
		unsigned char uc = out[i];
		if (uc >= 0xF0 && (i + 4) <= out.size()) {
			vtStr.push_back(out.substr(i, 4));
			i += 4;
			continue;
		}
		if (uc >= 0xE0 && (i + 3) <= out.size()) {
			vtStr.push_back(out.substr(i,3));
			i += 3;
			continue;
		}
		if (uc >= 0xC0 && (i + 2) <= out.size()) {
			vtStr.push_back(out.substr(i,2));
			i += 2;
			continue;
		}
		if ((i + 1) <= out.size()) {
			vtStr.push_back(out.substr(i,1));
			i++;
			continue;
		}
		else {
            THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] invalid utf8 string | conversion error");
		}
	}
}

void TC_Encoder::utf82gbk(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen,int mode)
{
	iconv_t cd;
    switch(mode){
    case TC_Encoder::ICONV_TRANSLIT:
        cd = iconv_open("GBK//TRANSLIT", "UTF-8");
        break;
    case TC_Encoder::ICONV_IGNORE:
        cd = iconv_open("GBK//IGNORE", "UTF-8");
        break;
    default:
        cd = iconv_open("GBK", "UTF-8");
        break;
    }
	if (cd == (iconv_t)-1){
        THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] iconv_open error");
    }

    char * pIn = (char*)sIn;
    size_t sizeLeftLen      = iMaxOutLen;
    size_t sizeInLen        = iInLen;
    char* pOut = sOut;

    size_t ret = iconv(cd, &pIn, &sizeInLen, (char **)&sOut, &sizeLeftLen);
    if (ret == (size_t) - 1 && TC_Encoder::ICONV_NORMAL == mode){
        iconv_close(cd);
        THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::utf82gbk] iconv error");
        return;
    }

    iconv_close(cd);
    pOut[iMaxOutLen - (int)sizeLeftLen] = '\0';
    iMaxOutLen = iMaxOutLen - (int)sizeLeftLen;
}

string TC_Encoder::utf82gbk(const string &sIn,int mode)
{
    if(sIn.length() == 0)
    {
        return "";
    }

    string sOut;

    int iLen = sIn.length() * 2 + 1;
    char *pOut = new char[iLen];

    try
    {
        utf82gbk(pOut, iLen, sIn.c_str(), sIn.length(),mode);
    }
    catch (TC_Encoder_Exception& e)
    {
        delete[] pOut;

        throw e;
    }

    sOut.assign(pOut, iLen);

    delete[] pOut;

	return sOut;
}

#endif

/**
 * \n -> \r\0
 * \r -> \r\r
 */
string TC_Encoder::transTo(const string& str, char f /*='\n'*/, char t /*= '\r'*/, char u /*= '\0'*/)
{
	string ret = str;

	for (size_t i = 0; i < ret.length(); ++i)
	{
		if (ret[i] == f)
		{
			ret[i] = t;

			ret.insert(++i, 1, u);
		}
		else if (ret[i] == t)
		{
			ret.insert(++i, 1, t);
		}
	}
	return ret;
}

/**
 * \r\0 -> \n
 * \r\r -> \r
 */
string TC_Encoder::transFrom(const string& str, char f /*= '\n'*/, char t /*= '\r'*/, char u /*= '\0'*/)
{
	string ret = "";

	for (string::const_iterator it = str.begin()
		; it != str.end()
		; ++it)
	{
		ret.append(1, *it);

		if (*it == t)
		{
			if (*(++it) == u)
			{
				*ret.rbegin() = f;
			}
		}
	}
	return ret;
}

}



