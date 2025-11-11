#include "AppAcountStatusManager.h"

namespace SipGateway
{

    AppAccountStatus::AppAccountStatus(const std::string & appAccount, const std::string & userId, const VoipApp::SipAccountInfo & sipAccountInfo,
        const VoipApp::AppAccountStatusType & statusType)
        :m_appAccount(appAccount),m_userId(userId),m_sipAccountInfo(sipAccountInfo),m_statusType(statusType)
    {
        m_updateTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }

    void AppAccountStatus::updateSipAccountInfo(const VoipApp::SipAccountInfo & sipAccountInfo)
    {
        m_sipAccountInfo = sipAccountInfo;
        m_updateTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }

    void AppAccountStatus::updateStatus(const VoipApp::AppAccountStatusType & statusType)
    {
        m_statusType = statusType;
        m_updateTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }

    void AppAccountStatus::updateTime(const int64_t & time)
    {
        m_updateTime = tars::TC_TimeProvider::getInstance()->getNowMs();
    }

    int64_t AppAccountStatus::getUpdateTime()
    {
        return m_updateTime;
    }

    VoipApp::AppAccountStatusType AppAccountStatus::getStatusType()
    {
        return m_statusType;
    }

    VoipApp::SipAccountInfo AppAccountStatus::getSipAccountInfo()
    {
        return m_sipAccountInfo;
    }

    std::string AppAccountStatus::getUserId()
    {
        return m_userId;
    }

    // ---------------------------------------------   SipProxyRegResStatus     -----------------------------------------------

    SipProxyRegResStatus::SipProxyRegResStatus(const std::string & id, bool tcpUsed, const std::string & coreNetId)
    {
        m_sipProxyId = id;
        m_tcpUsed = tcpUsed;
        m_coreNetId = coreNetId;
        m_updateTicks = tars::TC_TimeProvider::getInstance()->getNowMs();
        m_loadSeq = 0;
        m_status = kServiceStatusAlive;
    }

    SipProxyRegResStatus::SipProxyRegResStatus(const std::string & id, const std::string & coreNetId, bool isbcAccessed)
    {
        m_sipProxyId = id;
        m_tcpUsed = false;
        m_coreNetId = coreNetId;
        m_updateTicks = tars::TC_TimeProvider::getInstance()->getNowMs();
        m_isbcAccessed = isbcAccessed;
        m_loadSeq = 0;
        m_status = kServiceStatusAlive;
    }

    SipProxyRegResStatus::SipProxyRegResStatus(const SipProxyRegResStatus & reg)
    {
        m_sipProxyId = reg.m_sipProxyId;
        m_tcpUsed = reg.m_tcpUsed;
        m_coreNetId = reg.m_coreNetId;
        m_updateTicks = tars::TC_TimeProvider::getInstance()->getNowMs();
        m_isbcAccessed = reg.m_isbcAccessed;
        m_loadSeq = reg.m_loadSeq;
        m_status = reg.m_status;
    }

    bool SipProxyRegResStatus::operator < (const SipProxyRegResStatus & reg)const
    {
        return m_sipProxyId < reg.m_sipProxyId;
    }

    SipProxyRegResStatus & SipProxyRegResStatus::operator = (const SipProxyRegResStatus & reg)
    {
        if (&reg == this)
            return *this;

        m_sipProxyId = reg.m_sipProxyId;
        m_tcpUsed = reg.m_tcpUsed;
        m_coreNetId = reg.m_coreNetId;
        m_updateTicks = reg.m_updateTicks;
        m_isbcAccessed = reg.m_isbcAccessed;
        m_loadSeq = reg.m_loadSeq;
        m_status = reg.m_status;
        return *this;
    }

    // -------------------------------------------  AppAccountStatusManager ----------------------------------------------------
    AppAccountStatusManager::AppAccountStatusManager()
    {
        m_queryTime = 0;
        m_baseTime = 0;
        m_latestAccountInsertedTime = 0;
        m_proxyedCount = 0;
    }

