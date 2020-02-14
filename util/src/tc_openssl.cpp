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


namespace tars
{


//////////////////////////////////////////////////////////////////////////////////////////

TC_OpenSSL::TC_OpenSSL(SSL* ssl)
: _ssl(ssl)
, _bHandshaked(false)
, _isServer(false)
, _err(0)
, _plainBuf(NULL)
{
}

TC_OpenSSL::~TC_OpenSSL()
{
	release();
}

void TC_OpenSSL::release()
{
    if (_ssl)
    {
        SSL_free(_ssl);
        _ssl = NULL;
    }
    _bHandshaked = false;
    _err = 0;
}

void TC_OpenSSL::init(bool isServer)
{
    _bHandshaked = false;
    _isServer = isServer;
    _err = 0;
}

std::string TC_OpenSSL::getErrMsg() const
{
	std::shared_ptr<BIO> bio( BIO_new( BIO_s_mem() ), BIO_free );
	ERR_print_errors(bio.get());
	string buffer;
	buffer.resize(255);

	unsigned int startPos = 0;
	unsigned int bytesRead = 0;
	while (true)
	{
		int ret = BIO_read(bio.get(), &buffer[startPos], static_cast<int>(buffer.size()-startPos));
		if (ret > 0)
		{
			bytesRead += ret;
		}
		if (bytesRead < buffer.size())
		{
			break;
		}
		startPos = static_cast<unsigned int>(buffer.size());
		buffer.resize( 2*buffer.size() );
	}

	buffer.resize(bytesRead);
	return buffer;
}

bool TC_OpenSSL::isHandshaked() const
{
    return _bHandshaked;
}


int TC_OpenSSL::doHandshake(TC_NetWorkBuffer &out, const void* data, size_t size)
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

	_err = 0;

	if (ret <= 0)
    {
	    _err = SSL_get_error(_ssl, ret);

	    if (_err != SSL_ERROR_WANT_READ)
        {
            return _err;
        }
    }

    if (ret == 1)
    {
        _bHandshaked = true;
    }

    getMemData(SSL_get_wbio(_ssl), out);

	return 0;
}

int TC_OpenSSL::write(const char* data, size_t size, TC_NetWorkBuffer &out)
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
	    _err = SSL_get_error(_ssl, ret);
	    return _err;
    }
    _err = 0;

    getMemData(SSL_get_wbio(_ssl), out);
    return _err;
}

int TC_OpenSSL::read(const void* data, size_t size, TC_NetWorkBuffer &out)
{
    bool usedData = false;
    if (!_bHandshaked)
    {
        usedData = true;
        _plainBuf.clearBuffers();
	    int ret = doHandshake(out, data, size);

        if (ret != 0)
            return ret;

//        if (_bHandshaked)
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

	    _err = doSSLRead(_ssl, _plainBuf);
		if(_err != 0)
	    {
		    return SSL_ERROR_SSL;
	    }
    }

    return 0;
}

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

