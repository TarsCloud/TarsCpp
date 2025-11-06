#ifndef _SipChargeImp_H_
#define _SipChargeImp_H_

#include "servant/Application.h"
#include "SipCharge.h"

/**
 *
 *
 */
class SipChargeImp : public VoipApp::SipCharge
{
public:
    /**
     *
     */
    virtual ~SipChargeImp() {}

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
    virtual tars::Bool getCallDuration(const std::string & sipGatewayId,const std::string & callId,tars::Int32 sessId,tars::Int32 ccId,tars::Int32 usedTime,std::string &atsOid,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getOcsChargeStatus(const std::string & sipGatewayId,const std::string & callId,const VoipApp::OcsChargeInfo & chargeInfo,std::string &atsOid,tars::Int32 &iSessId,tars::Int32 &iCCId,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getOcsConnectStatus(const std::string & sipGatewayId,std::string &atsOid,tars::Int32 &sessId,tars::Int32 &status,tars::Int32 &statCode,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool stopOcsCharge(const std::string & sipGatewayId,const std::string & callId,tars::Int32 sessId,tars::Int32 ccId,tars::Int32 usedTime,tars::Int32 ringTime,tars::Int32 finishType,const map<std::string, std::string> & params,std::string &atsOid,tars::TarsCurrentPtr _current_) override;

};
/////////////////////////////////////////////////////
#endif
