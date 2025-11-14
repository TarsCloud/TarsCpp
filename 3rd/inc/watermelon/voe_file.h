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
//
//  - File playback.
//  - File recording.
//  - File conversion.
//
// Usage example, omitting error checking:
//
//  using namespace jssmme;
//  VoiceEngine* voe = VoiceEngine::Create();
//  VoEBase* base = VoEBase::GetInterface(voe);
//  VoEFile* file  = VoEFile::GetInterface(voe);
//  base->Init();
//  int ch = base->CreateChannel();
//  ...
//  base->StartPlayout(ch);
//  file->StartPlayingFileAsMicrophone(ch, "data_file_16kHz.pcm", true);
//  ...
//  file->StopPlayingFileAsMicrophone(ch);
//  base->StopPlayout(ch);
//  ...
//  base->DeleteChannel(ch);
//  base->Terminate();
//  base->Release();
//  file->Release();
//  VoiceEngine::Delete(voe);
//
#ifndef WEBRTC_VOICE_ENGINE_VOE_FILE_H
#define WEBRTC_VOICE_ENGINE_VOE_FILE_H

#include "common_types.h"

namespace jssmme {

class VoiceEngine;

class WEBRTC_DLLEXPORT VoEFileObserver {
 public:
  // This method is called when the end is reached of a played file.
  virtual void PlayFileEnded(const int channel) = 0;

 protected:
  virtual ~VoEFileObserver() {}
};

class WEBRTC_DLLEXPORT VoEFile
{
public:
    // Factory for the VoEFile sub-API. Increases an internal
    // reference counter if successful. Returns NULL if the API is not
    // supported or if construction fails.
    static VoEFile* GetInterface(VoiceEngine* voiceEngine);

    // Releases the VoEFile sub-API and decreases an internal
    // reference counter. Returns the new reference count. This value should
    // be zero for all sub-API:s before the VoiceEngine object can be safely
    // deleted.
    virtual int Release() = 0;
    // Registers/deregister an instance of a user implementation of the VoEFileObserver.
    virtual int RegisterObserver(int channel, VoEFileObserver* observer) = 0;

    virtual int StartPlayingRtpFile(int channel, const char fileNameUTF8[1024]) = 0;

    virtual int StopPlayingRtpFile(int channel) = 0;

    virtual int PausePlayingRtpFile(int channel, bool pause) = 0;

    // Starts playing and mixing files with the local speaker signal for
    // playout.
    virtual int StartPlayingFileLocally(
        int channel,
        const char fileNameUTF8[1024],
        bool loop = false,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0,
        int startPointMs = 0,
        int stopPointMs = 0) = 0;

    // Starts playing and mixing streams with the local speaker signal for
    // playout.
    virtual int StartPlayingFileLocally(
        int channel,
        InStream* stream,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0,
        int startPointMs = 0, int stopPointMs = 0) = 0;

    // Stops playback of a file on a specific |channel|.
    virtual int StopPlayingFileLocally(int channel) = 0;

    virtual int StartPlayingFileLocally(
        InStream* stream,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0,
        int startPointMs = 0, int stopPointMs = 0) = 0;

    // Stops playback of a file on output_mixer.
    virtual int StopPlayingFileLocally() = 0;

    // Returns the current file playing state for a specific |channel|.
    virtual int IsPlayingFileLocally(int channel) = 0;

    // Sets the volume scaling for a speaker file that is already playing.
    virtual int ScaleLocalFilePlayout(int channel, float scale) = 0;

    // Starts reading data from a file and transmits the data either
    // mixed with or instead of the microphone signal.
    virtual int StartPlayingFileAsMicrophone(
        int channel,
        const char fileNameUTF8[1024],
        bool loop = false ,
        bool mixWithMicrophone = false,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0,
        int startTimeMs = 0,
        int stopTimeMs = 0) = 0;

    // Starts reading data from a stream and transmits the data either
    // mixed with or instead of the microphone signal.
    virtual int StartPlayingFileAsMicrophone(
        int channel,
        InStream* stream,
        bool mixWithMicrophone = false,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0,
        int starTimeMs = 0,
        int stopTimeMs = 0) = 0;

    // Stops playing of a file as microphone signal for a specific |channel|.
    virtual int StopPlayingFileAsMicrophone(int channel) = 0;
    // Suspend or resume playing of a file as microphone signal for a specific |channel|.
    virtual int SuspendPlayingFileAsMicrophone(int channel, bool pause) = 0;
    virtual int StartSendingFile(
        int channel,
        const char fileNameUTF8[1024],
        bool loop,
        FileFormats format) = 0;

