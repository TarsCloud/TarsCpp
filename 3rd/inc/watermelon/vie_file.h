/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This sub-API supports the following functionalities:
//  - File recording and playing.
//  - Snapshots.
//  - Background images.

#ifndef WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_FILE_H_
#define WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_FILE_H_

#include "common_types.h"
#include "media_flow_c_struct.h"

namespace jssmme {

class VideoEngine;
struct VideoCodec;

// This structure contains picture data and describes the picture type.
struct ViEPicture {
  unsigned char* data;
  unsigned int size;
  unsigned int width;
  unsigned int height;
  RawVideoType type;

  ViEPicture() {
    data = NULL;
    size = 0;
    width = 0;
    height = 0;
    type = kVideoI420;
  }

  // Call FreePicture to free data.
  ~ViEPicture() {
    data = NULL;
    size = 0;
    width = 0;
    height = 0;
    type = kVideoUnknown;
  }
};

// This enumerator tells which audio source to use for media files.
enum AudioSource {
  NO_AUDIO,
  MICROPHONE,
  PLAYOUT,
  VOICECALL
};

// This enumerator tells which audio source to use for media files.
enum VideoFillMode {
  FILL_AUTO,
  FULL_SCREEN,
  FULL_CONTENT
};
// This class declares an abstract interface for a user defined observer. It is
// up to the VideoEngine user to implement a derived class which implements the
// observer class. The observer is registered using RegisterObserver() and
// deregistered using DeregisterObserver().
class WEBRTC_DLLEXPORT ViEFileObserver {
 public:
  virtual void PlayFileBegin(const WebRtc_Word32 file_id) {}
  // This method is called when the end is reached of a played file.
  virtual void PlayFileEnded(const WebRtc_Word32 file_id) = 0;

  virtual void RecordFileBegin(const char* fileName) {}
  virtual void RecordFileEnded(const char* fileName) = 0;
  virtual void RecordFileEndedError(const WebRtc_Word32 id) = 0;
  // This method is called when splitting the recording file.
  virtual void RecordFileSplit(const WebRtc_Word32 splitCount,
      const char* originName, const char* lastName,  const char* newName)=0;
 protected:
  virtual ~ViEFileObserver() {}
};

class WEBRTC_DLLEXPORT ViEFile {
 public:
  // Factory for the ViEFile sub‐API and increases an internal reference
  // counter if successful. Returns NULL if the API is not supported or if
  // construction fails.
  static ViEFile* GetInterface(VideoEngine* video_engine);

  // Releases the ViEFile sub-API and decreases an internal reference counter.
  // Returns the new reference count. This value should be zero
  // for all sub-API:s before the VideoEngine object can be safely deleted.
  virtual int Release() = 0;
  virtual int StartPlayingRtpFile(int channel, const char fileNameUTF8[1024]) = 0;
  
  virtual int StopPlayingRtpFile(int channel) = 0;
  virtual int PausePlayingRtpFile(int channel, bool pause) = 0;
  // Starts playing a video file.
  virtual int StartPlayFile(
      const char* file_name_utf8,
      int& file_id,
      const bool loop = false,
      const FileFormats file_format = kFileFormatAviFile) = 0;

  // Starts playing a video file.
  virtual int StartPlayFileLocally(
      const char* file_name_utf8,
      int& file_id,
      bool audio,
      const bool loop = false,
      const bool pause  = false,
      const FileFormats file_format = kFileFormatAviFile) = 0;

  // Stops a file from being played.
  virtual int StopPlayFile(const int file_id) = 0;

  virtual int SetPlayStatus(const int file_id, bool pause) = 0;
  virtual int Proceed(const int file_id, int frameNo) = 0;
  virtual int SetPlaySpeed(const int file_id, float speed) = 0;
  virtual int SeekAvi(const int file_id, WebRtc_UWord32 startMs) = 0;
  // Registers an instance of a user implementation of the ViEFileObserver.
  virtual int RegisterObserver(int file_id, ViEFileObserver& observer) = 0;

  // Removes an already registered instance of ViEFileObserver.
  virtual int DeregisterObserver(int file_id, ViEFileObserver& observer) = 0;

  // This function tells which channel, if any, the file should be sent on.
  virtual int SendFileOnChannel(const int file_id, const int video_channel,const int video_type) = 0;

