/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_COMMON_TYPES_H
#define WEBRTC_COMMON_TYPES_H

#include "typedefs.h"

#ifdef WEBRTC_EXPORT
    #define WEBRTC_DLLEXPORT _declspec(dllexport)
#elif WEBRTC_DLL
    #define WEBRTC_DLLEXPORT _declspec(dllimport)
#else
    #define WEBRTC_DLLEXPORT
#endif

#ifndef NULL
    #define NULL 0
#endif

#define SVC_MAX_SPATIAL_LAYER_NUM 4
#define SVC_MAX_TEMPORAL_LAYER_NUM 4
#define RTP_PAYLOAD_NAME_SIZE 32
#define RTP_APPLICATION_DATA_SIZE 15  //subType + data
namespace jssmme {

/* err_type: 1 - standard system error 
 */
typedef void(*pfn_excetion_callback)(const int err_type, const int err_no, const char* err_info);

class InStream
{
public:
    virtual int Read(void *buf,int len) = 0;
    virtual int Rewind() {return -1;}
    virtual ~InStream() {}
protected:
    InStream() {}
};

class OutStream
{
public:
    virtual bool Write(const void *buf,int len) = 0;
    virtual int Rewind() {return -1;}
    virtual ~OutStream() {}
protected:
    OutStream() {}
};

enum TraceModule
{
    // not a module, triggered from the engine code
    kTraceVoice              = 0x0001,
    // not a module, triggered from the engine code
    kTraceVideo              = 0x0002,
    // not a module, triggered from the utility code
    kTraceUtility            = 0x0003,
    kTraceRtpRtcp            = 0x0004,
    kTraceTransport          = 0x0005,
    kTraceSrtp               = 0x0006,
    kTraceAudioCoding        = 0x0007,
    kTraceAudioMixerServer   = 0x0008,
    kTraceAudioMixerClient   = 0x0009,
    kTraceFile               = 0x000a,
    kTraceAudioProcessing    = 0x000b,
    kTraceVideoCoding        = 0x0010,
    kTraceVideoMixer         = 0x0011,
    kTraceAudioDevice        = 0x0012,
    kTraceVideoRenderer      = 0x0014,
    kTraceVideoCapture       = 0x0015,
    kTraceVideoPreocessing   = 0x0016,
    kTraceGeneralProcess     = 0x0017,
};

enum TraceLevel
{
    kTraceNone               = 0x0000,    // no trace
    kTraceStateInfo          = 0x0001,
    kTraceWarning            = 0x0002,
    kTraceError              = 0x0004,
    kTraceCritical           = 0x0008,
    kTraceApiCall            = 0x0010,
#ifdef NDEBUG
    kTraceDefault            = 0x0004,
#else
    kTraceDefault            = 0x00ff,
#endif
    kTraceModuleCall         = 0x0020,
    kTraceMemory             = 0x0100,   // memory info
    kTraceTimer              = 0x0200,   // timing info
    kTraceStream             = 0x0400,   // "continuous" stream of data

    // used for debug purposes
    kTraceDebug              = 0x0800,  // debug
    kTraceInfo               = 0x1000,  // debug info
    kTraceTest               = 0x2000,  // debug for MmePerform Test