    bool AppAccountStatusManager::updateAppAccountStatus(const vector<VoipApp::SipAccountInfo> & vectAccounts, const std::map<std::string, std::map<std::string, std::string>> & statusesMap,
        const map<std::string, std::string> & userIdMap, const int64_t & baseTime, const int64_t & queryTime, map<std::string, AppAccountStatus> & mapChangedStatus)
    {
        mapChangedStatus.clear();
        map<std::string, VoipApp::SipAccountInfo> mapSipAccountInfo;//temp saved
        vector<VoipApp::SipAccountInfo> vectAccountsTemp = vectAccounts;
        for (unsigned int i = 0; i < vectAccountsTemp.size(); i++)
        {
            std::string appAccount = vectAccountsTemp.at(i).m_strAppAccount;
            std::string appAccountNumber;
            std::string domainId;

            getAppAcountNumber(appAccount, appAccountNumber);                        //get the accountId(13900090012) of appAccount,[username:13900090012@sample.cloud.justalk.com]
            vectAccountsTemp.at(i).m_strAppAccountNumber = appAccountNumber;
            domainId = vectAccountsTemp.at(i).m_strDomainId;

            std::string sipInfoKey = domainId + "_" + appAccountNumber;           //to avoid the same accountId(13900090012) in different domain, formating this key: 9999_13900090012
            mapSipAccountInfo.insert(make_pair(sipInfoKey, vectAccountsTemp.at(i)));
        }

        std::lock_guard<std::mutex> temLock(m_mutexMapAppAccountStatus);
        map<std::string, std::string>::const_iterator iterUserId = userIdMap.begin();
        for ( ; iterUserId != userIdMap.end(); iterUserId++)
        {
            std::string userId = iterUserId->first;                  //for example:9999_3
            std::string appAccountNumber = iterUserId->second;       //for example: 15802586775
            Status::Statuses statusValue;
            VoipApp::AppAccountStatusType  statusType;
            bool result = false;
            std::string reason;
            std::string domainId;

            if (false == getDomainId(userId, domainId))
            {
                UTIL_LOG_WRN("AppAccountStatusManager", std::string(__FUNCTION__) + "() called getDomainId() return FALSE: "
                    + userId + ",statusesMap.size()==" + std::string(int(statusesMap.size())));
                continue;
            }

            Status::StatusesMap::const_iterator iterStatus = statusesMap.find(userId);
            if (iterStatus == statusesMap.end())
            {
                continue;
            }
            else
            {
                statusValue = iterStatus->second;
            }
            std::string sipInfoKey = domainId + "_" + appAccountNumber;

            result = checkAppAccountStatus(statusValue, statusType, reason);
            if (!result)
            {
                UTIL_LOG_DBG("AppAccountStatusManager", std::string(__FUNCTION__) + "() called .checkAppAccountStatus() return FALSE! userId:"
                    + userId + " ,reason:" + reason);
                continue;
            }

            map<std::string, VoipApp::SipAccountInfo>::iterator iterSipAccountInfo = mapSipAccountInfo.find(sipInfoKey);
            if(iterSipAccountInfo != mapSipAccountInfo.end())
            {
                std::string appAccount = iterSipAccountInfo->second.m_strAppAccount;
                AppAccountStatus accountStatus(appAccount, userId, iterSipAccountInfo->second, statusType);

                map<std::string, AppAccountStatus>::iterator iterAppStatus = m_mapAppAccountStatus.find(appAccount);
                if (iterAppStatus == m_mapAppAccountStatus.end())
                {
                    m_mapAppAccountStatus.insert(make_pair(appAccount, accountStatus));
                    mapChangedStatus.insert(make_pair(appAccount, accountStatus));

                    UTIL_LOG_DBG("AppAccountStatusManager", std::string(__FUNCTION__) + "() called  sipUri:"
                        + accountStatus.getSipAccountInfo().m_sipUri + " ,userId:" + userId + " ,statusType:" + std::string(int(statusType)));
                }
                else
                {
                    if(iterAppStatus->second.getStatusType() != statusType)
                    {
                        iterAppStatus->second.updateStatus(statusType);
                        mapChangedStatus.insert(make_pair(appAccount, accountStatus));
                    }
                }
            }
            else
            {
                UTIL_LOG_WRN("AppAccountStatusManager", std::string(__FUNCTION__) + "() called.mapSipAccountInfo() can't find this appAccount,sipInfoKey:" + sipInfoKey);
            }
        }
        return true;
    }


