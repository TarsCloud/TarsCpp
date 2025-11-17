#include "MediaStreamReceivingPortsManager.h"

#ifdef _MSC_VER

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#endif
#include "servant/RemoteLogger.h"

namespace SipRtpGateway
{
    MediaStreamReceivingPortsManager::MediaStreamReceivingPortsManager()
    {
        m_totalFreePortsNum = 0;
        m_iTotalPoppedPorts = 0;
        m_iTotalInsertedPorts = 0;
        m_iMaxPort = 0;
        m_iMinPort = 0;
    }

    void MediaStreamReceivingPortsManager::initFreeReceivingPorts(int minPort, int maxPort, int avoidMinNmsPort, int avoidMaxNmsPort)
    {
        std::lock_guard<std::mutex> temLock(m_mutexListPorts);
        m_listFreeEvenPorts.clear();
        m_iMinPort = (minPort % 2 == 0) ? minPort : minPort + 1;
        m_iMaxPort = (maxPort % 2 == 0) ? maxPort - 2 : maxPort - 1;

        for (int  i = m_iMinPort; i <= m_iMaxPort; )
        {
            if ( (i < avoidMinNmsPort) || (i > avoidMaxNmsPort))
            {
                m_listFreeEvenPorts.push_back(i);
            }
            i = i + 2;
        }
        m_totalFreePortsNum = m_listFreeEvenPorts.size();
        TLOGINFO(__FUNCTION__ << "() called. total even port:" << m_totalFreePortsNum << endl);

        m_iTotalPoppedPorts = 0;
        m_iTotalInsertedPorts = 0;
    }

    bool MediaStreamReceivingPortsManager::getAFreeEvenPort(const std::string & callId, int & evenPort)
    {
        std::string reason;
        bool result = false;
        do
        {
            std::lock_guard<std::mutex> temLock(m_mutexListPorts);
            {
                for (int i = 0; i < MaxPortAllocChance; i++)
                {
                    if (m_listFreeEvenPorts.size() < 1)
                    {
                        reason = "free port list size < 1";
                        break;
                    }
                    evenPort = *m_listFreeEvenPorts.begin();
                    m_listFreeEvenPorts.pop_front();

                    std::string reason1, reason2;
                    if (checkPortAvailable(evenPort, reason1) && checkPortAvailable(evenPort + 1, reason2))
                    {
                        result = true;
                        break;
                    }
                    else
                    {
                        m_listFreeEvenPorts.push_back(evenPort);
                    }
                    TLOGWARN(__FUNCTION__ << "() called.port not available:" << evenPort << ",port:" << (evenPort + 1)
                        << "  ,call id :" << callId << ",reason1:" << reason1 << ",reason2:" << reason2 << endl);
                }
                reason = "Can't found a available free even port.";
            }
        } while (0);

        if (!result)
        {
            TLOGWARN(__FUNCTION__ << "() called. callId:" << callId << ",reason:" << reason << endl);
            return false;
        }

        m_iTotalPoppedPorts++;
        insertAnAlignedPortObj(callId, evenPort);
        TLOGINFO(__FUNCTION__ << "() called. got a port:" << evenPort << "  ,call id :" << callId << endl);
        return true;
    }

    bool MediaStreamReceivingPortsManager::insertAFreeEvenPort(const std::string & callId, int evenPort)
    {
        if (evenPort % 2 != 0)
            return false;

        {
            std::lock_guard<std::mutex> temLock(m_mutexListPorts);
            std::list<int>::iterator iter = m_listFreeEvenPorts.begin();
            for (; iter != m_listFreeEvenPorts.end(); iter++)
            {
                if (*iter == evenPort)
                {
                    TLOGWARN(__FUNCTION__ << "() called .this port: " << evenPort << " exists in the list! callId:" << callId << endl);
                    return false;
                }
            }
            m_listFreeEvenPorts.push_back(evenPort);
        }

        m_iTotalInsertedPorts++;
        eraseAFreeedPortObj(callId);

        TLOGINFO(__FUNCTION__ << "() called. freed port:" << evenPort << " ,call id:" << callId << endl);
        return true;
    }

