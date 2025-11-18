#include "CallRoutingManager.h"

namespace SipENUMProxy
{
    HttpRequestItem::HttpRequestItem(int64_t tid, const tars::TarsCurrentPtr & current, const std::string &callId, const std::string &caller,
        const std::string &callee, const std::string &enterIp, const std::string &gwIp)
        :m_tid(tid),m_current(current),m_callId(callId),m_caller(caller),m_callee(callee),m_enterIp(enterIp),m_gwAddr(gwIp)
    {
        m_reqTime = TNOWMS;
    }

    void HttpReqItemManager::insertHttpReqRecord(const tars::TarsCurrentPtr & current, int64_t tid, const std::string &callId,
        const std::string &caller, const std::string &callee, const std::string &enterIp, const std::string &gwIp)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexHttpReqs);
        map<int64_t, HttpRequestItemPtr>::iterator itId = m_httpReqRecords.find(tid);
        if (itId == m_httpReqRecords.end())
        {
            HttpRequestItemPtr ptr = new HttpRequestItem(tid, current, callId, caller, callee, enterIp, gwIp);
            m_httpReqRecords[tid] = ptr;
            TLOGDEBUG("[HttpReqItemManager] " << std::string(__FUNCTION__) + ", insert tid:" + std::to_string(tid) + ", callId:" + callId << endl);
        }
        else
        {
            TLOGWARN("[HttpReqItemManager] " << std::string(__FUNCTION__) + "tid:" + std::to_string(tid) + ", callId:" + callId << endl);
        }
    }

    HttpRequestItemPtr HttpReqItemManager::getHttpRequestRecord(int64_t tid)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexHttpReqs);
        map<int64_t, HttpRequestItemPtr>::iterator itorId = m_httpReqRecords.find(tid);
        if (itorId != m_httpReqRecords.end())
        {
            return itorId->second;
        }
        return 0;
    }

    void HttpReqItemManager::removeHttpRequestRecord(int64_t tid)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexHttpReqs);
        map<int64_t, HttpRequestItemPtr>::iterator itId = m_httpReqRecords.find(tid);
        if (itId != m_httpReqRecords.end())
        {
            m_httpReqRecords.erase(itId);
        }
    }

    void HttpReqItemManager::clearExpiredHttpRequestRecords()
    {
        int64_t curTime = TNOWMS;

        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexHttpReqs);
        map<int64_t, HttpRequestItemPtr>::iterator itId = m_httpReqRecords.begin();
        for (; itId != m_httpReqRecords.end(); )
        {
            const HttpRequestItemPtr & pRecord = itId->second;

            if (!pRecord)
                m_httpReqRecords.erase(itId++);

            if ((curTime - pRecord->m_reqTime) > ENUM_HTTP_REQ_TIME_OUT)
            {
                TLOGWARN("[HttpReqItemManager] " << std::string(__FUNCTION__) + "req time out tid:" + std::to_string(itId->first) << endl);
                m_httpReqRecords.erase(itId++);
            }
            else
                itId++;
        }
    }

    int64_t HttpReqItemManager::generateTid()
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexHttpReqs);
        do
        {
            int64_t tid = rand() % 10000000;
            map<int64_t, HttpRequestItemPtr>::iterator itId = m_httpReqRecords.find(tid);
            if (itId == m_httpReqRecords.end())
            {
                return tid;
            }
        } while (1);
    }

    void OfflineRoutingManager::insertCallRoutingRecord(const std::string &caller, const std::string &callee, const std::string &enterIp, const std::string &gwIp, VoipApp::JegoRouterInfo & info)
    {
        std::string key;
        int i = (!caller.empty() && caller[0] == '+') ? 1 : 0;

        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexCallRoutingRecord);
        switch (info.serviceType)
        {
        case SipCloudGateway::EN_ENTERPRISE_OUTGOING:
        case SipCloudGateway::EN_APP_OUTGOING:
            key = gwIp + enterIp + caller + callee;
            m_mapCallRoutingRecord[key] = info;
            break;
        case SipCloudGateway::EN_ENTERPRISE_INCOMMING:
        case SipCloudGateway::EN_APP_INCOMMING:
            key = gwIp + enterIp + callee;
            m_mapCallRoutingRecord[key] = info;
            break;
        case SipCloudGateway::EN_ENTERPRISE_MIDDLE_CALL_FORWARDING:
            key = gwIp + enterIp + caller + callee;
            m_mapCallRoutingRecord[key] = info;
            break;
        case SipCloudGateway::EN_MGCF_CALL_FORWARDING:
            for (i; i < caller.size(); i++)
            {
                std::string tmp = caller.substr(0, i + 1);
                key = gwIp + enterIp + tmp + callee;
                m_mapCallRoutingRecord[key] = info;
            }
            break;
        case SipCloudGateway::EN_ENTERPRISE_CALL_FORWARDING:
            for (i; i < caller.size(); i++)
            {
                std::string tmp = caller.substr(0, i + 1);
                key = gwIp + enterIp + tmp + callee;
                m_mapCallRoutingRecord[key] = info;
            }
            break;
        default:
            TLOGWARN("[OfflineRoutingManager] " << std::string(__FUNCTION__) + "() service type not surpport, callId:" + info.callId << endl);
            break;
        }

        TLOGINFO("[OfflineRoutingManager] " << std::string(__FUNCTION__) + "() key:" + key + ", callId:" + info.callId + ", serviceType:" + std::to_string(info.serviceType) << endl);
    }

    bool OfflineRoutingManager::getCallRoutingRecord(const std::string &callId, const std::string &caller, const std::string &callee, const std::string &enterIp,
        const std::string &gwIp, VoipApp::JegoRouterInfo & info)
    {
        std::string reason;
        info.callId = callId;
        std::string key = gwIp + enterIp + caller + callee;
        do
        {
            tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexCallRoutingRecord);
            map<std::string, VoipApp::JegoRouterInfo>::iterator itor = m_mapCallRoutingRecord.find(key);
            if (itor != m_mapCallRoutingRecord.end())
            {
                info = itor->second;
                break;
            }

            bool find = false;
            int j = (!caller.empty() && caller[0] == '+') ? 1 : 0;
            int len = caller.size();
            for (len; len >= j; len--)
            {
                key = gwIp + enterIp + caller.substr(0, len) + callee;
                itor = m_mapCallRoutingRecord.find(key);
                if (itor != m_mapCallRoutingRecord.end())
                {
                    info = itor->second;
                    find = true;
                    break;
                }
            }

            if (find)
                break;

            if (info.serviceType != SipCloudGateway::EN_ENTERPRISE_CALL_FORWARDING)
            {
                key = gwIp + enterIp + callee;
                itor = m_mapCallRoutingRecord.find(key);
                if (itor != m_mapCallRoutingRecord.end())
                {
                    info = itor->second;
                    break;
                }
            }

            reason = "not found callId:" + callId;
        } while (0);

        if (reason.empty())
        {
            int pos = caller.find(":");
            int pos2 = caller.find("@");

            switch (info.serviceType)
            {
            case SipCloudGateway::EN_ENTERPRISE_OUTGOING:
            case SipCloudGateway::EN_APP_OUTGOING:
                break;
            case SipCloudGateway::EN_ENTERPRISE_INCOMMING:
            case SipCloudGateway::EN_APP_INCOMMING:
                info.caller = caller;
                if (pos > 0 && pos2 > 0)
                {
                    info.number = caller.substr(pos + 1, pos2 - pos - 1);
                }
                else if (pos > 0)
                {
                    info.number = caller.substr(pos + 1);
                }
                break;
            case SipCloudGateway::EN_ENTERPRISE_MIDDLE_CALL_FORWARDING:
                break;
            case SipCloudGateway::EN_MGCF_CALL_FORWARDING:
                info.origSrcUser = caller;
                info.caller = "sip:";
                if (pos > 0 && pos2 > 0)
                {
                    info.number = caller.substr(pos + 1, pos2 - pos - 1);
                }
                else if (pos > 0)
                {
                    info.number = caller.substr(pos + 1);
                }
                info.caller += info.number;

                pos = info.midCallUser.find("@");
                if (pos > 0)
                {
                    info.caller += info.midCallUser.substr(pos);
                }
                break;
            case SipCloudGateway::EN_ENTERPRISE_CALL_FORWARDING:
                info.origSrcUser = caller;
                info.caller = caller;
                if (pos > 0 && pos2 > 0)
                {
                    info.number = caller.substr(pos + 1, pos2 - pos - 1);
                }
                else if (pos > 0)
                {
                    info.number = caller.substr(pos + 1);
                }
                break;
            default:
                break;
            }

            TLOGINFO("[OfflineRoutingManager] " << std::string(__FUNCTION__) + "() called, callId:" + info.callId + "\n callee:" + info.callee + "\n caller:" + info.caller +"\n mgcfAddr:" + info.gwAddr
                + "\n chargeAccount:" + info.chargeAccount + "\n displayName:" + info.number + "\n version:" + std::to_string(info.version) + "\n enterpriseIp:" + info.enterpriseIp + "\n enterprisePort:" + std::to_string(info.enterprisePort)
                + "\n middleUser:" + info.midCallUser + "\n originSrcUser:" + info.origSrcUser + "\n originDstUser:" + info.origDstUser + "\n routeId:" + info.routeId + "\n atomId" + info.atomId + "\n priceNumber:" + info.priceNumber
                + "\n record:" + std::to_string(info.record) + "\n beep:"+ std::to_string(info.beep) + "\n serviceType:" + std::to_string(info.serviceType) + "\n routeInfoList:" + std::to_string(info.routeInfoList.size())
                + "\n key:" + key);
        }
        else
        {
            TLOGWARN("[OfflineRoutingManager] " << std::string(__FUNCTION__) + "() failed caller:" + caller + ", callId:" + callId + ", callee:" + callee + ", reason:" + reason << endl);
        }

        return reason.empty() ?  true : false;
    }

    int OfflineRoutingManager::getsize()
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(m_mutexCallRoutingRecord);
        return m_mapCallRoutingRecord.size();
    }
}
