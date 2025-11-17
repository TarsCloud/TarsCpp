#ifndef _MediaStreamReceivingPortsManager_H_
#define _MediaStreamReceivingPortsManager_H_

#include <time.h>
#include <string>
#include <list>
#include <mutex>
#include <map>
#include <memory>

namespace SipRtpGateway
{
    typedef struct EvenPortAlign
    {
        std::string              _strCallId;
        int                         _iEvenPort;
        int64_t                _lAlignTime;
        std::string              _strAlignTime;

        EvenPortAlign(std::string callid, int port,int64_t lTime, std::string strTime)
        {
            _strCallId = callid;
            _iEvenPort = port;
            _lAlignTime = lTime;
            _strAlignTime = strTime;
        }

    }EvenPortAlign;

    static int64_t      TimeOutMaxPortDuration =  10 * 60 * 1000;                   //TEST.... 端口最大分配时间
    static int          MaxPortAllocChance = 10;
    static int          RemainingPortsLimit = 10;

    class MediaStreamReceivingPortsManager
    {

    private:

        std::list<int>                      m_listFreeEvenPorts;                    //空闲的偶数端口
        std::mutex                          m_mutexListPorts;
        int                                 m_iMinPort;                             //配置文件中配置的最小接收媒体流端口
        int                                 m_iMaxPort;
        int                                 m_totalFreePortsNum;

        //statistics
        std::map<std::string, EvenPortAlign>  m_mapAlignedPorts;                      //key : call id
        std::mutex                          m_mutexAlignedPorts;

        int                                 m_iTotalPoppedPorts;                    //the total times of getting a even port
        int                                 m_iTotalInsertedPorts;

    public:

        MediaStreamReceivingPortsManager();
        void initFreeReceivingPorts(int minPort, int maxPort, int avoidMinNmsPort, int avoidMaxNmsPort);            //初始化，生成空闲端口列表（偶数RTP端口， RTCP = RTP+1）
        bool getAFreeEvenPort(const std::string & callId, int & evenPort);                                       //获取空闲偶数端口
        bool insertAFreeEvenPort(const std::string & callId, int evenPort);                                      //RTP 会话结束，端口释放，重新放回列表

        bool insertAnAlignedPortObj(const std::string & callid, const int & evenPort);
        bool eraseAFreeedPortObj(const std::string & callid);
        void printfMaxDurationPopedPorts();

        void getPortNumbers(int & total, int & free, int & poped, int & inserted);
        int getFreePercent();
        bool checkPortAvailable(int port, std::string & reason);

    };

    typedef std::shared_ptr<MediaStreamReceivingPortsManager> MediaStreamReceivingPortsManagerPtr;
};





#endif