  // Stops a file from being sent on a a channel.
  virtual int StopSendFileOnChannel(const int video_channel, const int file_id) = 0;

  // Starts playing the file audio as microphone input for the specified voice
  // channel.
  virtual int StartPlayFileAsMicrophone(const int file_id,
                                        const int audio_channel,
                                        bool mix_microphone = false,
                                        float volume_scaling = 1) = 0;

  // The function stop the audio from being played on a VoiceEngine channel.
  virtual int StopPlayFileAsMicrophone(const int file_id,
                                       const int audio_channel) = 0;

  // The function plays and mixes the file audio with the local speaker signal
  // for playout.
  virtual int StartPlayAudioLocally(const int file_id, const int audio_channel,
                                    float volume_scaling = 1) = 0;

  // Stops the audio from a file from being played locally.
  virtual int StopPlayAudioLocally(const int file_id,
                                   const int audio_channel) = 0;

  // Registers an instance of a user implementation of the ViEFileObserver.
  virtual int SetRecordObserver(ViEFileObserver* observer) = 0;
  virtual int SetRecordFileMaxSize(const int fileLimitKB, const int maxSplitNum=0) = 0;
  virtual int SetRecordFileSplitInterval(const int splitIntervalMs, const int maxSplitNum) = 0;
  virtual int SetRecordInterval(const int keyInterval, const int flushInterval) = 0;
  // This function starts recording the capture result.
  virtual int StartRecordCapturingVideo (
      const int capture_id,
      const char* file_name_utf8,
      AudioSource audio_source,
      const CodecInst& audio_codec,
      const VideoCodec& video_codec,
      const FileFormats file_format = kFileFormatAviFile,
      const VideoFillMode fill_mode = FULL_SCREEN) = 0;

  // This function starts recording the video transmitted to another endpoint.
  virtual int StartRecordOutgoingVideo(
      const int video_channel,
      const char* file_name_utf8,
      AudioSource audio_source,
      const CodecInst& audio_codec,
      const VideoCodec& video_codec,
      const FileFormats file_format = kFileFormatAviFile,
      const VideoFillMode fill_mode = FULL_SCREEN) = 0;

  virtual int SetOutgoingRecordPause(const int video_channel,
                                     AudioSource audio_source,
                                     bool pause) = 0;

  virtual void RegisterExceptionEventCallback(pfn_excetion_callback callback) = 0;

  virtual int MediaInitiate(const char* ffmpeg_url, void* pfn_msg_callback) = 0;
  virtual int MediaCreate() = 0;
  virtual int MediaDestroy(const int32_t instance_id) = 0;
  virtual int MediaAddStream(const int32_t instance_id,
      const int32_t type,
      const char* url) = 0;
  virtual int MediaAddProxy(const int32_t instance_id,
      const media_instream_ctx_t *info) = 0;
  virtual int MediaDeleteStream(const int32_t instance_id,
      const int32_t stream_id) = 0;
  virtual int MediaGetMediaInfo(const int32_t instance_id,
      const int32_t stream_id,
      media_instream_ctx_t *info) = 0;
  virtual int MediaSetMediaInfo(const int32_t instance_id,
      const int32_t stream_id,
      const media_instream_ctx_t *info) = 0;
  virtual int MediaStartStream(const int32_t instance_id,
      const int32_t stream_id) = 0;
  virtual int MediaStartAllStream(const int32_t instance_id) = 0;
  virtual int MediaAddProcess(const int32_t instance_id,
      const int32_t nb_id,
      const int32_t* track_ids,
      const int32_t nb_module,
      const media_module_ctx_t** modules) = 0;
  virtual int MediaDeleteProcess(const int32_t instance_id,
      const int32_t process_id) = 0;
  virtual int MediaProcessRegisterTrack(const int32_t instance_id,
      const int32_t process_id,
      const int32_t id) = 0;
  virtual int MediaProcessDeRegisterTrack(const int32_t instance_id,
      const int32_t process_id,
      const int32_t id) = 0;
  virtual int MediaProcessSetLayout(const int32_t instance_id,
      const int32_t process_id,
      const int32_t nb_item,
      const merge_layout_item_t* items) = 0;
  virtual int MediaAddOutput(const int32_t instance_id,
      const int32_t nb_id,
      const int32_t* process_ids,
      const media_outstream_ctx_t* output) = 0;
  virtual int MediaDeleteOutput(const int32_t instance_id,
      const int32_t output_id) = 0;
  virtual int MediaStop(const int32_t instance_id) = 0;

