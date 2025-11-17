#ifndef __RtpGatewayInterfaceI_h__
#define __RtpGatewayInterfaceI_h__

#include <atomic>
//#include <string>
//#include <memory>

#include "RtpGatewayInterface.h"

namespace SipRtpGateway
{
    class RtpGatewayInterfaceI : public RtpGatewayInterface
    {
    public:
        RtpGatewayInterfaceI()
            :m_channelIdCount(0)
        {

        }

        bool create();
        bool release();
        int createChannel();
        int delChannel(int channelId);
        int setMaxChannelsPerThread(int channels);
        int getMaxNumOfChannels();
        int setCallBack(int channelId, jssmme::Transport& callback);
        int connectChannel(int callerChannelId, int calleeChannelId, int callerDtmf = -1, int calleeDtmf = -1);
        int disconnectChannel(int callerChannelId, int calleeChannelId);

        int setSendCodec(int channelId, jssmme::CodecInst codec);
        bool setSendCodec(int channelId, const std::string& name);
        int setSendRedCodec(int channelId, int iPayload);
        int setLocalReceiver(int channelId, int port);
        int setSendDestination(int channel1, int port, const std::string& ip);

        int receivedRTPPacket(int channelId, const unsigned char *msg, int size);
        int receivedRTCPPacket(int channelId, const unsigned char *msg, int size);

        int setFecStatus(int channelId, int red);
        int GetCodecByName(const char *pcName, jssmme::CodecInst *pCodec, int iFreq);

        int getReceivedRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
            unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms);

        int getSentRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
            unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms);

        int getRTPStatistics(int channelId, unsigned int& averageJitterMs, unsigned int& maxJitterMs,
            unsigned int& discardedPackets);

        int getTotalDataCounters(int channelId, unsigned int& bytesSent, unsigned int& packetsSent,
            unsigned int& bytesReceived, unsigned int& packetsReceived);

        int mediaEmptyPktCheck(int channelId, unsigned int &level);
        bool startCallRecord(int channelId, const std::string& path);
        bool stopCallRecord(int channelId);

    public:
        int getChannelCnt();

    private:
        jssmme::VoiceEngine        *m_voiceEngine;
        jssmme::VoEBase            *m_voEBase;
        jssmme::VoECodec           *m_voECodec;
        jssmme::VoEVolumeControl   *m_voEVolumeControl;
        jssmme::VoEDtmf            *m_voEDtmf;
        jssmme::VoERTP_RTCP        *m_voERtpRtcp;
        jssmme::VoEAudioProcessing *m_voEAudioProcessing;
        jssmme::VoENetwork         *m_voENetwork;
        jssmme::VoEFile            *m_voEFile;
        jssmme::VoEVideoSync       *m_voEVideoSync;
        jssmme::VoEEncryption      *m_voEEncryption;
        jssmme::VoEHardware        *m_voEHardware;
        jssmme::VoEExternalMedia   *m_voEExternalMedia;
        jssmme::VoENetEqStats      *m_voENetEqStats;

        std::atomic<int>            m_channelIdCount;
    };

    typedef std::shared_ptr<RtpGatewayInterfaceI> RtpGatewayInterfaceIPtr;
}

#endif //__RtpGatewayInterfaceI_h__
