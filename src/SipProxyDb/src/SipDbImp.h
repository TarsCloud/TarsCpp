#ifndef _SipDbImp_H_
#define _SipDbImp_H_

#include "servant/Application.h"
#include "SipDb.h"
#include "DbConn/DbConn.h"
#include "SipProxyDbCommon.h"

struct CallStatistic
{
    std::string          m_interfaceName;
    int                     m_callTimes;
    int64_t            m_maxPeriod;
    int64_t            m_avgPeriod;
    int64_t            m_totalPeriod;
};
/**
 *
 *
 */
class SipDbImp : public VoipApp::SipDb
{
public:
    /**
     *
     */
    virtual ~SipDbImp() {}

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
    virtual tars::Bool GetDomainAccessInfo(const std::string & tableName,vector<VoipApp::DomainAccessedConfig> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool GetDomainAccessInfo2(const std::string & tableName,vector<VoipApp::DomainAccessedConfig2> &vectDomainAccessed,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool GetRtpGatewayconfigInfo(const std::string & tableName,vector<VoipApp::RtpGatewayAccessedConfig> &rtpGatewayConfigLst,tars::TarsCurrentPtr _current_);
    virtual tars::Bool GetSipProxyAccessCoreNetInfo(const std::string & tableName,vector<VoipApp::SipProxyAccessedCoreNetConfig> &vectSipProxyAccessedConfig,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool SetRtpGatewayAccessedId(const std::string & tableName,const std::string & rtpGatewayName,const std::string & rtpGatewayId,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool SetSipProxyAccessedId(const std::string & tableName,const std::string & sipProxyName,const std::string & sipProxyid,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool UpdateCallSipPolicys(const std::string & tableName,const vector<VoipApp::CallSipPolicyOperation> & OpSequence,tars::TarsCurrentPtr _current_);
    virtual tars::Bool UpdateDomainAccessConfig(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_);
    virtual tars::Bool UpdateDomainAccessConfig2(const std::string & tableName,const vector<VoipApp::DomainAccessConfigOperation2> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_);
    virtual tars::Bool UpdateSipProxyAccessConfig(const std::string & tableName,const vector<VoipApp::SipProxyConfigOperation> & OpSequence,tars::Int64 insertedTime,tars::TarsCurrentPtr _current_);
    virtual tars::Bool getDomainLineConfig(const std::string & domainId,const std::string & appId,vector<VoipApp::DomainLineConfig> &config,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool getNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const std::string & callee,std::string &displayName,std::string &coreNetId,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool getSipCallRecordEx(const std::string & domainId,const std::string & appId,const std::string & rcpCallId,VoipApp::SipCallRecord &record,tars::TarsCurrentPtr _current_);
    virtual tars::Bool insertCallRecords(const std::string & tableName,const std::string & domainId,const std::string & appId,const vector<VoipApp::CallRecord> & records,const map<std::string, std::string> & params,tars::TarsCurrentPtr _current_);
    virtual tars::Bool insertNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool insertRecordsBatch(const std::string & domainId,const std::string & appId,const vector<VoipApp::SipCallRecord> & records,tars::TarsCurrentPtr _current_);
    virtual tars::Bool removeNumberRouting(const std::string & tableName,const std::string & domainIdAppId,const vector<VoipApp::NumRouting> & numRouting,std::string &reason,tars::TarsCurrentPtr _current_);
    virtual tars::Bool updateDomainLineConfig(const std::string & domainId,const std::string & appId,const std::string & coreNetId,const std::string & origSbcIp,const std::string & origSbcPort,const std::string & dstSbcIp,const std::string & dstSbcPort,std::string &reason,tars::TarsCurrentPtr _current_);

    private:
        bool UpdateDomainAccessConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::DomainAccessConfigOperation>&  opSequence,
            const int64_t & insertTime, unsigned int & mysqlErrno, std::string & reason);
        bool UpdateDomainAccessConfigToDb2(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::DomainAccessConfigOperation2>& opSequence,
            const int64_t & insertTime, unsigned int & mysqlErrno, std::string & reason);
        bool UpdateSipProxyAccessConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::SipProxyConfigOperation>& opSequence,
            const int64_t & insertTime, unsigned int & mysqlErrno, std::string & reason);
        bool UpdateCallSipPolicyConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::CallSipPolicyOperation> & opSequence,
             unsigned int & mysqlErrno, std::string & reason);
        bool UpdateNumRoutingConfigToDb(const std::string& tableName,const DbConn::SQL::ConnPtr conn, const std::vector<VoipApp::NumRouting> & opSequence, int type,
            const int64_t & insert_time, unsigned int & mysqlErrno, std::string & reason);

        bool decodeDomainIdAppId(const std::string & domainIdAndAppId, std::string & domainId, std::string & appId);

        bool CreateTable(SipProxyDbCommon::SipTableType tableType, std::string & reason, std::string tableName = "");
        bool createDatabase(const std::string & name, std::string & reason);

        void updateCallTime(const std::string & interfaceName , int64_t period);
        void setStatisticInfo();

        bool tryExecuteInsert(const DbConn::SQL::ConnPtr& conn, const std::string& tableName, SipProxyDbCommon::SipTableType tableType, const std::string& statement, unsigned int& mysqlErrno, std::string& reason);
        bool insertSipRecordsToDb(const DbConn::SQL::ConnPtr & conn, const std::string& domainId,const std::string& appId,const std::vector<VoipApp::SipCallRecord>& records,
            unsigned int & mysqlErrno, std::string & reason);
        std::string getRecordTableName(const std::string& domainId,const std::string& appId);

        bool insertCallRecordsToDb(const DbConn::SQL::ConnPtr& conn, const std::string& tableName, const std::string& domainId, const std::string& appId,
            const std::vector<VoipApp::CallRecord>& records, unsigned int& mysqlErrno, std::string& reason);

    private:

        DbConn::SQL::ConnPoolPtr            _sql;
        std::string                         _sqlUri;
        bool                                m_bServiceActibed;

        std::string                         m_redisUri;
        //Redis::CliPtr                       m_redis;
        std::string                         m_keyPrifixCall;
        tars::TC_ThreadRecMutex             m_mutexCallStatis;
        std::map<std::string, CallStatistic>  m_mapCallStatis;

        int64_t                             m_lastSchdTime;
        int64_t                             m_baseSchdTime;

        std::string                         m_date;
};
/////////////////////////////////////////////////////
#endif
