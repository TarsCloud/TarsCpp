
#include <iterator>
#include <algorithm>
#include "DbHandle.h"

TC_ReadersWriterData<ObjectsCache> CDbHandle::_objectsCache;
TC_ReadersWriterData<CDbHandle::SetDivisionCache> CDbHandle::_setDivisionCache;
TC_ReadersWriterData<std::map<int, CDbHandle::GroupPriorityEntry> > CDbHandle::_mapGroupPriority;

//key-ip, value-组编号
TC_ReadersWriterData<map<string, int> > CDbHandle::_groupIdMap;
//key-group_name, value-组编号
TC_ReadersWriterData<map<string, int> > CDbHandle::_groupNameMap;

int CDbHandle::init(TC_Config *pconf)
{
    return 0;
}

vector<EndpointF> CDbHandle::findObjectById(const string& id)
{
    ObjectsCache::iterator it;
    ObjectsCache& usingCache = _objectsCache.getReaderData();

    if ((it = usingCache.find(id)) != usingCache.end())
    {
        return it->second.vActiveEndpoints;
    }
    else
    {
        vector<EndpointF> activeEp;
        return activeEp;
    }
}


int CDbHandle::findObjectById4All(const string& id, vector<EndpointF>& activeEp, vector<EndpointF>& inactiveEp)
{

    TLOGDEBUG(__FUNCTION__ << " id: " << id << endl);

    ObjectsCache::iterator it;
    ObjectsCache& usingCache = _objectsCache.getReaderData();

    if ((it = usingCache.find(id)) != usingCache.end())
    {
        activeEp   = it->second.vActiveEndpoints;
        inactiveEp = it->second.vInactiveEndpoints;
    }
    else
    {
        activeEp.clear();
        inactiveEp.clear();
    }

    return  0;
}

vector<EndpointF> CDbHandle::getEpsByGroupId(const vector<EndpointF>& vecEps, const GroupUseSelect GroupSelect, int iGroupId, ostringstream& os)
{
    os << "|";
    vector<EndpointF> vResult;

    for (unsigned i = 0; i < vecEps.size(); i++)
    {
        os << vecEps[i].host << ":" << vecEps[i].port << "(" << vecEps[i].groupworkid << ");";
        if (GroupSelect == ENUM_USE_WORK_GROUPID && vecEps[i].groupworkid == iGroupId)
        {
            vResult.push_back(vecEps[i]);
        }
        if (GroupSelect == ENUM_USE_REAL_GROUPID && vecEps[i].grouprealid == iGroupId)
        {
            vResult.push_back(vecEps[i]);
        }
    }

    return vResult;
}

vector<EndpointF> CDbHandle::getEpsByGroupId(const vector<EndpointF>& vecEps, const GroupUseSelect GroupSelect, const set<int>& setGroupID, ostringstream& os)
{
    os << "|";
    std::vector<EndpointF> vecResult;

    for (std::vector<EndpointF>::size_type i = 0; i < vecEps.size(); i++)
    {
        os << vecEps[i].host << ":" << vecEps[i].port << "(" << vecEps[i].groupworkid << ")";
        if (GroupSelect == ENUM_USE_WORK_GROUPID && setGroupID.count(vecEps[i].groupworkid) == 1)
        {
            vecResult.push_back(vecEps[i]);
        }
        if (GroupSelect == ENUM_USE_REAL_GROUPID && setGroupID.count(vecEps[i].grouprealid) == 1)
        {
            vecResult.push_back(vecEps[i]);
        }
    }

    return vecResult;
}

