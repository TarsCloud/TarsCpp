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
//  - Setting send and receive codecs.
//  - Codec specific settings.
//  - Key frame signaling.
//  - Stream management settings.

#ifndef WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_CODEC_H_
#define WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_CODEC_H_

#include "common_types.h"

namespace jssmme {

class VideoEngine;
struct VideoCodec;
enum ViECodecError {
  H264_CDC_FALLBACK = -1,                       /* h264 encoder fallback */
  H264_HW_CDC_FALLBACK = -2,                      /* h264 hardware encoder fallback*/
  AV1_CDC_FALLBACK = -3,                          /* av1 encoder fallback */
};
// This class declares an abstract interface for a user defined observer. It is
// up to the VideoEngine user to implement a derived class which implements the
// observer class. The observer is registered using RegisterEncoderObserver()
// and deregistered using DeregisterEncoderObserver().
class WEBRTC_DLLEXPORT ViEEncoderObserver {
 public:
  // This method is called once per second with the current encoded frame rate
  // and bit rate.
  virtual void OutgoingRate(const int video_channel,
                            const unsigned char framerate[],
                            const unsigned int bitrate,
                            const unsigned int avgQp,
                            const unsigned int realQp,
                            const double       psnr,
                            const unsigned int encodedWidth,
                            const unsigned int encodedHeight,
                            const unsigned int sliNum,
                            const unsigned int idrNum,
                            const unsigned int targetBr,
                            const unsigned int resendNum,
                            const unsigned int encodeTotalTimeMs) = 0;
  virtual void ProtectionRate(const int video_channel,
                             unsigned int totalBw,
                             unsigned int totalRate,
                             unsigned int videoRate,
                             unsigned int fecRate,
                             unsigned int nackRate) = 0;
  virtual void CameraOveruseAlert() = 0;
  virtual void SendCodecSize(const int video_channel,
                             unsigned int width, unsigned int height) = 0;
  virtual void SendSvcLevel(const int video_channel,
                            unsigned int width, unsigned int height,
                            unsigned char numberOfSpatialLayers,
                            const unsigned char temporalMask[SVC_MAX_SPATIAL_LAYER_NUM]) = 0;
  virtual void CodecError(const int video_channel, const int error, const int info) = 0;

 protected:
  virtual ~ViEEncoderObserver() {}
};

// This class declares an abstract interface for a user defined observer. It is
// up to the VideoEngine user to implement a derived class which implements the
// observer class. The observer is registered using RegisterDecoderObserver()
// and deregistered using DeregisterDecoderObserver().
class WEBRTC_DLLEXPORT ViEDecoderObserver {
 public:
  // This method is called when a new incoming stream is detected, normally
  // triggered by a new incoming SSRC or payload type.
  virtual void IncomingCodecChanged(const int video_channel,
                                    const VideoCodec& video_codec) = 0;

  // This method is called once per second containing the frame rate and bit
  // rate for the incoming stream
  virtual void IncomingRate(const int video_channel,
                            const unsigned int decodedFps,
                            const unsigned int bitrate,
                            const unsigned int recvFps,
                            const unsigned int decodeTotalTimeMs) = 0;

  // This method is called once 15 second containing the decode disconTime
  // and conTime for the incoming stream
  virtual void DecodeDiscontinuity(const int video_channel,
                                const unsigned int disconRate) = 0;

  //the method is called if the key frame request is not sent in the latest one second.
  virtual void SendFirReqPerSec(const int video_channel,
                                const bool bFirReq) = 0;

  // This method is called when the decoder needs a new key frame from encoder
  // on the sender.
  virtual void RequestNewKeyFrame(const int video_channel) = 0;

  virtual void OnRtpExtRotationSupportedOrNot(const int video_channel,
                                            bool supported) = 0;

 protected:
  virtual ~ViEDecoderObserver() {}
};

class WEBRTC_DLLEXPORT ViEDecodeProcess {
 public:
  // This method is called when a new incoming frame is prepared for decoded.
  // if return < 0, will request key frame.
  // it will skip the next standard decoder.
  virtual int Decode  (const int video_channel, char payload, bool compele,
                         unsigned time_stamp, const char* data, int fragmentationVectorSize,
                         int fragmentationLength[], int fragmentationOffset[]) = 0;

  // This method is called when a prepared frame stream is into decoder.
  // return: -1 request keyframe and skip internal decoding; 0: ok and skip internal decoding; 1: ok and continue internal decoding
  virtual int OnEncodedImage(const int video_channel, unsigned char* pSrc, int iSrcLen) = 0;

