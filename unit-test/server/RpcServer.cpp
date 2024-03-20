//
// Created by jarod on 2024/3/14.
//

#include "RpcServer.h"
#include "HelloImp.h"
#include "HttpImp.h"

RpcServer::~RpcServer()
{

}


void
RpcServer::initialize()
{
    addServant<HelloImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".HelloObj");
    addServant<HttpImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".HttpObj");
    addServantProtocol(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".HttpObj", &TC_NetWorkBuffer::parseHttp);

}

/////////////////////////////////////////////////////////////////
void RpcServer::destroyApp()
{
}

void RpcServer::run()
{
    this->waitForShutdown();
}