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

#ifndef __TARS_COOKIE_H__
#define __TARS_COOKIE_H__

#include "servant/ServantProxy.h"

/**
 * cookie操作类
 */
class CookieOp
{
public:
    /**
     * 构造函数
     */
    CookieOp()
    {
    }

    /**
     * 析构函数，清理掉已设置的cookie
     */
    ~CookieOp()
    {
        ServantProxyThreadData * td = ServantProxyThreadData::getData();
        assert(NULL != td);
        if (td)
        {
            td->_cookie.clear();
        }
    }

    /**
     * 获取cookie
     */
    static map<string, string> & getCookie()
    {
        ServantProxyThreadData * td = ServantProxyThreadData::getData();
        assert(NULL != td);

        return td->_cookie;
    }

    /**
     * 设置cookie
     */
    void setCookie(const map<string, string> &cookie)
    {
        ServantProxyThreadData * td = ServantProxyThreadData::getData();
        assert(NULL != td);
        if(td)
        {
            td->_cookie = cookie;
        }
    }
};

#endif