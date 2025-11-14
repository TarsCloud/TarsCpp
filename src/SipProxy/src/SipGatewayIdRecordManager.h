#ifndef SIPGATEWAY_ID_RECORD_MANAGER_H_
#define SIPGATEWAY_ID_RECORD_MANAGER_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

#include "SipProxyDb/src/SipDb.h"
#include "SipGateway/src/AppAcountStatusManager.h"

#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"

namespace SipProxy
{
    enum NotifyType
    {
        kNotifyInvalid = -1,
        kNotifyOutOfService = 0,
        kNotifySipBind = 1,
        kNotifyAliveService = 2,
        kNotifySipAccountStatus = 3,
        kNotifyAccountOccupyed = 4,     //finished accounts reading and to notify others to release these accounts;
        kNotifyAccountToOccupy = 5,      //notify others to read accounts and report result back
        kNotifyToOutOfService = 6,
    };

    enum SipBindType
    {
        kSipBindNTo1Empty = 0,          //可以多个sip账号绑定同一个Cloud账号，也允许不绑定
        kSipBindNTo1NoEmpty = 1,        //可以多个sip账号绑定同一个Cloud账号，但是不允许不绑定
        kSipBind1To1Empty = 2,          //只能sip账号与cloud账号一一对应，但允许sip账号不绑定cloud账号
        kSipBind1To1NoEmpty = 3,        //只能sip账号与cloud账号一一对应，且不允许sip账号不绑定cloud账号,如果存在，则删除该未绑定cloud账号的sip账号
        kSipBind1To1NumEuqal = 4,       //只能一对一，且号码一致，如+8615802586775@kcc.chinamb.com 绑定app账号 [username:+8615802586775@102286.justalk.cloud.com]
        kSipBind1To1NumEuqalDelIntCode = 5,  //只能一对一，且号码一致，如15802586775@kcc.chinamb.com 绑定app账号 [username:+8615802586775@102286.justalk.cloud.com]
    };


    class SipDialogRecord
    {
    public:
        SipDialogRecord() {}
        SipDialogRecord(const std::string & sipDialogId, const std::string & callId, const std::string & sipGatewayId, const std::string & sipPrxoyId);
        SipDialogRecord(const std::string & sipDialogId, const std::string & callId, const std::string & sipGatewayId, const std::string & sipPrxoyId,void * respCon, bool appCallCs);


        std::string    m_sipGetwayId;
        std::string    m_sipProxyId;
        std::string    m_sipDialogId;
        std::string    m_callId;
        int64_t      m_updateTime;

        void *            m_respCon;
        bool              m_appCallCs;
    };

    class SipDialogRecordManager
    {
    public:
        SipDialogRecordManager();

        bool insertSipDialogRecord(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId);
        bool insertSipDialogRecord2(const std::string & callId, const std::string & sipDialogId, const std::string & sipGatewayId, const std::string & sipProxyId,void * respCon, bool appCallCs);
        bool ereaseSipDialogRecord(const std::string & callId);
        bool obtainSipDialogRecord(const std::string & callId, SipDialogRecord & sipDialogRecord);
        bool ereaseSipDialogRecordBySipDialogId(const std::string & sipDialogId);

        int getTotalInsertIdNum();
        int getTotalEreaseIdNum();
        int getRecordSize();
        void exceptionRecordDeals();
        bool updateSipGatewayId(const std::string & callId, const std::string & sipGatewayId);

    private:
        std::map<std::string, SipDialogRecord>        m_mapSipDilaogRecords;      //key-callId
        std::mutex                            m_mutexMapSipDilaogRecords;

        std::atomic<int>                            m_iTotalInsertIdNum;
        std::atomic<int>                            m_iTotalEreaseIdNum;
    };

    class SipRegStausDbManager
    {
    public:

        typedef std::map<std::string, VoipApp::RegStaus2> MapRegStatus;
        typedef std::map<std::string, MapRegStatus> DomainIdMapRegStatus;

