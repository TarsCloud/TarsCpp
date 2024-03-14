//
// Created by jarod on 2024/3/6.
//

#include "mock/TarsMockUtil.h"
#include "mock/FrameworkServer.h"
#include "mock/framework.h"

TarsMockUtil::TarsMockUtil()
{
    _fs = std::make_shared<FrameworkServer>();
}

void TarsMockUtil::startFramework()
{
    _fs->main(MOCK_FRAMEWORK_CONFIG);
    _fs->start();
    _fs->waitForReady();
}

void TarsMockUtil::stopFramework()
{
    _fs->terminate();
}

std::shared_ptr<FrameworkServer> TarsMockUtil::getFrameworkServer()
{
    return _fs;
}

void TarsMockUtil::refreshRegistry(const string &obj, size_t count)
{
    do
    {
        TC_Common::sleep(1);

        vector<tars::EndpointF> activeEp;
        vector<tars::EndpointF> inactiveEp;

        _fs->getApplicationCommunicator()->getLocatorPrx()->findObjectById4All(obj, activeEp, inactiveEp);

        auto eps1 = EndpointInfo::toNodeEndpointF(activeEp);

        if(eps1.size() == count)
        {
            break;
        }

    }while(true);
}

