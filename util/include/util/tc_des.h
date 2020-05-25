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
 * @brief DES encryption and decryption class (translated to C code)
 *  
 * @author  ruanshudong@qq.com
 */             
/////////////////////////////////////////////////


/**
 * @brief des异常. 
 * @brief des exception
 */
struct TC_DES_Exception : public TC_Exception
{
    TC_DES_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_DES_Exception() throw(){};
};


/**
 * @brief des/3des加密解密源码, 不依赖任何库.
 * @brief Use des/3des to encrypt and decrypt the source code, independent of any library.
 *
 * 在网上流行的d3des.h d3des.c修改完成.
 * It is modified by the popular 'd3des.h' and 'd3des.c' on the web.
 *  
 * 对于des加密,8位密钥,不足8位的右补0x00,多于8位,只取左8位有效. 
 * For des encryption, use 8-bit key. If it is less than 8 bits, right complete 0x00. If it is more than 8 bits, only the left 8 bits are valid.
 *  
 * 加密内容8位补齐,补齐方式为:少1位补一个0x01,少2位补两个0x02,...
 * The encrypted content follow 8-bit completion, the completion method is: less 1 bit to complete one 0x01, less 2 bits to complete two 0x02,...
 *  
 * 本身已8位对齐的,后面补八个0x08.
 * If itself is 8-bit aligned, complete eight '0x08' at the tail.
 *
 * 对于3des加解密,如下:只支持3des-ecb加密方式； 
 * For 3des encryption and decryption, only 3des-ecb encryption is supported;
 *  
 * 24位密钥,不足24位的右补0x00,多余24位,只取左24位有效; 
 * For 24-bit key, if it is less than 24 bits, right complete 0x00. If it is more than 24 bits, only the left 24 bits are valid.
 *  
 * 加密内容8位补齐，补齐方式为：少1位补一个0x01,少2位补两个0x02,...
 * The encrypted content follow 8-bit completion, the completion method is: less 1 bit to complete one 0x01, less 2 bits to complete two 0x02,...
 *  
 * 本身已8位对齐的，后面补八个0x08.
 * If itself is 8-bit aligned, complete eight '0x08' at the tail.
 *
 * Key必须是null结束的字符串.
 * The key must be a null-terminated string.
 *
 */
class TC_Des
{
public:
    /**
	 * @brief des加密.
     * @brief des encryption
	 *  
	 * @param key     key, 8个字节
     * @param key     key, 8 bits
	 * @param sIn     输入buffer
     * @param sIn     input buffer
	 * @param iInLen  输入buffer长度
     * @param iInLen  input buffer length
	 * @return        string 加密后的内容
     * @return        string, the encrypted content
     */
    static string encrypt(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief des解密.
     * @brief des decryption
     *
	 * @param key     key, 8个字节
     * @param key     key, 8 bits
	 * @param sIn     输入buffer
     * @param sIn     input buffer
	 * @param iInlen  输入buffer长度
     * @param iInLen  input buffer length
	 * @return        string 解码后的内容, 如果解密失败, 则为空
     * @return        string, the decrypted content, if decryption failed, return null.
     */
    static string decrypt(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  3des加密. 
     * @brief  3des encryption
	 *
     * @param key     key, 24个字节
     * @param key     key, 24 bits
	 * @param sIn     输入buffer
     * @param sIn     input buffer
	 * @param iInLen  输入buffer长度
     * @param iInLen  input buffer length 
	 * @return        string 加密后的内容
     * @return        string, the encrypted content
     */
    static string encrypt3(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  3des解密. 
     * @brief  3des decryption
	 * @param key      key, 24个字节
     * @param key      key, 24 bits
	 * @param sIn      输入buffer
     * @param sIn      input buffer
	 * @param iInlen   输入buffer长度
     * @param iInLen   input buffer length 
	 * @return         string解码后的内容, 如果解密失败, 则为空
     * @return         string, the dcrypted content, if decryption failed, return null.
     */
    static string decrypt3(const char *key, const char *sIn, size_t iInlen);

    /**
	 * @brief  定义加密/解密 . 
     * @brief  Define Encryption/Decryption
     */
    enum
    {
        /**Encryption*/
        EN0 = 0, /**加密*/
        /**Decryption*/
        DE1 = 1  /**解密*/
    };

protected:
    /**
	 * @brief  获取key. 
     * @brief  Get key.
	 *  
     * @param key  key值
     * @param key  key value
     * @param mode 模式 ：0代表加密, 1代表解密
     * @param mode mode : 0(encryption), 1(decryption)
     */
	static void deskey(const char *key, short mode, uint32_t *k);

    /**
	 * @brief  des加密/解密. 
     * @brief  DES Encryption/Decryption
	 *  
     * @param from  8个字节
     * @param from  8 bits
     * @param to    加密解密只有的8个字节
     * @param to    encrypt and decrypt the only 8 bits.
     */
	static void des(const char *from, char *to, uint32_t *KnL);

    /**
	 * @brief  获取key. 
     * @brief  Get key.
	 *  
     * @param key   key值
     * @param key   key value
     * @param mode  模式，0代表加密, 1代表解密
     * @param mode  mode : 0(encryption), 1(decryption)
     */
    static void des3key(const char *key, short mode, uint32_t *KnL, uint32_t *KnR, uint32_t *Kn3);

    /**
	 * @brief  3des. 
     * @brief  3DES
	 *  
     * @param from   8个字节
     * @param from  8 bits
     * @param into  加密解密只有的8个字节
     * @param to    encrypt and decrypt the only 8 bits.
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

