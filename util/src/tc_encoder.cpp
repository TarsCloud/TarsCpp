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

void TC_Encoder::gbk2utf8(const string &sIn, vector<string> &vtStr)
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

std::string TC_Encoder::gbk2utf8(const std::string &strGbk)
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

std::string TC_Encoder::utf82gbk(const std::string &strUtf8)
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
// void TC_Encoder::gbk2utf8(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen)
// {
// 	char * pIn = (char*)sIn;
// 	char * pEnd = pIn+iInLen;
// 	char * pOut = sOut;
// 	size_t iLeftLen;
//     size_t iGbkLen;
// 	iconv_t cd;

// 	if (iInLen > iMaxOutLen)
// 	{
// 		throw TC_Encoder_Exception("[TC_Encoder::gbk2utf8] iInLen > iMaxOutLen error : ", errno);
// 	}

// 	cd = iconv_open("UTF-8","GBK");
// 	if (cd == (iconv_t)-1)
//     {
//         throw TC_Encoder_Exception("[TC_Encoder::gbk2utf8] iconv_open error : ", errno);
// 	}

// 	iLeftLen = iMaxOutLen;
// 	while(pIn < pEnd)
//     {
// 		if((unsigned char)(*pIn)==0x80)
//         {
//             //注意GBK的0x80转换为UTF-8时为E2 82 AC
// 			*pOut = 0xe2; pOut++; iLeftLen--;
// 			*pOut = 0x82; pOut++; iLeftLen--;
// 			*pOut = 0xac; pOut++; iLeftLen--;
// 			pIn++;
// 		}
// 		else if((unsigned char)(*pIn)<0x80)
//         {
//             //单字节(GBK: 0x00-0x7F)
// 			*pOut = *pIn;
// 			pIn++;pOut++;iLeftLen--;
// 		}
//         else
//         {
//             //双字节
// 			iGbkLen=2;
//             int iRet=iconv(cd, &pIn, (size_t *)&iGbkLen, (char **)&pOut, (size_t *)&iLeftLen);
//             if(iRet < 0)
//             {
// 				*pOut = ' '; //转换不了替换为空格
// 				pIn+=2; pOut++; iLeftLen--;
// 			}
// 		}
// 	}

// 	iconv_close(cd);
// 	sOut[iMaxOutLen - iLeftLen] = '\0';
//     iMaxOutLen = iMaxOutLen - iLeftLen;
// }

string TC_Encoder::gbk2utf8(const string &sIn)
{
	iconv_t cd;

	cd = iconv_open("UTF-8","GBK");
	if (cd == (iconv_t)-1)
    {
        TARS_THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] iconv_open error");
 
        // throw TC_Encoder_Exception("[TC_Encoder::gbk2utf8] iconv_open error", TC_Exception::getSystemCode());
    }

    string sOut;

    for(string::size_type pos = 0; pos < sIn.length(); ++pos)
    {
		if((unsigned char)sIn[pos] == 0x80)
        {
            //注意GBK的0x80转换为UTF-8时为E2 82 AC
            sOut += 0xe2;
            sOut += 0x82;
            sOut += 0xac;
		}
		else if((unsigned char)sIn[pos] < 0x80)
        {
            //单字节(GBK: 0x00-0x7F)
			sOut += sIn[pos];
		}
        else
        {
            //双字节
			size_t sizeGbkLen = 2;
            char pIn[128] = "\0";

            strncpy(pIn, sIn.c_str() + pos, sizeGbkLen);
            char *p = pIn;

            size_t sizeLeftLen = 128;
            char pOut[128] = "\0";
            char *o = pOut;
            int iRet = iconv(cd, &p, &sizeGbkLen, (char **)&o, &sizeLeftLen);
			if(iRet < 0)
            {
                //转换不了, 暂时替换为空格
				sOut += ' ';
			}
            else
            {
                sOut += pOut;
            }

            ++pos;

		}
	}

	iconv_close(cd);
	return sOut;
}

