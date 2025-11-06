#include "SipRelayImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipRelayImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipRelayImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool SipRelayImp::eraseSipDlgIdAndConnection(const std::string & sipGatewayId,const std::string & dialogId,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipRelayImp::getIntranetIp(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,std::string &intranetIp,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipRelayImp::getMgcfConnectStatus(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,map<std::string, std::string> &params,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipRelayImp::recvSipMsgFromCloudSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
