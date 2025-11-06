#ifndef _SipControllerImp_H_
#define _SipControllerImp_H_

#include "servant/Application.h"
#include "SipController.h"

/**
 *
 *
 */
class SipControllerImp : public VoipApp::SipController
{
public:
    /**
     *
     */
    virtual ~SipControllerImp() {}

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
    virtual tars::Bool getCloudSipGatewayIp(const std::string & sipGatewayId,std::string &sipGatewayOid,std::string &sipGatewayIp,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool mrfpMediaTransFinished(const std::string & callId,const std::string & caller,const std::string & callee,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool ocsChargeStatusReport(const std::string & sipGatewayId,const std::string & callId,tars::Int32 iSessId,tars::Int32 iCCId,tars::Int32 iCCStatusType,tars::Int32 iStatCode,tars::Int32 reqTime,tars::Int32 resultCode,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool recvSipMsgFromMgcfSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool recvSipMsgFromOtherSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;

};
/////////////////////////////////////////////////////
#endif
