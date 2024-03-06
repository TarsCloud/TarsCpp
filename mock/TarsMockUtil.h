//
// Created by jarod on 2024/3/6.
//

#pragma once

#include <string>
#include <memory>

class FrameworkServer;

class TarsMockUtil
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

protected:
    std::shared_ptr<FrameworkServer> _fs;
};

