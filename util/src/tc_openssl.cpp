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

#if TARS_SSL

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "util/tc_openssl.h"
//#include "util/tc_buffer.h"


namespace tars
{

TC_OpenSSL::TC_OpenSSL()
: _ssl(NULL)
, _bHandshaked(false)
, _isServer(false)
, _plainBuf(NULL)
{
}

TC_OpenSSL::~TC_OpenSSL()
{
    Release();
}

void TC_OpenSSL::Release()
{
    if (_ssl)
    {
        SSL_free(_ssl);
        _ssl = NULL;
    }
    _bHandshaked = false;
//    _err = 0;
}

void TC_OpenSSL::Init(SSL* ssl, bool isServer)
{
    assert (_ssl == NULL);
    _ssl = ssl;
    _bHandshaked = false;
    _isServer = isServer;
//    _err = 0;
}

bool TC_OpenSSL::IsHandshaked() const
{
    return _bHandshaked;
}

//bool TC_OpenSSL::HasError() const
//{
//    return _err != 0;
//}

int TC_OpenSSL::DoHandshake(TC_NetWorkBuffer &out, const void* data, size_t size)
{
    assert (!_bHandshaked);
    assert (_ssl);

    if (data && size)
    {
        // 写入ssl内存缓冲区
        BIO_write(SSL_get_rbio(_ssl), data, size);
    }

    ERR_clear_error(); 
    int ret = _isServer ? SSL_accept(_ssl) : SSL_connect(_ssl);

	int err = 0;

	if (ret <= 0)
    {
	    err = SSL_get_error(_ssl, ret);
        if (err != SSL_ERROR_WANT_READ)
        {
            return err;
        }
    }

    if (ret == 1)
    {
        _bHandshaked = true;
    }

    // the encrypted data from write buffer
//	vector<char> out;
//    TC_Buffer outdata;
    getMemData(SSL_get_wbio(_ssl), out);
//    if (!outdata.IsEmpty())
//    {
//        out.assign(outdata.ReadAddr(), outdata.ReadableSize());
//    }

//    return out;

	return 0;
}

int TC_OpenSSL::Write(const char* data, size_t size, TC_NetWorkBuffer &out)
{
    if (!_bHandshaked)
    {
	    //握手数据不用加密
	    out.addBuffer(data, size);
	    return 0;
    }

    // 会话数据需加密
    ERR_clear_error(); 
    int ret = SSL_write(_ssl, data, size); 
    if (ret <= 0) 
    {
        return SSL_get_error(_ssl, ret);
    }
//    _err = 0;

//    TC_Buffer toSend;
    getMemData(SSL_get_wbio(_ssl), out);
    return 0;
//    return std::string(toSend.ReadAddr(), toSend.ReadableSize());
}

int TC_OpenSSL::Read(const void* data, size_t size, TC_NetWorkBuffer &out)
{
    bool usedData = false;
    if (!_bHandshaked)
    {
        usedData = true;
        _plainBuf.clearBuffers();
	    int ret = DoHandshake(out, data, size);

        if (ret != 0)
            return false;

        if (_bHandshaked)
            ; // TODO onHandshake
    }

    // 不要用else，因为数据可能紧跟着最后的握手而来
    if (_bHandshaked)
    {
        if (!usedData)
        {
            // 写入ssl内存缓冲区
            BIO_write(SSL_get_rbio(_ssl), data, size);
        }

	    if (!doSSLRead(_ssl, _plainBuf))
	    {
		    return SSL_ERROR_SSL;
	    }
    }

    return 0;
}
//
//SSL* TC_OpenSSL::newSSL(const std::string& ctxName)
//{
//	SSL_CTX* ctx = TC_SSLManager::getInstance()->GetCtx(ctxName);
//	if (!ctx)
//		return NULL;
//
//	SSL* ssl = SSL_new(ctx);
//
//	SSL_set_mode(ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER); // allow retry ssl-write with different args
//	SSL_set_bio(ssl, BIO_new(BIO_s_mem()), BIO_new(BIO_s_mem()));
//
//	BIO_set_mem_eof_return(SSL_get_rbio(ssl), -1);
//	BIO_set_mem_eof_return(SSL_get_wbio(ssl), -1);
//
//	return ssl;
//}

void TC_OpenSSL::getMemData(BIO* bio, TC_NetWorkBuffer& buf)
{
	while (true)
	{
		char data[8*1024];
		int bytes = BIO_read(bio, data, sizeof(data));
		if (bytes <= 0)
			return;

		buf.addBuffer(data, bytes);
	}
}
//
//void TC_OpenSSL::getSSLHead(const char* data, char& type, unsigned short& ver, unsigned short& len)
//{
//	type = data[0];
//	ver = *(unsigned short*)(data + 1);
//	len = *(unsigned short*)(data + 3);
//
//	ver = ntohs(ver);
//	len = ntohs(len);
//}

int TC_OpenSSL::doSSLRead(SSL* ssl, TC_NetWorkBuffer& out)
{
	while (true)
	{
		char plainBuf[32 * 1024];

		ERR_clear_error();
		int bytes = SSL_read(ssl, plainBuf, sizeof plainBuf);
		if (bytes > 0)
		{
			out.addBuffer(plainBuf, bytes);
		}
		else
		{
			int err = SSL_get_error(ssl, bytes);

			// when peer issued renegotiation, here will demand us to send handshake data.
			// write to mem bio will always success, only need to check whether has data to send.
			//assert (err != SSL_ERROR_WANT_WRITE);

			if (err != SSL_ERROR_WANT_READ && err != SSL_ERROR_ZERO_RETURN)
			{
//				printf("DoSSLRead err %d\n", err);
				return err;
			}

			break;
		}
	}

	return 0;
}

} // end namespace tars

#endif