int CDbHandle::findObjectByIdInSameGroup(const string& id, const string& ip, vector<EndpointF>& activeEp, vector<EndpointF>& inactiveEp, ostringstream& os)
{
    activeEp.clear();
    inactiveEp.clear();

    int iClientGroupId  = getGroupId(ip);

    os << "|(" << iClientGroupId << ")";

    if (iClientGroupId == -1)
    {
        return findObjectById4All(id, activeEp, inactiveEp);
    }

    ObjectsCache::iterator it;
    ObjectsCache& usingCache = _objectsCache.getReaderData();

    if ((it = usingCache.find(id)) != usingCache.end())
    {
        activeEp    = getEpsByGroupId(it->second.vActiveEndpoints, ENUM_USE_WORK_GROUPID, iClientGroupId, os);
        inactiveEp  = getEpsByGroupId(it->second.vInactiveEndpoints, ENUM_USE_WORK_GROUPID, iClientGroupId, os);

        if (activeEp.size() == 0) //没有同组的endpoit,匹配未启用分组的服务
        {
            activeEp    = getEpsByGroupId(it->second.vActiveEndpoints, ENUM_USE_WORK_GROUPID, -1, os);
            inactiveEp  = getEpsByGroupId(it->second.vInactiveEndpoints, ENUM_USE_WORK_GROUPID, -1, os);
        }
        if (activeEp.size() == 0) //没有同组的endpoit
        {
            activeEp   = it->second.vActiveEndpoints;
            inactiveEp = it->second.vInactiveEndpoints;
        }
    }

    return  0;
}

int CDbHandle::findObjectByIdInGroupPriority(const std::string& sID, const std::string& sIP, std::vector<EndpointF>& vecActive, std::vector<EndpointF>& vecInactive, std::ostringstream& os)
{
    vecActive.clear();
    vecInactive.clear();

    int iClientGroupID = getGroupId(sIP);
    os << "|(" << iClientGroupID << ")";
    if (iClientGroupID == -1)
    {
        return findObjectById4All(sID, vecActive, vecInactive);
    }

    ObjectsCache& usingCache = _objectsCache.getReaderData();
    ObjectsCache::iterator itObject = usingCache.find(sID);
    if (itObject == usingCache.end()) return 0;

    //首先在同组中查找
    {
        vecActive     = getEpsByGroupId(itObject->second.vActiveEndpoints, ENUM_USE_WORK_GROUPID, iClientGroupID, os);
        vecInactive    = getEpsByGroupId(itObject->second.vInactiveEndpoints, ENUM_USE_WORK_GROUPID, iClientGroupID, os);
        os << "|(In Same Group: " << iClientGroupID << " Active=" << vecActive.size() << " Inactive=" << vecInactive.size() << ")";
    }

    //启用分组，但同组中没有找到，在优先级序列中查找
    std::map<int, GroupPriorityEntry> & mapPriority = _mapGroupPriority.getReaderData();
    for (std::map<int, GroupPriorityEntry>::iterator it = mapPriority.begin(); it != mapPriority.end() && vecActive.empty(); it++)
    {
        if (it->second.setGroupID.count(iClientGroupID) == 0)
        {
            os << "|(Not In Priority " << it->second.sGroupID << ")";
            continue;
        }
        vecActive    = getEpsByGroupId(itObject->second.vActiveEndpoints, ENUM_USE_WORK_GROUPID, it->second.setGroupID, os);
        vecInactive    = getEpsByGroupId(itObject->second.vInactiveEndpoints, ENUM_USE_WORK_GROUPID, it->second.setGroupID, os);
        os << "|(In Priority: " << it->second.sGroupID << " Active=" << vecActive.size() << " Inactive=" << vecInactive.size() << ")";
    }

    //没有同组的endpoit,匹配未启用分组的服务
    if (vecActive.empty())
    {
        vecActive    = getEpsByGroupId(itObject->second.vActiveEndpoints, ENUM_USE_WORK_GROUPID, -1, os);
        vecInactive    = getEpsByGroupId(itObject->second.vInactiveEndpoints, ENUM_USE_WORK_GROUPID, -1, os);
        os << "|(In No Grouop: Active=" << vecActive.size() << " Inactive=" << vecInactive.size() << ")";
    }

    //在未分组的情况下也没有找到，返回全部地址(此时基本上所有的服务都已挂掉)
    if (vecActive.empty())
    {
        vecActive    = itObject->second.vActiveEndpoints;
        vecInactive    = itObject->second.vInactiveEndpoints;
        os << "|(In All: Active=" << vecActive.size() << " Inactive=" << vecInactive.size() << ")";
    }

    return 0;
}

