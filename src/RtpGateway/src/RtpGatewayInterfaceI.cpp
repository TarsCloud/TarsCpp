#include "RtpGatewayInterfaceI.h"
#include "servant/RemoteLogger.h"

namespace SipRtpGateway
{
     bool RtpGatewayInterfaceI::create()
    {
        TLOGINFO("RtpGateway", " RtpGatewayInterfaceI::create() called,");

        m_voiceEngine = jssmme::VoiceEngine::Create();
        m_voEBase= jssmme::VoEBase::GetInterface(m_voiceEngine);
        m_voECodec = jssmme::VoECodec::GetInterface(m_voiceEngine);
        m_voEVolumeControl = jssmme::VoEVolumeControl::GetInterface(m_voiceEngine);
        m_voEDtmf = jssmme::VoEDtmf::GetInterface(m_voiceEngine);
        m_voERtpRtcp = jssmme::VoERTP_RTCP::GetInterface(m_voiceEngine);
        m_voEAudioProcessing = jssmme::VoEAudioProcessing::GetInterface(m_voiceEngine);
        m_voENetwork = jssmme::VoENetwork::GetInterface(m_voiceEngine);
        m_voEFile = jssmme::VoEFile::GetInterface(m_voiceEngine);
        m_voEVideoSync = jssmme::VoEVideoSync::GetInterface(m_voiceEngine);
        m_voEEncryption = jssmme::VoEEncryption::GetInterface(m_voiceEngine);
        m_voEHardware = jssmme::VoEHardware::GetInterface(m_voiceEngine);
        m_voEExternalMedia = jssmme::VoEExternalMedia::GetInterface(m_voiceEngine);
        m_voENetEqStats = jssmme::VoENetEqStats::GetInterface(m_voiceEngine);

        m_voEAudioProcessing->SetEcStatus(false);
        m_voEAudioProcessing->SetAgcStatus(false);

        m_voEBase->Init();
        m_voEBase->SetTraceFilter(0x1f);
        m_voEBase->SetTraceFile("./mme.log", 0, 10);

        return true;
    }

    bool RtpGatewayInterfaceI::release()
    {
        TLOGINFO("RtpGateway", std::string(__FUNCTION__) + " called,");

        if (!m_voiceEngine) 
            return false;

        m_voEBase->Terminate();
        m_voEBase->Release();
        m_voECodec->Release();
        m_voEVolumeControl->Release();
        m_voEDtmf->Release();
        m_voERtpRtcp->Release();
        m_voEAudioProcessing->Release();
        m_voENetwork->Release();
        m_voEFile->Release();
        m_voEVideoSync->Release();
        m_voEEncryption->Release();
        m_voEHardware->Release();
        m_voEExternalMedia->Release();
        m_voENetEqStats->Release();

        JssMmeDeleteEngine(VoiceEngine, m_voiceEngine);

        return true;
    }

    int RtpGatewayInterfaceI::createChannel()
    {
        int channelId = m_voEBase->CreateChannel();
        if (channelId > 0)
            m_channelIdCount++;

        return channelId;
    }

    int RtpGatewayInterfaceI::delChannel(int channelId)
    {
        int ret = m_voEBase->DeleteChannel(channelId);
        if (!ret)
            m_channelIdCount--;

        return ret;
    }

    int RtpGatewayInterfaceI::setMaxChannelsPerThread(int channels)
    {
        //return m_voEBase->SetMaxChannelsPerThread(channels);
    }

    int RtpGatewayInterfaceI::getMaxNumOfChannels()
    {
        return m_voEBase->MaxNumOfChannels();
    }

    int RtpGatewayInterfaceI::setCallBack(int channelId, jssmme::Transport& callback)
    {
        return m_voENetwork->RegisterExternalTransport(channelId, callback);
    }

    int RtpGatewayInterfaceI::connectChannel(int callerChannelId, int calleeChannelId, int callerDtmf, int calleeDtmf)
    {
        TLOGINFO("RtpGateway", std::string(__FUNCTION__) + " called, callerChannelId:" + std::to_string(callerChannelId) + ", calleeChannelId:" + std::to_string(calleeChannelId)
            + ", callerDtmf:" + std::to_string(callerDtmf) + ", calleeDtmf:" + std::to_string(calleeDtmf));

        return m_voEBase->ConnectChannel(callerChannelId, calleeChannelId, callerDtmf, calleeDtmf);
    }

    int RtpGatewayInterfaceI::disconnectChannel(int callerChannelId, int calleeChannelId)
    {
        TLOGINFO("RtpGateway", std::string(__FUNCTION__) + " called, callerChannelId:" + std::to_string(callerChannelId) + ", calleeChannelId:" + std::to_string(calleeChannelId));

        int ret = m_voEBase->DisconnectChannel(callerChannelId, calleeChannelId);
        ret = delChannel(callerChannelId);
        ret = delChannel(calleeChannelId);
        return ret;
    }

    int RtpGatewayInterfaceI::setSendCodec(int channelId, jssmme::CodecInst codec)
    {
        m_voECodec->SetRecPayloadType(channelId, codec);
        return m_voECodec->SetSendCodec(channelId, codec);
    }

    bool RtpGatewayInterfaceI::setSendCodec(int channelId, const std::string& name)
    {
        if (name.empty())
        {
            return false;
        }

        for (int i = m_voECodec->NumOfCodecs() - 1; i >= 0; --i)
        {
            jssmme::CodecInst codec;
            m_voECodec->GetCodec(i, codec);

            std::string localCode = codec.plname;
            int pos = localCode.find(name);
            if (pos >= 0)
            {
                TLOGINFO("RtpGateway", std::string(__FUNCTION__) + "() called, channelId:" + std::to_string(channelId) + ", jssmme local codec.plname:" + localCode);
                m_voECodec->SetRecPayloadType(channelId, codec);
                m_voECodec->SetSendCodec(channelId, codec);
                return true;
            }
        }
    }