    kTraceAll                = 0xffff
};

// External Trace API
class TraceCallback
{
public:
    virtual void Print(const TraceLevel level,
                       const char *traceString,
                       const int length) = 0;
protected:
    virtual ~TraceCallback() {}
    TraceCallback() {}
};

enum FileFormats
{
    kFileFormatWavFile        = 1,
    kFileFormatCompressedFile = 2,
    kFileFormatAviFile        = 3,
    kFileFormatPreencodedFile = 4,
    kFileFormatAAC            = 5,
    kFileFormatPcm16kHzFile   = 7,
    kFileFormatPcm8kHzFile    = 8,
    kFileFormatPcm32kHzFile   = 9,
    kFileFormatMp4File        = 10,
};


enum ProcessingTypes
{
    kPlaybackPerChannel = 0,
    kPlaybackAllChannelsMixed,
    kRecordingPerChannel,
    kRecordingAllChannelsMixed
};

// Encryption enums
enum CipherTypes
{
    kCipherNull               = 0,
    kCipherAes128CounterMode  = 1
};

enum AuthenticationTypes
{
    kAuthNull       = 0,
    kAuthHmacSha1   = 3
};

enum SecurityLevels
{
    kNoProtection                    = 0,
    kEncryption                      = 1,
    kAuthentication                  = 2,
    kEncryptionAndAuthentication     = 3
};

// Interface for encrypting and decrypting regular data and rtp/rtcp packets.
// Implement this interface if you wish to provide an encryption scheme to
// the voice or video engines.
class Encryption
{
public:
    // Encrypt the given data.
    //
    // Args:
    //   channel: The channel to encrypt data for.
    //   in_data: The data to encrypt. This data is bytes_in bytes long.
    //   out_data: The buffer to write the encrypted data to. You may write more
    //       bytes of encrypted data than what you got as input, up to a maximum
    //       of jssmme::kViEMaxMtu if you are encrypting in the video engine, or
    //       jssmme::kVoiceEngineMaxIpPacketSizeBytes for the voice engine.
    //   bytes_in: The number of bytes in the input buffer.
    //   bytes_out: The number of bytes written in out_data.
    virtual void encrypt(
        int channel,
        unsigned char* in_data,
        unsigned char* out_data,
        int bytes_in,
        int* bytes_out) = 0;

    // Decrypts the given data. This should reverse the effects of encrypt().
    //
    // Args:
    //   channel_no: The channel to decrypt data for.
    //   in_data: The data to decrypt. This data is bytes_in bytes long.
    //   out_data: The buffer to write the decrypted data to. You may write more
    //       bytes of decrypted data than what you got as input, up to a maximum
    //       of jssmme::kViEMaxMtu if you are encrypting in the video engine, or
    //       jssmme::kVoiceEngineMaxIpPacketSizeBytes for the voice engine.
    //   bytes_in: The number of bytes in the input buffer.
    //   bytes_out: The number of bytes written in out_data.
    virtual void decrypt(
        int channel,
        unsigned char* in_data,
        unsigned char* out_data,
        int bytes_in,
        int* bytes_out,
        const char* fromIP,
        const WebRtc_UWord16 fromPort) = 0;

    // Encrypts a RTCP packet. Otherwise, this method has the same contract as
    // encrypt().
    virtual void encrypt_rtcp(
        int channel,
        unsigned char* in_data,
        unsigned char* out_data,
        int bytes_in,
        int* bytes_out) = 0;

