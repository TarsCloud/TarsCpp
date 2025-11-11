#ifndef _SipDbImp_H_
#define _SipDbImp_H_

#include "servant/Application.h"
#include "SipDb.h"

/**
 *
 *
 */
class SipDbImp : public VoipApp::SipDb
{
public:
    /**
     *
     */
    virtual ~SipDbImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual tars::Bool GetAllAllowedIps(const std::string & tableName,vector<VoipApp::AllowedIp> &vectIps,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetAllCallSipPolicys(const std::string & tableName,vector<VoipApp::CallSipPolicy> &policys,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetAllNationCodeIps(const std::string & tableName,vector<VoipApp::NationCodeIp> &vectIps,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetDomainAccessInfo(const std::string & tableName,vector<VoipApp::DomainAccessedConfig> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetDomainAccessInfo2(const std::string & tableName,vector<VoipApp::DomainAccessedConfig2> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetRtpGatewayconfigInfo(const std::string & tableName,vector<VoipApp::RtpGatewayAccessedConfig> &rtpGatewayConfigLst,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool GetSipProxyAccessCoreNetInfo(const std::string & tableName,vector<VoipApp::SipProxyAccessedCoreNetConfig> &vectSipProxyAccessedConfig,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool SetRtpGatewayAccessedId(const std::string & tableName,const std::string & rtpGatewayName,const std::string & rtpGatewayId,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool SetSipProxyAccessedId(const std::string & tableName,const std::string & sipProxyName,const std::string & sipProxyid,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool UpdateCallSipPolicys(const std::string & tableName,const vector<VoipApp::CallSipPolicyOperation> & OpSequence,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool UpdateDomainAccessConfig(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool UpdateDomainAccessConfig2(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation2> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool UpdateSipProxyAccessConfig(const std::string & tableName,const vector<VoipApp::SipProxyConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool getDomainLineConfig(const std::string & domainId,const std::string & appId,vector<VoipApp::DomainLineConfig> &config,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool getNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const std::string & callee,std::string &displayName,std::string &coreNetId,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool getSipCallRecordEx(const std::string & domainId,const std::string & appId,const std::string & rcpCallId,VoipApp::SipCallRecord &record,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool insertCallRecords(const std::string & tableName,const std::string & domainId,const std::string & appId,const vector<VoipApp::CallRecord> & records,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool insertNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool insertRecordsBatch(const std::string & domainId,const std::string & appId,const vector<VoipApp::SipCallRecord> & records,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool removeNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_) = 0;
    virtual tars::Bool updateDomainLineConfig(const std::string & domainId,const std::string & appId,const std::string & coreNetId,const std::string & origSbcIp,const std::string & origSbcPort,const std::string & dstSbcIp,const std::string & dstSbcPort,std::string &reason,tars::TarsCurrentPtr _current_) = 0;

};
/////////////////////////////////////////////////////
#endif
