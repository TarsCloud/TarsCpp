#ifndef _CONNECTION_CALLID_MANAGER_H_
#define _CONNECTION_CALLID_MANAGER_H_

#include <string>
#include <map>
#include <set>
#include <mutex>
#include <atomic>
#include "SipGatewayConnection.h"

namespace SipGateway
{

    typedef std::map<std::string, GatewayConnectionPtr> ConnectionsMap;
    typedef std::map<std::string, ConnectionsMap> DialogConnectionSMap;

    class ConnectionCallIdManager
    {
    private:
        DialogConnectionSMap                _mapCallConnections;
        std::mutex                          _mutexCallConnections;

        ConnectionsMap                      _mapExceptionalGatewayConnections;
        std::mutex                          _mutexExceptionalGatewayConnetionMap;

        std::mutex                          _mutexSetSBCConnId;
        std::set<std::string>               _setToSBCConectionId;
        std::string                         _strToSBCSipDialogId;

        ConnectionsMap                      _fromSBCTcpLinks;
        std::mutex                          _mutexTcpLinks;

    public:

        ConnectionCallIdManager();
        bool insertConnection(const std::string& strSipDialogId, const std::string& strConnectionId, const GatewayConnectionPtr& pConnection);
        bool removeConnection(const std::string& sipdialogId, const std::string strConnectionId);
        bool removeAllConnectionsOfSipDialog(const std::string& sipDialogId);

        GatewayConnectionPtr getConnection(const std::string& strSipDialogId, const std::string strConnectionId);
        bool checkConnectionIdExist(const std::string& strSipDialogId, const std::string& strConnectionId);

        int getMapGatewayConnectionSize();
        int getMapExceptionalConnectionSize();
        int getRegOrSipDialogNum();

        bool insertToSBCConnectionId(const std::string& id);
        bool checkBelongToSBCConnectionIds(const std::string& id);
        void setToSBCSipDialogId(const std::string& simulatedSipDialogId);

        int getToSBCConnectionNumber();
        bool getToSBCConnections(ConnectionsMap& connections);

        bool insertTcpFromSBC(const std::string& connId, const GatewayConnectionPtr& pConnection);
        void clearExpiredTcpFromSBC();
        int getTcpLinkSizeFromSbc();
        bool checkTCPLinkFromSbc(const GatewayConnectionPtr& pConnection);

        //statistics
        static std::atomic<int>                     _insertedNum;
        static std::atomic<int>                     _removedNum;

        static std::atomic<int>                     _totalCallNum;
        static std::atomic<int>                     _insertCallNum;
        static std::atomic<int>                     _deleteCallNum;

    public:
        //Exceptional Connection deal

        bool insertExceptionalConnection(const std::string& strConnectionId, const GatewayConnectionPtr& pConnection);
        bool removeExceptionalConnection(std::string strConnectionId);
        bool eraseAllExceptionalConnections();
        int64_t                                     _insertExceptionNum;
    };

    class MsgRemainManager
    {
    public:

        void insertRemainMsg(const std::string& connectionId, const std::string& msg);
        bool getRemainMsg(const std::string& connectionId, std::string& msg);
        void cleanRemainMsg(const std::string& connectionId);
        int getRemainSize();
        void cleanExpiredRemains();

        std::map<std::string, MsgRemain>    m_mapRemainMsgs;
        std::mutex                          m_mutexRemainMsgs;


    };

};


#endif





