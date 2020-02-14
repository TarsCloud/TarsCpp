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

#ifndef __TC_SSLMANAGER_H
#define __TC_SSLMANAGER_H

#if TARS_SSL

#include <map>
#include <string>
#include "util/tc_network_buffer.h"
#include "util/tc_singleton.h"

struct ssl_st;
typedef struct ssl_st SSL;

struct ssl_ctx_st;
typedef struct ssl_ctx_st SSL_CTX;

namespace tars
{

/////////////////////////////////////////////////
/** 
 *@file   tc_sslmgr.h
 *@brief  manager ssl, not thread safe!!
 */
/////////////////////////////////////////////////

class TC_OpenSSL;

class TC_SSLManager : public TC_Singleton<TC_SSLManager>
{
public:
	/**
	 * @brief constructor.
	 */
    TC_SSLManager();

	/**
 	* @brief deconstructor.
 	*/
    ~TC_SSLManager();

	/**
	 * @brief new ssl
	 */
	shared_ptr<TC_OpenSSL> newSSL(const std::string& ctxName);

	/**
	 * @brief add ctx
	 * @param name: ctx name
	 * @param cafile: peer cafile
	 * @param certfile: cert cafile
	 * @param keyfile: key file
	 * @param verifyClient: if verfy client
	 */
	bool addCtx(const std::string& name,
                const std::string& cafile, 
                const std::string& certfile, 
                const std::string& keyfile,
                bool verifyClient);

protected:
	SSL_CTX* getCtx(const std::string& name) const;
	SSL* newSSL(SSL_CTX *ctx);
	SSL_CTX *newCtx(const std::string& cafile, const std::string& certfile, const std::string& keyfile, bool verifyClient);

private:

    typedef std::map<std::string, SSL_CTX*> CTX_MAP;

    CTX_MAP _ctxSet;
};

} // end namespace tars

#endif // end #if TARS_SSL

#endif

