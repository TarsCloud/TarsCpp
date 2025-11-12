#ifndef _SipRegImp_H_
#define _SipRegImp_H_

#include "servant/Application.h"
#include "SipReg.h"

/**
 *
 *
 */
class SipRegImp : public VoipApp::SipReg
{
public:
    /**
     *
     */
    virtual ~SipRegImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    virtual tars::Int32 registerService(
        const VoipApp::ServiceInfo & serviceInfo,
        tars::TarsCurrentPtr current);
    
    virtual tars::Int32 queryServices(
        const VoipApp::QueryCondition & condition,
        vector<VoipApp::ServiceInfo> &services,
        tars::TarsCurrentPtr current);
    
    virtual tars::Int32 unregisterService(
        const string & serviceId,
        tars::TarsCurrentPtr current);
    
    virtual tars::Int32 heartbeat(
        const string & serviceId,
        tars::TarsCurrentPtr current);

private:
    // 服务列表（key=serviceId, value=ServiceInfo）
    map<string, VoipApp::ServiceInfo> m_services;
    std::mutex m_mutex;
    
    // 定时清理过期服务的线程
    std::thread m_cleanupThread;
    bool m_running;
    
    // 清理过期服务
    void cleanupExpiredServices();
};
/////////////////////////////////////////////////////
#endif
