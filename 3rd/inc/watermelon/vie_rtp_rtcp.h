/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This sub-API supports the following functionalities:
//  - Callbacks for RTP and RTCP events such as modified SSRC or CSRC.
//  - SSRC handling.
//  - Transmission of RTCP reports.
//  - Obtaining RTCP data from incoming RTCP sender reports.
//  - RTP and RTCP statistics (jitter, packet loss, RTT etc.).
//  - Forward Error Correction (FEC).
//  - RTP Keep‐alive for maintaining the NAT mappings associated to RTP flows.
//  - Writing RTP and RTCP packets to binary files for off‐line analysis of the
//    call quality.
//  - Inserting extra RTP packets into active audio stream.

#ifndef WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_RTP_RTCP_H_
#define WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_RTP_RTCP_H_

#include "common_types.h"

namespace jssmme {

class VideoEngine;

// This enumerator sets the RTCP mode.
enum ViERTCPMode {
  kRtcpNone = 0,
  kRtcpCompound_RFC4585 = 1,
  kRtcpNonCompound_RFC5506 = 2
};

// This enumerator describes the key frame request mode.
enum ViEKeyFrameRequestMethod {
  kViEKeyFrameRequestNone = 0,
  kViEKeyFrameRequestPliRtcp = 1,
  kViEKeyFrameRequestFirRtp = 2,
  kViEKeyFrameRequestFirRtcp = 3
};

enum ViEStreamType {
  kViEStreamTypeNormal = 0,  // Normal media stream
  kViEStreamTypeRtx = 1,  // Retransmission media stream
  kVieStreamTypeFec = 2,  // fec stream protecting media
  kVieStreamTypeFecRtx = 3 // retransmission fec stream
};

// This enumerator describes the rtp header extension mode.
enum ViERTPExtensionType {
  kVieRtpExtensionTransmissionTimeOffset = 0,
  kVieRtpExtensionAudioLevel = 1,
  kVieRtpExtensionEncodeRotateAngle = 2,
  kVieRtpExtensionCoordinationVideoOrientation = 3,
  kVieRtpExtensionApplicationData = 4,
  kVieRtpExtensionPictureId = 5,
  kVieRtpExtensionTransportTime = 6,
  kVieRtpExtensionPadding = 7,
};

// This class declares an abstract interface for a user defined observer. It is
// up to the VideoEngine user to implement a derived class which implements the
// observer class. The observer is registered using RegisterRTPObserver() and
// deregistered using DeregisterRTPObserver().
class WEBRTC_DLLEXPORT ViERTPObserver {
 public:
  // This method is called if SSRC of the incoming stream is changed.
  virtual void IncomingSSRCChanged(const int video_channel,
                                   const unsigned int SSRC) = 0;

  // This method is called if a field in CSRC changes or if the number of
  // CSRCs changes.
  virtual void IncomingCSRCChanged(const int video_channel,
                                   const unsigned int CSRC,
                                   const bool added) = 0;
  virtual void OnApplicationDataReceived(const WebRtc_Word32 video_channel,
		                                 const WebRtc_UWord8 subType,
		                                 const WebRtc_UWord8 *puData,
		                                 const WebRtc_UWord8 dataLength,
                                         const WebRtc_UWord32 timeStampMs) = 0;

 protected:
  virtual ~ViERTPObserver() {}
};

// This class declares an abstract interface for a user defined observer. It is
// up to the VideoEngine user to implement a derived class which implements the
// observer class. The observer is registered using RegisterRTCPObserver() and
// deregistered using DeregisterRTCPObserver().

class WEBRTC_DLLEXPORT ViERTCPObserver {
 public:
  // This method is called if a application-defined RTCP packet has been
  // received.
  virtual void OnApplicationDataReceived(
      const int video_channel,
      const unsigned char sub_type,
      const unsigned int name,
      const char* data,
      const unsigned short data_length_in_bytes) = 0;
  virtual void OnSDESNameReceived(
      const int video_channel,
      const unsigned char* name,
      const unsigned char length) = 0;
 protected:
  virtual ~ViERTCPObserver() {}
};

class WEBRTC_DLLEXPORT ViERTP_RTCP {
 public:
  enum { KDefaultDeltaTransmitTimeSeconds = 15 };
  enum { KMaxRTCPCNameLength = 256 };

