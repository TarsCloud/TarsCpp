#include "SipControllerImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipControllerImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipControllerImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool SipControllerImp::getCloudSipGatewayIp(const std::string & sipGatewayId,std::string &sipGatewayOid,std::string &sipGatewayIp,tars::TarsCurrentPtr _current_) 
{
    return true;
}
tars::Bool SipControllerImp::mrfpMediaTransFinished(const std::string & callId,const std::string & caller,const std::string & callee,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipControllerImp::ocsChargeStatusReport(const std::string & sipGatewayId,const std::string & callId,tars::Int32 iSessId,tars::Int32 iCCId,tars::Int32 iCCStatusType,tars::Int32 iStatCode,tars::Int32 reqTime,tars::Int32 resultCode,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipControllerImp::recvSipMsgFromMgcfSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipControllerImp::recvSipMsgFromOtherSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