int CDbHandle::findObjectByIdInSameStation(const std::string& sID, const std::string& sStation, std::vector<EndpointF>& vecActive, std::vector<EndpointF>& vecInactive, std::ostringstream& os)
{
    vecActive.clear();
    vecInactive.clear();

    //获得station所有组
    std::map<int, GroupPriorityEntry> & mapPriority         = _mapGroupPriority.getReaderData();
    std::map<int, GroupPriorityEntry>::iterator itGroup     = mapPriority.end();
    for (itGroup = mapPriority.begin(); itGroup != mapPriority.end(); itGroup++)
    {
        if (itGroup->second.sStation != sStation) continue;

        break;
    }

    if (itGroup == mapPriority.end())
    {
        os << "|not found station:" << sStation;
        return -1;
    }

    ObjectsCache& usingCache = _objectsCache.getReaderData();
    ObjectsCache::iterator itObject = usingCache.find(sID);
    if (itObject == usingCache.end()) return 0;

    //查找对应所有组下的IP地址
    vecActive    = getEpsByGroupId(itObject->second.vActiveEndpoints, ENUM_USE_REAL_GROUPID, itGroup->second.setGroupID, os);
    vecInactive    = getEpsByGroupId(itObject->second.vInactiveEndpoints, ENUM_USE_REAL_GROUPID, itGroup->second.setGroupID, os);

    return 0;
}

int CDbHandle::findObjectByIdInSameSet(const string& sID, const vector<string>& vtSetInfo, std::vector<EndpointF>& vecActive, std::vector<EndpointF>& vecInactive, std::ostringstream& os)
{
    string sSetName   = vtSetInfo[0];
    string sSetArea   = vtSetInfo[0] + "." + vtSetInfo[1];
    string sSetId     = vtSetInfo[0] + "." + vtSetInfo[1] + "." + vtSetInfo[2];

    SetDivisionCache& usingSetDivisionCache = _setDivisionCache.getReaderData();
    SetDivisionCache::iterator it = usingSetDivisionCache.find(sID);
    if (it == usingSetDivisionCache.end())
    {
        //此情况下没启动set
        TLOGINFO("CDbHandle::findObjectByIdInSameSet:" << __LINE__ << "|" << sID << " haven't start set|" << sSetId << endl);
        return -1;
    }

    map<string, vector<SetServerInfo> >::iterator setNameIt = it->second.find(sSetName);
    if (setNameIt == (it->second).end())
    {
        //此情况下没启动set
        TLOGINFO("CDbHandle::findObjectByIdInSameSet:" << __LINE__ << "|" << sID << " haven't start set|" << sSetId << endl);
        return -1;
    }

    if (vtSetInfo[2] == "*")
    {
        //检索通配组和set组中的所有服务
        vector<SetServerInfo>  vServerInfo = setNameIt->second;
        for (size_t i = 0; i < vServerInfo.size(); i++)
        {
            if (vServerInfo[i].sSetArea == sSetArea)
            {
                if (vServerInfo[i].bActive)
                {
                    vecActive.push_back(vServerInfo[i].epf);
                }
                else
                {
                    vecInactive.push_back(vServerInfo[i].epf);
                }
            }
        }

        return (vecActive.empty() && vecInactive.empty()) ? -2 : 0;
    }
    else
    {

        // 1.从指定set组中查找
        int iRet = findObjectByIdInSameSet(sSetId, setNameIt->second, vecActive, vecInactive, os);
        if (iRet != 0 && vtSetInfo[2] != "*")
        {
            // 2. 步骤1中没找到，在通配组里找
            string sWildSetId =  vtSetInfo[0] + "." + vtSetInfo[1] + ".*";
            iRet = findObjectByIdInSameSet(sWildSetId, setNameIt->second, vecActive, vecInactive, os);
        }

        return iRet;
    }
}

int CDbHandle::findObjectByIdInSameSet(const string& sSetId, const vector<SetServerInfo>& vSetServerInfo, std::vector<EndpointF>& vecActive, std::vector<EndpointF>& vecInactive, std::ostringstream& os)
{
    for (size_t i = 0; i < vSetServerInfo.size(); ++i)
    {
        if (vSetServerInfo[i].sSetId == sSetId)
        {
            if (vSetServerInfo[i].bActive)
            {
                vecActive.push_back(vSetServerInfo[i].epf);
            }
            else
            {
                vecInactive.push_back(vSetServerInfo[i].epf);
            }
        }
    }

    int iRet = (vecActive.empty() && vecInactive.empty()) ? -2 : 0;
    return iRet;
}