    bool MediaStreamReceivingPortsManager::insertAnAlignedPortObj(const std::string & callid, const int & evenPort)
    {
        if (callid.empty())
        {
            TLOGWARN(__FUNCTION__ << "() called.callid is EMPTY. evenPort:" << evenPort << endl);
            return false;
        }

        std::lock_guard<std::mutex> temLock(m_mutexAlignedPorts);
        std::map<std::string, EvenPortAlign> ::iterator iter = m_mapAlignedPorts.find(callid);
        if (iter != m_mapAlignedPorts.end())
            return false;

        int64_t curTime = TNOWMS;
        std::string strTime =  TC_Common::tm2str(curTime, "%d-%02d-%02d %02d:%02d:%02d:%03d");
        m_mapAlignedPorts.insert(make_pair(callid, EvenPortAlign(callid, evenPort, curTime, strTime)));
        TLOGDEBUG(__FUNCTION__ << "() called. port:" << evenPort << " ,call id:" << callid << endl);
        return true;
    }

    bool MediaStreamReceivingPortsManager::eraseAFreeedPortObj(const std::string & callid)
    {
        if (callid.empty())
        {
            TLOGWARN(__FUNCTION__ << "() called .callid is EMPTY!" << endl);
            return false;
        }

        std::lock_guard<std::mutex> temLock(m_mutexAlignedPorts);
        std::map<std::string, EvenPortAlign> ::iterator iter = m_mapAlignedPorts.find(callid);
        if (iter == m_mapAlignedPorts.end())
        {
            TLOGWARN(__FUNCTION__ << "() called.callid :" << callid << " not exists in the map!" << endl);
            return false;
        }

        m_mapAlignedPorts.erase(iter);
        return true;
    }

    void MediaStreamReceivingPortsManager::printfMaxDurationPopedPorts()
    {
        std::lock_guard<std::mutex> temLock(m_mutexAlignedPorts);
        if (m_mapAlignedPorts.size() == 0)
            return;

        int i = 0;
        std::map<std::string, EvenPortAlign>::iterator iter = m_mapAlignedPorts.begin();
        for (; iter != m_mapAlignedPorts.end(); iter ++)
        {
            EvenPortAlign portAlign = iter->second;

            int64_t lCurTime = TNOWMS;
            int64_t lAlignedTime = portAlign._lAlignTime;
            std::string strAlignedTime = portAlign._strAlignTime;
            std::string strCallId = portAlign._strCallId;
            int port = portAlign._iEvenPort;

            //TimeOutMaxDialogActiveDuration
            if ((lCurTime - lAlignedTime) > 10 * 60 * 1000)
            {
                TLOGINFO(__FUNCTION__ << "() called .callid :" << strCallId << " ,port:" << port << " ,aligned time:" << strAlignedTime << endl);
                i++;
                if (i > 10)
                    break;
            }
        }
    }

    void MediaStreamReceivingPortsManager::getPortNumbers(int & total, int & free, int & poped, int & inserted)
    {
        total = m_totalFreePortsNum;
        poped = m_iTotalPoppedPorts;
        inserted = m_iTotalInsertedPorts;

        std::lock_guard<std::mutex> temLock(m_mutexListPorts);
        free = int(m_listFreeEvenPorts.size());
    }

    int MediaStreamReceivingPortsManager::getFreePercent()
    {
        if (!m_totalFreePortsNum)
            return 0;

        std::lock_guard<std::mutex> temLock(m_mutexListPorts);
        if(m_listFreeEvenPorts.size() < RemainingPortsLimit)
            return -1;

        return int(float(m_listFreeEvenPorts.size()) / (float)m_totalFreePortsNum * 100);
    }

#ifdef _MSC_VER
    bool MediaStreamReceivingPortsManager::checkPortAvailable(int port, std::string & reason)
    {
        return true;
    }
#else
    bool MediaStreamReceivingPortsManager::checkPortAvailable(int port, std::string & reason)
    {
        int server_sockfd;
        struct sockaddr_in my_addr;

        my_addr.sin_family=AF_INET;
        my_addr.sin_addr.s_addr=INADDR_ANY;
        my_addr.sin_port=htons(port);
        int error = 0;

        if((server_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
        {
            reason = "socket error :" + std::to_string(error);
            return false;
        } 
        error = ::bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr));
        if (error)
        {
            reason = "bind error :" + std::to_string(error) + ",errno:" + std::to_string(errno);
            close(server_sockfd);
            return false;
        }
        close(server_sockfd);
        return true;
    }


#endif


};


