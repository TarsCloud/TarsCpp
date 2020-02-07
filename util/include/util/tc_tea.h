#ifndef _TC_TEA_H_
#define _TC_TEA_H_

#include "util/tc_platform.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>

#include "util/tc_ex.h"

using namespace std;

namespace tars
{
/////////////////////////////////////////////////
/**
 * @file tc_tea.h 
 * @brief tea加解密类(修改至c版本) . 
 *  
 * @author jarodruan@tencent.com 
 */
/////////////////////////////////////////////////
           
/**
 * @brief 加密异常类
 */            
struct TC_Tea_Exception : public TC_Exception
{
    TC_Tea_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_Tea_Exception() throw(){};
};

/**
 * @brief tea算法，通常用第二代算法 
 */
class TC_Tea
{
public:

    /**
     * @brief 加密.
     *
     * @param key       加密的key, 16个字节 
     * @param sIn       输入buffer 
     * @param iLength   输入buffer长度 
     * @param buffer    vector<char>, 加密后二进制串
     */
    static void encrypt(const char *key, const char *sIn, size_t iLength, vector<char> &buffer);

    /**
     * @brief 解密.
     *   
     * @param key      解密的key, 16个字节 
     * @param sIn      需要解密的buffer 
     * @param iLength  buffer长度 
     * @param buffer   vector<char>, 解密后二进制串
     * @return bool,   成功失败
     */
    static bool decrypt(const char *key, const char *sIn, size_t iLength, vector<char> &buffer);
};

}

#endif

