#ifndef _GENERAL_DESIGN_MEDIA_FLOW_C_STRUCT_H_
#define _GENERAL_DESIGN_MEDIA_FLOW_C_STRUCT_H_
#include "media_flow_c_type.h"
#include "stdio.h"
#include "string.h"

struct media_track_t
{
    //void init()
    //{
    //   type = 0;
    //   id = -1;
    //   program_id = -1;
    //   is_decode = 1;
    //   is_enable = 1;
    //   //index = -1;
    //   //time_base_num = -1;
    //   //time_base_den = -1;
    //   //duration = 0;
    //   width = -1;
    //   height = -1;
    //   ratio_num = -1;
    //   ratio_den = -1;
    //   fps = -1;
    //   frame_rate_num = -1;
    //   frame_rate_den = -1;
    //   codec_id = -1;
    //   sample_rate = -1;
    //   channels = -1;
    //}

    /* 0 - VIDEO,
       1 - AUDIO,
       2 - DATA,          
       3 - SUBTITLE */
    enum MEDIA_TRACK_TYPE type;  /* 轨道类型 */

    int id;
    int program_id;

    /* if or not need decode*/
    int is_decode;   /* 是否需要解码 */
    int is_enable;   /* 是否读取该轨道的数据 */

    /* for ffmpeg */
    //int32_t index;
    //int64_t time_base_num;
    //int64_t time_base_den;
    //int64_t duration;
    int width;
    int height;

    // 视频宽高比
    int ratio_num;
    int ratio_den;

    // fps
    int fps;         /* 视频帧率 */
    int frame_rate_num;
    int frame_rate_den;

    int codec_id;    /* MEDIA_CODEC_ID 中的类型 */
    int sample_rate; /* 音频采样率 */
    int channels;    /* 音频声道数 */
};

#define MAX_MEDIA_TRACK 3
struct media_instream_ctx_t 
{
    //void init()
    //{
    //    type = 2;
    //    memset(url, 0x00, 1024);
    //    nb_track = 0;
    //    memset(tracks, 0x00, sizeof(media_track_t) * 3);
    //}

    enum MEDIA_IN_STREAM_TYPE type;

    /* for stream type of proxy, 
       the value of url is an actor id */
    char url[1024];
    int nb_track;
    struct media_track_t tracks[MAX_MEDIA_TRACK];
};

struct media_encode_ctx_t
{
    //void init()
    //{
    //}

    enum MEDIA_ENCODE_TYPE type;

    int bit_rate;          /* bps */
    int sample_rate;       /*audio:sample rate*/
    int channels;          /*audio:channel*/
  
    int width;
    int height;
    int key_interval;
};

struct media_filter_ctx_t
{
    enum MEDIA_FILTER_TYPE type;

    /* resample */
    enum MEDIA_SAMPLERATE out_samplerate;
    int out_channel;

    /* scale */
    int out_width;
    int out_height;
};

/* 布局 */
struct merge_layout_item_t {
    int id;

    /* video */
    int x;
    int y;
    int w;
    int h;

    /* audio */
    /* 0 - left, 1 - right, 2 - center */
    int ch_idx;   /* channel index */
};

/* 混合模块 */
struct media_merger_ctx_t
{
    //void init()
    //{
    //    out_channel = 2;
    //}

    enum MEDIA_MERGER_TYPE type;
    int fps;  /* 输出帧率， 默认值: 视频-24, 音频-100 */

    /* 视频参数 */
    int out_width;   /* 输出宽度 */
    int out_height;  /* 输出高度 */

    /* 音频参数 */
    enum MEDIA_SAMPLERATE out_samplerate; /* 输出采样率*/
    int out_channel;  /* 输出声道数 */
};

struct media_outstream_ctx_t
{
    //void init()
    //{
    //    video_only = false;
    //}

    enum MEDIA_OUT_STREAM_TYPE type;
    int is_offline; /* 1 - when input stream type is file */
    char url[1024];

    /* 0 - PCMA, 1 - PCMU, 2 - L16*/
    int wav_type;

    /* for video render */
    int render_id;
    void* window;
    int z_order;
    float left;
    float top;
    float right;
    float bottom;

    /* for callback interface */
    void* callback; /* callback function pointer,
                       type definition is PFN_OUT_STREAM_CALLBACK */
    void* object;

    /* for video */
    int   width;
    int   height;
    int   start_bitrate;
    int   max_bitrate; //kbps
    int   min_bitrate;
    int   max_fps;
    int   max_qp;
    int   min_qp;
};

struct media_text_watermark_ctx_t {
    // text watermark
    char text_font_dir[1024];
    char text_font_name[1024];
    int text_font_size;
    int text_color;         /* default 0xffffff */
    int text_back_color;
    int text_bold;          /* 0 - no, 1 - yes */
    int text_italic;        /* 0 - no, 1 - yes */
    int text_underline;     /* 0 - no, 1 - yes */
    int text_alignment;
    int text_borderstyle;
};

struct media_ts_watermark_ctx_t {
    //fontfile = consolas:x = (w - tw) / 2 : y = (h - th) / 2 : fontcolor = white : fontsize = 30 : borderw = 2
    // time stamp watermark
    int ts_is_ms;   /* whether or not show millisecond mark */
    char ts_font_file[1024];
    int ts_font_size;
    int ts_color_type;
    int ts_border_width;
    int ts_pos_type;
    int ts_pos_x;
    int ts_pos_y;
    int ts_start_time;    /* the base time of local */
                          /* the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds. */
    int ts_base_time;     /* the base time set by user */
                          /* the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds. */
};

struct media_pic_watermark_ctx_t {
    char pic_url[1024];
    int pic_time_start;
    int pic_time_end;
    //int pic_pos_type;
    int pic_pos_x;
    int pic_pos_y;
    //"(main_w-overlay_w)/2:(main_h-overlay_h)/2"
    //char pic_pos[128];
    int pic_width;
    int pic_height;
};

struct media_watermark_ctx_t
{
    //AvWatermarkContext& operator=(const AvWatermarkContext& rhs)
    //{
    //    this->type = rhs.type;
    //    this->width = rhs.width;
    //    this->height = rhs.height;

    //    memcpy(&this->pic, &rhs.pic, sizeof(PictureWatermarkContext));
    //    memcpy(&this->text, &rhs.text, sizeof(TextWatermarkContext));
    //    memcpy(&this->ts, &rhs.ts, sizeof(TSWatermarkContext));

    //    return *this;
    //}

    enum MEDIA_WATERMARK_TYPE type;

    struct media_text_watermark_ctx_t text;
    struct media_ts_watermark_ctx_t ts;
    struct media_pic_watermark_ctx_t pic;
};

struct media_module_ctx_t 
{
    enum MEDIA_MODULE_TYPE type;

    struct media_merger_ctx_t merger;
    struct media_watermark_ctx_t marker;
    struct media_filter_ctx_t filter;
    struct media_encode_ctx_t encoder;
};

#endif // !_GENERAL_DESIGN_MEDIA_ENGINE_C_STRUCT_H_