 protected:
  virtual ~ViEDecodeProcess() {}
};

class WEBRTC_DLLEXPORT ViELayout {
public:
    struct Item {
        int id, x, y, w, h;
        const char* name;
    };
    struct LvlItem {
        int id, level;
        const char* name;
    };
    // used in auto layout mode;
    // callback to request user-defined layout
    virtual int Layout(const int video_channel, int width, int height, Item item[], int size, int sharerIndex) = 0;
    // used in intelligent merge mode;
    // callback to request difference video level subscription
    virtual int RequestLevel(const int video_channel, LvlItem item[], int size) = 0;
protected:
    virtual ~ViELayout() {}
};
class WEBRTC_DLLEXPORT ViECodec {
 public:
  // Factory for the ViECodec sub‐API and increases an internal reference
  // counter if successful. Returns NULL if the API is not supported or if
  // construction fails.
  static ViECodec* GetInterface(VideoEngine* video_engine);

  // Releases the ViECodec sub-API and decreases an internal reference
  // counter.
  // Returns the new reference count. This value should be zero
  // for all sub-API:s before the VideoEngine object can be safely deleted.
  virtual int Release() = 0;

  // Gets the number of available codecs for the VideoEngine build.
  virtual int NumberOfCodecs() const = 0;

  // Gets a VideoCodec struct for a codec containing the default configuration
  // for that codec type.
  virtual int GetCodec(const unsigned char list_number,
                       VideoCodec& video_codec) const = 0;

  // for dynamically registerred codec, use this api to query before usage
  // currently only h265 is dynamically registerred.
  virtual int IsCodecSupported(VideoCodecType type, bool &encoder, bool &decoder) = 0;

  // Sets the send codec to use for a specified channel.
  virtual int SetSendCodec(const int video_channel,
                           const VideoCodec& video_codec, const bool bReset=true) = 0;

  // Gets the current send codec settings.
  virtual int GetSendCodec(const int video_channel,
                           VideoCodec& video_codec) const = 0;

  // Prepares VideoEngine to receive a certain codec type and setting for a
  // specified payload type.
  virtual int SetReceiveCodec(const int video_channel,
                              const VideoCodec& video_codec) = 0;

  // Gets the current receive codec.
  virtual int GetReceiveCodec(const int video_channel,
                              VideoCodec& video_codec) const = 0;

  // This function is used to get codec configuration parameters to be
  // signaled from the encoder to the decoder in the call setup.
  virtual int GetCodecConfigParameters(
      const int video_channel,
      unsigned char config_parameters[kConfigParameterSize],
      unsigned char& config_parameters_size) const = 0;

  // Enables advanced scaling of the captured video stream if the stream
  // differs from the send codec settings.
  virtual int SetImageScaleStatus(const int video_channel,
                                  const bool enable) = 0;

  // Gets the number of sent key frames and number of sent delta frames.
  virtual int GetSendCodecStastistics(const int video_channel,
                                      unsigned int& key_frames,
                                      unsigned int& delta_frames) const = 0;

  // Gets the number of decoded key frames and number of decoded delta frames.
  virtual int GetReceiveCodecStastistics(const int video_channel,
                                         unsigned int& key_frames,
                                         unsigned int& delta_frames) const = 0;

  // Gets the bitrate targeted by the video codec rate control in kbit/s.
  virtual int GetCodecTargetBitrate(const int video_channel,
                                    unsigned int* bitrate) const = 0;

  // Gets the number of packets discarded by the jitter buffer because they
  // arrived too late.
  virtual unsigned int GetDiscardedPackets(const int video_channel) const = 0;

  // Enables key frame request callback in ViEDecoderObserver.
  virtual int SetKeyFrameRequestCallbackStatus(const int video_channel,
                                               const bool enable) = 0;

  // Enables key frame requests for detected lost packets.
  virtual int SetSignalKeyPacketLossStatus(
      const int video_channel,
      const bool enable,
      const bool only_key_frames = false) = 0;

  virtual int RegisterSendTransport(const int video_channel,
                                    Transport& sendTransport) = 0;

  // Registers an instance of a user implementation of the ViEEncoderObserver.
  virtual int RegisterEncoderObserver(const int video_channel,
                                      ViEEncoderObserver& observer) = 0;

  // Removes an already registered instance of ViEEncoderObserver.
  virtual int DeregisterEncoderObserver(const int video_channel) = 0;

  // Registers an instance of a user implementation of the ViEDecoderObserver.
  virtual int RegisterDecoderObserver(const int video_channel,
                                      ViEDecoderObserver& observer) = 0;

  // Removes an already registered instance of ViEDecoderObserver.
  virtual int DeregisterDecoderObserver(const int video_channel) = 0;

  // Registers an instance of a user implementation of the ViEDecodeProcess.
  virtual int RegisterDecodeProcess(const int video_channel,
                                    ViEDecodeProcess& process,
                                    const bool replace) = 0;

