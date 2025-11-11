#ifndef _APP_ACCOUNT_STATUS_MANAGER_H_
#define _APP_ACCOUNT_STATUS_MANAGER_H_

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <mutex>

#include "util/tc_timeprovider.h"
#include "util/tc_uuid_generator.h"
#include "SipProxyDb/src/SipDb.h"
#include "SipProxy/src/SipPrx.h"

#include "SipGateway/src/SipGatewayCommon.h"

// #include "Service/ServiceI.h"
// #include "SipProxyDb/SipProxyDbAgent.h"
// #include "SipProxyDb/SipProxyDbCommon.h"
// #include "SipProxyDb/SipProxyDbPub.h"
// #include "SipProxy/SipProxyNotifyPub.h"

// #include "Status/StatusPub1Agent.h"
// #include "Status/StatusPub1Pub.h"

namespace SipGateway
{

#define def_link_node(struct_name)\
struct {\
    struct_name *prev;\
    struct_name *next;\
}

#define def_link_entity(struct_name)\
struct {\
    struct_name *head;\
    struct_name *tail;\
    int node_num;\
}

#define link_node_init(entity)\
do {\
    (entity).prev = 0;\
    (entity).next = 0;\
} while (0)

#define link_init(entity)\
do {\
    (entity).head = 0;\
    (entity).tail = 0;\
    (entity).node_num = 0;\
}while(0)

#define link_swap(entity1,entity2,struct_name)\
do {\
    struct_name *_node;int _node_num;\
    _node = (entity1).head;\
    (entity1).head = (entity2).head;\
    (entity2).head = _node;\
    _node = (entity1).tail;\
    (entity1).tail = (entity2).tail;\
    (entity2).tail = _node;\
    _node_num = (entity1).node_num;\
    (entity1).node_num = (entity2).node_num;\
    (entity2).node_num = _node_num;\
}while(0)

#define link_head(entity)               ((entity).head)
#define link_tail(entity)               ((entity).tail)
#define link_node_num(entity)           ((entity).node_num)

#define link_next(node, node_name)      ((node)->node_name.next)
#define link_prev(node, node_name)      ((node)->node_name.prev)

#define link_add_head(entity, node, node_name)\
do {\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    (node)->node_name.prev = 0;\
    (node)->node_name.next = (entity).head;\
     if ((entity).head)\
         (entity).head->node_name.prev = (node);\
     else\
         (entity).tail = (node);\
     (entity).head = (node);\
     (entity).node_num ++;\
}while(0)

#define link_add_tail(entity, node, node_name)\
do {\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    (node)->node_name.next = 0;\
    (node)->node_name.prev = (entity).tail;\
    if ((entity).tail)\
        (entity).tail->node_name.next = (node);\
    else\
        (entity).head = (node);\
    (entity).tail = (node);\
    (entity).node_num ++;\
}while(0)

#define link_remove_head(entity, node_name)\
do {\
    Assert((entity).head);\
    Assert((entity).head->node_name.prev == 0);\
    void **head_next = (void **)&((entity).head->node_name.next);\
    (entity).head = (entity).head->node_name.next;\
    *head_next = 0;\
    if ((entity).head)\
        (entity).head->node_name.prev = 0;\
    else\
        (entity).tail = 0;\
    Assert((entity).node_num > 0);\
    (entity).node_num --;\
    Assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    Assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_remove_tail(entity, node_name)\
do {\
    Assert((entity).tail);\
    Assert((entity).head->node_name.next == 0);\
    void **tail_prev = (void **)&((entity).head->node_name.prev);\
    (entity).tail = (entity).tail->node_name.prev;\
    *tail_prev = 0;\
    if ((entity).tail)\
        (entity).tail->node_name.next = 0;\
    else\
        (entity).head = 0;\
    Assert((entity).node_num > 0);\
    (entity).node_num --;\
    Assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    Assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_remove_node(entity, node, node_name)\
do {\
    if ((node)->node_name.prev)\
    {\
        Assert((node)->node_name.prev->node_name.next == node);\
        (node)->node_name.prev->node_name.next = (node)->node_name.next;\
    }\
    else\
    {\
        Assert((entity).head == (node));\
        (entity).head = (node)->node_name.next;\
    }\
    if ((node)->node_name.next)\
    {\
        Assert((node)->node_name.next->node_name.prev == node);\
        (node)->node_name.next->node_name.prev = (node)->node_name.prev;\
    }\
    else\
    {\
        Assert((entity).tail == (node));\
        (entity).tail = (node)->node_name.prev;\
    }\
    (node)->node_name.prev = 0;\
    (node)->node_name.next = 0;\
    Assert((entity).node_num > 0);\
    (entity).node_num --;\
    Assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    Assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_insert_prev(entity, next_node, node, node_name)\
do {\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    Assert(next_node);\
    (node)->node_name.next = (next_node);\
    (node)->node_name.prev = (next_node)->node_name.prev;\
    if ((next_node)->node_name.prev)\
    {\
        (next_node)->node_name.prev->node_name.next = (node);\
    }\
    else\
    {\
        Assert((entity).head == (next_node));\
        (entity).head = (node);\
    }\
    (next_node)->node_name.prev = (node);\
    (entity).node_num ++;\
}while(0)

#define link_insert_next(entity, prev_node, node, node_name)\
do {\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    Assert(prev_node);\
    (node)->node_name.prev = (prev_node);\
    (node)->node_name.next = (prev_node)->node_name.next;\
    if ((prev_node)->node_name.next)\
    {\
        (prev_node)->node_name.next->node_name.prev = (node);\
    }\
    else\
    {\
        Assert((entity).tail == (prev_node));\
        (entity).tail = (node);\
    }\
    (prev_node)->node_name.next = (node);\
    (entity).node_num ++;\
}while(0)

#define link_no_entity_remove_node(node, node_name)\
do {\
    if ((node)->node_name.prev)\
        (node)->node_name.prev->node_name.next = (node)->node_name.next;\
    if ((node)->node_name.next)\
        (node)->node_name.next->node_name.prev = (node)->node_name.prev;\
    (node)->node_name.next = 0;\
    (node)->node_name.prev = 0;\
}while(0)

#define link_no_entity_insert_prev(next_node, node, node_name)\
do {\
    Assert(next_node);\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    (node)->node_name.next = (next_node);\
    (node)->node_name.prev = (next_node)->node_name.prev;\
    if ((next_node)->node_name.prev)\
        (next_node)->node_name.prev->node_name.next = (node);\
    (next_node)->node_name.prev = (node);\
}while(0)

#define link_no_entity_insert_next(prev_node, node, node_name)\
do {\
    Assert(prev_node);\
    Assert((node)->node_name.prev == 0);\
    Assert((node)->node_name.next == 0);\
    (node)->node_name.prev = (prev_node);\
    (node)->node_name.next = (prev_node)->node_name.next;\
    if ((prev_node)->node_name.next)\
        (prev_node)->node_name.next->node_name.prev = (node);\
    (prev_node)->node_name.next = (node);\
}while(0)

    class AppAccountStatus
    {
    public:
        AppAccountStatus(const std::string & appAccount, const std::string & userId, const VoipApp::SipAccountInfo & sipAccountInfo,
            const VoipApp::AppAccountStatusType & statusType);

        void updateSipAccountInfo(const VoipApp::SipAccountInfo & sipAccountInfo);
        void updateStatus(const VoipApp::AppAccountStatusType & statusType);
        void updateTime(const int64_t & time);
        int64_t getUpdateTime();
        VoipApp::AppAccountStatusType getStatusType();
        VoipApp::SipAccountInfo getSipAccountInfo();

        std::string getUserId();

    private:
        std::string                  m_appAccount;
        std::string                  m_userId;
        VoipApp::SipAccountInfo      m_sipAccountInfo;

        VoipApp::AppAccountStatusType  m_statusType;
        int64_t                    m_updateTime;
    };

    enum ServiceStatus
    {
        kServiceStatusAlive = 0,
        kServiceStatusToOutOfService = 1,
        kServiceStatusOutOfService = 2,
        kServiceStatusStop = 3,
    };

    class SipProxyRegResStatus
    {
    public:
        std::string                  m_sipProxyId;
        bool                            m_tcpUsed;
        std::string                  m_coreNetId;
        int64_t                    m_updateTicks;
        bool                            m_isbcAccessed;
        int                             m_loadSeq;
        ServiceStatus                   m_status;

        SipProxyRegResStatus(){}
        SipProxyRegResStatus(const std::string & id, bool tcpUsed, const std::string & coreNetId);
        SipProxyRegResStatus(const std::string & id, const std::string & coreNetId, bool isbcAccessed);
        SipProxyRegResStatus(const SipProxyRegResStatus & reg);

        bool operator < (const SipProxyRegResStatus & reg)const;
        SipProxyRegResStatus & operator = (const SipProxyRegResStatus & reg);
    };

    class SipAccountInfoEx
    {
        public:
        SipAccountInfoEx()
        {
            _selfProxyed = true;
        }

        SipAccountInfoEx(const VoipApp::SipAccountInfo & info, bool selfProxyed)
        {
            _info = info;
            _selfProxyed = selfProxyed;
        }

        VoipApp::SipAccountInfo  _info;
        bool  _selfProxyed;
    };

    class AppAccountStatusManager
    {
    public:
        AppAccountStatusManager();

        bool updateAppAccountStatus(const vector<VoipApp::SipAccountInfo> & vectAccounts, const std::map<std::string, std::map<std::string, std::string>> & statusesMap,
            const map<std::string, std::string> & userIdMap, const int64_t & baseTime, const int64_t & queryTime,
            map<std::string, AppAccountStatus> & mapChangedStatus);

        bool importSipAccounts(const vector<VoipApp::SipAccountInfo> & vectAppAccounts, const set<std::string> & selfProxyeds, const int64_t & latestInsertTime);
        bool obtainSipAccounts(vector<VoipApp::SipAccountInfo> & vectAppAccounts, int64_t & latestInsertTime);
        bool updateSipProxyId(const vector<VoipApp::SipAccountRegInfo> & vectRegInfos);
        void updateSipProxyId(const std::string& newSipProxyId,const std::set<std::string>& setSipUris);

        bool importSipProxyAccessedConfig(const vector<VoipApp::SipProxyAccessedCoreNetConfig> & sipProxyAccessedConfig);
        bool importDomainAccessedCfgs(const vector<VoipApp::DomainAccessedConfig2> & domainAccessedCfgs);
        void clearDeletedAccountStatus(const vector<VoipApp::SipAccountInfo> & vectAccounts);
        void exportSipProxyAccessedConfigs(vector<VoipApp::SipProxyAccessedCoreNetConfig> & vectSipProxyCfgs);
        void exportDomainAccessedConfigs(vector<VoipApp::DomainAccessedConfig2> & vectDomainCfgs);
        bool getSipProxyConfig(const std::string & sipProxyName, const std::string & sipProxyId, VoipApp::SipProxyAccessedCoreNetConfig & proxyCfg);

        int64_t getQueryTime();
        void setQueryTime(const int64_t & queryTime);

        int64_t getBaseTime();
        void setBaseTime(const int64_t & baseTime);

        void setLatestInsertTime(const int64_t  & latestTime);
        int64_t getLatestInsertTime();

        static bool getAppAcountNumber(const std::string & appAccount, std::string & appAccountNumber);
        static bool checkAppAccountStatus(const std::map<std::string, std::string> & statusValue, VoipApp::AppAccountStatusType & statusType, std::string & reason);
        static bool checkSipAccountRegStatus(const std::string & statusVal, SipGateway::SipAccountRegisterType & statusType);

        int getAppAccountStatusNumber();
        int getSipProxyAccessConfigNumber();
        int getSipAccountNumber();
        int getDomainCfgNumber();
        bool getAppStatusInfo(std::string & sipAccouts, int & size);

        bool getSipProxyListenIpAndPort(const std::string & sipProxyId, const std::string & sipProxyName,  std::string & listenIp, int & port);
        bool getSipProxyToCoreNet(const std::string & coreNetId, vector<std::string> & sipProxyIds);
        bool getDomainId(const std::string & userId, std::string & domainId);

        void importNationCodeIps(const vector<VoipApp::NationCodeIp> & vectIps);
        bool exportNationCodeIps(map<std::string, VoipApp::NationCodeIp> & mapIps);
        void initInterCodes();
        void exportInterCodes(vector<std::string> & vectInterCodes);
        std::string getChinaIp();
        bool getCalleeIp(const std::string & calleeNumber, std::string & nationCode, std::string & ip);
        void updateProxyedRelation(const vector<std::string> & vectSipUris, bool bProxyed);
        int getProxyedAccountNum();
        int getNationIpSize();


    private:
        std::mutex                                 m_mutex;
        
        map<std::string, AppAccountStatus>            m_mapAppAccountStatus;              //key:appAccount, format:[phone:+8613987562025@justalk.com]
        std::mutex                                 m_mutexMapAppAccountStatus;

        typedef map<std::string, SipAccountInfoEx>   SipAccountsMapType;
        SipAccountsMapType                               m_mapSipAccounts;
        std::mutex                                 m_mutexSipAccounts;

        vector<VoipApp::SipProxyAccessedCoreNetConfig>           m_vectSipProxyAccessedConfigs;
        std::mutex                                 m_mutexSipProxyAccessedConfig;

        vector<VoipApp::DomainAccessedConfig2>            m_vectDomainAccesssedCfgs;
        std::mutex                                 m_mutexDomainAccesssedCfgs;

        map<std::string, VoipApp::NationCodeIp>    m_mapNationIps;                   //国家码，ip
        std::mutex                                 m_mutexNationIps;
        vector<std::string>                           m_vectInterCodes;                  //国际冠码
        int                                              m_proxyedCount;

        int64_t                             m_queryTime;
        int64_t                             m_baseTime;
        int64_t                             m_latestAccountInsertedTime;
    };

    //Status changes Record
    class StatusChangeRecord
    {
    public:

        StatusChangeRecord(const std::string & sipUri, const VoipApp::AppAccountStatusType & statusType, int period);

        void updateType(const VoipApp::AppAccountStatusType & statusType);
        VoipApp::AppAccountStatusType getStatusType();
        int64_t getUpdateTime();
        int getPeriod();

    private:

        std::string                           m_sipUri;
        VoipApp::AppAccountStatusType           m_statusType;
        int64_t                             m_createTime;
        int64_t                             m_updateTime;
        int                                      m_period;
    };

    class StatusChangeRecordManager
    {
    public:
        StatusChangeRecordManager();

        bool insertStatusRecord(const std::string & sipUri, const VoipApp::AppAccountStatusType & statusType, int period);
        void clearExpiredStatusRecord();
        int getSize();

    private:
        map<std::string, StatusChangeRecord>     m_mapStatusChanges;             // key- sipUri, format:15802586775@ebupt.com, not 15802586775
        std::mutex                            m_mutexStatusChanges;
    };

    //.......................... BindLocator ............
    class BindLocator;
    typedef std::shared_ptr<BindLocator> BindLocatorPtr;

    class BindLocator
    {
    public:
        BindLocator(const std::string & account,const std::string & sipUri, const std::string & coreNetId, const std::string & sipProxyId);
        ~BindLocator();

        struct {
            BindLocator *prev;
            BindLocator *next;
        } _link;

        std::string              m_account;
        std::string              m_sipUri;
        std::string              m_coreNetId;
        std::string              m_sipProxyId;

        int64_t                m_updateTimeMs;
        bool                        m_bLogined;
        std::string              m_randomId;
    };


    class BindLocatorList
    {
    public:
        BindLocatorList(const std::string & moduleName);

        BindLocatorPtr getBindLocator(const std::string & account);
        bool updateBindLocator(const std::string & account,const std::string & sipUri, const std::string & coreNetId, const std::string & sipProxyId);
        bool updateBindLocator(const std::string & account, bool logined, const std::string & sipProxyId);
        void clearExpiredBinds();
        void getBindsSize(int & mapSize, int & linkNodeSize);
        void setMaxNodeCapacity(int maxCapacity);
        void setBindExpiredTime(int64_t expiredTime);

        bool insertSipUri2AccountItem(const std::string & sipUri, const std::string & account);
        bool deleteSipUri2AccountItem(const std::string & sipUri);
        bool deleteBindItem(const std::string & account);
        bool updateSipProxyIds(const std::string & newSipProxyId, const set<std::string> & setSipUris);

    private:
        def_link_entity(class BindLocator)  m_linkBinds;

        std::mutex                    m_mutexBinds;
        map<std::string, BindLocatorPtr> m_mapBinds;

        std::mutex                    m_mutexSipUri2Account;
        map<std::string, std::string> m_mapSipUri2Account;

        int                                 m_maxCapacity;
        int64_t                        m_expiredTime;
        std::string                      m_moduleName;
    };
    typedef std::shared_ptr<BindLocatorList> BindLocatorListPtr;
}



#endif