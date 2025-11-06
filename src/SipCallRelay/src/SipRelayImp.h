#ifndef _SipRelayImp_H_
#define _SipRelayImp_H_

#include "servant/Application.h"
#include "SipRelay.h"

/**
 *
 *
 */
class SipRelayImp : public VoipApp::SipRelay
{
public:
    /**
     *
     */
    virtual ~SipRelayImp() {}

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
    virtual tars::Bool eraseSipDlgIdAndConnection(const std::string & sipGatewayId,const std::string & dialogId,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getIntranetIp(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,std::string &intranetIp,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool getMgcfConnectStatus(const std::string & sipGatewayId,std::string &mgcfSipGatewayOid,map<std::string, std::string> &params,tars::TarsCurrentPtr _current_) override;
    virtual tars::Bool recvSipMsgFromCloudSipGateway(const std::string & sipGatewayId,const std::string & sipMsg,const std::string & connectionId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;

};
/////////////////////////////////////////////////////
#endif