  // Factory for the ViERTP_RTCP sub‐API and increases an internal reference
  // counter if successful. Returns NULL if the API is not supported or if
  // construction fails.
  static ViERTP_RTCP* GetInterface(VideoEngine* video_engine);

  // Releases the ViERTP_RTCP sub-API and decreases an internal reference
  // counter. Returns the new reference count. This value should be zero
  // for all sub-API:s before the VideoEngine object can be safely deleted.
  virtual int Release() = 0;

  // This function enables you to specify the RTP synchronization source
  // identifier (SSRC) explicitly.
  virtual int SetLocalSSRC(const int video_channel,
                           const unsigned int SSRC,
                           const ViEStreamType usage = kViEStreamTypeNormal,
                           const unsigned char simulcast_idx = 0) = 0;

  // This function gets the SSRC for the outgoing RTP stream for the specified
  // channel.
  virtual int GetLocalSSRC(const int video_channel,
                           unsigned int& SSRC) const = 0;

  // This function map a incoming SSRC to a StreamType so that the engine
  // can know which is the normal stream and which is the RTX
  virtual int SetRemoteSSRCType(const int video_channel,
                                const ViEStreamType usage,
                                const unsigned int SSRC) const = 0;

  // This function gets the SSRC for the incoming RTP stream for the specified
  // channel.
  virtual int GetRemoteSSRC(const int video_channel,
                            unsigned int& SSRC) const = 0;

  // This function returns the CSRCs of the incoming RTP packets.
  virtual int GetRemoteCSRCs(const int video_channel,
                             unsigned int CSRCs[kRtpCsrcSize]) const = 0;

  // This function enables manual initialization of the sequence number. The
  // start sequence number is normally a random number.
  virtual int SetStartSequenceNumber(const int video_channel, 
                                     unsigned short sequence_number) = 0;

  virtual int SetRTXReceiveStatus(const int video_channel,
                                  const bool enable) = 0;
  virtual int SetRTXReceivePayloadType(const int video_channel,
                                       const char rtxPayloadType,
                                       const char associatedPayloadType) = 0;
  virtual int SetRTXSendStatus(const int video_channel,
                               const bool enable) = 0;
  virtual int SetRTXSendPayloadType(const int video_channel,
                                    const char rtxPayloadType,
                                    const char associatedPayloadType) = 0;

  // This function sets the RTCP status for the specified channel.
  // Default mode is kRtcpCompound_RFC4585.
  virtual int SetRTCPStatus(const int video_channel,
                            const ViERTCPMode rtcp_mode) = 0;

  // This function gets the RTCP status for the specified channel.
  virtual int GetRTCPStatus(const int video_channel,
                            ViERTCPMode& rtcp_mode) const = 0;

  // This function sets the RTCP canonical name (CNAME) for the RTCP reports
  // on a specific channel.
  virtual int SetRTCPCName(const int video_channel,
                           const char rtcp_cname[KMaxRTCPCNameLength]) = 0;

  // This function gets the RTCP canonical name (CNAME) for the RTCP reports
  // sent the specified channel.
  virtual int GetRTCPCName(const int video_channel,
                           char rtcp_cname[KMaxRTCPCNameLength]) const = 0;
  
  virtual int SetRTCPName(const int video_channel,
                           const unsigned char* name,
                           unsigned char len) = 0;

  // This function gets the RTCP canonical name (CNAME) for the RTCP reports
  // received on the specified channel.
  virtual int GetRemoteRTCPCName(
      const int video_channel,
      char rtcp_cname[KMaxRTCPCNameLength]) const = 0;

  // This function sends an RTCP APP packet on a specific channel.
  virtual int SendApplicationDefinedRTCPPacket(
      const int video_channel,
      const unsigned char sub_type,
      unsigned int name,
      const char* data,
      unsigned short data_length_in_bytes) = 0;
  