void CDbHandle::updateObjectsCache(const ObjectsCache& objCache, bool updateAll)
{
    //全量更新
    if (updateAll)
    {
        _objectsCache.getWriterData() = objCache;
        _objectsCache.swap();
    }
    else
    {
        //用查询数据覆盖一下
        _objectsCache.getWriterData() = _objectsCache.getReaderData();
        ObjectsCache& tmpObjCache = _objectsCache.getWriterData();

        ObjectsCache::const_iterator it = objCache.begin();
        for (; it != objCache.end(); it++)
        {
            //增量的时候加载的是服务的所有节点，因此这里直接替换
            tmpObjCache[it->first] = it->second;
        }
        _objectsCache.swap();
    }
}

void CDbHandle::updateInactiveObjectsCache(const ObjectsCache& objCache, bool updateAll)
{
    //全量更新
    if (updateAll)
    {
        _objectsCache.getWriterData() = objCache;
        _objectsCache.swap();
    }
    else
    {
        //用查询数据覆盖一下
        _objectsCache.getWriterData() = _objectsCache.getReaderData();
        ObjectsCache& tmpObjCache = _objectsCache.getWriterData();

        ObjectsCache::const_iterator it = objCache.begin();
        for (; it != objCache.end(); it++)
        {
            //增量的时候加载的是服务的所有节点，因此这里直接替换
            tmpObjCache[it->first].vInactiveEndpoints.push_back((it->second).vInactiveEndpoints[0]);
        }
        _objectsCache.swap();
    }
}


void CDbHandle::updateActiveObjectsCache(const ObjectsCache& objCache, bool updateAll)
{
    //全量更新
    if (updateAll)
    {
        _objectsCache.getWriterData() = objCache;
        _objectsCache.swap();
    }
    else
    {
        //用查询数据覆盖一下
        _objectsCache.getWriterData() = _objectsCache.getReaderData();
        ObjectsCache& tmpObjCache = _objectsCache.getWriterData();

        ObjectsCache::const_iterator it = objCache.begin();
        for (; it != objCache.end(); it++)
        {
            //增量的时候加载的是服务的所有节点，因此这里直接替换
            tmpObjCache[it->first].vActiveEndpoints.push_back((it->second).vActiveEndpoints[0]);
        }
        _objectsCache.swap();
    }
}


void CDbHandle::addActiveEndPoint(const string& objName, const Int32 port, const Int32 istcp)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    //endPoint.setId = setName + "." + setArea + "." + setGroup;
    objectsCache[objName].vActiveEndpoints.push_back(endPoint);
    updateActiveObjectsCache(objectsCache, false);
}

void CDbHandle::addEndPointbySet(const string& objName, const Int32 port, const Int32 istcp, const string& setName, const string& setArea, const string& setGroup)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    endPoint.setId = setName + "." + setArea + "." + setGroup;
    objectsCache[objName].vActiveEndpoints.push_back(endPoint);
    updateActiveObjectsCache(objectsCache, false);

    if (setName.size())
    {
        InsertSetRecord(objName, setName, setArea, setGroup, endPoint);
    }
}

void CDbHandle::addActiveWeight1EndPoint(const string& objName, const Int32 port, const Int32 istcp, const string& setName)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    endPoint.setId = setName;
    endPoint.weight = 2;
    endPoint.weightType = 1;
    objectsCache[objName].vActiveEndpoints.push_back(endPoint);
    updateActiveObjectsCache(objectsCache, false);
}

void CDbHandle::addInActiveWeight1EndPoint(const string& objName, const Int32 port, const Int32 istcp, const string& setName)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    endPoint.setId = setName;
    endPoint.weight = 2;
    endPoint.weightType = 1;
    objectsCache[objName].vInactiveEndpoints.push_back(endPoint);
    updateInactiveObjectsCache(objectsCache, false);
}


