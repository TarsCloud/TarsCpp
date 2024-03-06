#include "mock/FrameworkServer.h"
#include "mock/QueryImp.h"

using namespace std;

FrameworkServer::~FrameworkServer()
{

}


void
FrameworkServer::initialize()
{
	addServant<QueryImp>(_serverBaseInfo.Application + "." + _serverBaseInfo.ServerName + ".QueryObj");
}

void FrameworkServer::destroyApp()
{
}

void FrameworkServer::run()
{
	waitForShutdown();
}
