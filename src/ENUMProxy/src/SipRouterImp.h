#ifndef _SipRouterImp_H_
#define _SipRouterImp_H_

#include "servant/Application.h"
#include "SipRouter.h"

/**
 *
 *
 */
class SipRouterImp : public VoipApp::SipRouter
{
public:
    /**
     *
     */
    virtual ~SipRouterImp() {}

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
    virtual tars::Bool getRouteInfo(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getRouteInfo2(const std::string & gwOid,const std::string & caller,const std::string & callee,const std::string & callId,const std::string & callForwording,const std::string & sipgatewayIp,const std::string & enterpriseIp,const map<std::string, std::string> & params,VoipApp::JegoRouterInfo &info,map<std::string, std::string> &outParams,tars::TarsCurrentPtr _current_) override;

};
/////////////////////////////////////////////////////
#endif