void CDbHandle::addActiveWeight2EndPoint(const string& objName, const Int32 port, const Int32 istcp, const string& setName)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    endPoint.setId = setName;
    endPoint.weight = 2;
    endPoint.weightType = 2;
    objectsCache[objName].vActiveEndpoints.push_back(endPoint);
    updateActiveObjectsCache(objectsCache, false);
}


void CDbHandle::addInactiveEndPoint(const string& objName, const Int32 port, const Int32 istcp)
{
#define LOCAL_HOST "127.0.0.1"
    ObjectsCache objectsCache;
    EndpointF endPoint;
    endPoint.host        = LOCAL_HOST;
    endPoint.port        = port;
    endPoint.timeout     = 30000;
    endPoint.istcp = istcp;
    //endPoint.setId = setName;
    objectsCache[objName].vInactiveEndpoints.push_back(endPoint);
    updateInactiveObjectsCache(objectsCache, false);

}

void CDbHandle::cleanEndPoint()
{
    ObjectsCache objectsCache;
    updateObjectsCache(objectsCache, true);
}

int CDbHandle::getGroupId(const string& ip)
{

    map<string, int>& groupIdMap = _groupIdMap.getReaderData();
    map<string, int>::iterator it = groupIdMap.find(ip);
    if (it != groupIdMap.end())
    {
        return it->second;
    }

    uint32_t uip = stringIpToInt(ip);
    string ipStar = Ip2StarStr(uip);
    it = groupIdMap.find(ipStar);
    if (it != groupIdMap.end())
    {
        return it->second;
    }

    return -1;
}

uint32_t CDbHandle::stringIpToInt(const std::string& sip)
{
    string ip1, ip2, ip3, ip4;
    uint32_t dip, p1, p2, p3;
    dip = 0;
    p1 = sip.find('.');
    p2 = sip.find('.', p1 + 1);
    p3 = sip.find('.', p2 + 1);
    ip1 = sip.substr(0, p1);
    ip2 = sip.substr(p1 + 1, p2 - p1 - 1);
    ip3 = sip.substr(p2 + 1, p3 - p2 - 1);
    ip4 = sip.substr(p3 + 1, sip.size() - p3 - 1);
    (((unsigned char *)&dip)[0]) = TC_Common::strto<unsigned int>(ip1);
    (((unsigned char *)&dip)[1]) = TC_Common::strto<unsigned int>(ip2);
    (((unsigned char *)&dip)[2]) = TC_Common::strto<unsigned int>(ip3);
    (((unsigned char *)&dip)[3]) = TC_Common::strto<unsigned int>(ip4);
    return htonl(dip);
}

string CDbHandle::Ip2Str(uint32_t ip)
{
    char str[50];
    unsigned char  *p = (unsigned char *)&ip;
    sprintf(str, "%u.%u.%u.%u", p[3], p[2], p[1], p[0]);
    return string(str);
}

string CDbHandle::Ip2StarStr(uint32_t ip)
{
    char str[50];
    unsigned char  *p = (unsigned char *)&ip;
    sprintf(str, "%u.%u.%u.*", p[3], p[2], p[1]);
    return string(str);
}


void CDbHandle::InsertSetRecord(const string& objName, const string& setName, const string& setArea, const string& setGroup, EndpointF epf)
{
    SetDivisionCache setDivisionCache;

    string setId = setName + "." + setArea + "." + setGroup;
    SetServerInfo setServerInfo;
    setServerInfo.bActive = true;
    setServerInfo.epf    = epf;

    setServerInfo.sSetId = setId;
    setServerInfo.sSetArea = setArea;

    setDivisionCache[objName][setName].push_back(setServerInfo);

    setServerInfo.bActive = false;
    setServerInfo.epf.port = 10204;

    setDivisionCache[objName][setName].push_back(setServerInfo);
    
    updateDivisionCache(setDivisionCache, true);
}


