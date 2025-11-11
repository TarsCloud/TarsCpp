#include "ConnectionCallIdManager.h"
#include "util/tc_timeprovider.h"
#include "servant/Application.h"

namespace SipGateway
{
    //............ ConnectionCallIdManager ................
    ConnectionCallIdManager::ConnectionCallIdManager()
    {
        _insertExceptionNum = 0;
    }

    bool ConnectionCallIdManager::insertConnection( const std::string & strSipDialogId, const std::string & strConnectionId, const GatewayConnectionPtr & pConnection)
    {
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);

        bool isSBCConnectionId = checkBelongToSBCConnectionIds(strConnectionId);
        if (_strToSBCSipDialogId != strSipDialogId && isSBCConnectionId)
            return true;

        DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(strSipDialogId);
        if (iterSipDialog == _mapCallConnections.end())
        {
            ConnectionsMap mapConnections;
            mapConnections.insert(make_pair(strConnectionId, pConnection));
            _mapCallConnections.insert(make_pair(strSipDialogId, mapConnections));

            ++_insertedNum;
            ++_totalCallNum;
            ++_insertCallNum;
            TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, insert a item in map, Reg or SipDialog id : " << strSipDialogId << ",ConnectionId:" << strConnectionId << endl);
        }
        else
        {
            bool bExist = iterSipDialog->second.find(strConnectionId) == iterSipDialog->second.end() ? false : true;
            if (!bExist)
            {
                iterSipDialog->second.insert(make_pair(strConnectionId, pConnection));
                ++_insertedNum;
            }
            else
                return false;;
        }
        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, insert a GatewayConnectionPtr,connection id:" << strConnectionId << ",sipDialogId:" << strSipDialogId << endl);
        return true;
    }

    bool ConnectionCallIdManager::removeConnection(const std::string & sipdialogId, const std::string strConnectionId)
    {
        std::string errReason;
        do
        {
            GatewayConnectionPtr pCon;
            {
                std::lock_guard<std::mutex> temLock(_mutexCallConnections);
                DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(sipdialogId);
                if (iterSipDialog == _mapCallConnections.end())
                {
                    errReason = "SipDialog does not exists in the map";
                    break;
                }

                if (iterSipDialog->second.size() < 1)
                {
                    _mapCallConnections.erase(iterSipDialog);
                    errReason = "this SipDialog has not any GatewayConnection exist in the map";
                    break;
                }

                ConnectionsMap::iterator iterConnetion = iterSipDialog->second.find(strConnectionId);
                if (iterConnetion != iterSipDialog->second.end())
                {
                    pCon = iterConnetion->second;
                    if (pCon == 0)
                    {
                        iterSipDialog->second.erase(iterConnetion);
                        if (iterSipDialog->second.size() == 0)
                        {
                            _mapCallConnections.erase(iterSipDialog);
                        }
                        ++_removedNum;
                        return true;
                    }

                    pCon->setEraseTime("ConnectionCallIdManager::RemoveConnection(string, string)");
                    iterSipDialog->second.erase(iterConnetion);
                    if (iterSipDialog->second.size() == 0)
                    {
                        _mapCallConnections.erase(iterSipDialog);
                    }
                    ++_removedNum;
                }
                else
                {
                    errReason = "this GatewayConnection doesn't exist";
                    break;
                }
            }
            pCon->clearUp();
            pCon = 0;

        } while (0);

        bool ret = errReason.empty();
        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, remove a GatewayConnectionPtr,call id: " << strConnectionId << " ,SipDialogid:" << sipdialogId << ",ret:" << ret << ",errReason:" << errReason << endl);
        return ret;
    }

    bool ConnectionCallIdManager::removeAllConnectionsOfSipDialog(const std::string & sipDialogId)
    {
        std::string errReason;
        do
        {
            std::lock_guard<std::mutex> temLock(_mutexCallConnections);
            DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(sipDialogId);
            if (iterSipDialog == _mapCallConnections.end())
            {
                errReason = "SipDialog does not exists in the map";
                break;
            }

            bool bExist = false;
            ConnectionsMap::iterator iterConnetion = iterSipDialog->second.begin();
            int conNum = iterSipDialog->second.size();
            for (; iterConnetion != iterSipDialog->second.end(); )
            {
                GatewayConnectionPtr pCon = iterConnetion->second;
                if (pCon != 0)
                {
                    std::string temId = pCon->getConnectionId();
                    bool isSBCConnectionId = checkBelongToSBCConnectionIds(temId);
                    if (isSBCConnectionId)
                    {
                        iterConnetion ++;
                    }
                    else
                    {
                        {
                            pCon->setEraseTime("ConnectionCallIdManager::RemoveAllConnectionsOfSipDialog()");
                            iterSipDialog->second.erase(iterConnetion++);
                            TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, erased this Connectionn in the vector, SipDialogid:" << sipDialogId << " ,Connection Id:" << temId << endl);
                        }
                        pCon->clearUp();
                        pCon = 0;
                    }
                }
            }

            _mapCallConnections.erase(iterSipDialog);
            TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called,Removed this SipDialog , SipDialogid:" << sipDialogId << endl);

            --_totalCallNum;
            ++_deleteCallNum;
            _removedNum += conNum;

        } while (0);

        bool ret = errReason.empty();
        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, SipDialogid:" << sipDialogId << ",ret:" << ret << ",errReason:" << errReason << endl);
        return ret;
    }

    GatewayConnectionPtr ConnectionCallIdManager::getConnection(const std::string & strSipDialogId, const std::string strConnectionId)
    {
        std::string errReason;
        do
        {
            std::lock_guard<std::mutex> temLock(_mutexCallConnections);
            DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(strSipDialogId);
            if (iterSipDialog == _mapCallConnections.end())
            {
                errReason = "this SipDialog does not exists in the map";
                break;
            }

            if(iterSipDialog->second.size() == 0)
            {
                errReason = "this SipDialog  has not any Connections in the map";
                _mapCallConnections.erase(iterSipDialog);
                break;
            }
            else
            {
                ConnectionsMap::iterator iterConnetion = iterSipDialog->second.find(strConnectionId);
                if (iterConnetion == iterSipDialog->second.end())
                {
                    errReason = "connection not exist";
                    break;
                }
                else
                {
                    return iterConnetion->second;
                }
            }
        } while (0);

        if (false == errReason.empty())
        {
            TLOGWARN("[ConnectionManager] " << __FUNCTION__ << "() called, and return FALSE,strSipDialogId:" << strSipDialogId << ", connection id:" << strConnectionId << ",reason:" << errReason << endl);
            return 0;
        }
        return 0;
    }

    bool ConnectionCallIdManager::checkConnectionIdExist( const std::string & strSipDialogId, const std::string & strConnectionId)
    {
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);
        DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(strSipDialogId);
        if (iterSipDialog == _mapCallConnections.end())
        {
            return false;
        }
        ConnectionsMap::iterator iterCon = iterSipDialog->second.find(strConnectionId);
        return iterCon == iterSipDialog->second.end() ? false : true;
    }

    int ConnectionCallIdManager::getMapGatewayConnectionSize()
    {
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);
        int totalNum = 0;
        DialogConnectionSMap::iterator iterSipDialog;
        for (iterSipDialog = _mapCallConnections.begin(); iterSipDialog != _mapCallConnections.end(); iterSipDialog++)
        {
            totalNum += iterSipDialog->second.size();
        }
        return  totalNum;
    }

    int ConnectionCallIdManager::getMapExceptionalConnectionSize()
    {
        std::lock_guard<std::mutex> temLock(_mutexExceptionalGatewayConnetionMap);
        return _mapExceptionalGatewayConnections.size();
    }

    int ConnectionCallIdManager::getRegOrSipDialogNum()
    {
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);
        return _mapCallConnections.size();
    }

    bool ConnectionCallIdManager::insertExceptionalConnection(const std::string & strConnectionId, const GatewayConnectionPtr & pConnection)
    {
        std::lock_guard<std::mutex> temLock(_mutexExceptionalGatewayConnetionMap);
        ConnectionsMap::iterator iterConnection = _mapExceptionalGatewayConnections.find(strConnectionId);
        if (iterConnection != _mapExceptionalGatewayConnections.end())
            return false;

        _insertExceptionNum ++;
        _mapExceptionalGatewayConnections.insert(make_pair(strConnectionId, pConnection));

        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called,sipDialogID: " << strConnectionId << ", now the total is :" << int(_mapExceptionalGatewayConnections.size()) << endl);
        return true;
    }

    bool ConnectionCallIdManager::removeExceptionalConnection(std::string strConnectionId)
    {
        GatewayConnectionPtr p;
        {
            std::lock_guard<std::mutex> temLock(_mutexExceptionalGatewayConnetionMap);
            ConnectionsMap::iterator iterConnection = _mapExceptionalGatewayConnections.find(strConnectionId);
            if (iterConnection == _mapExceptionalGatewayConnections.end())
            {
                TLOGWARN("[ConnectionManager] " << __FUNCTION__ << "() called, GatewayConnection not exists in the map, SipDialog id:" << strConnectionId << endl);
                return false;
            }
            p = iterConnection->second;
            p->setEraseTime("ConnectionCallIdManager::RemoveExceptionalConnection()");
            _mapExceptionalGatewayConnections.erase(iterConnection);
        }
        p->clearUp();
        p = 0;

        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, remove a GatewayConnectionPtr,call id: " << strConnectionId << ", now the total is : " << (int)_mapExceptionalGatewayConnections.size() << endl);
        return true;
    }

    bool ConnectionCallIdManager::eraseAllExceptionalConnections()
    {
        int64_t cutTime = TNOWMS;
        std::lock_guard<std::mutex> temLock(_mutexExceptionalGatewayConnetionMap);
        GatewayConnectionPtr p;
        ConnectionsMap::iterator iterConnection = _mapExceptionalGatewayConnections.begin();
        for (; iterConnection != _mapExceptionalGatewayConnections.end(); )
        {
            bool dFlag = false;
            int64_t diffTime = 0;
            {
                p = iterConnection->second;
                if (p)
                {
                    diffTime = cutTime - p->_createTime;
                    if (diffTime > 10 * 1000)
                    {
                        dFlag = true;
                        p->clearUp();
                        p = 0;
                    }
                }
                else
                {
                    dFlag = true;
                }
            }

            if (dFlag)
            {
                _mapExceptionalGatewayConnections.erase(iterConnection++);
            }
            else
            {
                iterConnection++;
            }
        }
        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, after ERASE, now the total is : " << (int)_mapExceptionalGatewayConnections.size() << endl);
        return true;
    }

    bool ConnectionCallIdManager::insertToSBCConnectionId(const std::string & connectionId)
    {
        std::lock_guard<std::mutex> temLock(_mutexSetSBCConnId);
        std::set<std::string>::iterator iter = _setToSBCConectionId.find(connectionId);
        if (iter == _setToSBCConectionId.end())
        {
            _setToSBCConectionId.insert(connectionId);
            return true;
        }
        return false;
    }

    bool ConnectionCallIdManager::checkBelongToSBCConnectionIds(const std::string & connectionId)
    {
        std::lock_guard<std::mutex> temLock(_mutexSetSBCConnId);
        std::set<std::string>::iterator iter = _setToSBCConectionId.find(connectionId);
        if (iter == _setToSBCConectionId.end())
            return false;
        return true;
    }

    void ConnectionCallIdManager::setToSBCSipDialogId(const std::string & simulatedSipDialogId)
    {
        _strToSBCSipDialogId = simulatedSipDialogId;
    }

    int ConnectionCallIdManager::getToSBCConnectionNumber()
    {
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);
        DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(_strToSBCSipDialogId);
        if (iterSipDialog == _mapCallConnections.end())
            return 0;

        return iterSipDialog->second.size();
    }

    bool ConnectionCallIdManager::getToSBCConnections(ConnectionsMap & connections)
    {
        connections.clear();
        std::lock_guard<std::mutex> temLock(_mutexCallConnections);
        DialogConnectionSMap::iterator iterSipDialog = _mapCallConnections.find(_strToSBCSipDialogId);
        if (iterSipDialog == _mapCallConnections.end())
            return false;

        connections = iterSipDialog->second;
        return true;
    }

    bool ConnectionCallIdManager::insertTcpFromSBC(const std::string & connId, const GatewayConnectionPtr & pConnection)
    {
        if (pConnection->getToSBCConnection())
            return false;

        std::lock_guard<std::mutex> lkc(_mutexTcpLinks);
        ConnectionsMap::iterator itId = _fromSBCTcpLinks.find(connId);
        if (itId == _fromSBCTcpLinks.end())
        {
            TLOGINFO("[ConnectionManager] " << __FUNCTION__ << "() called,id: " << pConnection->getConnectionId() << endl);
            _fromSBCTcpLinks.insert(make_pair(connId, pConnection));
            return true;
        }
        return false;
    }

    void ConnectionCallIdManager::clearExpiredTcpFromSBC()
    {
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();
        std::lock_guard<std::mutex> lkc(_mutexTcpLinks);
        ConnectionsMap::iterator itId = _fromSBCTcpLinks.begin();
        for (; itId != _fromSBCTcpLinks.end(); )
        {
            const GatewayConnectionPtr & pCon = itId->second;
            if ((curTicks - pCon->getActiveTicks()) > 10 * 60 * 1000)
            {
                pCon->clearUp();
                TLOGINFO("[ConnectionManager] " << __FUNCTION__ << "() called,id: " << pCon->getConnectionId() << ",create time:" << pCon->getCreateTime() << endl);
                _fromSBCTcpLinks.erase(itId++);
            }
            else
            {
                itId++;
            }
        }
    }

    int ConnectionCallIdManager::getTcpLinkSizeFromSbc()
    {
        std::lock_guard<std::mutex> lkc(_mutexTcpLinks);
        return _fromSBCTcpLinks.size();
    }

    bool ConnectionCallIdManager::checkTCPLinkFromSbc(const GatewayConnectionPtr & pConnection)
    {
        if (!pConnection)
            return false;

        if (pConnection->getTransProtocol() == kTransUDP)
            return false;

        std::lock_guard<std::mutex> lkc(_mutexTcpLinks);
        ConnectionsMap::iterator itId = _fromSBCTcpLinks.find(pConnection->getConnectionId());
        return (itId == _fromSBCTcpLinks.end()) ? false : true;
    }

    std::atomic<int> ConnectionCallIdManager::_insertedNum(0);
    std::atomic<int> ConnectionCallIdManager::_removedNum(0);
    std::atomic<int> ConnectionCallIdManager::_totalCallNum(0);
    std::atomic<int> ConnectionCallIdManager::_insertCallNum(0);
    std::atomic<int> ConnectionCallIdManager::_deleteCallNum(0);

    //***********************   MsgRemainManager    ***********************

    void MsgRemainManager::insertRemainMsg(const std::string & connectionId, const std::string & msg)
    {
        std::lock_guard<std::mutex> lock(m_mutexRemainMsgs);

        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, connectionId:" << connectionId << ",msg size:" << int(msg.size()) << ":\n" << msg << endl);
        std::map<std::string, MsgRemain>::iterator itId = m_mapRemainMsgs.find(connectionId);
        if (itId == m_mapRemainMsgs.end())
        {
            if (msg.empty())
                return;

            MsgRemain remain(msg, connectionId);
            m_mapRemainMsgs.insert(make_pair(connectionId, remain));
        }
        else
        {
            if (msg.empty())
                m_mapRemainMsgs.erase(itId);
            else
            {
                itId->second._msg = msg;
                itId->second._ticks = TC_TimeProvider::getInstance()->getNowMs();
            }
        }
    }

    bool MsgRemainManager::getRemainMsg(const std::string & connectionId, std::string & msg)
    {
        std::lock_guard<std::mutex> lock(m_mutexRemainMsgs);

        std::map<std::string, MsgRemain>::iterator itId = m_mapRemainMsgs.find(connectionId);
        if (itId == m_mapRemainMsgs.end())
        {
            return false;
        }
        else
        {
           msg = itId->second._msg;
           return true;
        }
    }

    void MsgRemainManager::cleanRemainMsg(const std::string & connectionId)
    {
        std::lock_guard<std::mutex> lock(m_mutexRemainMsgs);

        TLOGDEBUG("[ConnectionManager] " << __FUNCTION__ << "() called, connectionId:" << connectionId << endl);
        std::map<std::string, MsgRemain>::iterator itId = m_mapRemainMsgs.find(connectionId);
        if (itId == m_mapRemainMsgs.end())
        {
            return;
        }
        else
        {
            m_mapRemainMsgs.erase(itId);
        }
    }

    int MsgRemainManager::getRemainSize()
    {
        std::lock_guard<std::mutex> lock(m_mutexRemainMsgs);
        return m_mapRemainMsgs.size();
    }

    void MsgRemainManager::cleanExpiredRemains()
    {
        std::lock_guard<std::mutex> lock(m_mutexRemainMsgs);
        int64_t curTicks = TC_TimeProvider::getInstance()->getNowMs();

        std::map<std::string, MsgRemain>::iterator itId = m_mapRemainMsgs.begin();
        for (; itId != m_mapRemainMsgs.end(); )
        {
            if ((curTicks - itId->second._ticks) > 60 * 1000)
            {
                m_mapRemainMsgs.erase(itId++);
            }
            else
            {
                itId++;
            }
        }
    }
};
