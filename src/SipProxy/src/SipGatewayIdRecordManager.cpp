#include "SipGatewayIdRecordManager.h"
#include "servant/Application.h"
#include "SipGateway/src/SipDialog.h"

namespace SipProxy
{
    //SipDialogRecord manager ...............................................
    SipDialogRecord::SipDialogRecord(const std::string & sipDialogId, const std::string & callId,const std::string & sipGatewayId, const std::string & sipPrxoyId)
    {
        m_sipGetwayId = sipGatewayId;
        m_sipProxyId = sipPrxoyId;
        m_sipDialogId = sipDialogId;
        m_callId = callId;
        m_updateTime = TNOWMS;
        m_respCon = 0;
        m_appCallCs = true;
    }

    SipDialogRecord::SipDialogRecord(const std::string & sipDialogId, const std::string & callId, const std::string & sipGatewayId, const std::string & sipPrxoyId,void * respCon, bool appCallCs)
    {
        m_sipGetwayId = sipGatewayId;
        m_sipProxyId = sipPrxoyId;
        m_sipDialogId = sipDialogId;
        m_callId = callId;
        m_updateTime = TNOWMS;
        m_appCallCs = appCallCs;
        m_respCon = respCon;
    }

    SipDialogRecordManager::SipDialogRecordManager()
    {
        m_iTotalInsertIdNum = 0;
        m_iTotalEreaseIdNum = 0;
    }

    bool SipDialogRecordManager::insertSipDialogRecord(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.find(callId);
        if (iterRecord == m_mapSipDilaogRecords.end())
        {
            m_mapSipDilaogRecords.insert(make_pair(callId, SipDialogRecord(sipDialogId, callId, sipGatewayId, sipProxyId)));
            TLOGINFO("[SipGatewayIdRecord] " << __FUNCTION__ << "() called. insert SipDialog Id :" << sipDialogId << " ,callId:" << callId << ",sipGatewayId:" << sipGatewayId << endl);

            ++m_iTotalInsertIdNum;
            return true;
        }
        else
        {
            TLOGWARN("[SipGatewayIdRecord] " << __FUNCTION__ << "() called. insert a DUPLICATE  SipDialog Id :" << sipDialogId << " ,callId:" << callId << endl);
        }
        return false;
    }