  // This function sends an RTP APP packet on a specific channel.
  // dataLength less than 16
  virtual int SendApplicationDefinedRTPExtension(
      const int video_channel,
      const unsigned char subType,
      const unsigned char* pudata,
      const unsigned char dataLength,
      const unsigned char counter) = 0;

  //record the reference tmmbr values to the log.
  virtual bool SetRTCPRefTmmbrToLog(const int video_channel, WebRtc_Word32* piTmmbr) = 0;

  // This function enables Negative Acknowledgment (NACK) using RTCP,
  // implemented based on RFC 4585. NACK retransmits RTP packets if lost on
  // the network. This creates a lossless transport at the expense of delay.
  // If using NACK, NACK should be enabled on both endpoints in a call.
  virtual int SetNACKStatus(const int video_channel, const bool enable) = 0;

  // This function enables Forward Error Correction (FEC) using RTCP,
  // implemented based on RFC 5109, to improve packet loss robustness. Extra
  // FEC packets are sent together with the usual media packets, hence
  // part of the bitrate will be used for FEC packets.
  virtual int SetFECStatus(const int video_channel,
                           const bool enable,
                           const unsigned char payload_typeRED,
                           const unsigned char payload_typeFEC) = 0;

  // This function enables hybrid Negative Acknowledgment using RTCP
  // and Forward Error Correction (FEC) implemented based on RFC 5109,
  // to improve packet loss robustness. Extra
  // FEC packets are sent together with the usual media packets, hence will
  // part of the bitrate be used for FEC packets.
  // The hybrid mode will choose between nack only, fec only and both based on
  // network conditions. When both are applied, only packets that were not
  // recovered by the FEC will be nacked.
  virtual int SetHybridNACKFECStatus(const int video_channel,
                                     const bool enable,
                                     const unsigned char payload_typeRED,
                                     const unsigned char payload_typeFEC,
                                     int lowRtt = kLowRttNackMs, int highRtt = kHighRttNackMs) = 0;

  // This function enables FEC protection with fixed protection factor.
  virtual  int SetFECFixedProtection(const int videoChannel,
                                     const bool bEnable,
                                     const unsigned char ucFactor) = 0;

  // This function enables RTCP key frame requests.
  virtual int SetKeyFrameRequestMethod(
    const int video_channel, const ViEKeyFrameRequestMethod method) = 0;

  //
  virtual int RequestKeyFrame(const int video_channel) = 0;

  // This function enables signaling of temporary bitrate constraints using
  // RTCP, implemented based on RFC4585.
  virtual int SetTMMBRStatus(const int video_channel, const bool enable) = 0;

  // Enables and disables REMB packets for this channel. |sender| indicates
  // this channel is encoding, |receiver| tells the bitrate estimate for
  // this channel should be included in the REMB packet.
  virtual int SetRembStatus(int video_channel,
                            bool sender,
                            bool receiver) = 0;

  // This function returns our locally created statistics of the received RTP
  // stream.
  virtual int GetReceivedRTCPStatistics(const int video_channel,
                                        unsigned short& fraction_lost,
                                        unsigned int& cumulative_lost,
                                        unsigned int& average_lost,
                                        unsigned int& extended_max,
                                        unsigned int& jitter,
                                        int& rtt_ms) const = 0;

  // This function returns statistics reported by the remote client in a RTCP
  // packet.
  virtual int GetSentRTCPStatistics(const int video_channel,
                                    unsigned short& fraction_lost,
                                    unsigned int& cumulative_lost,
                                    unsigned int& average_lost,
                                    unsigned int& extended_max,
                                    unsigned int& jitter,
                                    int& rtt_ms) const = 0;

  virtual int SetExternalEstimatedRtt(const int video_channel, const int iRtt)=0;

  // The function gets statistics from the sent and received RTP streams.
  virtual int GetRTPStatistics(const int video_channel,
                               unsigned int& bytes_sent,
                               unsigned int& packets_sent,
                               unsigned int& bytes_received,
                               unsigned int& packets_received) const = 0;