    bool AppAccountStatusManager::importSipAccounts(const vector<VoipApp::SipAccountInfo> & vectAppAccounts, const set<std::string> & selfProxyeds, const int64_t & latestInsertTime)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        for (unsigned int i = 0; i < vectAppAccounts.size(); i++)
        {
            VoipApp::SipAccountInfo sipAccount = vectAppAccounts.at(i);
            if (sipAccount.m_strDomainId == "101460" || sipAccount.m_strDomainId == "100167" || sipAccount.m_strDomainId == "1" || sipAccount.m_strDomainId == "9999")
            {
                bool selfProxyed = (selfProxyeds.find(sipAccount.m_sipUri) != selfProxyeds.end()) ? true : false;
                m_mapSipAccounts[sipAccount.m_sipUri] = SipAccountInfoEx(sipAccount, selfProxyed);
                if (selfProxyed)
                    m_proxyedCount++;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    bool AppAccountStatusManager::obtainSipAccounts(vector<VoipApp::SipAccountInfo> & vectAppAccounts, int64_t & latestInsertTime)
    {
        latestInsertTime = m_latestAccountInsertedTime;
        vectAppAccounts.clear();
        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        SipAccountsMapType::iterator iterUri = m_mapSipAccounts.begin();
        for (; iterUri != m_mapSipAccounts.end(); iterUri++)
        {
            if (iterUri->second._selfProxyed)
                vectAppAccounts.push_back(iterUri->second._info);
        }
        return true;
    }

    bool AppAccountStatusManager::updateSipProxyId(const vector<VoipApp::SipAccountRegInfo> & vectRegInfos)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        for (unsigned int i = 0; i < vectRegInfos.size(); i++)
        {
            SipProxyDb::SipAccountRegInfo regInfo = vectRegInfos.at(i);
            SipAccountsMapType::iterator iterUri = m_mapSipAccounts.find(regInfo.m_sipUri);
            if (iterUri != m_mapSipAccounts.end())
            {
                if (regInfo.m_proxyId.find("SipProxy") < 0)
                    continue;
                iterUri->second._info.m_strSipProxyId = regInfo.m_proxyId;
            }
        }
        return true;
    }

    void AppAccountStatusManager::updateSipProxyId(const std::string& newSipProxyId,const Common::StrSet& setSipUris)
    {
        int notFound = 0;
        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        Common::StrSet::const_iterator iterSipUri = setSipUris.begin();
        for (; iterSipUri != setSipUris.end(); iterSipUri++)
        {
            SipAccountsMapType::iterator iterUri = m_mapSipAccounts.find(*iterSipUri);
            if (iterUri != m_mapSipAccounts.end())
            {
                iterUri->second._info.m_strSipProxyId = newSipProxyId;
            }
            else
            {
                notFound ++;
                UTIL_LOG_DBG("AppAccountStatusManager", std::string(__FUNCTION__) + "() called.mapSipAccountInfo() can't find this appAccount,sipUri:" + *iterSipUri);
            }
        }
        UTIL_LOG_IFO("AppAccountStatusManager", std::string(__FUNCTION__) + "() called.total:" + std::string(int(setSipUris.size())) + ",not found:" + std::string(notFound));
    }

    bool AppAccountStatusManager::importSipProxyAccessedConfig(const vector<VoipApp::SipProxyAccessedCoreNetConfig> & sipProxyAccessedConfig)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipProxyAccessedConfig);
        if (sipProxyAccessedConfig.size() == 0)
        {
            return true;
        }
        m_vectSipProxyAccessedConfigs.clear();
        m_vectSipProxyAccessedConfigs.insert(m_vectSipProxyAccessedConfigs.begin(), sipProxyAccessedConfig.begin(), sipProxyAccessedConfig.end());
        return true;
    }

    bool AppAccountStatusManager::importDomainAccessedCfgs(const vector<VoipApp::DomainAccessedConfig2> & domainAccessedCfgs)
    {
        std::lock_guard<std::mutex> lock(m_mutexDomainAccesssedCfgs);
        if (domainAccessedCfgs.size() == 0)
        {
            return true;
        }
        m_vectDomainAccesssedCfgs.clear();
        m_vectDomainAccesssedCfgs.insert(m_vectDomainAccesssedCfgs.begin(), domainAccessedCfgs.begin(), domainAccessedCfgs.end());
        return true;
    }

    void AppAccountStatusManager::clearDeletedAccountStatus(const vector<VoipApp::SipAccountInfo> & vectAccounts)
    {
        std::lock_guard<std::mutex> temLock(m_mutexMapAppAccountStatus);

        int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
        map<std::string, AppAccountStatus>::iterator iterDes;
        for (unsigned int i = 0; i < vectAccounts.size(); i++)
        {
            VoipApp::SipAccountInfo accountInfo = vectAccounts.at(i);
            std::string appAccount = accountInfo.m_strAppAccount;

            iterDes = m_mapAppAccountStatus.find(appAccount);
            if (iterDes != m_mapAppAccountStatus.end())
            {
                iterDes->second.updateTime(curTime);
            }
        }

        iterDes = m_mapAppAccountStatus.begin();
        for ( ; iterDes != m_mapAppAccountStatus.end(); )
        {
            if (curTime != iterDes->second.getUpdateTime())
            {
                std::string sipUri = iterDes->second.getSipAccountInfo().m_sipUri;
                std::string  userId = iterDes->second.getUserId();

                UTIL_LOG_DBG("AppAccountStatusManager", "clearDeletedAccountStatus() called sipUri:" + sipUri +" ,userId:"
                    + userId + " ,statusType:" + std::string(int(iterDes->second.getStatusType())));

                m_mapAppAccountStatus.erase(iterDes++);
            }
            else
            {
                iterDes ++;
            }
        }
    }

    void AppAccountStatusManager::exportSipProxyAccessedConfigs(vector<VoipApp::SipProxyAccessedCoreNetConfig> & vectSipProxyCfgs)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipProxyAccessedConfig);
        vectSipProxyCfgs.clear();
        vectSipProxyCfgs.insert(vectSipProxyCfgs.begin(), m_vectSipProxyAccessedConfigs.begin(), m_vectSipProxyAccessedConfigs.end());
    }

    void AppAccountStatusManager::exportDomainAccessedConfigs(vector<VoipApp::DomainAccessedConfig2> & vectDomainCfgs)
    {
        std::lock_guard<std::mutex> lock(m_mutexDomainAccesssedCfgs);
        vectDomainCfgs.clear();
        vectDomainCfgs.insert(vectDomainCfgs.end(), m_vectDomainAccesssedCfgs.begin(), m_vectDomainAccesssedCfgs.end());
    }

    bool AppAccountStatusManager::getSipProxyConfig(const std::string & sipProxyName, const std::string & sipProxyId, VoipApp::SipProxyAccessedCoreNetConfig & proxyCfg)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipProxyAccessedConfig);
        for (unsigned int i = 0; i < m_vectSipProxyAccessedConfigs.size(); i++)
        {
            if (m_vectSipProxyAccessedConfigs.at(i).m_strSipProxyName == sipProxyName || m_vectSipProxyAccessedConfigs.at(i).m_strSipProxyId == sipProxyId)
            {
                proxyCfg = m_vectSipProxyAccessedConfigs.at(i);
                return true;
            }
        }
        return false;
    }

    int64_t AppAccountStatusManager::getQueryTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queryTime;
    }
    void AppAccountStatusManager::setQueryTime(const int64_t & queryTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queryTime = queryTime;
    }

    int64_t AppAccountStatusManager::getBaseTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_baseTime;
    }
    void AppAccountStatusManager::setBaseTime(const int64_t & baseTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_baseTime = baseTime;
    }

    void AppAccountStatusManager::setLatestInsertTime(const int64_t  & latestTime)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_latestAccountInsertedTime = latestTime;
    }

    int64_t AppAccountStatusManager::getLatestInsertTime()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_latestAccountInsertedTime;
    }

    bool AppAccountStatusManager::getAppAcountNumber(const std::string & appAccount, std::string & appAccountNumber)
    {
        //appAccount format: [phone:+8615802145203@justalk.com]
        if (appAccount.empty())
            return false;

        int pos1 = appAccount.find("@");
        int pos3 = appAccount.find(":");

        if (pos3 == -1 || pos1 == -1)
            return false;

        appAccountNumber = appAccount.substr(pos3 + 1, pos1 - pos3 - 1);   //get +8615802145203
        return true;
    }

    bool AppAccountStatusManager::checkAppAccountStatus(const std::map<std::string, std::string> & statusValue, VoipApp::AppAccountStatusType & statusType, std::string & reason)
    {
        if (statusValue.size() == 0)
        {
            reason = "status value is EMPTY!";
            return false;
        }

        std::map<std::string, std::string>::const_iterator iterStatusValue = statusValue.find(SIP_GW_SIP_REGISTER_KEY);
        if (iterStatusValue != statusValue.end())
        {
            if (iterStatusValue->second == "0")
            {
                statusType = VoipApp::kAppAccountStatusUnRegister;
            }
            else if (iterStatusValue->second == "1")
            {
                statusType = VoipApp::kAppAccountStatusRegister;
            }
            else if (iterStatusValue->second == "2")
            {
                statusType = VoipApp::kAppAccountStatusReRegister;
            }
            else if (iterStatusValue->second == "3")
            {
                statusType = VoipApp::kAppAccountStatusTmpUnRegister;
            }
            else if (iterStatusValue->second == "4")
            {
                statusType = VoipApp::kAppAccountStatusTmpRegister;
            }
            else
            {
                statusType = VoipApp::kAppAccountStatusInvalid;
                reason = "status value invalid";
                return false;
            }
            return true;
        }
        else
        {
            reason = "the status key \'SipFlag\' not exists!";
            return false;
        }
    }

    bool AppAccountStatusManager::checkSipAccountRegStatus(const std::string & statusVal, SipGateway::SipAccountRegisterType & statusType)
    {
        if (statusVal.empty())
        {
            return false;
        }

        if (statusVal == "0")
        {
            statusType = SipGateway::kRegisterTypeUnReg;
        }
        else if (statusVal == "1")
        {
            statusType = SipGateway::kRegisterTypePerioReg;
        }
        else if (statusVal == "2")
        {
            statusType = SipGateway::kRegisterTypeMandated;
        }
        else if (statusVal == "3")
        {
            statusType = SipGateway::kRegisterTypeUnMandated;
        }
        else if (statusVal == "4")
        {
            statusType = SipGateway::kRegisterTypeCancelMandated;
        }
        else if (statusVal == "6")
        {
            statusType = SipGateway::kRegisterTypeInvokeMandatedPeriodReg;
        }
        else
        {
            statusType = SipGateway::kRegisterTypeInvalid;
            return false;
        }
        return true;
    }

    int AppAccountStatusManager::getAppAccountStatusNumber()
    {
        std::lock_guard<std::mutex> temLock(m_mutexMapAppAccountStatus);
        return m_mapAppAccountStatus.size();
    }

    int AppAccountStatusManager::getSipProxyAccessConfigNumber()
    {
        std::lock_guard<std::mutex> lock(m_mutexSipProxyAccessedConfig);
        return m_vectSipProxyAccessedConfigs.size();
    }

    int AppAccountStatusManager::getSipAccountNumber()
    {
        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        return m_mapSipAccounts.size();
    }

    int AppAccountStatusManager::getDomainCfgNumber()
    {
        std::lock_guard<std::mutex> lock(m_mutexDomainAccesssedCfgs);
        return m_vectDomainAccesssedCfgs.size();
    }

    bool AppAccountStatusManager::getAppStatusInfo(std::string & sipAccouts, int & size)
    {
        std::lock_guard<std::mutex> temLock(m_mutexMapAppAccountStatus);
        if (m_mapAppAccountStatus.size() == 0)
            return false;

        sipAccouts.clear();
        size = m_mapAppAccountStatus.size();
        map<std::string, AppAccountStatus>::iterator iterAppStatus = m_mapAppAccountStatus.begin();
        for ( ; iterAppStatus != m_mapAppAccountStatus.end(); iterAppStatus++)
        {
            sipAccouts += " ->sipUri:";
            sipAccouts += iterAppStatus->second.getSipAccountInfo().m_sipUri;
            sipAccouts += " ,userId:";
            sipAccouts += iterAppStatus->second.getUserId();
            sipAccouts += " ,statusType:";
            sipAccouts += std::string(int(iterAppStatus->second.getStatusType()));
            sipAccouts += "; ";
        }
        return true;
    }

    bool AppAccountStatusManager::getSipProxyListenIpAndPort(const std::string & sipProxyId,const std::string & sipProxyName,  std::string & listenIp, int & port)
    {
        std::lock_guard<std::mutex> lc(m_mutexSipProxyAccessedConfig);
        if (m_vectSipProxyAccessedConfigs.size() == 0)
            return false;

        for (unsigned int i = 0; i < m_vectSipProxyAccessedConfigs.size(); i++)
        {
            VoipApp::SipProxyAccessedCoreNetConfig accessConfig = m_vectSipProxyAccessedConfigs.at(i);
            if (accessConfig.m_strSipProxyName == sipProxyName || accessConfig.m_strSipProxyId == sipProxyId)
            {
                listenIp = accessConfig.m_listenIp;
                port = accessConfig.m_listenPort;
                return true;
            }
        }
        return false;
    }

    bool AppAccountStatusManager::getSipProxyToCoreNet(const std::string & coreNetId, vector<std::string> & sipProxyIds)
    {
        std::lock_guard<std::mutex> lc(m_mutexSipProxyAccessedConfig);
        if (m_vectSipProxyAccessedConfigs.size() == 0)
            return false;

        for (unsigned int i = 0; i < m_vectSipProxyAccessedConfigs.size(); i++)
        {
            VoipApp::SipProxyAccessedCoreNetConfig accessConfig = m_vectSipProxyAccessedConfigs.at(i);
            if (accessConfig.m_strCoreNetIdName == coreNetId)
            {
                sipProxyIds.push_back(accessConfig.m_strSipProxyId);
            }
        }
        return true;
    }

    bool AppAccountStatusManager::getDomainId(const std::string & userId, std::string & domainId)
    {
        int pos = userId.find("_");
        if (pos < 0)
            return false;

        domainId = userId.substr(0, pos);
        return true;
    }

    void AppAccountStatusManager::importNationCodeIps(const vector<VoipApp::NationCodeIp> & vectIps)
    {
        if (vectIps.size() == 0)
            return;

        std::lock_guard<std::mutex> lock(m_mutexNationIps);
        m_mapNationIps.clear();
        for (unsigned int i = 0; i < vectIps.size(); i++)
        {
            VoipApp::NationCodeIp natioIp = vectIps.at(i);
            m_mapNationIps[natioIp.m_nationCode] = natioIp;
        }
    }

    bool AppAccountStatusManager::exportNationCodeIps(map<std::string, VoipApp::NationCodeIp> & mapIps)
    {
        std::lock_guard<std::mutex> lock(m_mutexNationIps);
        mapIps = m_mapNationIps;
        return true;
    }

    void AppAccountStatusManager::initInterCodes()
    {
        m_vectInterCodes.push_back("00");
        m_vectInterCodes.push_back("01");
        m_vectInterCodes.push_back("05");
        m_vectInterCodes.push_back("09");
        m_vectInterCodes.push_back("11");
        m_vectInterCodes.push_back("60");

        m_vectInterCodes.push_back("001");
        m_vectInterCodes.push_back("002");
        m_vectInterCodes.push_back("009");
        m_vectInterCodes.push_back("011");
        m_vectInterCodes.push_back("095");
        m_vectInterCodes.push_back("810");

        m_vectInterCodes.push_back("0011");
    }
    void AppAccountStatusManager::exportInterCodes(vector<std::string> & vectInterCodes)
    {
        vectInterCodes = m_vectInterCodes;
    }

    std::string AppAccountStatusManager::getChinaIp()
    {
        std::lock_guard<std::mutex> lock(m_mutexNationIps);
        std::string ip;
        if (m_mapNationIps.empty())
        {
            return "114.55.253.221";
        }
        map<std::string, VoipApp::NationCodeIp>::iterator iterCode = m_mapNationIps.find("86");
        if (iterCode != m_mapNationIps.end())
        {
            ip = iterCode->second.m_ip;
        }
        return ip;
    }

    bool AppAccountStatusManager::getCalleeIp(const std::string & calleeNumber, std::string & nationCode, std::string & ip)
    {
        std::string pureNumber;
        bool bInterExist = false;

        if (calleeNumber.find("+") == 0)
        {
            pureNumber = calleeNumber.substr(1, calleeNumber.size() - 1);
            bInterExist = true;
        }
        else
        {
            for (unsigned int i = 0; i < m_vectInterCodes.size(); i ++)
            {
                std::string interCode = m_vectInterCodes.at(i);
                int interCodeLen = interCode.size();
                if (calleeNumber.find(interCode) == 0)
                {
                    pureNumber = calleeNumber.substr(interCodeLen, calleeNumber.size() - interCodeLen);
                    bInterExist = true;
                    break;
                }
            }
        }

        if (!bInterExist || calleeNumber.size() <= 5)
        {
            nationCode = "86";
            ip = getChinaIp();
            return true;
        }

        int oriLen = 4;
        std::lock_guard<std::mutex> lock(m_mutexNationIps);
        while (oriLen >= 1)
        {
            std::string strPre = pureNumber.substr(0, oriLen);
            map<std::string, VoipApp::NationCodeIp>::iterator iterCode = m_mapNationIps.find(strPre);
            if (iterCode != m_mapNationIps.end())
            {
                nationCode = iterCode->first;
                ip = iterCode->second.m_ip;
                return true;
            }
            oriLen--;
        }

        nationCode = "86";
        ip = getChinaIp();
        return true;
    }

    void AppAccountStatusManager::updateProxyedRelation(const vector<std::string> & vectSipUris, bool bProxyed)
    {
        if (vectSipUris.empty())
            return;

        std::lock_guard<std::mutex> lock(m_mutexSipAccounts);
        for (unsigned int i = 0; i < vectSipUris.size(); i++)
        {
            SipAccountsMapType::iterator iterSipUri = m_mapSipAccounts.find(vectSipUris.at(i));
            if (iterSipUri != m_mapSipAccounts.end())
            {
                iterSipUri->second._selfProxyed = bProxyed;

                if (bProxyed)
                    m_proxyedCount++;
                else
                    m_proxyedCount--;
            }
        }
    }

    int AppAccountStatusManager::getProxyedAccountNum()
    {
        return m_proxyedCount;
    }

    int AppAccountStatusManager::getNationIpSize()
    {
        return m_mapNationIps.size();
    }

    // ***************** StatusChangeRecordManager *******************************//
    StatusChangeRecord::StatusChangeRecord(const std::string & sipUri, const VoipApp::AppAccountStatusType & statusType, int period)
        :m_sipUri(sipUri),m_statusType(statusType)
    {
        int64_t curTime =tars::TC_TimeProvider::getInstance()->getNowMs();
        m_createTime = curTime;
        m_updateTime = curTime;
        m_period = period;
    }

    void StatusChangeRecord::updateType(const VoipApp::AppAccountStatusType & statusType)
    {
        m_statusType = statusType;
        int64_t curTime =tars::TC_TimeProvider::getInstance()->getNowMs();
        m_updateTime = curTime;
    }

    VoipApp::AppAccountStatusType StatusChangeRecord::getStatusType()
    {
        return m_statusType;
    }

    int64_t StatusChangeRecord::getUpdateTime()
    {
        return m_updateTime;
    }

    int StatusChangeRecord::getPeriod()
    {
        return m_period;
    }

    StatusChangeRecordManager::StatusChangeRecordManager()
    {
    }

    bool StatusChangeRecordManager::insertStatusRecord(const std::string & sipUri, const VoipApp::AppAccountStatusType & statusType, int period)
    {
        std::lock_guard<std::mutex> lock(m_mutexStatusChanges);
        map<std::string, StatusChangeRecord>::iterator iterStatus = m_mapStatusChanges.find(sipUri);
        if (iterStatus == m_mapStatusChanges.end())
        {
            m_mapStatusChanges.insert(make_pair(sipUri, StatusChangeRecord(sipUri, statusType, period)));
            return true;
        }
        else
        {
            if (iterStatus->second.getStatusType() == statusType)
            {
                return false;
            }
            iterStatus->second.updateType(statusType);
        }
        return true;
    }

    void StatusChangeRecordManager::clearExpiredStatusRecord()
    {
        std::lock_guard<std::mutex> lock(m_mutexStatusChanges);
        int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
        map<std::string, StatusChangeRecord>::iterator iterStatus = m_mapStatusChanges.begin();
        for ( ; iterStatus != m_mapStatusChanges.end(); )
        {
            if ((curTime - iterStatus->second.getUpdateTime()) > iterStatus->second.getPeriod())
            {
                m_mapStatusChanges.erase(iterStatus++);
            }
            else
            {
                iterStatus++;
            }
        }
    }

    int StatusChangeRecordManager::getSize()
    {
        std::lock_guard<std::mutex> lock(m_mutexStatusChanges);
        return m_mapStatusChanges.size();
    }

    // ********************************** BindLocator **********************************
    BindLocator::BindLocator(const std::string & account,const std::string & sipUri, const std::string & coreNetId, const std::string & sipProxyId)
        :m_account(account), m_sipUri(sipUri),m_coreNetId(coreNetId),m_sipProxyId(sipProxyId)
    {
        m_updateTimeMs = tars::TC_TimeProvider::getInstance()->getNowMs();
        m_bLogined = true;
        m_randomId = tars::TC_Common::tostr(tars::TC_UUIDGenerator::genID());
        UTIL_LOG_IFO("AppAccountStatusManager", "**** BindLocator()****  called, account:" + account +" ,sipUri:" + sipUri + " ,coreNetId:" + coreNetId + ",sipProxyId:" + sipProxyId + ",m_randomId:" + m_randomId);
    }

    BindLocator::~BindLocator()
    {
        std::string strTime = tars::TC_Common::tm2str(m_updateTimeMs/1000, "%Y-%m-%d %H:%M:%S");
        UTIL_LOG_IFO("AppAccountStatusManager", "**** ~BindLocator()****  called, account:" + m_account +" ,sipUri:" + m_sipUri + " ,coreNetId:" + m_coreNetId
            + ",sipProxyId:" + m_sipProxyId + ",create Time:" + strTime + ",m_randomId:" + m_randomId);
    }

    BindLocatorList::BindLocatorList(const std::string & moduleName)
        :m_moduleName(moduleName)
    {
        m_maxCapacity = MAX_ACCOUNT_BIND_SIPURI_COUNT;
        m_expiredTime = MAX_ACCOUNT_BIND_SIPURI_EXPIRED_TIME;

        //link_init(m_linkBinds);
    }

    BindLocatorPtr BindLocatorList::getBindLocator(const std::string & account)
    {
        std::lock_guard<std::mutex> lock(m_mutexBinds);
        map<std::string, BindLocatorPtr>::iterator iterBind = m_mapBinds.find(account);
        if (iterBind == m_mapBinds.end())
            return nullptr;
        return iterBind->second;
    }

    bool BindLocatorList::updateBindLocator(const std::string & account,const std::string & sipUri, const std::string & coreNetId, const std::string & sipProxyId)
    {
        if (m_expiredTime == 0)
            return true;

        std::lock_guard<std::mutex> lock(m_mutexBinds);
        BindLocatorPtr pBind = nullptr;
        map<std::string, BindLocatorPtr>::iterator it1 = m_mapBinds.find(account);
        if (it1 != m_mapBinds.end())
        {
            pBind = it1->second;
            pBind->m_updateTimeMs = tars::TC_TimeProvider::getInstance()->getNowMs();
            pBind->m_sipProxyId = sipProxyId;
            //link_remove_node(m_linkBinds, pBind.get(), _link);
        }
        else
        {
            pBind = new BindLocator(account,sipUri, coreNetId, sipProxyId);
            m_mapBinds.insert(make_pair(account, pBind));
            insertSipUri2AccountItem(sipUri, account);
        }
        //link_add_tail(m_linkBinds, pBind.get(), _link);
        return true;
    }

    bool BindLocatorList::updateBindLocator(const std::string & account, bool logined, const std::string & sipProxyId)
    {
        if (m_expiredTime == 0)
            return true;

        std::lock_guard<std::mutex> lock(m_mutexBinds);
        BindLocatorPtr pBind = nullptr;
        map<std::string, BindLocatorPtr>::iterator it1 = m_mapBinds.find(account);
        if (it1 != m_mapBinds.end())
        {
            pBind = it1->second;
            pBind->m_updateTimeMs = tars::TC_TimeProvider::getInstance()->getNowMs();
            pBind->m_bLogined = logined;
            pBind->m_sipProxyId = sipProxyId;
            //link_remove_node(m_linkBinds, pBind.get(), _link);
        }
        else
        {
            return false;
        }
        //link_add_tail(m_linkBinds, pBind.get(), _link);
        return true;
    }

    void BindLocatorList::clearExpiredBinds()
    {
        std::lock_guard<std::mutex> lock(m_mutexBinds);
        int64_t curTime = tars::TC_TimeProvider::getInstance()->getNowMs();
        BindLocatorPtr pBind;
        if (m_mapBinds.size() == 0)
            return;

        while ((pBind = /*link_head(m_linkBinds)*/nullptr))
        {
            if ((curTime - pBind->m_updateTimeMs) < m_expiredTime)
                break;

            std::string strTime = tars::TC_Common::tm2str("%d-%02d-%02d %02d:%02d:%02d:%03d", pBind->m_updateTimeMs);
            UTIL_LOG_IFO("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, to remove a TimeOut bind, account:" + pBind->m_account
                +" ,sipUri:" + pBind->m_sipUri + ",sipProxyId:" + pBind->m_sipProxyId + ",last update time:" + strTime);

            //link_remove_head(m_linkBinds,_link);
            deleteSipUri2AccountItem(pBind->m_sipUri);
            m_mapBinds.erase(pBind->m_account);
            pBind = 0;
        }

        while ((pBind = /*link_head(m_linkBinds)*/nullptr))
        {
            if (m_linkBinds.node_num < m_maxCapacity)
                break;

            UTIL_LOG_IFO("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, to remove a Exceed limit bind, account:" + pBind->m_account
                +" ,sipUri:" + pBind->m_sipUri + ",sipProxyId:" + pBind->m_sipProxyId);

            //link_remove_head(m_linkBinds,_link);
            deleteSipUri2AccountItem(pBind->m_sipUri);
            m_mapBinds.erase(pBind->m_account);
            pBind = 0;
        }
    }

    void BindLocatorList::getBindsSize(int & mapSize, int & linkNodeSize)
    {
        std::lock_guard<std::mutex> lock(m_mutexBinds);
        mapSize = m_mapBinds.size();
        linkNodeSize = /*m_linkBinds.node_num*/0;
    }

    void BindLocatorList::setMaxNodeCapacity(int maxCapacity)
    {
        if (m_maxCapacity != maxCapacity && maxCapacity != 0)
        {
            m_maxCapacity = maxCapacity;
        }
    }

    void BindLocatorList::setBindExpiredTime(int64_t expiredTime)
    {
        if (m_expiredTime != expiredTime)
        {
            m_expiredTime = expiredTime;
        }
    }

    bool BindLocatorList::insertSipUri2AccountItem(const std::string & sipUri, const std::string & account)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipUri2Account);
        map<std::string, std::string>::iterator iterUri = m_mapSipUri2Account.find(sipUri);
        if (iterUri != m_mapSipUri2Account.end())
            return false;

        m_mapSipUri2Account.insert(make_pair(sipUri, account));
        return true;
    }

    bool BindLocatorList::deleteSipUri2AccountItem(const std::string & sipUri)
    {
        std::lock_guard<std::mutex> lock(m_mutexSipUri2Account);
        map<std::string, std::string>::iterator iterUri = m_mapSipUri2Account.find(sipUri);
        if (iterUri == m_mapSipUri2Account.end())
            return false;

        m_mapSipUri2Account.erase(iterUri);
        return true;
    }

    bool BindLocatorList::deleteBindItem(const std::string & account)
    {
        std::string sipUri;

        {
            std::lock_guard<std::mutex> lock2(m_mutexBinds);
            map<std::string, BindLocatorPtr>::iterator iterBind = m_mapBinds.find(account);
            if (iterBind == m_mapBinds.end())
                UTIL_LOG_WRN("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, can't find the Bind, account:" + account);
            else
            {
                iterBind->second->m_sipUri;
                //link_remove_node(m_linkBinds, iterBind->second.get(), _link);
                m_mapBinds.erase(iterBind);
            }
        }

        if (!sipUri.empty())
        {
            std::lock_guard<std::mutex> lock1(m_mutexSipUri2Account);
            map<std::string, std::string>::iterator iterUri = m_mapSipUri2Account.find(sipUri);
            if (iterUri != m_mapSipUri2Account.end())
            {
                m_mapSipUri2Account.erase(iterUri);
            }
            else
            {
                UTIL_LOG_WRN("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, can't find the Bind, account:" + account +" ,sipUri:" + sipUri);
            }
        }
        return true;
    }

    bool BindLocatorList::updateSipProxyIds(const std::string & newSipProxyId, const set<std::string> & setSipUris)
    {
        set<std::string>::const_iterator iterSipUri = setSipUris.begin();
        for (; iterSipUri != setSipUris.end(); iterSipUri++)
        {
            std::string account;
            std::string sipUri = *iterSipUri;

            {
                std::lock_guard<std::mutex> lock1(m_mutexSipUri2Account);
                map<std::string, std::string>::iterator iterUri = m_mapSipUri2Account.find(sipUri);
                if (iterUri == m_mapSipUri2Account.end())
                    continue;
                account = iterUri->second;
            }

            std::string oldProxyId;
            {
                std::lock_guard<std::mutex> lock2(m_mutexBinds);
                map<std::string, BindLocatorPtr>::iterator iterBind = m_mapBinds.find(account);
                if (iterBind == m_mapBinds.end())
                {
                    UTIL_LOG_WRN("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, can't find the Bind, account:" + account +" ,sipUri:" + sipUri);
                    continue;
                }
                oldProxyId = iterBind->second->m_sipProxyId;
                iterBind->second->m_sipProxyId = newSipProxyId;
            }
            UTIL_LOG_DBG("AppAccountStatusManager", std::string(__FUNCTION__) + "() called, account:" + account +" ,sipUri:" + sipUri + ",oldProxyId:" + oldProxyId + ",new id:" + newSipProxyId);
        }
        return true;
    }

}

