//
// Created by jarod on 2024/3/6.
//

#pragma once

#include <string>
#include <memory>
#include "servant/Application.h"
#include "mock/FrameworkServer.h"

class FrameworkServer;

class UTIL_DLL_API TarsMockUtil
{
public:
    TarsMockUtil();

    /**
     * 启动framework
     * @param fs
     */
    void startFramework();

    /**
     * 停止framework
     * @param fs
     */
    void stopFramework();

    /**
     * 等待主控返回count个服务
     * @param obj
     * @param count
     */
    void refreshRegistry(const std::string &obj, size_t count);

    /**
     *
     * @return
     */
    std::shared_ptr<FrameworkServer> getFrameworkServer();

protected:
    std::shared_ptr<FrameworkServer> _fs;
};

