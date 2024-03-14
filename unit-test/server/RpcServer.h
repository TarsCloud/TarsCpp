//
// Created by jarod on 2024/3/14.
//

#pragma once
#include <iostream>
#include "servant/Application.h"

using namespace tars;

class RpcServer : public Application, public TC_Thread
{
public:
    /**
     * 析构函数
     **/
    virtual ~RpcServer();

    /**
     * 服务初始化
     **/
    virtual void initialize();

    /**
     * 服务销毁
     **/
    virtual void destroyApp();

protected:
    virtual void run();

};