    // play locally, and also play as microphone 
    virtual int StartPlayingFile(
        int channel,
        const char fileNameUTF8[1024],
        bool loop = false ,
        bool mixWithMicrophone = false,
        FileFormats format = kFileFormatPcm16kHzFile,
        float volumeScaling = 1.0) = 0;
    virtual int StopPlayingFile(int channel) = 0;
    virtual int StopSendingFile(int channel) = 0;

    // Returns whether the |channel| is currently playing a file as microphone.
    virtual int IsPlayingFileAsMicrophone(int channel) = 0;

    // Sets the volume scaling for a microphone file that is already playing.
    virtual int ScaleFileAsMicrophonePlayout(int channel, float scale) = 0;

    /**
    * record pause and restore
    */
    virtual int SetRecordPause(bool pause) = 0;

    // Starts recording the mixed playout audio.
    virtual int StartRecordingPlayout(int channel,
                                      const char* fileNameUTF8,
                                      CodecInst* compression = NULL,
                                      int maxSizeBytes = -1) = 0;

    // Stops recording the mixed playout audio.
    virtual int StopRecordingPlayout(int channel) = 0;

    virtual int StartRecordingPlayout(int channel,
                                      OutStream* stream,
                                      CodecInst* compression = NULL) = 0;

    // Starts recording the microphone signal to a file.
    virtual int StartRecordingMicrophone(const char* fileNameUTF8,
                                         CodecInst* compression = NULL,
                                         int maxSizeBytes = -1) = 0;

    // Starts recording the microphone signal to a stream.
    virtual int StartRecordingMicrophone(OutStream* stream,
                                         CodecInst* compression = NULL) = 0;

    // Stops recording the microphone signal.
    virtual int StopRecordingMicrophone() = 0;

    // Starts recording the call signal to a file.
    virtual int StartRecordingCall(int channel,
                                   const char* fileNameUTF8,
                                   CodecInst* compression = NULL,
                                   int maxSizeBytes = -1) = 0;

    // Starts recording the call signal to a stream.
    virtual int StartRecordingCall(int channel,
                                   OutStream* stream,
                                   CodecInst* compression = NULL) = 0;

    // Stops recording the call signal.
    virtual int StopRecordingCall(int channel) = 0;


    // Gets the duration of a file.
    virtual int GetFileDuration(const char* fileNameUTF8, int& durationMs,
        FileFormats format = kFileFormatPcm16kHzFile) = 0;

    // Gets the current played position of a file on a specific |channel|.
    virtual int GetPlaybackPosition(int channel, int& positionMs) = 0;

    // Gets the current played position of a file on a specific |channel|.
    // different between GetPlaybackPosition and GetSndSendPlayPosition:
    // GetPlaybackPosition: get the position of the local played file
    // GetSndSendPlayPosition: get the position of the file which send to remote
    virtual int GetSndSendPlayPosition(int channel, int& positionMs) = 0;

    virtual int ConvertPCMToWAV(const char* fileNameInUTF8,
                                const char* fileNameOutUTF8) = 0;

    virtual int ConvertPCMToWAV(InStream* streamIn,
                                OutStream* streamOut) = 0;

    virtual int ConvertWAVToPCM(const char* fileNameInUTF8,
                                const char* fileNameOutUTF8) = 0;

    virtual int ConvertWAVToPCM(InStream* streamIn,
                                OutStream* streamOut) = 0;

    virtual int ConvertPCMToCompressed(const char* fileNameInUTF8,
                                       const char* fileNameOutUTF8,
                                       CodecInst* compression) = 0;

    virtual int ConvertPCMToCompressed(InStream* streamIn,
                                       OutStream* streamOut,
                                       CodecInst* compression) = 0;

    virtual int ConvertCompressedToPCM(const char* fileNameInUTF8,
                                       const char* fileNameOutUTF8) = 0;

    virtual int ConvertCompressedToPCM(InStream* streamIn,
                                       OutStream* streamOut) = 0;

    virtual int ConvertWAVToCompressed(const char* fileNameInUTF8,
                                const char* fileNameOutUTF8,
                                CodecInst* compression) = 0;
    virtual int ConvertCompressedToWAV(const char* fileNameInUTF8,
                                const char* fileNameOutUTF8) = 0;

protected:
    VoEFile() {}
    virtual ~VoEFile() {}
};

}  // namespace jssmme

#endif  // WEBRTC_VOICE_ENGINE_VOE_FILE_H