    // Decrypts a RTCP packet. Otherwise, this method has the same contract as
    // decrypt().
    virtual void decrypt_rtcp(
        int channel,
        unsigned char* in_data,
        unsigned char* out_data,
        int bytes_in,
        int* bytes_out,
        const char* fromIP,
        const WebRtc_UWord16 fromPort) = 0;

protected:
    virtual ~Encryption() {}
    Encryption() {}
};

// External transport callback interface
class Transport
{
public:
    virtual int SendPacket(int channel, const void *data, int len, unsigned int level, unsigned int timestamp) = 0;
    virtual int SendRTCPPacket(int channel, const void *data, int len) = 0;
    virtual bool SendingMedia(int channel) { return true;}
protected:
    virtual ~Transport() {}
    Transport() {}
};

// ==================================================================
// Voice specific types
// ==================================================================

// Each codec supported can be described by this structure.
struct CodecInst
{
    int pltype;
    char plname[RTP_PAYLOAD_NAME_SIZE];
    int plfreq;
    int pacsize;
    int channels;
    int rate;
    int bufLen;
    unsigned char cookie[128];
};

enum FrameType
{
    kFrameEmpty            = 0,
    kAudioFrameSpeech      = 1,
    kAudioFrameCN          = 2,
    kVideoFrameKey         = 3,    // independent frame
    kVideoFrameDelta       = 4,    // depends on the previus frame
    kVideoFrameGolden      = 5,    // depends on a old known previus frame
    kVideoFrameAltRef      = 6
};

// RTP
enum {kRtpCsrcSize = 15}; // RFC 3550 page 13

enum RTPDirections
{
    kRtpIncoming = 0,
    kRtpOutgoing
};

enum PayloadFrequencies
{
    kFreq8000Hz = 8000,
    kFreq16000Hz = 16000,
    kFreq32000Hz = 32000
};

enum VadModes                 // degree of bandwidth reduction
{
    kVadConventional = 0,      // lowest reduction
    kVadAggressiveLow,
    kVadAggressiveMid,
    kVadAggressiveHigh         // highest reduction
};

struct NetworkStatistics           // NETEQ statistics
{
    // current jitter buffer size in ms
    WebRtc_UWord16 currentBufferSize;
    // preferred (optimal) buffer size in ms
    WebRtc_UWord16 preferredBufferSize;
    // adding extra delay due to "peaky jitter"
    bool jitterPeaksFound;
    // loss rate (network + late) in percent (in Q14)
    WebRtc_UWord16 currentPacketLossRate;
    // late loss rate in percent (in Q14)
    WebRtc_UWord16 currentDiscardRate;
    // fraction (of original stream) of synthesized speech inserted through
    // expansion (in Q14)
    WebRtc_UWord16 currentExpandRate;
    // fraction of synthesized speech inserted through pre-emptive expansion
    // (in Q14)
    WebRtc_UWord16 currentPreemptiveRate;
    // fraction of data removed through acceleration (in Q14)
    WebRtc_UWord16 currentAccelerateRate;
    // clock-drift in parts-per-million (negative or positive)
    int32_t clockDriftPPM;
    // average packet waiting time in the jitter buffer (ms)
    int meanWaitingTimeMs;
    // median packet waiting time in the jitter buffer (ms)
    int medianWaitingTimeMs;
    // min packet waiting time in the jitter buffer (ms)
    int minWaitingTimeMs;
    // max packet waiting time in the jitter buffer (ms)
    int maxWaitingTimeMs;
};

typedef struct
{
    int min;              // minumum
    int max;              // maximum
    int average;          // average
} StatVal;

typedef struct           // All levels are reported in dBm0
{
    StatVal speech_rx;   // long-term speech levels on receiving side
    StatVal speech_tx;   // long-term speech levels on transmitting side
    StatVal noise_rx;    // long-term noise/silence levels on receiving side
    StatVal noise_tx;    // long-term noise/silence levels on transmitting side
} LevelStatistics;

typedef struct        // All levels are reported in dB
{
    StatVal erl;      // Echo Return Loss
    StatVal erle;     // Echo Return Loss Enhancement
    StatVal rerl;     // RERL = ERL + ERLE
    // Echo suppression inside EC at the point just before its NLP
    StatVal a_nlp;
} EchoStatistics;

enum TelephoneEventDetectionMethods
{
    kInBand = 0,
    kOutOfBand = 1,
    kInAndOutOfBand = 2
};

enum NsModes    // type of Noise Suppression
{
    kNsUnchanged = 0,   // previously set mode
    kNsDefault,         // platform default
    kNsConference,      // conferencing default
    kNsLowSuppression,  // lowest suppression
    kNsModerateSuppression,
    kNsHighSuppression,
    kNsVeryHighSuppression,     // highest suppression
};

enum AgcModes                  // type of Automatic Gain Control
{
    kAgcUnchanged = 0,        // previously set mode
    kAgcDefault,              // platform default
    // adaptive mode for use when analog volume control exists (e.g. for
    // PC softphone)
    kAgcAdaptiveAnalog,
    // scaling takes place in the digital domain (e.g. for conference servers
    // and embedded devices)
    kAgcAdaptiveDigital,
    // can be used on embedded devices where the capture signal level
    // is predictable
    kAgcFixedDigital
};

// EC modes
enum EcModes                   // type of Echo Control
{
    kEcUnchanged = 0,          // previously set mode
    kEcDefault,                // platform default
    kEcConference,             // conferencing default (aggressive AEC)
    kEcAec,                    // Acoustic Echo Cancellation
    kEcAecm,                   // AEC mobile
    kEcAeci,                   // Acoustic Echo Cancellation sound card independent
    kEcAecd,                   // AEC based on delay-log
    kEcAes,                    // simple echo suppress
    kEcfraunhofer,             //  Fraunhofer IIS echo Cancellation
};

enum EcInitSuppMode
{
    kEcInitSuppNone = 0,
    kEcInitSuppWeak,
    kEcInitSuppStrong,
};

enum EcSelfCancellationMode {
    kEcSelfCancellationClose = 0,
    kEcSelfCancellationAuto,
    kEcSelfCancellationOpen
};

enum EcSingletalkSuppMode {
    kEcSingletalkSuppClose = 0,
    kEcSingletalkSuppAuto,
    kEcSingletalkSuppOpen
};

enum EcNlpSuppLevel {
    kEcLowSuppression = 0,
    kEcModerateSuppression,
    kEcHighSuppression,
    kEcOverModerateSuppression,
    kEcOverHighSuppression
};

// AECM modes
enum AecmModes                 // mode of AECM
{
    kAecmQuietEarpieceOrHeadset = 0,
                               // Quiet earpiece or headset use
    kAecmEarpiece,             // most earpiece use
    kAecmLoudEarpiece,         // Loud earpiece or quiet speakerphone use
    kAecmSpeakerphone,         // most speakerphone use (default)
    kAecmLoudSpeakerphone      // Loud speakerphone
};

// AGC configuration
typedef struct
{
    unsigned short targetLeveldBOv;
    unsigned short digitalCompressionGaindB;
    bool           limiterEnable;
} AgcConfig;                  // AGC configuration parameters

enum StereoChannel
{
    kStereoLeft = 0,
    kStereoRight,
    kStereoBoth
};

// Audio device layers
enum AudioLayers
{
    kAudioPlatformDefault = 0,
    kAudioWindowsWave = 1,
    kAudioWindowsCore = 2,
    kAudioLinuxAlsa = 3,
    kAudioLinuxPulse = 4
};

enum NetEqModes             // NetEQ playout configurations
{
    // Optimized trade-off between low delay and jitter robustness for two-way
    // communication.
    kNetEqDefault = 0,
    // Improved jitter robustness at the cost of increased delay. Can be
    // used in one-way communication.
    kNetEqStreaming = 1,
    // Optimzed for decodability of fax signals rather than for perceived audio
    // quality.
    kNetEqFax = 2,
};

enum NetEqBgnModes          // NetEQ Background Noise (BGN) configurations
{
    // BGN is always on and will be generated when the incoming RTP stream
    // stops (default).
    kBgnOn = 0,
    // The BGN is faded to zero (complete silence) after a few seconds.
    kBgnFade = 1,
    // BGN is not used at all. Silence is produced after speech extrapolation
    // has faded.
    kBgnOff = 2,
};

enum OnHoldModes            // On Hold direction
{
    kHoldSendAndPlay = 0,    // Put both sending and playing in on-hold state.
    kHoldSendOnly,           // Put only sending in on-hold state.
    kHoldPlayOnly            // Put only playing in on-hold state.
};

enum AmrMode
{
    kRfc3267BwEfficient = 0,
    kRfc3267OctetAligned = 1,
    kRfc3267FileStorage = 2,
};

enum OpusMode
{
    kOpusSilkOnly = 0,
    kOpusHybrid = 1,
};

enum FecMode
{
    kFecOff = 0,          //disable fec
    kFecAuto,         //configure fec automatically according to network
    kFecProbe        //when network bw is allowed, use fec data to probe bw
};

// ==================================================================
// Video specific types
// ==================================================================

// Raw video types
enum RawVideoType
{
    kVideoI420     = 0,
    kVideoYV12     = 1,
    kVideoYUY2     = 2,
    kVideoUYVY     = 3,
    kVideoIYUV     = 4,
    kVideoARGB     = 5,
    kVideoRGB24    = 6,
    kVideoRGB565   = 7,
    kVideoARGB4444 = 8,
    kVideoARGB1555 = 9,
    kVideoMJPEG    = 10,
    kVideoNV12     = 11,
    kVideoNV21     = 12,
    kVideoBGRA     = 13,
    kVideoUnknown  = 99
};

// Video codec
enum { kConfigParameterSize = 128};
enum { kPayloadNameSize = 32};
enum { kMaxSimulcastStreams = 4};
enum { kMaxTemporalStreams = 4};

enum VideoCodecComplexity
{
    kComplexityNormal = 0,
    kComplexityHigh    = 1,
    kComplexityHigher  = 2,
    kComplexityMax     = 3
};

enum VideoCodecProfile
{
    kProfileBase = 0x00,
    kProfileMain = 0x01
};

// H.264 specific
enum H264Packetization
{
    kH264NonInterleavedMode,
    kH264InterleavedMode
};


// The packetization types that we support: single, aggregated, and fragmented.
enum H264PacketizationTypes {
  kH264SingleNalu,  // This packet contains a single NAL unit.
  kH264StapA,       // This packet contains STAP-A (single time
                    // aggregation) packets. If this packet has an
                    // associated NAL unit type, it'll be for the
                    // first such aggregated packet.
  kH264FuA,         // This packet contains a FU-A (fragmentation
                    // unit) packet, meaning it is a part of a frame
                    // that was too large to fit into a single packet.
};

enum H264Profile
{
    kH264ProfileBase = 0x42,
    kH264ProfileMain = 0x4D,
    kH264ProfileExtend = 0x58,
    kH264ProfileHigh = 0x64,
    kH264ProfileSvcBase,
    kH264ProfileSvcHigh,
};

// level of h264 ref H.264-201003-I
enum H264Level
{
    kH264Level_1  = 0x0A,
    kH264Level_1b = 0x09,
    kH264Level_1_1= 0x0B,
    kH264Level_1_2= 0x0C,
    kH264Level_1_3= 0x0D,
    kH264Level_2  = 0x14,
    kH264Level_2_1= 0x15,
    kH264Level_2_2= 0x16,
    kH264Level_3  = 0x1E,
    kH264Level_3_1= 0x1F,
    kH264Level_3_2= 0x20,
    kH264Level_4  = 0x28,
    kH264Level_4_1= 0x29,
    kH264Level_4_2= 0x2A,
    kH264Level_5  = 0x32,
    kH264Level_5_1= 0x33,
};

//H264 SVC spatial layer config
typedef struct {
  int	iVideoWidth;           ///< width of picture in luminance samples of a layer
  int	iVideoHeight;          ///< height of picture in luminance samples of a layer
  float	fFrameRate;            ///< frame rate specified for a layer
  int   iSpatialBitrate;        ///< target bitrate specified for a layer
}SvcLayerConfig;

struct VideoCodecSVC
{
    H264Profile         profile;
    H264Level           level;
    H264PacketizationTypes   packetization;
    bool                errorConcealOn;
    bool                smallNalu;
    bool                feedbackModeOn;
    bool                dropFrame;
    bool                constSpsPpsId;
    bool                simulcast;
    unsigned char       numberOfTemporalLayers;
    unsigned char       numberOfSpatialLayers;
    SvcLayerConfig      layers[SVC_MAX_SPATIAL_LAYER_NUM];
};

enum VP8ResilienceMode {
  kResilienceOff,    // The stream produced by the encoder requires a
                     // recovery frame (typically a key frame) to be
                     // decodable after a packet loss.
  kResilientStream,  // A stream produced by the encoder is resilient to
                     // packet losses, but packets within a frame subsequent
                     // to a loss can't be decoded.
  kResilientFrames   // Same as kResilientStream but with added resilience
                     // within a frame.
};

// VP8 specific
struct VideoCodecVP8
{
    bool                 pictureLossIndicationOn;
    bool                 feedbackModeOn;
    VideoCodecComplexity complexity;
    VP8ResilienceMode    resilience;
    unsigned char        numberOfTemporalLayers;
    bool                 errorConcealOn;
};

// VP9 specific
struct VideoCodecVP9
{
    bool                 dropFrame;
    bool                 autoResize;
};

// Unknown specific
struct VideoCodecGeneric
{
};

// Video codec types
enum VideoCodecType
{
    kVideoCodecH263,
    kVideoCodecH263_1998,
    kVideoCodecH264,
    kVideoCodecSVC,
    kVideoCodecVP8,
    kVideoCodecH265,
    kVideoCodecVP9,
    kVideoCodecAV1,
    kVideoCodecMJPG,
    kVideoCodecI420,
    kVideoCodecRED,
    kVideoCodecULPFEC,
    kVideoCodecUnknown
};

//Encode qp mode: support h264 and vp8
// H264 full 0-51; save 24-51
// vp8 full 2-56; save 24-56
enum VideoCodecQpMode
{ kCodecQpModeFull ,
  kCodecQpModeSave ,
  kCodecQpModeReduceBr,
  kCodecQpModeReduceBrEx
};

union VideoCodecUnion
{
    VideoCodecSVC       H264;
    VideoCodecSVC       SVC;
    VideoCodecVP8       VP8;
    VideoCodecVP9       VP9;
    VideoCodecSVC       H265;
    VideoCodecSVC		AV1;
    VideoCodecGeneric   Generic;
};

// Simulcast is when the same stream is encoded multiple times with different
// settings such as resolution.
struct SimulcastStream
{
    unsigned short      width;
    unsigned short      height;
    unsigned char       numberOfTemporalLayers;
    unsigned int        maxBitrate;
    unsigned int        qpMax; // minimum quality
};

enum VideoCodecMode {
  kRealtimeVideo,       /**< @brief Real time video. */
  kScreensharing,       /**< @brief Screen sharing. */
  kMergeScreen,         /**< @brief tiled, all in screen, need merge image. */
  kMergePlatform,       /**< @brief Screen sharing with small camera video, need merge image. */
  kMergeSpeaker,        /**< @brief Screen sharing with speaker's camera video, need merge image. */
  kMergeLayout,         /**< @brief use ViELayout, full_content */
  kIntellegenceLayout,         /**< @brief Intellegence Layout */
  kMergeLayout_Screen   /**< @brief use ViELayout, full_screen */
};

enum VideoIntellegenceMode {
    kIntellegenceFreeLayout,           /**< @brief Free layout */
    kIntellegenceRectLayout,           /**< @brief Screen sharing */
    kIntellegenceBigSmallX2,           /**< @brief Big small zoom 2 times*/
    kIntellegenceBigSmallX3,           /**< @brief Big small zoom 3 times*/
    kIntellegenceBigSmallX4,           /**< @brief Big small zoom 4 times*/
    kIntellegenceBigSmallX5,           /**< @brief Big small zoom 5 times*/
    kIntellegenceBigSmallX6,           /**< @brief Big small zoom 6 times*/
    kIntellegenceBigSmallX7,           /**< @brief Big small zoom 7 times*/
    kIntellegenceBigSmallX8,           /**< @brief Big small zoom 8 times*/
    kIntellegenceBigSmallX9,           /**< @brief Big small zoom 9 times*/
    kIntellegenceBigSmallX10,          /**< @brief Big small zoom 10 times*/
    kIntellegenceBigSmallTOP,          /**< @brief Big small like Platform*/
    kIntellegenceBigSmallBOTTOM,       /**< @brief Big small like Platform*/
    kIntellegenceDivideLayout,         /**< @brief Divide layout */
    kIntellegenceDivideLayoutNoAngle   /**< @brief Divide layout without image angle*/
};  

enum VideotIntellegenceScreenshareMode {
    kIntellegenceScreenshare,         /**< @brief Screen share */
    kIntellegencePlatform,            /**< @brief Screen sharing with small camera video, need merge image. */
    kIntellegencePlatformCover,       /**< @brief Screen sharing cover with small camera video, need merge image. */
    kIntellegenceSpeaker,             /**< @brief Screen sharing with speaker's camera video, need merge image. */
    kIntellegenceWithoutScreenshare,  /**< @brief all video without screenshare in mode what VideoIntellegenceMode, need merge image. */
    kIntellegenceSameAsNoShare        /**< @brief Screen sharing and all video layout same as no share in what VideoIntellegenceMode right now, need merge image. */
};


// Common video codec properties
struct VideoCodec
{
    VideoCodecType      codecType;
    char                plName[kPayloadNameSize];
    unsigned char       plType;