void CDbHandle::InsertSetRecord4Inactive(const string& objName, const string& setName, const string& setArea, const string& setGroup, EndpointF epf)
{
    SetDivisionCache setDivisionCache;

    string setId = setName + "." + setArea + "." + setGroup;
    SetServerInfo setServerInfo;
    setServerInfo.bActive = false;
    setServerInfo.epf    = epf;

    setServerInfo.sSetId = setId;
    setServerInfo.sSetArea = setArea;

    setDivisionCache[objName][setName].push_back(setServerInfo);
    
    updateDivisionCache(setDivisionCache, false);
}


void CDbHandle::updateDivisionCache(SetDivisionCache& setDivisionCache,bool updateAll)
{
    //ȫ������
    if(updateAll)
    {
        if (setDivisionCache.size() == 0)
        {
            return;
        }
        SetDivisionCache::iterator it = setDivisionCache.begin();
        for(;it != setDivisionCache.end();it++)
        {
            if(it->second.size() > 0)
            {
                map<string,vector<CDbHandle::SetServerInfo> >::iterator it_inner = it->second.begin();
                for(;it_inner != it->second.end();it_inner++)
                {
                    //updateCpuLoadInfo(it_inner->second);
                }
            }
        }
        _setDivisionCache.getWriterData() = setDivisionCache;
        _setDivisionCache.swap();
    }
    else
    {
        _setDivisionCache.getWriterData() = _setDivisionCache.getReaderData();
        SetDivisionCache& tmpsetCache = _setDivisionCache.getWriterData();
        SetDivisionCache::const_iterator it = setDivisionCache.begin();
        for(;it != setDivisionCache.end();it++)
        {
            //��set��Ϣ�Ÿ���
            if(it->second.size() > 0)
            {
                tmpsetCache[it->first] = it->second;
            }
            else if(tmpsetCache.count(it->first))
            {
                //�����������нڵ㶼û������set��ɾ�������е�set��Ϣ
                tmpsetCache.erase(it->first);
            }

        }
        
        _setDivisionCache.swap();
    }
}
#if 0

void CDbHandle::updateCpuLoadInfo(vector<EndpointF> &vEndpointF)
{
    CpuLoadCache &cpuLoadCacheMap = _cpuLoadCacheMap.getReaderData();
    for(size_t i = 0; i < vEndpointF.size(); ++i)
    {
        map<string,CpuLoadInfo>::const_iterator const_it_cpu = cpuLoadCacheMap.find(vEndpointF[i].host);
        if(const_it_cpu != cpuLoadCacheMap.end())
        {
            struct tm tb;
            int ret = TC_Common::str2tm(const_it_cpu->second.sHeartTime, "%Y-%m-%d %H:%M:%S", tb);
            if(ret == 0)
            {
                vEndpointF[i].cpuload = const_it_cpu->second.iCpuLoad;
                vEndpointF[i].sampletime = mktime(&tb);
            }
            else
            {
                vEndpointF[i].cpuload = -1;
                vEndpointF[i].sampletime = 0;
            }
        }
        else
        {
            vEndpointF[i].cpuload = -1;
            vEndpointF[i].sampletime = 0;
        }
    }
}

void CDbHandle::updateCpuLoadInfo(vector<CDbHandle::SetServerInfo> &vSetServerInfo)
{
    CpuLoadCache &cpuLoadCacheMap = _cpuLoadCacheMap.getReaderData();
    for(size_t i = 0; i < vSetServerInfo.size(); ++i)
    {
        map<string,CpuLoadInfo>::const_iterator const_it_cpu = cpuLoadCacheMap.find(vSetServerInfo[i].epf.host);
        if(const_it_cpu != cpuLoadCacheMap.end())
        {
            struct tm tb;
            int ret = TC_Common::str2tm(const_it_cpu->second.sHeartTime, "%Y-%m-%d %H:%M:%S", tb);
            if(ret == 0)
            {
                vSetServerInfo[i].epf.cpuload = const_it_cpu->second.iCpuLoad;
                vSetServerInfo[i].epf.sampletime = mktime(&tb);
            }
            else
            {
                vSetServerInfo[i].epf.cpuload = -1;
                vSetServerInfo[i].epf.sampletime = 0;
            }
        }
        else
        {
            vSetServerInfo[i].epf.cpuload = -1;
            vSetServerInfo[i].epf.sampletime = 0;
        }
    }
}
#endif
