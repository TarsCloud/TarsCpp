#include "CustomServantImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void CustomServantImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void CustomServantImp::destroy()
{
    //destroy servant here:
    //...
}


int CustomServantImp::doRequest(tars::TarsCurrentPtr current, vector<char>& response)
{
	//Return to the data package requested by the client itself, that is, the original package return (4-byte length + 4-byte request + buffer)
	const vector<char>& request = current->getRequestBuffer();
	response = request;

//	cout << "doRequest: requestId:" << current->getRequestId() << ", funcName:" << current->getFuncName() << endl;
	return 0;
}

int CustomServantImp::doClose(TarsCurrentPtr current)
{
	LOG->debug() << "close ip: " << current->getIp() << endl;

	return 0;
}