    unsigned short      width;
    unsigned short      height;

    unsigned int        startBitrate; //kbps
    unsigned int        maxBitrate;
    unsigned int        minBitrate;
    unsigned char       maxFramerate;

    VideoCodecUnion     codecSpecific;

    unsigned int        qpMax;
    unsigned int        qpMin;
    bool                keepAspect;
    unsigned char       numberOfSimulcastStreams;
    SimulcastStream     simulcastStream[kMaxSimulcastStreams];

    VideoCodecMode      mode;
    bool                doCalculatePSNR;
};

//network warning route direction
enum NetworkDirection {
    kDirectionSend = 1,
    kDirectionReceive = 2,
    kDirectionBoth = 3,
};

//network bw status
enum NetwBandwidthStatus {
    kNetworkBwUnknown = 0,
    kNetworkBwVeryLow,
    kNetworkBwLow,
    kNetworkBwNormal,
    kNetworkBwGood,
    kNetworkBwVeryGood,
};

enum ImageFileFormat {
    kImageJpeg = 0,
    kImageH264,
    kImageI420
};

/** @brief Type of video content. */
enum VideoContentType
{
    kVideoContentAlone = 0,     /**< @brief not merge this data. */
    kVideoContentCamera,        /**< @brief Camera Data. */
    kVideoContentScreen,        /**< @brief Screen Data. */
    kVideoContentSharer,        /**< @brief Camera Data from screen sharer. */
};

// Identifiers for preencoded files.
enum MediaFileUtility_CodecType
{
    kCodecNoCodec  = 0,
    kCodecIsac,
    kCodecIsacSwb,
    kCodecIsacLc,
    kCodecL16_8Khz,
    kCodecL16_16kHz,
    kCodecL16_32Khz,
    kCodecPcmu,
    kCodecPcma,
    kCodecIlbc20Ms,
    kCodecIlbc30Ms,
    kCodecG722,
    kCodecG722_1_32Kbps,
    kCodecG722_1_24Kbps,
    kCodecG722_1_16Kbps,
    kCodecG722_1c_48,
    kCodecG722_1c_32,
    kCodecG722_1c_24,
    kCodecAmr,
    kCodecAmrWb,
    kCodecG729,
    kCodecG729_1,
    kCodecG726_40,
    kCodecG726_32,
    kCodecG726_24,
    kCodecG726_16,
    kCodecSpeex8Khz,
    kCodecSpeex16Khz,
    kCodecOpus,
    kCodecAAC
};

enum WaveFormats
{
    kWaveFormatPcm   = 0x0001,
    kWaveFormatALaw  = 0x0006,
    kWaveFormatMuLaw = 0x0007,
    kWaveFormatAAC   = 0x1000,
    kWaveFormatRawAAC= 0x00FF,
};

// Thresholds for hybrid NACK/FEC
// common to media optimization and the jitter buffer.
enum HybridNackTH {
    kHighRttNackMs = 300, //100
    kLowRttNackMs = 30      //20
};
}  // namespace jssmme
#endif  // WEBRTC_COMMON_TYPES_H
