#ifndef __TC_DES_H
#define __TC_DES_H

#include <string>
#include <stdint.h>
#include "util/tc_ex.h"
using namespace std;

namespace tars
{

/////////////////////////////////////////////////
/** 
 * @file tc_des.h
 * @brief des加解密类(翻译至c代码)
 *  
 * @author  ruanshudong@qq.com
 */             
/////////////////////////////////////////////////


/**
 * @brief des异常. 
 */
struct TC_DES_Exception : public TC_Exception
{
    TC_DES_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_DES_Exception() throw(){};
};


/**
 * @brief des/3des加密解密源码, 不依赖任何库.
 *
 * 在网上流行的d3des.h d3des.c修改完成.
 *  
 * 对于des加密,8位密钥,不足8位的右补0x00,多余8位,只取左8位有效. 
 *  
 * 加密内容8位补齐,补齐方式为:少1位补一个0x01,少2位补两个0x02,...
 *  
 * 本身已8位对齐的,后面补八个0x08.
 *
 * 对于3des加解密,如下:只支持3des-ecb加密方式； 
 *  
 * 24位密钥,不足24位的右补0x00,多余24位,只取左24位有效; 
 *  
 * 加密内容8位补齐，补齐方式为：少1位补一个0x01,少2位补两个0x02,...
 *  
 * 本身已8位对齐的，后面补八个0x08.
 *
 * Key必须是null结束的字符串.
 *
 */
class TC_Des
{
public:
    /**
	 * @brief des加密.
	 *  
	 * @param key     key, 8个字节
	 * @param sIn     输入buffer
	 * @param iInLen  输入buffer长度
	 * @return        string 加密后的内容
     */
    static string encrypt(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief des解密.
     *
	 * @param key     key, 8个字节
	 * @param sIn     输入buffer
	 * @param iInlen  输入buffer长度
	 * @return        string 解码后的内容, 如果解密失败, 则为空
     */
    static string decrypt(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  3des加密. 
	 *
     * @param key     key, 24个字节
	 * @param sIn     输入buffer
	 * @param iInLen  输入buffer长度
	 * @return        string 加密后的内容
     */
    static string encrypt3(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  3des解密. 
	 * @param key      key, 24个字节
	 * @param sIn      输入buffer
	 * @param iInlen   输入buffer长度
	 * @return         string解码后的内容, 如果解密失败, 则为空
     */
    static string decrypt3(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  定义加密/解密 . 
     */
    enum
    {
        EN0 = 0, /**加密*/
        DE1 = 1  /**解密*/
    };

protected:
    /**
	 * @brief  获取key. 
	 *  
     * @param key  key值
     * @param mode 模式 ：0代表加密, 1代表解密
     */
	static void deskey(const char *key, short mode, uint32_t *k);

    /**
	 * @brief  des加密/解密. 
	 *  
     * @param from  8个字节
     * @param to    加密解密只有的8个字节
     */
	static void des(const char *from, char *to, uint32_t *KnL);

    /**
	 * @brief  获取key. 
	 *  
     * @param key   key值
     * @param mode  模式，0代表加密, 1代表解密
     */
    static void des3key(const char *key, short mode, uint32_t *KnL, uint32_t *KnR, uint32_t *Kn3);

    /**
	 * @brief  3des. 
	 *  
     * @param from   8个字节
     * @param into  加密解密只有的8个字节
     */
    static void des3(const char *from, char *into, uint32_t *KnL, uint32_t *KnR, uint32_t *Kn3);

private:
    static void cookey(register uint32_t *raw1, uint32_t *k);
    static void scrunch(register const char *outof, register uint32_t *into);
    static void unscrun(register uint32_t *outof, register char *into);
    static void desfunc(register uint32_t *block, register uint32_t *keys);
};

}
#endif