        bool insertRegRecord(const std::string & domainId, const std::string & sipUri, bool bProxyed, int failedTimes, int64_t time, const std::string & proxyId, bool tmpReg);
        bool exportRegRecords(DomainIdMapRegStatus & records);
        void removeRegRecord(const std::string & domainId, const std::string & sipUri);
        int getSize();

    private:

        std::mutex                            m_mutexRegStatus;
        DomainIdMapRegStatus                        m_mapRegStatus;
    };

    //............................................ OutOfServService ................................../
    class OutOfServService
    {
    public:
        OutOfServService(const std::string & serviceId, uint64_t ticks);

        std::string                              m_serviceId;
        uint64_t                               m_insertTicks;
    };

    class OutOfServServiceManager
    {
    public:

        OutOfServServiceManager();
        void onSchd();

        bool insertOutOfSerServiceId(const std::string & serviceId);
        bool exportOutOfSerServiceIds(std::set<std::string> & serviceIds);

    private:

        std::map<std::string, OutOfServService>       m_mapServices;
        std::mutex                            m_mutexServices;
    };

    // ----------------------------------- Sip account read failed info ----------------------
    class SipAccountReadCache
    {
    public:
        SipAccountReadCache()
        {
            m_lastTicks =  TC_TimeProvider::getInstance()->getNowMs();
            m_reading = false;
        }

        SipAccountReadCache(const std::string & id, const std::string & domainId,
            const std::string & coreNetId, const std::string & caller,const std::vector<std::string> & vectSipUris);

        std::string          m_id;
        std::string          m_domainId;
        std::string          m_coreNetId;
        std::string          m_caller;
        std::vector<std::string>  m_vectSipUris;

        int64_t            m_lastTicks;
        bool                    m_reading;
    };
    typedef std::map<std::string, SipAccountReadCache> AccountReadCacheMap;

    class SipAccountReadCacheManager
    {
    public:

        std::string getCacheId();
        bool insertReadCache(const std::string & domainId,
            const std::string & coreNetId, const std::string & caller,const std::vector<std::string> & vectSipUris);
        void getReadCaches(AccountReadCacheMap & mapCaches);

        bool updateReadCacheTicks(const std::string & id);
        bool deleteReadCache(const std::string & id);

    private:

        std::mutex        m_mutexCaches;
        AccountReadCacheMap     m_mapCaches;
    };


    // --------------------------- sip account transfer assistant class ---------------------------

    enum CacheType
    {
        kCacheWaitToAdd = 0,
        kCacheWaitToRemove = 1
    };

    class AccountTransCache
    {
    public:
        AccountTransCache();
        AccountTransCache(const std::string & sipUri, const std::string & oriProxyId, CacheType typeVal);

        std::string      m_sipUri;
        std::string      m_originProxyId;
        CacheType           m_cacheType;

        bool                m_finished;          //可以作为获取SIP账号时的判断依据，判断是否已完成迁移
        int64_t        m_ticks;
        int64_t        m_finishedTicks;
    };
    typedef std::shared_ptr<AccountTransCache> AccountTransCachePtr;


    class AccountTransNotifyCache
    {
    public:
        AccountTransNotifyCache()
        {
            m_id = TC_UUIDGenerator::getInstance()->genID();
            m_insertTicks = TC_TimeProvider::getInstance()->getNowMs();
        }

        AccountTransNotifyCache(const std::string & toProxyId, NotifyType typeVal, const std::map<std::string, std::string> & sipUris, int64_t insertTicks):
          m_toProxyId(toProxyId), m_notifyType(typeVal), m_sipUris(sipUris)
        {
            m_id = TC_UUIDGenerator::getInstance()->genID();
            m_insertTicks = insertTicks == 0 ? TC_TimeProvider::getInstance()->getNowMs() : insertTicks;
        }

        std::string      m_id;
        std::string      m_toProxyId;
        NotifyType          m_notifyType;
        std::map<std::string, std::string>   m_sipUris;
        int64_t        m_insertTicks;
    };

