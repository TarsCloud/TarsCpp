#ifndef _RTPGATEWAY_COMMON_H_
#define _RTPGATEWAY_COMMON_H_

namespace RtpGateway
{
#define JSSMME_MEDIA_CHANNEL_RESERVE 40

#define MVC_PCMU_NAME      "PCMU"
#define MVC_GSM_NAME       "GSM"
#define MVC_G723_NAME      "G723"
#define MVC_PCMA_NAME      "PCMA"
#define MVC_G722_NAME      "G722"
#define MVC_AAC_NAME       "AAC"
#define MVC_G7221_NAME     "G7221"
#define MVC_G726_16_NAME   "G726-16"
#define MVC_G726_24_NAME   "G726-24"
#define MVC_G726_32_NAME   "G726-32"
#define MVC_G726_40_NAME   "G726-40"
#define MVC_G728_NAME      "G728"
#define MVC_CN_NAME        "CN"
#define MVC_G729_NAME      "G729"
#define MVC_G729D_NAME     "G729D"
#define MVC_G729E_NAME     "G729E"
#define MVC_ILBC_NAME      "iLBC"
#define MVC_TEL_EVNT_NAME  "telephone-event"
#define MVC_TONE_NAME      "tone"
#define MVC_DIV4_NAME      "DVI4"
#define MVC_AMR_NAME       "AMR"
#define MVC_AMR_WB_NAME    "AMR-WB"
#define MVC_EVRC_NAME      "EVRC"
#define MVC_EVRC0_NAME     "EVRC0"
#define MVC_EVRC1_NAME     "EVRC1"
#define MVC_EVRCB_NAME     "EVRCB"
#define MVC_EVRCB0_NAME    "EVRCB0"
#define MVC_EVRCB1_NAME    "EVRCB1"
#define MVC_ISAC_NAME      "iSAC"
#define MVC_OPUS_NAME      "opus"
#define MVC_PARITYFEC_NAME "parityfec"
#define MVC_ULPFEC_NAME    "ulpfec"
#define MVC_RED_NAME       "red"

    /** @brief default payload type */
    typedef enum EN_DFT_PAYLOAD_TYPE
    {
        EN_DFT_PAYLOAD_PCMU = 0,
        EN_DFT_PAYLOAD_GSM = 3,
        EN_DFT_PAYLOAD_G723 = 4,
        EN_DFT_PAYLOAD_DVI4_8K = 5,
        EN_DFT_PAYLOAD_DVI4_16K = 6,
        EN_DFT_PAYLOAD_LPC = 7,
        EN_DFT_PAYLOAD_PCMA = 8,
        EN_DFT_PAYLOAD_G722 = 9,
        EN_DFT_PAYLOAD_L16_STEREO = 10,
        EN_DFT_PAYLOAD_L16_MONO = 11,
        EN_DFT_PAYLOAD_QCELP = 12,
        EN_DFT_PAYLOAD_CN = 13,
        EN_DFT_PAYLOAD_MPA = 14,
        EN_DFT_PAYLOAD_G728 = 15,
        EN_DFT_PAYLOAD_DVI4_11K = 16,
        EN_DFT_PAYLOAD_DVI4_22K = 17,
        EN_DFT_PAYLOAD_G729 = 18,
        EN_DFT_PAYLOAD_CELB = 25,
        EN_DFT_PAYLOAD_JPEG = 26,
        EN_DFT_PAYLOAD_NV = 28,
        EN_DFT_PAYLOAD_H261 = 31,
        EN_DFT_PAYLOAD_MPV = 32,
        EN_DFT_PAYLOAD_MP2T = 33,
        EN_DFT_PAYLOAD_H263 = 34,

        EN_DFT_PAYLOAD_DYNAMIC = 96,
        EN_DFT_PAYLOAD_AUDIO = EN_DFT_PAYLOAD_DYNAMIC,
        EN_DFT_PAYLOAD_PCMU_WB,
        EN_DFT_PAYLOAD_PCMA_WB,
        EN_DFT_PAYLOAD_TEL_EVNT = 101,
        EN_DFT_PAYLOAD_AMR = 103,
        EN_DFT_PAYLOAD_AMR_WB = 104,
        EN_DFT_PAYLOAD_ILBC = 105,
        EN_DFT_PAYLOAD_AAC = 111,
        EN_DFT_PAYLOAD_AUDIO_RED = 114,
        EN_DFT_PAYLOAD_ISAC,
        EN_DFT_PAYLOAD_OPUS = 116,

        EN_DFT_PAYLOAD_VIDEO = 117,
        EN_DFT_PAYLOAD_VP9 = 118,
        EN_DFT_PAYLOAD_H265 = 120,
        EN_DFT_PAYLOAD_H264 = 121,
        EN_DFT_PAYLOAD_VP8,
        EN_DFT_PAYLOAD_VIDEO_ULPFEC,
        EN_DFT_PAYLOAD_VIDEO_RED,
        EN_DFT_PAYLOAD_SVC,

        EN_DFT_PAYLOAD_KEEPALIVE = 126,

        EN_DFT_PAYLOAD_MAX = 128
    } EN_DFT_PAYLOAD_TYPE;

