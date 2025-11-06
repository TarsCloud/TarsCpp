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
//
//  - Creating and deleting VideoEngine instances.
//  - Creating and deleting channels.
//  - Connect a video channel with a corresponding voice channel for audio/video
//    synchronization.
//  - Start and stop sending and receiving.

#ifndef WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_BASE_H_
#define WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_BASE_H_

#include "common_types.h"

namespace jssmme {

class VoiceEngine;

// Class used for all callbacks from ViEBase.
class WEBRTC_DLLEXPORT ViEBaseObserver {
 public:
  // This method will be called periodically if the average system CPU usage
  // exceeds 75%.
  virtual void PerformanceAlarm(const unsigned int cpu_load) = 0;
  virtual void CurrentCpuLoad(WebRtc_Word32 process,WebRtc_Word32 total) = 0;

 protected:
  virtual ~ViEBaseObserver() {}
};

class WEBRTC_DLLEXPORT VideoEngine {
 public:
  // Creates a VideoEngine object, which can then be used to acquire sub‐APIs.
  static VideoEngine* Create();

  // Deletes a VideoEngine instance.
  static bool Delete(VideoEngine*& video_engine);

  // Android specific.
  // Provides VideoEngine with pointers to objects supplied by the Java
  // applications JNI interface.
  static int SetAndroidObjects(void* java_vm, void* java_context);
  // Android native surface
  static int SetPreviewSurface(void* surface);
 protected:
  VideoEngine() {}
  virtual ~VideoEngine() {}
};

class WEBRTC_DLLEXPORT ViEBase {
 public:
  // Factory for the ViEBase sub‐API and increases an internal reference
  // counter if successful. Returns NULL if the API is not supported or if
  // construction fails.
  static ViEBase* GetInterface(VideoEngine* video_engine);

  // Releases the ViEBase sub-API and decreases an internal reference counter.
  // Returns the new reference count. This value should be zero
  // for all sub-API:s before the VideoEngine object can be safely deleted.
  virtual int Release() = 0;

  // Initiates all common parts of the VideoEngine.
  virtual int Init() = 0;

  // Connects a VideoEngine instance to a VoiceEngine instance for audio video
  // synchronization.
  virtual int SetVoiceEngine(VoiceEngine* voice_engine) = 0;

  // Creates a new channel.
  virtual int CreateChannel(int& video_channel) = 0;

  // Creates a new channel grouped together with |original_channel|. The channel
  // can both send and receive video. It is assumed the channel is sending
  // and/or receiving video to the same end-point.
  // Note: |CreateReceiveChannel| will give better performance and network
  // properties for receive only channels.
  virtual int CreateChannel(int& video_channel,
                            int original_channel) = 0;

  // Creates a new channel grouped together with |original_channel|. The channel
  // can only receive video and it is assumed the remote end-point is the same
  // as for |original_channel|.
  virtual int CreateReceiveChannel(int& video_channel,
                                   int original_channel) = 0;

  // Deletes an existing channel and releases the utilized resources.
  virtual int DeleteChannel(const int video_channel) = 0;

  virtual int RegisterGeneralProcessObserver(const int video_channel,
      const int gd_instance_id, const int gd_stream_id, 
      const int gd_track_id, const int gd_is_server, 
      const int gd_local, const int gd_is_decode) = 0;
  virtual int DeRegisterGeneralProcessObserver(const int video_channel, 
      const int gd_local) = 0;

  // route the receive image from video_channel to other_channel 
  virtual int ConnectOtherChannel (const int video_channel,
                                   const int other_channel,
                                   const int video_channel_type) = 0;

  // Disconnects a previously paired from video_channel to other_channel
  virtual int DisconnectOtherChannel (const int video_channel,
                                      const int other_channel) = 0;

  // Specifies the VoiceEngine and VideoEngine channel pair to use for
  // audio/video synchronization.
  virtual int ConnectAudioChannel(const int video_channel,
                                  const int audio_channel) = 0;

  // Disconnects a previously paired VideoEngine and VoiceEngine channel pair.
  virtual int DisconnectAudioChannel(const int video_channel) = 0;

  // Starts sending packets to an already specified IP address and port number
  // for a specified channel.
  virtual int StartSend(const int video_channel) = 0;

  // Stops packets from being sent for a specified channel.
  virtual int StopSend(const int video_channel) = 0;

  // Prepares VideoEngine for receiving packets on the specified channel.
  virtual int StartReceive(const int video_channel) = 0;

  // Stops receiving incoming RTP and RTCP packets on the specified channel.
  virtual int StopReceive(const int video_channel, bool keepRtcp = false) = 0;

  // Registers an instance of a user implementation of the ViEBase
  // observer.
  virtual int RegisterObserver(ViEBaseObserver& observer) = 0;

  // Removes an already registered instance of ViEBaseObserver.
  virtual int DeregisterObserver() = 0;

  // Retrieves the version information for VideoEngine and its components.
  virtual int GetVersion(char version[1024]) = 0;

  // Returns the last VideoEngine error code.
  virtual int LastError() = 0;

  // Specifies the amount and type of trace information, which will be created
  // by the VideoEngine.
  virtual int SetTraceFilter(const unsigned int filter) = 0;

  // Sets the name of the trace file and enables non‐encrypted trace messages.
  virtual int SetTraceFile(const char* file_nameUTF8,
      const int add_file_counter = -1, const int max_file_counter = 5, const int file_size = 4000) = 0;

  // Installs the TraceCallback implementation to ensure that the VideoEngine
  // user receives callbacks for generated trace messages.
  virtual int SetTraceCallback(TraceCallback* callback) = 0;

  // Get media sdp of receive setting
  virtual int GetReceiveDescription(const int video_channel, char sdp_media[], const char* codecNames) = 0;

  // Set media send setting by media sdp
  virtual int SetSendDescription(const int video_channel, const char* sdp_media) = 0;
 
 // set shared lib dir path
  virtual int SetLibDir(const char* libDirPath) = 0;

 protected:
  ViEBase() {}
  virtual ~ViEBase() {}
};

}  // namespace jssmme

#endif  // #define WEBRTC_VIDEO_ENGINE_MAIN_INTERFACE_VIE_BASE_H_
