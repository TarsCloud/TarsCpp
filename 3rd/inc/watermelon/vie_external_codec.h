/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_EXTERNAL_CODEC_H_
#define WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_EXTERNAL_CODEC_H_

#include "common_types.h"

namespace jssmme {

class VideoDecoder;
class VideoEncoder;
class VideoEngine;

class WEBRTC_DLLEXPORT ViEExternalCodec {
 public:
  static ViEExternalCodec* GetInterface(VideoEngine* video_engine);

  virtual int Release() = 0;

  virtual int RegisterExternalSendCodec(const int video_channel,
                                        const unsigned char pl_type,
                                        const char *codecName) = 0;

  virtual int DeRegisterExternalSendCodec(const int video_channel,
                                          const unsigned char pl_type) = 0;

  virtual int RegisterExternalReceiveCodec(const int video_channel,
                                           const unsigned int pl_type,
                                           const char *codecName,
                                           bool decoder_render = false,
                                           int render_delay = 0) = 0;

  virtual int DeRegisterExternalReceiveCodec(const int video_channel,
                                             const unsigned char pl_type) = 0;

 protected:
  ViEExternalCodec() {}
  virtual ~ViEExternalCodec() {}
};

}  // namespace jssmme

#endif  // WEBRTC_VIDEO_ENGINE_INCLUDE_VIE_EXTERNAL_CODEC_H_