  // Removes an already registered instance of ViEDecodeProcess.
  virtual int DeregisterDecodeProcess(const int video_channel) = 0;

  // This function forces the next encoded frame to be a key frame. This is
  // normally used when the remote endpoint only supports out‐band key frame
  // request.
  virtual int SendKeyFrame(const int video_channel) = 0;

  // This function makes the decoder wait for a key frame before starting to
  // decode the incoming video stream.
  virtual int WaitForFirstKeyFrame(const int video_channel,
                                   const bool wait) = 0;

  // This function makes VideoEngine drop the frame during video capture
  virtual int EnableFrameDropper(const int videoChannel, bool enable) = 0;

  //enable/disable resolution control
  virtual int EnableResolutionControl(const int video_channel, bool enable)=0;
  virtual int EnableResolutionControl(const int video_channel, bool enable, WebRtc_UWord16 ucmode)=0;

  virtual const char* GetEncodeType(const int video_channel) = 0;

  //Cpu control related
  virtual int EnableCpuControl(const int video_channel,bool enable) = 0;

  virtual int RUdpSetParm(const int video_channel, bool enable, WebRtc_UWord32 bitrate) = 0;
    
  //set overall cpu load target and program cpu load target
  virtual int SetCpuControlTarget(const int video_channel,WebRtc_UWord16 overall,WebRtc_UWord16 program) = 0;

  // set rotation angle of image before encoding
  virtual int SetEncoderRotation(const int video_channel,
                                 const int angle,
                                 const int cameraAngle,
                                 const int direction) = 0;

  virtual int SetEncoderLayout(const int video_channel, ViELayout* layout) = 0;

  virtual int SetRecordBlackFrame(const int video_channel, bool is_record) = 0;

  virtual int SetBlackScreen(const int video_channel, int is_black) = 0;

  virtual int LoadWatermarkLibraries(const int video_channel, const char *dir_path) = 0;

  virtual int SetWatermarkState(const int video_channel, int state) = 0;

  virtual int SetTextWatermarkStyle(const int video_channel, const char *font_file, int font_size, int font_color, int back_color,
	  int is_bold, int is_italic, int is_underline, int alignment, int borderstyle) = 0;

  virtual int SetTextWatermarkEvent(const int video_channel, int index, int state, const char *text_event) = 0;

  virtual int SetTimestampWatermark(const int video_channel, int state, const char *font_file, int ts_font_size, int ts_color_type, int ts_border_width,
	  int ts_pos_type, int ts_pos_x, int ts_pos_y, int ts_is_ms, int ts_base_time) = 0;

  virtual int SetOnePictureWatermark(const int video_channel, int index, int state, const char *url, int pic_time_start,
	  int pic_time_end, int pic_pos_x, int pic_pos_y) = 0;

  virtual int SetEncoderMergeType_i(const int video_channel, WebRtc_UWord8 vlMode, WebRtc_UWord8 scsMode, int bigSmallId, bool hasPerRowOnePic, int platformModeHeight) = 0;

  virtual int PublishEncodingFrame(const int video_channel, const char* renderId) = 0;

  // set rotation angle of image after decoding
  virtual int SetDecoderRotation(const int video_channel, const int angle) = 0;

  // set rudp enable or disable
  virtual int RUdpSetEnable(const int video_channel, bool enable) = 0;

  // enable bitrate save mode
  virtual int EnableBrSaveMode(const int video_channel, bool enable) = 0;

  // Prefer spatial quality over temporal quality
  virtual int PreferSpatialOverTemporal(const int video_channel, bool enable) = 0;

  virtual int EnhanceBrightness(const int video_channel,const bool enable) = 0;

  virtual int EnhanceContrast(const int video_channel,const bool enable) = 0;

  virtual int AntiLoss(const int video_channel, const bool enable) = 0;

  virtual int SetRtpExtApplicationPackets(const int vido_channel, WebRtc_UWord8 rxSub_type, WebRtc_UWord8 *rxData,
          WebRtc_UWord8 rxLength, WebRtc_UWord8 rxCounter, bool rxbSent) = 0;

  virtual int SetEncoderSendLevel(const int video_channel,
                                           const bool bKeyFrame,
                                           WebRtc_UWord32 uiLevel) = 0;

  // Enables dynamic reset encoder to change aspect
  // if fixedAspect <=0, will adaptive the capture, or fixed aspect
  virtual int EnableAdaptiveAspect(const int video_channel,
                                   const bool adaptiveAspect,
                                   const float fixedAspect) = 0;

  virtual int EnableRotation(const int video_channel, const bool enable) = 0;

 protected:

  ViECodec() {}
  virtual ~ViECodec() {}
};

}  // namespace jssmme

#endif  // WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_CODEC_H_
