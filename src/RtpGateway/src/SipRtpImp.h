#ifndef _SipRtpImp_H_
#define _SipRtpImp_H_

#include "servant/Application.h"
#include "SipRtp.h"

/**
 *
 *
 */
class SipRtpImp : public VoipApp::SipRtp
{
public:
    /**
     *
     */
    virtual ~SipRtpImp() {}

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


    virtual tars::Bool allocRtpResource(tars::Bool isCaller,const map<std::string, std::string> & params,map<std::string, std::string> &retValus,tars::TarsCurrentPtr _current_) override;

    virtual tars::Bool releaseRtpResource(const std::string & callId,tars::TarsCurrentPtr _current_) override;

    virtual tars::Bool updateRecordStatus(const std::string & callId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_) override;

};
/////////////////////////////////////////////////////
#endif
