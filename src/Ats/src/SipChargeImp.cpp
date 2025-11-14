#include "SipChargeImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipChargeImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipChargeImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool SipChargeImp::getCallDuration(const std::string & sipGatewayId,const std::string & callId,tars::Int32 sessId,tars::Int32 ccId,tars::Int32 usedTime,std::string &atsOid,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipChargeImp::getOcsChargeStatus(const std::string & sipGatewayId,const std::string & callId,const VoipApp::OcsChargeInfo & chargeInfo,std::string &atsOid,tars::Int32 &iSessId,tars::Int32 &iCCId,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipChargeImp::getOcsConnectStatus(const std::string & sipGatewayId,std::string &atsOid,tars::Int32 &sessId,tars::Int32 &status,tars::Int32 &statCode,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipChargeImp::stopOcsCharge(const std::string & sipGatewayId,const std::string & callId,tars::Int32 sessId,tars::Int32 ccId,tars::Int32 usedTime,tars::Int32 ringTime,tars::Int32 finishType,const map<std::string, std::string> & params,std::string &atsOid,tars::TarsCurrentPtr _current_)
{
    return true;
}