    class AccountTransCacheManager
    {
    public:

        bool insertTransCaches(const std::string & originProxyId, CacheType typeVal, const std::vector<std::string> & sipUris);
        void finishTrans2(const std::vector<std::string> & sipUris, CacheType typeVal);
        void finishTrans(const std::string & sipUri, CacheType typeVal);
        void getFinishedAccounts(std::vector<std::string> & sipUris, CacheType typeVal);
        void deleteFinishedCaches(const std::set<std::string> & sipUris, CacheType typeVal);
        void deleteExpiredCaches();
        void getOccupyedNotifyUris(std::map<std::string, std::set<std::string> > & sipUris);
        void getNeedRemovedSipUris(std::set<std::string> & sipUris);
        int getTransCacheSize();

        //......
        void insertNotifyCache(const std::string & toProxyId, NotifyType typeVal, const std::map<std::string, std::string> & sipUris, const std::string & cacheId, int64_t insertTicks);
        void deleteNotifyCache(const std::string & cacheId);
        void exportNotifyCaches(std::map<std::string, AccountTransNotifyCache> & caches);
        void deleteExpiredNotifyCaches();
        int getNotifyCacheSize();
        void onSchd();

    private:

        typedef std::map<std::string, AccountTransCachePtr> TransCacheMap;
        std::mutex          m_mutexTransCaches;
        TransCacheMap             m_mapTransCaches;

        std::map<std::string, AccountTransNotifyCache> m_mapNotifyCaches;
        std::mutex          m_mutexNofityCaches;

    };

    struct AccountOpeRecord
    {
        VoipApp::OperationCategory   _operateType;
        VoipApp::SipAccountInfo      _sipAccount;
    };

    struct AccountTransTask
    {
        std::string _domainId;
        std::string _coreNetId;
        std::set<std::string> _setSipUris;
        std::string _caller;
        std::string _id;
    };


    //                              MappingTable
    class MappingTable
    {
    public:
        void getSipUris(const std::string & appAccount, std::set<std::string> & uris);
        bool insertMappingItem(const std::string & sipUri, const std::string & appAccount);
        bool insertMappingItems(const std::map<std::string, std::string> & uriToAccounts);
        bool deleteMappingItem(const std::string & sipUri, const std::string & appAccount);
        bool deleteMappingItems(const std::map<std::string, std::string> & uriToAccounts);

        int countMappingUriToAccount();
        int countMappingAccountToUri();

    private:
        std::map<std::string, std::string>     m_mapSipUriToAccount;
        std::mutex                        m_mutexSipUriToAccount;
        std::map<std::string, std::set<std::string> > m_mapAccountToSipUri;
        std::mutex                        m_mutexAccountToSipUri;

    };

    struct AccountExcpDealCache
    {
        AccountExcpDealCache(const std::string & coreNet)
        {
            m_coreNetId = coreNet;
            m_insertTikcs = TC_TimeProvider::getInstance()->getNowMs();
        }
        std::string      m_coreNetId;
        int64_t        m_insertTikcs;
    };

    typedef std::vector<AccountTransTask> AccountTransTasks;
    typedef std::map<std::string, AccountTransTask> AccountTransItems;
    typedef std::map<std::string, SipGateway::SipProxyRegResStatus> RegProxyMapType;
    typedef std::map<std::string, RegProxyMapType > AliveProxyMapType;

    struct SipAccountLoadRecord
    {
        SipAccountLoadRecord()
        {
            m_finished = true;
            m_times = 1;
        }
        SipAccountLoadRecord(const std::string & key)
        {
            m_key = key;
            m_finished = true;
            m_times = 1;
        }
        std::string      m_key;
        bool                m_finished;
        int                 m_times;
    };
    typedef std::vector<SipAccountLoadRecord> SipAccountLoadRecordVect;
    typedef std::map<std::string, SipAccountLoadRecord> SipAccountLoadRecordMap;

}


#endif


