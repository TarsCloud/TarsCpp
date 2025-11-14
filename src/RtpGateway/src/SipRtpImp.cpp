#include "SipRtpImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void SipRtpImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void SipRtpImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Bool allocRtpResource(tars::Bool isCaller,const map<std::string, std::string> & params,map<std::string, std::string> &retValus,tars::TarsCurrentPtr _current_)
{
    return true;
}

tars::Bool releaseRtpResource(const std::string & callId,tars::TarsCurrentPtr _current_)
{
    return true;
}

tars::Bool updateRecordStatus(const std::string & callId,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_)
{
    return true;
}