    /* webrtc voice codec */
    typedef struct ST_SDP_CODEC
    {
        int iPayload;                /**< @brief Rtp payload type EN_DFT_PAYLOAD_TYPE */
        std::string pcName;       /**< @brief codec name */
        int iSplRate;                /**< @brief sample rate (Hz) */
        int iBitsPerSpl;             /**< @brief bits per sample */
        int iChnlNum;                /**< @brief channel number */
        int iPktTime;                /**< @brief packet time */
        int iBitRate;                /**< @brief codec bit rate */
    } ST_SDP_CODEC;

    const ST_SDP_CODEC astSdpCodecTbl[] =
    {
        {EN_DFT_PAYLOAD_PCMU, "PCMU", 8000, 16, 1, 20, 64000},
        {EN_DFT_PAYLOAD_PCMA, "PCMA", 8000, 16, 1, 20, 64000},
        {EN_DFT_PAYLOAD_G729, "G729", 8000, 16, 1, 20, 8000},
        {EN_DFT_PAYLOAD_G722, "G722", 16000, 16, 1, 20, 64000},
        {EN_DFT_PAYLOAD_AAC, "AAC", 32000, 16, 1, 32, 32000},
        {EN_DFT_PAYLOAD_OPUS, "opus", 16000, 16, 1, 60, 16000},
        {EN_DFT_PAYLOAD_AMR, "AMR", 8000, 16, 1, 60, 12200},
        {EN_DFT_PAYLOAD_AMR_WB, "AMR-WB", 16000, 16, 1, 20, 23850},
        {EN_DFT_PAYLOAD_ILBC, "ILBC", 8000, 16, 1, 20, 15200},
        {EN_DFT_PAYLOAD_TEL_EVNT, "telephone-event", 8000, 16, 1, 20, 0},
        {EN_DFT_PAYLOAD_AUDIO_RED, "red", 8000, 16, 1, 0, 0}
    };

    static const std::string RTP_GW_CONTENT_TYPE = "RtpRelayEx";
    static const std::string RTP_GW_MODIFYED_SDP = "ModifyedSdp";
    static const std::string RTP_GW_RTP_GATEWAY_OID = "RtpGwOid";

    static const std::string RTP_GW_START_UP_PARAMS_NO_MEDIA_TIMEOUT = "RtpMediaTimeout";
    static const std::string RTP_GW_START_UP_PARAMS_NO_ACCEPTED_TIMEOUT = "AcceptedTimeout";

    static const std::string CALL_MEDIA_RECORD_DIR = "/home/mediaRecord";
}

#endif
