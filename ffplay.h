#ifndef FFPLAY_H
#define FFPLAY_H

#include "libavutil/avstring.h"
#include "libavutil/colorspace.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

typedef struct HwAccelContext {
    char  *hwaccel_device;
    void  *hwaccel_ctx;
    void (*hwaccel_uninit)(AVCodecContext *s);
    int  (*hwaccel_get_buffer)(AVCodecContext *s, AVFrame *frame, int flags);
    int  (*hwaccel_retrieve_data)(AVCodecContext *s, AVFrame *frame);
    enum AVPixelFormat hwaccel_pix_fmt;
    enum AVPixelFormat hwaccel_retrieved_pix_fmt;
} HwAccelContext;

int dxva2_init(AVCodecContext *s);

#endif
