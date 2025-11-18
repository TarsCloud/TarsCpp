#ifndef __CALL_ROUTING_MANAGER__
#define __CALL_ROUTING_MANAGER__

#include "SipRouter.h"
#include "SipCallController/src/SipGW3Common.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"
#include "servant/Current.h"

namespace SipENUMProxy
{
    static const int ENUM_HTTP_REQ_TIME_OUT = 60 * 1000;

    class HttpRequestItem: virtual public tars::TC_HandleBase
    {
    public:
        HttpRequestItem(int64_t tid, const tars::TarsCurrentPtr & call, const std::string &callId, const std::string &caller,
            const std::string &callee, const std::string &enterIp, const std::string &gwIp);

        int64_t                 m_tid;
        int64_t                 m_reqTime;
        std::string             m_callId;
        std::string             m_enterIp;
        std::string             m_gwAddr;
        std::string             m_caller;
        std::string             m_callee;
        tars::TarsCurrentPtr    m_current;
    };
    typedef tars::TC_AutoPtr<HttpRequestItem> HttpRequestItemPtr;

    class HttpReqItemManager : virtual public tars::TC_ThreadRecMutex
    {
    public:
        void insertHttpReqRecord(const tars::TarsCurrentPtr & current, int64_t tid, const std::string &callId,
            const std::string &caller, const std::string &callee, const std::string &enterIp, const std::string &gwIp);
        HttpRequestItemPtr getHttpRequestRecord(int64_t tid);
        void removeHttpRequestRecord(int64_t tid);
        void clearExpiredHttpRequestRecords();
        int64_t generateTid();

    private:
        tars::TC_ThreadRecMutex                      m_mutexHttpReqs;
        map<int64_t, HttpRequestItemPtr>    m_httpReqRecords;
    };

    class OfflineRoutingManager : virtual public tars::TC_ThreadRecMutex
    {
    public:
        void insertCallRoutingRecord(const std::string &caller, const std::string &callee, const std::string &enterIp, const std::string &gwIp, VoipApp::JegoRouterInfo & info);
        bool getCallRoutingRecord(const std::string &callId, const std::string &caller, const std::string &callee, const std::string &enterIp,
            const std::string &gwIp, VoipApp::JegoRouterInfo & info);
        int getsize();

    private:
        tars::TC_ThreadRecMutex                             m_mutexCallRoutingRecord;
        std::map<std::string, VoipApp::JegoRouterInfo>      m_mapCallRoutingRecord;
    };
}

#endif //__CALL_ROUTING_MANAGER__