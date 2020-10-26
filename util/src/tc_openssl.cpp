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

bool TC_OpenSSL::_initialize = false;
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

void TC_OpenSSL::setReadBufferSize(size_t size)
{
	BIO_set_read_buffer_size(SSL_get_rbio(_ssl), size);
}

void TC_OpenSSL::setWriteBufferSize(size_t size)
{
	BIO_set_write_buffer_size(SSL_get_rbio(_ssl), size);
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

void TC_OpenSSL::initialize()
{
	if(!_initialize)
	{
		_initialize = true;
		(void) SSL_library_init();
		OpenSSL_add_all_algorithms();

		ERR_load_ERR_strings();
		SSL_load_error_strings();
	}
}

shared_ptr<TC_OpenSSL::CTX> TC_OpenSSL::newCtx(const std::string& cafile, const std::string& certfile, const std::string& keyfile, bool verifyClient, const string &ciphers)
{
	initialize();

	SSL_CTX* ctx = SSL_CTX_new(SSLv23_method());
	if (!ctx)
		return NULL;

#define RETURN_IF_FAIL(call) \
    if ((call) <= 0) { \
        ERR_print_errors_fp(stderr); \
        return NULL;\
    }

	int mode = SSL_VERIFY_NONE;
	if (verifyClient)
		mode |= SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
	if (!cafile.empty())
		mode |= SSL_VERIFY_PEER;

	SSL_CTX_set_verify(ctx, mode, NULL);

	SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);
	SSL_CTX_clear_options(ctx, SSL_OP_LEGACY_SERVER_CONNECT);
	SSL_CTX_clear_options(ctx, SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION);

	RETURN_IF_FAIL (SSL_CTX_set_session_id_context(ctx, (const unsigned char*)ctx, sizeof ctx));
	if (!cafile.empty())
		RETURN_IF_FAIL (SSL_CTX_load_verify_locations(ctx, cafile.data(), NULL));

	// 客户端可以不提供证书的
	if (!certfile.empty())
		RETURN_IF_FAIL (SSL_CTX_use_certificate_file(ctx, certfile.data(), SSL_FILETYPE_PEM));

	if (!keyfile.empty())
	{
		RETURN_IF_FAIL (SSL_CTX_use_PrivateKey_file(ctx, keyfile.data(), SSL_FILETYPE_PEM));
		RETURN_IF_FAIL (SSL_CTX_check_private_key(ctx));
	}

	if(!ciphers.empty()) {
		RETURN_IF_FAIL (SSL_CTX_set_cipher_list(ctx, ciphers.c_str()));
	}
#undef RETURN_IF_FAIL

	return std::make_shared<TC_OpenSSL::CTX>(ctx);
}


shared_ptr<TC_OpenSSL> TC_OpenSSL::newSSL(const std::shared_ptr<TC_OpenSSL::CTX> &ctx)
{
	initialize();

	SSL* ssl = SSL_new(ctx->ctx);

	SSL_set_mode(ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER); // allow retry ssl-write with different args
	SSL_set_bio(ssl, BIO_new(BIO_s_mem()), BIO_new(BIO_s_mem()));

	BIO_set_mem_eof_return(SSL_get_rbio(ssl), -1);
	BIO_set_mem_eof_return(SSL_get_wbio(ssl), -1);

	return std::make_shared<TC_OpenSSL>(ssl);
}


} // end namespace tars

#endif

