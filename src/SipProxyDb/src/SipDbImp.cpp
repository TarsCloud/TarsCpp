#include "SipDbImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipDbImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipDbImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool SipDbImp::GetAllAllowedIps(const std::string & tableName,vector<VoipApp::AllowedIp> &vectIps,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::GetAllCallSipPolicys(const std::string & tableName,vector<VoipApp::CallSipPolicy> &policys,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::GetAllNationCodeIps(const std::string & tableName,vector<VoipApp::NationCodeIp> &vectIps,tars::TarsCurrentPtr _current_)
{
    return true;
}           
tars::Bool SipDbImp::GetDomainAccessInfo(const std::string & tableName,vector<VoipApp::DomainAccessedConfig> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::GetDomainAccessInfo2(const std::string & tableName,vector<VoipApp::DomainAccessedConfig2> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::GetRtpGatewayconfigInfo(const std::string & tableName,vector<VoipApp::RtpGatewayAccessedConfig> &rtpGatewayConfigLst,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::GetSipProxyAccessCoreNetInfo(const std::string & tableName,vector<VoipApp::SipProxyAccessedCoreNetConfig> &vectSipProxyAccessedConfig,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::SetRtpGatewayAccessedId(const std::string & tableName,const std::string & rtpGatewayName,const std::string & rtpGatewayId,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::SetSipProxyAccessedId(const std::string & tableName,const std::string & sipProxyName,const std::string & sipProxyid,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::UpdateCallSipPolicys(const std::string & tableName,const vector<VoipApp::CallSipPolicyOperation> & OpSequence,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::UpdateDomainAccessConfig(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::UpdateDomainAccessConfig2(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation2> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::UpdateSipProxyAccessConfig(const std::string & tableName,const vector<VoipApp::SipProxyConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::getDomainLineConfig(const std::string & domainId,const std::string & appId,vector<VoipApp::DomainLineConfig> &config,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::getNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const std::string & callee,std::string &displayName,std::string &coreNetId,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::getSipCallRecordEx(const std::string & domainId,const std::string & appId,const std::string & rcpCallId,VoipApp::SipCallRecord &record,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::insertCallRecords(const std::string & tableName,const std::string & domainId,const std::string & appId,const vector<VoipApp::CallRecord> & records,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::insertNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::insertRecordsBatch(const std::string & domainId,const std::string & appId,const vector<VoipApp::SipCallRecord> & records,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::removeNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}
tars::Bool SipDbImp::updateDomainLineConfig(const std::string & domainId,const std::string & appId,const std::string & coreNetId,const std::string & origSbcIp,const std::string & origSbcPort,const std::string & dstSbcIp,const std::string & dstSbcPort,std::string &reason,tars::TarsCurrentPtr _current_)
{
    return true;
}