  // The function gets statistics from the sent and received RTP streams.
  virtual int GetTotalDataCounters(const int video_channel,
                               unsigned int& bytes_sent,
                               unsigned int& packets_sent,
                               unsigned int& bytes_received,
                               unsigned int& packets_received) const = 0;

  // The function gets bandwidth usage statistics from the sent RTP streams in
  // bits/s.
  virtual int GetBandwidthUsage(const int video_channel,
                                unsigned int& total_bitrate_sent,
                                unsigned int& video_bitrate_sent,
                                unsigned int& fec_bitrate_sent,
                                unsigned int& nackBitrateSent) const = 0;

  // This function gets the send-side estimated bandwidth available for video,
  // including overhead, in bits/s.
  virtual int GetEstimatedSendBandwidth(
      const int video_channel,
      unsigned int* estimated_bandwidth) const = 0;

  // This function gets the receive-side estimated bandwidth available for
  // video, including overhead, in bits/s.
  // Returns -1 when no valid estimate is available.
  virtual int GetEstimatedReceiveBandwidth(
      const int video_channel,
      unsigned int* estimated_bandwidth) const = 0;

  // This function enables or disables an RTP keep-alive mechanism which can
  // be used to maintain an existing Network Address Translator (NAT) mapping
  // while regular RTP is no longer transmitted.
  virtual int SetRTPKeepAliveStatus(
      const int video_channel,
      bool enable,
      const int unknown_payload_type,
      const unsigned int delta_transmit_time_seconds =
          KDefaultDeltaTransmitTimeSeconds) = 0;

  // This function gets the RTP keep-alive status.
  virtual int GetRTPKeepAliveStatus(
      const int video_channel,
      bool& enabled,
      int& unkown_payload_type,
      unsigned int& delta_transmit_time_seconds) const = 0;

  // This function enables capturing of RTP packets to a binary file on a
  // specific channel and for a given direction. The file can later be
  // replayed using e.g. RTP Tools rtpplay since the binary file format is
  // compatible with the rtpdump format.
  virtual int StartRTPDump(const int video_channel,
                           const char file_nameUTF8[1024],
                           RTPDirections direction) = 0;

  // This function disables capturing of RTP packets to a binary file on a
  // specific channel and for a given direction.
  virtual int StopRTPDump(const int video_channel,
                          RTPDirections direction) = 0;

  // Registers an instance of a user implementation of the ViERTPObserver.
  virtual int RegisterRTPObserver(const int video_channel,
                                  ViERTPObserver& observer) = 0;

  // Removes a registered instance of ViERTPObserver.
  virtual int DeregisterRTPObserver(const int video_channel) = 0;

  // Registers an instance of a user implementation of the ViERTCPObserver.
  virtual int RegisterRTCPObserver(const int video_channel,
                                   ViERTCPObserver& observer) = 0;

  // Removes a registered instance of ViERTCPObserver.
  virtual int DeregisterRTCPObserver(const int video_channel) = 0;

  // Get rtp socket handle
  virtual int GetRtpSocket(const int video_channel) = 0;

  // Get rtp socket handle
  virtual int GetRtcpSocket(const int video_channel) = 0;

  virtual int EnableRtcpMux(const int video_channel, bool enable) = 0;

  virtual int RegisterReceiveRtpHeaderExtension(const int video_channel,
      const ViERTPExtensionType type,
      const unsigned char id) = 0;
  virtual int RegisterSendRtpHeaderExtension(const int video_channel,
      const ViERTPExtensionType type,
      const unsigned char id) = 0;

  virtual int DeregisterReceiveRtpHeaderExtension(const int video_channel,
      const ViERTPExtensionType type) = 0;
  virtual int DeregisterSendRtpHeaderExtension(const int video_channel,
      const ViERTPExtensionType type) = 0;
  virtual int ResetSsrcOnSeqWrap(const int video_channel,bool enable) = 0;

 protected:
  ViERTP_RTCP() {}
  virtual ~ViERTP_RTCP() {}
};

}  // namespace jssmme

#endif  // WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_RTP_RTCP_H_
