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

#pragma once


#include <string>
#include <vector>
#include "util/tc_network_buffer.h"

struct ssl_st;
typedef struct ssl_st SSL;

struct bio_st;
typedef struct bio_st BIO;

struct ssl_ctx_st;
typedef struct ssl_ctx_st SSL_CTX;

namespace tars
{
#if TARS_SSL
/////////////////////////////////////////////////
/** 
 *@file   tc_openssl.h
 *@brief  OpenSsl wrapper
 * 
 */
/////////////////////////////////////////////////

/** 
 *@brief  OpenSsl wrapper
 */
class TC_OpenSSL
{
public:

   /**
    * @brief constructor.
    */
    TC_OpenSSL(SSL* ssl);

   /**
    * @brief deconstructor.
    */
    ~TC_OpenSSL();

    /**
     * ctx wrapper
     */
    struct CTX
    {
	    CTX(SSL_CTX *x) : ctx(x) {}
	    SSL_CTX *ctx;
    };

	/**
     * new ssl ctx
     * @param cafile
     * @param certfile
     * @param keyfile
     * @param verifyClient
     * @return
     */
	static shared_ptr<CTX> newCtx(const std::string& cafile, const std::string& certfile, const std::string& keyfile, bool verifyClient, const string &ciphers);

	/**
	 * new ssl
	 * @param ctx
	 * @return
	 */
	static shared_ptr<TC_OpenSSL> newSSL(const std::shared_ptr<TC_OpenSSL::CTX> &ctx);

	static void getMemData(BIO* bio, TC_NetWorkBuffer& buf);
	static int doSSLRead(SSL* ssl, TC_NetWorkBuffer& out);

protected:
   /**
    * @brief deny
    */
    TC_OpenSSL(const TC_OpenSSL& );
    void operator=(const TC_OpenSSL& );

	/**
	 * init openssl
	 */
	static void initialize();
	static bool _initialize;
public:

    /** 
     * @brief release SSL
     */
    void release();

    /** 
     * @brief init SSL
     */
    void init(bool isServer);

    /**
     * @brief  is handshake finish
     * @return true: is handshake finish
     */
    bool isHandshaked() const;

    /**
     * get error message
     * @return
     */
	string getErrMsg() const;

	/**
     * @brief  get recv buffer
     */
    TC_NetWorkBuffer * recvBuffer() { return &_plainBuf; }

    /** 
     * @brief handshake
     * @return 0: succ, !=0: fail
     */
    int doHandshake(TC_NetWorkBuffer &out, const void* data = NULL, size_t size = 0);

    /** 
     * @brief encode data before send
     * @param data, data pointer
     * @param size, data size
     * @param out, out buffer
     * @return 0: succ, !=0: fail
     */
    int write(const char* data, size_t size, TC_NetWorkBuffer &out);

    /** 
     * @brief decode data before parse protocol
     * @param data  data pointer
     * @param size  data size
     * @param out   out buffer
     * @return 0: succ, !=0: fail
     */
    int read(const void* data, size_t size, TC_NetWorkBuffer &out);

    /**
     * set read buffer size
     * @param size
     */
	void setReadBufferSize(size_t size);

	/**
	 * set write buffer size
	 * @param size
	 */
	void setWriteBufferSize(size_t size);

	friend class TC_SSLManager;
private:

    /**
     * ssl handle
     */
    SSL* _ssl;

    /**
     * is handeshake succ
     */
    bool _bHandshaked;

    /**
     * server/client
     */
    bool _isServer;

	/**
 	* ssl error code
 	*/
    int  _err;

    /**
     * recv buff
     */
    TC_NetWorkBuffer _plainBuf;

};
#else
//未开启openssl得时候，定义一个空得对象保留指针占位符
class TC_OpenSSL
{
public:

    /**
     * @brief constructor.
     */
    TC_OpenSSL(SSL* ssl) {};

    /**
     * @brief deconstructor.
     */
    ~TC_OpenSSL() {};

    struct CTX
    {
        CTX(SSL_CTX* x) : ctx(x) {}
        SSL_CTX* ctx;
    };
};

#endif

} // end namespace taf