  // Pause the record of video and audio.
  virtual int SetIncomingRecordPause(const int video_channel,
                                     AudioSource audio_source,
                                     bool pause) = 0;

  // This function starts recording the incoming video stream on a channel.
  virtual int StartRecordIncomingVideo(
      const int video_channel,
      const char* file_name_utf8,
      AudioSource audio_source,
      const CodecInst& audio_codec,
      const VideoCodec& video_codec,
      const FileFormats file_format = kFileFormatAviFile,
      const VideoFillMode fill_mode = FULL_SCREEN) = 0;

  // Stops the file recording of the capture.
  virtual int StopRecordCapturingVideo(const int capture_id) = 0;

  // Stops the file recording of the outgoing stream.
  virtual int StopRecordOutgoingVideo(const int video_channel) = 0;

  // Stops the file recording of the incoming stream.
  virtual int StopRecordIncomingVideo(const int video_channel) = 0;

  // Gets the audio codec, video codec and file format of a recorded file.
  virtual int GetFileInformation(
      const char* file_name,
      VideoCodec& video_codec,
      CodecInst& audio_codec,
      unsigned int& frame_no,
      unsigned int& key_period,
      const FileFormats file_format = kFileFormatAviFile) = 0;

  virtual int RecoverVideoFile(const char* file_name, FileFormats file_format) = 0;

  // The function takes a snapshot of the last rendered image for a video
  // channel.
  virtual int GetRenderSnapshot(const int video_channel,
                                const char* file_name_utf8,
                                ImageFileFormat format = kImageJpeg) = 0;

  // The function takes a snapshot of the last rendered image for a video
  // channel
  virtual int GetRenderSnapshot(const int video_channel,
                                ViEPicture& picture) = 0;

  // The function takes a snapshot of the last captured image by a specified
  // capture device.
  virtual int GetCaptureDeviceSnapshot(const int capture_id,
                                       const char* file_name_utf8,
                                       ImageFileFormat format = kImageJpeg) = 0;

  // The function takes a snapshot of the last captured image by a specified
  // capture device.
  virtual int GetCaptureDeviceSnapshot(const int capture_id,
                                       ViEPicture& picture) = 0;

  // This function sets a jpg image to show before the first frame is captured
  // by the capture device. This frame will be encoded and transmitted to a
  // possible receiver
  virtual int SetCaptureDeviceImage(const int capture_id,
                                    const char* file_name_utf8) = 0;

  // This function sets an image to show before the first frame is captured by
  // the capture device. This frame will be encoded and transmitted to a
  // possible receiver
  virtual int SetCaptureDeviceImage(const int capture_id,
                                    const ViEPicture& picture) = 0;

  virtual int FreePicture(ViEPicture& picture) = 0;

  // This function sets a jpg image to render before the first received video
  // frame is decoded for a specified channel.
  virtual int SetRenderStartImage(const int video_channel,
                                  const char* file_name_utf8) = 0;

  // This function sets an image to render before the first received video
  // frame is decoded for a specified channel.
  virtual int SetRenderStartImage(const int video_channel,
                                  const ViEPicture& picture) = 0;

  // This function sets a jpg image to render if no frame is decoded for a
  // specified time interval.
  virtual int SetRenderTimeoutImage(const int video_channel,
                                    const char* file_name_utf8,
                                    const unsigned int timeout_ms = 1000) = 0;

  // This function sets an image to render if no frame is decoded for a
  // specified time interval.
  virtual int SetRenderTimeoutImage(const int video_channel,
                                    const ViEPicture& picture,
                                    const unsigned int timeout_ms) = 0;

  virtual int ConvertImage(char *pcInFile, ImageFileFormat InFormat, 
            char *pcOutFile, ImageFileFormat OutFormat,
            unsigned int iWidth, unsigned int iHeight) = 0;

 protected:
  ViEFile() {}
  virtual ~ViEFile() {}
};

}  // namespace jssmme

#endif  // WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_FILE_H_