    bool SipDialogRecordManager::insertSipDialogRecord2(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId,void * respCon, bool appCallCs)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.find(callId);
        if (iterRecord == m_mapSipDilaogRecords.end())
        {
            m_mapSipDilaogRecords.insert(make_pair(callId, SipDialogRecord(sipDialogId, callId, sipGatewayId, sipProxyId, respCon, appCallCs)));
            TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. insert SipDialog Id :" + sipDialogId + " ,callId:" + callId + ",sipGatewayId:" + sipGatewayId);

            ++m_iTotalInsertIdNum;
            return true;
        }
        else
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. insert a DUPLICATE  SipDialog Id :" + sipDialogId + " ,callId:" + callId);
        }
        return false;
    }

    bool SipDialogRecordManager::ereaseSipDialogRecord(const std::string & callId)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.find(callId);
        if (iterRecord == m_mapSipDilaogRecords.end())
        {
            TLOGWARN("[SipGatewayIdRecord] " << __FUNCTION__ << "() called. Can not find the SipDialogId in the Map, callId:" << callId << endl);
            return false;
        }
        else
        {
            m_mapSipDilaogRecords.erase(iterRecord++);
        }

        TLOGINFO("[SipGatewayIdRecord] " << __FUNCTION__ << "() called.erased the SipDialogId in the Map,  callId:" << callId << endl);
        ++m_iTotalEreaseIdNum;
        return true;
    }

    bool SipDialogRecordManager::obtainSipDialogRecord(const std::string & callId, SipDialogRecord & sipDialogRecord)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.find(callId);
        if (iterRecord == m_mapSipDilaogRecords.end())
        {
            TLOGDEBUG("[SipGatewayIdRecord] " << __FUNCTION__ << "() called. Can not find the SipDialogId in the Map, callId:" << callId << endl);
            return false;
        }
        else
        {
            sipDialogRecord = iterRecord->second;
        }
        return true;
    }

    bool SipDialogRecordManager::ereaseSipDialogRecordBySipDialogId(const std::string & sipDialogId)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.begin();
        for ( ; iterRecord != m_mapSipDilaogRecords.end(); iterRecord++)
        {
            if (iterRecord->second.m_sipDialogId == sipDialogId)
            {
                std::string callId = iterRecord->first;
                m_mapSipDilaogRecords.erase(iterRecord);

                TLOGINFO("[SipGatewayIdRecord] " << __FUNCTION__ << "() called.erased the SipDialogId in the Map,  sipDialogId:" << sipDialogId << " ,callId:" << callId << endl);
                ++m_iTotalEreaseIdNum;
                return true;
            }
        }

        TLOGWARN("[SipGatewayIdRecord] " << __FUNCTION__ << "() called. Can not find the SipDialogId in the Map,sipDialogId:" << sipDialogId << endl);
        return false;
    }

    int SipDialogRecordManager::getTotalInsertIdNum()
    {
        return m_iTotalInsertIdNum;
    }

    int SipDialogRecordManager::getTotalEreaseIdNum()
    {
        return m_iTotalEreaseIdNum;
    }

    int SipDialogRecordManager::getRecordSize()
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        return int(m_mapSipDilaogRecords.size());
    }

    void SipDialogRecordManager::exceptionRecordDeals()
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        int64_t curTime = TNOWMS;
        int num = 0;

        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.begin();
        for (; iterRecord != m_mapSipDilaogRecords.end(); )
        {
            if ((curTime - iterRecord->second.m_updateTime) > SipGateway::TimeOutMaxDialogActiveDuration)
            {
                num++;
                m_mapSipDilaogRecords.erase(iterRecord++);
            }
            else
            {
                iterRecord++;
            }
        }
        TLOGINFO("SipGatewayIdRecord", "exceptionRecordDeals() called. exception num:" + std::to_string(num));
    }

    bool SipDialogRecordManager::updateSipGatewayId(const std::string & callId, const std::string & sipGatewayId)
    {
        std::lock_guard<std::mutex> lcok(m_mutexMapSipDilaogRecords);
        map<std::string, SipDialogRecord>::iterator iterRecord = m_mapSipDilaogRecords.find(callId);
        if (iterRecord == m_mapSipDilaogRecords.end())
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. Can not find the SipDialogId in the Map,  callId:" + callId);
            return false;
        }
        else
        {
            iterRecord->second.m_sipGetwayId = sipGatewayId;
        }
        return true;
    }

    // ............... SipRegStausDbManager ...................
    bool SipRegStausDbManager::insertRegRecord(const std::string & domainId, const std::string & sipUri, bool bProxyed, int failedTimes, int64_t time, const std::string & proxyId, bool tmpReg)
    {
        VoipApp::RegStaus2 record;
        record.m_sipUri = sipUri;
        record.m_proxyed = bProxyed;
        record.m_failedTimes = failedTimes;
        record.m_time = time;
        record.m_sipProxyId = proxyId;
        record.m_iTmpReg = tmpReg ? 1 : 0;

        std::lock_guard<std::mutex> lock(m_mutexRegStatus);
        DomainIdMapRegStatus::iterator itDomainId = m_mapRegStatus.find(domainId);
        if (itDomainId == m_mapRegStatus.end())
        {
            MapRegStatus regs;
            regs[sipUri] = record;
            m_mapRegStatus[domainId] = regs;
        }
        else
        {
            MapRegStatus & regs = itDomainId->second;
            regs[sipUri] = record;
        }
        return true;
    }

    bool SipRegStausDbManager::exportRegRecords(DomainIdMapRegStatus & records)
    {
        std::lock_guard<std::mutex> lock(m_mutexRegStatus);
        records.swap(m_mapRegStatus);
        return true;
    }

    void SipRegStausDbManager::removeRegRecord(const std::string & domainId, const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutexRegStatus);
        DomainIdMapRegStatus::iterator itDomainId = m_mapRegStatus.find(domainId);
        if (itDomainId == m_mapRegStatus.end())
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. domainId:" + domainId + " not find the sipUri Map, sipUri:" + sipUri);
            return;
        }

        MapRegStatus & regs = itDomainId->second;
        MapRegStatus::iterator iterUri = regs.find(sipUri);
        if (iterUri != regs.end())
        {
            regs.erase(sipUri);
        }
        else
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. Can not find the sipUri Map,  sipUri:" + sipUri);
        }
    }

    int SipRegStausDbManager::getSize()
    {
        std::lock_guard<std::mutex> lock(m_mutexRegStatus);
        int size = 0;
        DomainIdMapRegStatus::iterator itDomainId = m_mapRegStatus.begin();
        for (; itDomainId != m_mapRegStatus.end(); itDomainId++)
        {
            size += itDomainId->second.size();
        }
        return size;
    }

    //
    OutOfServService::OutOfServService(const std::string & serviceId, uint64_t ticks)
    {
        m_serviceId = serviceId;
        m_insertTicks = ticks;
    }

    OutOfServServiceManager::OutOfServServiceManager()
    {
    }

    void OutOfServServiceManager::onSchd()
    {
        std::lock_guard<std::mutex> lock(m_mutexServices);
        map<std::string, OutOfServService>::iterator iterId = m_mapServices.begin();
        for (; iterId != m_mapServices.end();  )
        {
            uint64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();
            if((curTicks - iterId->second.m_insertTicks) > 30 * 1000)
            {
                m_mapServices.erase(iterId++);
            }
            else
            {
                iterId++;
            }
        }
    }

    bool OutOfServServiceManager::insertOutOfSerServiceId(const std::string & serviceId)
    {
        std::lock_guard<std::mutex> lock(m_mutexServices);
        map<std::string, OutOfServService>::iterator iterId = m_mapServices.find(serviceId);
        if (iterId == m_mapServices.end())
        {
            uint64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();
            OutOfServService item(serviceId, curTicks);
            m_mapServices.insert(make_pair(serviceId, item));
            return true;
        }
        return false;
    }

    bool OutOfServServiceManager::exportOutOfSerServiceIds(set<std::string> & serviceIds)
    {
        std::lock_guard<std::mutex> lock(m_mutexServices);
        map<std::string, OutOfServService>::iterator iterId = m_mapServices.begin();
        for (; iterId != m_mapServices.end();  iterId++)
        {
            serviceIds.insert(iterId->second.m_serviceId);
        }
        return true;
    }

    //                                          SipAccountReadCache
    SipAccountReadCache::SipAccountReadCache(const std::string & id, const std::string & domainId,
        const std::string & coreNetId, const std::string & caller,const vector<std::string> & vectSipUris)
        :m_id(id), m_domainId(domainId),m_coreNetId(coreNetId),m_caller(caller),m_vectSipUris(vectSipUris)
    {
        m_lastTicks = TC_TimeProvider::getInstance()->getNowMs();
        m_reading = false;
    }

    std::string SipAccountReadCacheManager::getCacheId()
    {
        std::lock_guard<std::mutex> lock(m_mutexCaches);
        int times = 0;
        do
        {
            std::string id = TC_UUIDGenerator::getInstance()->genID();
            AccountReadCacheMap::iterator itId = m_mapCaches.find(id);
            if (itId == m_mapCaches.end())
                return id;

            times++;
            if (times > 10)
                break;

        } while (1);
        return TC_UUIDGenerator::getInstance()->genID();
    }

    bool SipAccountReadCacheManager::insertReadCache(const std::string & domainId,
        const std::string & coreNetId, const std::string & caller,const vector<std::string> & vectSipUris)
    {
        std::string id = getCacheId();

        std::lock_guard<std::mutex> lock(m_mutexCaches);
        AccountReadCacheMap::iterator itId = m_mapCaches.find(id);
        if (itId != m_mapCaches.end())
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.exist id:" + id);
            return false;
        }

        SipAccountReadCache req(id, domainId, coreNetId, caller, vectSipUris);
        m_mapCaches[id] = req;
        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.insert cache id:" + id + ",domainId:" + domainId + ",coreNetId:"+ coreNetId + ",size:" + std::to_string(int(vectSipUris.size())));
        return true;
    }

    void SipAccountReadCacheManager::getReadCaches(AccountReadCacheMap & mapCaches)
    {
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();

        std::lock_guard<std::mutex> lock(m_mutexCaches);
        AccountReadCacheMap::iterator itId = m_mapCaches.begin();
        for (; itId != m_mapCaches.end(); itId++)
        {
            if (((curTicks - itId->second.m_lastTicks) >= 2 * 60 * 1000) && itId->second.m_reading == false)
            {
                itId->second.m_reading = true;
                mapCaches[itId->first] = itId->second;
            }
        }
    }

    bool SipAccountReadCacheManager::updateReadCacheTicks(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutexCaches);
        AccountReadCacheMap::iterator itId = m_mapCaches.find(id);
        if (itId == m_mapCaches.end())
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache id:" + id);
            return false;
        }

        TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.cache id:" + id);
        itId->second.m_lastTicks = TC_TimeProvider::getInstance()->getNowMs();
        itId->second.m_reading = false;
        return true;
    }

    bool SipAccountReadCacheManager::deleteReadCache(const std::string & id)
    {
        std::lock_guard<std::mutex> lock(m_mutexCaches);
        AccountReadCacheMap::iterator itId = m_mapCaches.find(id);
        if (itId == m_mapCaches.end())
        {
            TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache id:" + id);
            return false;
        }

        m_mapCaches.erase(itId);
        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.delete cache:" + id);
        return true;
    }

    //                              AccountTransCache

    AccountTransCache::AccountTransCache()
    {
        m_ticks = TC_TimeProvider::getInstance()->getNowMs();
        m_finishedTicks = 0;
    }

    AccountTransCache::AccountTransCache(const std::string & sipUri, const std::string & oriProxyId, CacheType typeVal)
    {
        m_sipUri = sipUri;
        m_originProxyId = oriProxyId;
        m_cacheType = typeVal;
        m_finished = false;

        m_ticks = TC_TimeProvider::getInstance()->getNowMs();
        m_finishedTicks = 0;
    }

    bool AccountTransCacheManager::insertTransCaches(const std::string & originProxyId, CacheType typeVal, const vector<std::string> & sipUris)
    {
        int repeated  = 0;
        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        int uriSize = (int)sipUris.size();
        for (int i = 0; i < uriSize; i++)
        {
            const std::string & sipUri = sipUris.at(i);
            TransCacheMap::iterator itUri = m_mapTransCaches.find(sipUri);
            if (itUri != m_mapTransCaches.end())
            {
                TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.found cache sipUri:" + sipUri);
                repeated++;
                continue;
            }
            AccountTransCachePtr pCache = std::make_shared<AccountTransCache>(sipUri, originProxyId, typeVal);
            m_mapTransCaches[sipUri] = pCache;
        }

        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.originProxyId:" + originProxyId + " ,typeVal:"
                    + std::to_string(int(typeVal)) + ",size:" + std::to_string(uriSize) + ",repeated:" + std::to_string(repeated));
        return true;
    }

    void AccountTransCacheManager::finishTrans2(const vector<std::string> & sipUris, CacheType typeVal)
    {
        if (m_mapTransCaches.empty())
            return;

        int num = 0;
        int finished = 0;
        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        int uriSize = sipUris.size();
        for (int i = 0; i < uriSize; i++)
        {
            const std::string & sipUri = sipUris.at(i);
            TransCacheMap::iterator itUri = m_mapTransCaches.find(sipUri);
            if (itUri == m_mapTransCaches.end())
            {
                num++;
                if (num < 20)
                    TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache sipUri:" + sipUri);
                continue;
            }

            if (itUri->second->m_finished || itUri->second->m_cacheType != typeVal)
                continue;

            itUri->second->m_finished = true;
            itUri->second->m_finishedTicks = TC_TimeProvider::getInstance()->getNowMs();
            finished++;
        }

        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. size:" + std::to_string(uriSize)  + " ,typeVal:"
            + std::to_string(int(typeVal)) + ",not found cache:" + std::to_string(num) + ",finished:" + std::to_string(finished));
    }

    void AccountTransCacheManager::finishTrans(const std::string & sipUri, CacheType typeVal)
    {
        if (m_mapTransCaches.empty())
            return;

        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        TransCacheMap::iterator itUri = m_mapTransCaches.find(sipUri);
        if (itUri == m_mapTransCaches.end())
        {
            TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache sipUri:" + sipUri);
            return;
        }

        TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.cache sipUri:" + sipUri + " ,typeVal:"
            + std::to_string(int(typeVal)));
        if (itUri->second->m_finished || itUri->second->m_cacheType != typeVal)
            return;

        itUri->second->m_finished = true;
        itUri->second->m_finishedTicks = TC_TimeProvider::getInstance()->getNowMs();
    }

    void AccountTransCacheManager::getFinishedAccounts(vector<std::string> & sipUris, CacheType typeVal)
    {
        if (m_mapTransCaches.empty())
            return;

        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        TransCacheMap::iterator itUri = m_mapTransCaches.begin();
        for (; itUri != m_mapTransCaches.end(); itUri++)
        {
            if (itUri->second->m_finished && itUri->second->m_cacheType == typeVal)
                sipUris.push_back(itUri->first);
        }
    }

    void AccountTransCacheManager::deleteFinishedCaches(const set<std::string> & sipUris, CacheType typeVal)
    {
        if (m_mapTransCaches.empty())
            return;

        int num = 0;
         std::lock_guard<std::mutex> lck(m_mutexTransCaches);
         set<std::string>::const_iterator itSipUri = sipUris.begin();
         for (; itSipUri != sipUris.end(); itSipUri++)
         {
             const std::string & sipUri = *itSipUri;
             TransCacheMap::iterator itUri = m_mapTransCaches.find(sipUri);
             if (itUri == m_mapTransCaches.end())
             {
                 num ++;
                 TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache sipUri:" + sipUri);
                 continue;
             }

             if (itUri->second->m_cacheType != typeVal)
                 continue;

             TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called, erase cache sipUri:" + sipUri);
             m_mapTransCaches.erase(itUri);
         }

         TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cache:" + std::to_string(num) + ",total:" + std::to_string(int(sipUris.size())) + " ,typeVal:"
             + std::to_string(int(typeVal)));
    }

    void AccountTransCacheManager::deleteExpiredCaches()
    {
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();

        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        TransCacheMap::iterator itUri = m_mapTransCaches.begin();
        for (; itUri != m_mapTransCaches.end(); )
        {
            int64_t diffs1 = curTicks - itUri->second->m_finishedTicks;
            int64_t diffs2 = curTicks - itUri->second->m_ticks;

            if (itUri->second->m_finished && (diffs1 > 2 * 60 * 1000))
                m_mapTransCaches.erase(itUri++);
            else if (itUri->second->m_finished == false && diffs2 > 2 * 60 * 60 * 1000)
            {
                itUri->second->m_finished = true;
                itUri->second->m_finishedTicks = curTicks;
            }
            else if (diffs2 > (2 * 60 + 2) * 60 * 1000)
                m_mapTransCaches.erase(itUri++);
            else
                itUri++;
        }
    }

    void AccountTransCacheManager::getOccupyedNotifyUris(map<std::string, set<std::string> > & sipUris)
    {
        if (m_mapTransCaches.empty())
            return;

        sipUris.clear();
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();

        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        TransCacheMap::iterator itUri = m_mapTransCaches.begin();
        for (; itUri != m_mapTransCaches.end(); )
        {
            bool matched = false;
            if (itUri->second->m_finished && itUri->second->m_cacheType == kCacheWaitToAdd)
                matched = true;
            else if (itUri->second->m_cacheType == kCacheWaitToAdd && (curTicks - itUri->second->m_ticks) > 2 * 60 * 60 * 1000)
                matched = true;

            if (matched)
            {
                const std::string & oriProxyId = itUri->second->m_originProxyId;
                map<std::string, set<std::string> >::iterator itProxyId = sipUris.find(oriProxyId);
                if (itProxyId == sipUris.end())
                {
                    set<std::string> setUris;
                    setUris.insert(itUri->first);
                    sipUris.insert(make_pair(oriProxyId, setUris));
                }
                else
                {
                    itProxyId->second.insert(itUri->first);
                }

                m_mapTransCaches.erase(itUri ++);
            }
            else
                itUri ++;
        }

        TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.cache size:" + std::to_string(int(sipUris.size())));
    }

    void AccountTransCacheManager::getNeedRemovedSipUris(set<std::string> & sipUris)
    {
        if (m_mapTransCaches.empty())
            return;

        sipUris.clear();
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();

        std::lock_guard<std::mutex> lck(m_mutexTransCaches);
        TransCacheMap::iterator itUri = m_mapTransCaches.begin();
        for (; itUri != m_mapTransCaches.end(); itUri++)
        {
            bool matched = false;
            if (itUri->second->m_finished && itUri->second->m_cacheType == kCacheWaitToRemove)
                matched = true;
            else if (itUri->second->m_cacheType == kCacheWaitToRemove && (curTicks - itUri->second->m_ticks) > 2 * 60  * 60 * 1000)
                matched = true;

            if (matched)
                sipUris.insert(itUri->first);
        }

        TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.cache size:" + std::to_string(int(sipUris.size())));
    }

    int AccountTransCacheManager::getTransCacheSize()
    {
        return m_mapTransCaches.size();
    }



    //.................................

    void AccountTransCacheManager::insertNotifyCache(const std::string & toProxyId, NotifyType typeVal, const std::map<std::string, std::string> & sipUris, const std::string & cacheId, int64_t insertTicks)
    {
        std::lock_guard<std::mutex> lock(m_mutexNofityCaches);

        map<std::string, AccountTransNotifyCache>::iterator itId = m_mapNotifyCaches.find(cacheId);
        if (itId != m_mapNotifyCaches.end())
        {
            TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.cache id exist, toProxyId:" + toProxyId + ",type:" + std::to_string(int(typeVal)) + ",cache size:" + std::to_string(int(sipUris.size())));
            return;
        }

        AccountTransNotifyCache cache(toProxyId, typeVal, sipUris, insertTicks);
        if (!cacheId.empty())
            cache.m_id = cacheId;

        m_mapNotifyCaches[cache.m_id] = cache;
        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.toProxyId:" + toProxyId + ",type:" + std::to_string(int(typeVal)) + ",cache size:" + std::to_string(int(sipUris.size())) + ",id:" + cacheId);
    }

    void AccountTransCacheManager::deleteNotifyCache(const std::string & cacheId)
    {
        std::lock_guard<std::mutex> lock(m_mutexNofityCaches);
        if (m_mapNotifyCaches.empty())
            return;

        map<std::string, AccountTransNotifyCache>::iterator itId = m_mapNotifyCaches.find(cacheId);
        if (itId == m_mapNotifyCaches.end())
        {
            TLOGDEBUG("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.not found cahce, id:" + cacheId);
            return;
        }
        m_mapNotifyCaches.erase(itId);
        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called. erase cahce,id:" + cacheId);
    }

    void AccountTransCacheManager::exportNotifyCaches(map<std::string, AccountTransNotifyCache> & caches)
    {
        std::lock_guard<std::mutex> lock(m_mutexNofityCaches);
        if (m_mapNotifyCaches.empty())
            return;

        caches.swap(m_mapNotifyCaches);
        TLOGINFO("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.size:" + std::to_string(int(m_mapNotifyCaches.size())));
    }

    void AccountTransCacheManager::deleteExpiredNotifyCaches()
    {
        std::lock_guard<std::mutex> lock(m_mutexNofityCaches);
        if (m_mapNotifyCaches.empty())
            return;

        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();
        map<std::string, AccountTransNotifyCache>::iterator itId = m_mapNotifyCaches.begin();
        for (; itId != m_mapNotifyCaches.end(); )
        {
            int64_t diffs = curTicks - itId->second.m_insertTicks;
            if (diffs > 2 * 60 * 60 * 1000)
            {
                TLOGWARN("SipGatewayIdRecord", std::string(__FUNCTION__) + "() called.id:" + itId->first);
                m_mapNotifyCaches.erase(itId++);
            }
            else
            {
                itId++;
            }
        }
    }

    int AccountTransCacheManager::getNotifyCacheSize()
    {
        std::lock_guard<std::mutex> lock(m_mutexNofityCaches);
        if (m_mapNotifyCaches.empty())
            return 0;

        int sum = 0;
        map<std::string, AccountTransNotifyCache>::iterator itId = m_mapNotifyCaches.begin();
        for (; itId != m_mapNotifyCaches.end(); itId++)
        {
            sum += itId->second.m_sipUris.size();
        }
        return sum;
    }

    void AccountTransCacheManager::onSchd()
    {
        deleteExpiredCaches();
        deleteExpiredNotifyCaches();
    }

    // *************************************  MappingTable ************************
    void MappingTable::getSipUris(const std::string & appAccount, set<std::string> & uris)
    {
        if (appAccount.empty())
            return;

        std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
        map<std::string, set<std::string> >::iterator itAccount = m_mapAccountToSipUri.find(appAccount);
        if (itAccount != m_mapAccountToSipUri.end())
            uris = itAccount->second;
    }

    bool MappingTable::insertMappingItem(const std::string & sipUri, const std::string & appAccount)
    {
        if (sipUri.empty() || appAccount.empty())
            return false;

        {
            std::lock_guard<std::mutex> lock(m_mutexSipUriToAccount);
            map<std::string, std::string>::iterator itUri = m_mapSipUriToAccount.find(sipUri);
            if (itUri == m_mapSipUriToAccount.end())
                m_mapSipUriToAccount.insert(make_pair(sipUri, appAccount));
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
            map<std::string, set<std::string> >::iterator itAccount = m_mapAccountToSipUri.find(appAccount);
            if (itAccount == m_mapAccountToSipUri.end())
            {
                set<std::string> uris;
                uris.insert(sipUri);
                m_mapAccountToSipUri.insert(make_pair(appAccount, uris));
            }
            else
            {
                itAccount->second.insert(sipUri);
            }
        }
        return true;
    }

    bool MappingTable::insertMappingItems(const std::map<std::string, std::string> & uriToAccounts)
    {
        if (uriToAccounts.empty())
            return true;

        {
            std::lock_guard<std::mutex> lock(m_mutexSipUriToAccount);
            std::map<std::string, std::string>::const_iterator itKeys = uriToAccounts.begin();
            for (; itKeys != uriToAccounts.end();  itKeys++)
            {
                const std::string sipUri = itKeys->first;
                const std::string account = itKeys->second;

                map<std::string, std::string>::iterator itUri = m_mapSipUriToAccount.find(sipUri);
                if (itUri == m_mapSipUriToAccount.end())
                    m_mapSipUriToAccount.insert(make_pair(sipUri, account));
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
            std::map<std::string, std::string>::const_iterator itKeys = uriToAccounts.begin();
            for (; itKeys != uriToAccounts.end();  itKeys++)
            {
                const std::string sipUri = itKeys->first;
                const std::string account = itKeys->second;

                map<std::string, set<std::string> >::iterator itAccount = m_mapAccountToSipUri.find(account);
                if (itAccount == m_mapAccountToSipUri.end())
                {
                    set<std::string> uris;
                    uris.insert(sipUri);
                    m_mapAccountToSipUri.insert(make_pair(account, uris));
                }
                else
                {
                    itAccount->second.insert(sipUri);
                }
            }
        }
        return true;

    }

    bool MappingTable::deleteMappingItem(const std::string & sipUri, const std::string & appAccount)
    {
        if (sipUri.empty() || appAccount.empty())
            return false;

        {
            std::lock_guard<std::mutex> lock(m_mutexSipUriToAccount);
            map<std::string, std::string>::iterator itUri = m_mapSipUriToAccount.find(sipUri);
            if (itUri != m_mapSipUriToAccount.end())
                m_mapSipUriToAccount.erase(itUri);
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
            map<std::string, set<std::string> >::iterator itAccount = m_mapAccountToSipUri.find(appAccount);
            if (itAccount != m_mapAccountToSipUri.end())
            {
                itAccount->second.erase(sipUri);
                if (itAccount->second.empty())
                    m_mapAccountToSipUri.erase(itAccount);
            }
        }
        return true;
    }

    bool MappingTable::deleteMappingItems(const std::map<std::string, std::string> & uriToAccounts)
    {
        if (uriToAccounts.empty())
            return true;

        {
            std::lock_guard<std::mutex> lock(m_mutexSipUriToAccount);
            std::map<std::string, std::string>::const_iterator itKeys = uriToAccounts.begin();
            for (; itKeys != uriToAccounts.end();  itKeys++)
            {
                const std::string sipUri = itKeys->first;
                const std::string account = itKeys->second;

                map<std::string, std::string>::iterator itUri = m_mapSipUriToAccount.find(sipUri);
                if (itUri != m_mapSipUriToAccount.end())
                    m_mapSipUriToAccount.erase(itUri);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
            std::map<std::string, std::string>::const_iterator itKeys = uriToAccounts.begin();
            for (; itKeys != uriToAccounts.end();  itKeys++)
            {
                const std::string sipUri = itKeys->first;
                const std::string account = itKeys->second;

                map<std::string, set<std::string> >::iterator itAccount = m_mapAccountToSipUri.find(account);
                if (itAccount != m_mapAccountToSipUri.end())
                {
                    itAccount->second.erase(sipUri);
                    if (itAccount->second.empty())
                        m_mapAccountToSipUri.erase(itAccount);
                }
            }
        }
        return true;
    }

    int MappingTable::countMappingUriToAccount()
    {
        std::lock_guard<std::mutex> lock(m_mutexSipUriToAccount);
        return m_mapSipUriToAccount.size();
    }

    int MappingTable::countMappingAccountToUri()
    {
        std::lock_guard<std::mutex> lock(m_mutexAccountToSipUri);
        return m_mapAccountToSipUri.size();
    }

}
