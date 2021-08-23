#include "WinServer.h"
#include "HelloImp.h"
#include "servant/Application.h"

using namespace std;

WinServer::~WinServer()
{

}


void
WinServer::initialize()
{
	addServant<HelloImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".WinObj");

	TARS_ADD_ADMIN_CMD_NORMAL("AdminCmdNormalTest", WinServer::cmdAdd);

	TARS_ADD_ADMIN_CMD_NORMAL("CmdToDelete", WinServer::cmdAdd);

	getNotifyObserver()->unregisterNotify("CmdToDelete", this);

	TARS_ADD_ADMIN_CMD_NORMAL("DeletePrefixCmd", WinServer::delTarsViewVersion);

}

void WinServer::destroyApp()
{
}

void WinServer::run()
{
	this->waitForShutdown();
}

bool WinServer::cmdAdd(const string& command, const string& params, string& result)
{
	result = params + " AdminCmdNormalTest success!";
	return true;
}

bool WinServer::delTarsViewVersion(const string& command, const string& params, string& result)
{
	getNotifyObserver()->unregisterPrefix("tars.viewversion", this);

	result = "Delete success!";
	return true;
}