void TC_Encoder::gbk2utf8(const string &sIn, vector<string> &vtStr)
{
	iconv_t cd;

	cd = iconv_open("UTF-8","GBK");
	if (cd == (iconv_t)-1)
    {
        TARS_THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::gbk2utf8] iconv_open error");
        // throw TC_Encoder_Exception("[TC_Encoder::gbk2utf8] iconv_open error", TC_Exception::getSystemCode());
	}

    vtStr.clear();

    for(string::size_type pos = 0; pos < sIn.length(); ++pos)
    {
        string sOut;

		if((unsigned char)sIn[pos] == 0x80)
        {
            //注意GBK的0x80转换为UTF-8时为E2 82 AC
            sOut += 0xe2;
            sOut += 0x82;
            sOut += 0xac;
		}
		else if((unsigned char)sIn[pos] < 0x80)
        {
            //单字节(GBK: 0x00-0x7F)
			sOut += sIn[pos];
		}
        else
        {
            //双字节
			size_t iGbkLen = 2;
            char pIn[128] = "\0";

            strncpy(pIn, sIn.c_str() + pos, iGbkLen);
            char *p = pIn;

            size_t iLeftLen = 128;
            char pOut[128] = "\0";
            char *o = pOut;
            int iRet = iconv(cd, &p, (size_t *)&iGbkLen, (char **)&o, (size_t *)&iLeftLen);
			if(iRet < 0)
            {
                //转换不了, 暂时替换为空格
				sOut += ' ';
			}
            else
            {
                sOut += pOut;
            }

            ++pos;

		}

        vtStr.push_back(sOut);
	}

	iconv_close(cd);
}

// string TC_Encoder::utf82gbk(const string &sIn)
// {
//     if(sIn.length() == 0)
//     {
//         return "";
//     }    

// 	iconv_t cd;

// 	cd = iconv_open("GBK","UTF-8");
// 	if (cd == (iconv_t)-1)
//     {
//         throw TC_Encoder_Exception("[TC_Encoder::utf82gbk] iconv_open error", errno);
// 	}

//     size_t sizeLeftLen = sIn.length() * 2 + 1;
//     size_t iMaxOutLen  = sizeLeftLen;
//     // char *pOut = new char[iMaxOutLen];
//     char pOut[255];
//     // memset(pOut, iMaxOutLen, 0x00);
//     const char * pIn = sIn.c_str();
//     size_t sizeInLen   = sIn.length();

//     char *out = pOut;
//     cout << sizeInLen << ", " << iMaxOutLen << endl;
//     size_t ret = iconv(cd, (char**)&pIn, (size_t*)&sizeInLen, (char **)&out, (size_t*)&sizeLeftLen);
//     if (ret == (size_t) - 1)
//     {
//         // delete[] pOut;
//         iconv_close(cd);
//         throw TC_Encoder_Exception("[TC_Encoder::utf82gbk] iconv error", errno);
//     }

// 	iconv_close(cd);

//     out[iMaxOutLen - sizeLeftLen] = '\0';

//     cout << sIn << ", " <<  iMaxOutLen << "," << sizeLeftLen << "," << out << endl;

//     string sOut;
//     sOut.assign(out, (iMaxOutLen - sizeLeftLen));

//     // delete[] pOut;

// 	return sOut;
// }

void TC_Encoder::utf82gbk(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen)
{
    iconv_t cd;

    cd = iconv_open("GBK","UTF-8");
    if (cd == (iconv_t)-1)
    {
        TARS_THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::utf82gbk] iconv_open error");
        // throw TC_Encoder_Exception("[TC_Encoder::utf82gbk] iconv_open error", TC_Exception::getSystemCode());
    }

    char * pIn = (char*)sIn;
    size_t sizeLeftLen      = iMaxOutLen;
    size_t sizeInLen        = iInLen;
    char* pOut = sOut;

    size_t ret = iconv(cd, &pIn, &sizeInLen, (char **)&sOut, &sizeLeftLen);
    if (ret == (size_t) - 1)
    {
        iMaxOutLen = 0;
        iconv_close(cd);
        TARS_THROW_EXCEPTION_SYSCODE(TC_Encoder_Exception, "[TC_Encoder::utf82gbk] iconv error");
        // throw TC_Encoder_Exception("[TC_Encoder::utf82gbk] iconv error", TC_Exception::getSystemCode());
        return;
    }

    iconv_close(cd);

    pOut[iMaxOutLen - (int)sizeLeftLen] = '\0';

    iMaxOutLen = iMaxOutLen - (int)sizeLeftLen;
}

string TC_Encoder::utf82gbk(const string &sIn)
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
        utf82gbk(pOut, iLen, sIn.c_str(), sIn.length());
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



