#ifndef __RtpGatewayInterface_h__
#define __RtpGatewayInterface_h__

#include <memory>
#include <string>

#include "watermelon/common_types.h"
#include "watermelon/voe_errors.h"
#include "watermelon/voe_base.h"
#include "watermelon/voe_codec.h"
#include "watermelon/voe_volume_control.h"
#include "watermelon/voe_dtmf.h"
#include "watermelon/voe_rtp_rtcp.h"
#include "watermelon/voe_audio_processing.h"
#include "watermelon/voe_file.h"
#include "watermelon/voe_video_sync.h"
#include "watermelon/voe_encryption.h"
#include "watermelon/voe_hardware.h"
#include "watermelon/voe_external_media.h"
#include "watermelon/voe_network.h"
#include "watermelon/voe_neteq_stats.h"

namespace SipRtpGateway
{
    class RtpGatewayInterface
    {
    public:
        virtual bool create() = 0;
        virtual bool release() = 0;
        virtual int createChannel() = 0;
        virtual int delChannel(int channelId) = 0;
        virtual int setMaxChannelsPerThread(int channels) = 0;
        virtual int getMaxNumOfChannels() = 0;
        virtual int setCallBack(int channelId, jssmme::Transport& callback) = 0;
        virtual int connectChannel(int callerChannelId, int calleeChannelId, int callerDtmf = -1, int calleeDtmf = -1) = 0;
        virtual int disconnectChannel(int callerChannelId, int calleeChannelId) = 0;

        virtual int setSendCodec(int channelId, jssmme::CodecInst codec) = 0;
        virtual bool setSendCodec(int channelId, const std::string& name) = 0;
        virtual int setSendRedCodec(int channelId, int iPayload) = 0;
        virtual int setLocalReceiver(int channelId, int port) = 0;
        virtual int setSendDestination(int channel1, int port, const std::string & ip) = 0;

        virtual int receivedRTPPacket(int channelId, const unsigned char *msg, int size) = 0;
        virtual int receivedRTCPPacket(int channelId, const unsigned char *msg, int size) = 0;

        virtual int setFecStatus(int channelId, int red) = 0;
        virtual int GetCodecByName(const char *pcName, jssmme::CodecInst *pCodec, int iFreq) = 0;

        virtual int getReceivedRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
            unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms) = 0;

        virtual int getSentRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
            unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms) = 0;

        virtual int getRTPStatistics(int channelId, unsigned int& averageJitterMs, unsigned int& maxJitterMs,
            unsigned int& discardedPackets) = 0;

        virtual int getTotalDataCounters(int channelId, unsigned int& bytesSent, unsigned int& packetsSent,
            unsigned int& bytesReceived, unsigned int& packetsReceived) = 0;

        virtual int mediaEmptyPktCheck(int channelId, unsigned int &level) = 0;
        virtual bool startCallRecord(int channelId, const std::string& path) = 0;
        virtual bool stopCallRecord(int channelId) = 0;
    };

    typedef std::shared_ptr<RtpGatewayInterface> RtpGatewayInterfacePtr;

}

#endif //__RtpGatewayInterface_h__
