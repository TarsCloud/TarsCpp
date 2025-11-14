#include "SipRouterImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipRouterImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipRouterImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool SipRouterImp::getRouteInfo(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipRouterImp::getRouteInfo2(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,map<std::string, std::string> &outParams,tars::TarsCurrentPtr _current_)
{
    return true;
}