    int RtpGatewayInterfaceI::setSendRedCodec(int channelId, int iPayload)
    {
        for (int i = m_voECodec->NumOfCodecs() - 1; i >= 0; --i)
        {
            jssmme::CodecInst codec;
            m_voECodec->GetCodec(i, codec);

            if (strcmp("red",codec.plname) == 0)
            {
                TLOGINFO("RtpGateway", std::string(__FUNCTION__) + "() called, red channelId:" + std::to_string(channelId) + ", code.pcName:" + codec.plname);

                codec.pltype = iPayload;
                return m_voECodec->SetRecPayloadType(channelId, codec);
            }
        }
        return -1;
    }

    int RtpGatewayInterfaceI::setLocalReceiver(int channelId, int port)
    {
        return m_voEBase->SetLocalReceiver(channelId, port);
    }

    int RtpGatewayInterfaceI::setSendDestination(int channel1, int port, const std::string & ip)
    {
        return m_voEBase->SetSendDestination(channel1, port, ip.c_str());
    }

    int RtpGatewayInterfaceI::receivedRTPPacket(int channelId, const unsigned char *msg, int size)
    {
        return m_voENetwork->ReceivedRTPPacket(channelId, msg, size);
    }

    int RtpGatewayInterfaceI::receivedRTCPPacket(int channelId, const unsigned char *msg, int size)
    {
        return m_voENetwork->ReceivedRTCPPacket(channelId, msg, size);
    }

    int RtpGatewayInterfaceI::setFecStatus(int channelId, int red)
    {
        return m_voERtpRtcp->SetFECStatus(channelId, jssmme::kFecAuto, red);
    }

    int RtpGatewayInterfaceI::GetCodecByName(const char *pcName, jssmme::CodecInst *pCodec, int iFreq)
    {
        int i, iCnt = 0;
        iCnt = m_voECodec->NumOfCodecs();
        for (i = 0; i < iCnt; i++)
        {
            if (m_voECodec->GetCodec(i, *pCodec) != 0)
                continue;

            TLOGINFO("RtpGateway", std::string(__FUNCTION__) + "() called, codec.plname:" + pCodec->plname 
                + ", pcName:" + std::string(pcName) + ", codec.plfreq:" + std::to_string(pCodec->plfreq) + ", iFreq:" + std::to_string(iFreq));

            if (strcmp(pcName, pCodec->plname) == 0)
            {
                if (iFreq == -1 || iFreq == pCodec->plfreq)
                    return 0;
            }
        }

        return -1;
    }

    int RtpGatewayInterfaceI::getReceivedRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
        unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms)
    {
        return m_voERtpRtcp->GetReceivedRTCPStatistics(channelId, fraction_lost, cumulative_lost, average_lost, extended_max, jitter, rtt_ms);
    }

    int RtpGatewayInterfaceI::getSentRTCPStatistics(int channelId, unsigned short& fraction_lost, unsigned int& cumulative_lost,
        unsigned int& average_lost, unsigned int& extended_max, unsigned int& jitter, int& rtt_ms)
    {
        return m_voERtpRtcp->GetSentRTCPStatistics(channelId, fraction_lost, cumulative_lost, average_lost, extended_max, jitter, rtt_ms);
    }

    int RtpGatewayInterfaceI::getRTPStatistics(int channelId, unsigned int& averageJitterMs, unsigned int& maxJitterMs,
        unsigned int& discardedPackets)
    {
        return m_voERtpRtcp->GetRTPStatistics(channelId, averageJitterMs, maxJitterMs, discardedPackets);
    }

    int RtpGatewayInterfaceI::getTotalDataCounters(int channelId, unsigned int& bytesSent, unsigned int& packetsSent,
        unsigned int& bytesReceived, unsigned int& packetsReceived)
    {
        return m_voERtpRtcp->GetTotalDataCounters(channelId, bytesSent,packetsSent, bytesReceived, packetsReceived);
    }

    int RtpGatewayInterfaceI::mediaEmptyPktCheck(int channelId, unsigned int &level)
    {
        return m_voEVolumeControl->GetSpeechOutputLevelFullRange(channelId, level);
    }

    bool RtpGatewayInterfaceI::startCallRecord(int channelId, const std::string& path)
    {
        jssmme::CodecInst stCodec;
        int ret = GetCodecByName("PCMA", &stCodec, 8000);
        if (ret < 0)
        {
            TLOGWARN("RtpGateway", std::string(__FUNCTION__) + " called,GetCodecByName failed channelId:" + std::to_string(channelId) + ", ret:" + std::to_string(ret));
            return false;
        }

        ret = m_voEFile->StartRecordingCall(channelId, path.c_str(), &stCodec);
        if (ret < 0)
        {
            TLOGERROR("RtpGateway", std::string(__FUNCTION__) + " called,StartRecordingCall failed channelId:" + std::to_string(channelId) + ", ret:" + std::to_string(ret) + ", path:" + path);
        }

        return ret < 0 ? false : true;
    }

    bool RtpGatewayInterfaceI::stopCallRecord(int channelId)
    {
        return m_voEFile->StopRecordingCall(channelId);
    }

    int RtpGatewayInterfaceI::getChannelCnt()
    {
        return m_channelIdCount;
    }
}