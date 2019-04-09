//
// Created by CQNNJ-003 on 2019/4/8.
//
#include <libavformat/avformat.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <libavutil/imgutils.h>
#include <unistd.h>
#include <libswscale/swscale.h>
#include "com_jni_ndk_FFmpegPlayer.h"
#include "../libyuv.h"
JNIEXPORT void JNICALL
Java_com_jni_ndk_FFmpegPlayer_nicePlay(JNIEnv *env, jclass cls, jstring input_str, jobject surface){
    const char *inputString = (*env)->GetStringUTFChars(env, input_str, 0);
    //封装格式上下文
    AVFormatContext * pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    //int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
    //@return 0 on success, a negative AVERROR on failure.
    if (avformat_open_input(&pFormatCtx,inputString,NULL,NULL) != 0){
        LOG_E("%s","打开输入视频文件失败");
        return;
    }
    //获取视频信息
    //int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
    //@return >=0 if OK, AVERROR_xxx on error
    if(avformat_find_stream_info(pFormatCtx,NULL) < 0){
        LOG_E("%s","获取视频文件信息失败");
        return;
    }

    //视频解码
    //找到视频索引位置
    int video_stream_index = -1;
    int i= 0;
    for (; i < pFormatCtx->nb_streams; i++) {
        //判断是否是视频流
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            break;
        }
    }

    //获取视频解码器
    const AVCodec *avCodec = avcodec_find_decoder(pFormatCtx->streams[video_stream_index]->codecpar->codec_id);
    //创建视频流中的编解码上下文
    AVCodecContext* pCodecCtx = avcodec_alloc_context3(avCodec);
    //将paramter转换成编码器上下文
    avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[video_stream_index]->codecpar);
    if(avCodec == NULL){
        LOG_E("%s","无法解码");
        return;
    }

    //打开解码器
    if(avcodec_open2(pCodecCtx,avCodec,NULL)<0){
        LOG_E("%s","打开解码器失败");
        return;
    }

    //编码数据
    AVPacket *packet = av_malloc(sizeof(AVPacket));

    //像素数据(解码数据)
    AVFrame *yuv_frame = av_frame_alloc();
    AVFrame *rgb_frame = av_frame_alloc();

    //native绘制
    //窗体
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env,surface);
    //绘制时的缓冲区
    ANativeWindow_Buffer outBuffer;
    //缓冲区分配内存
    uint8_t *out_buffer = av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGBA,pCodecCtx->width,pCodecCtx->height,1)* sizeof(uint8_t));
    av_image_fill_arrays(rgb_frame->data,rgb_frame->linesize,out_buffer,AV_PIX_FMT_RGBA,pCodecCtx->width,pCodecCtx->height,1);

    //用于转码的参数,转之前得宽高,转之后的宽高,格式等
    /**                     方法一                         */
//    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
//                                                pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGBA,SWS_BICUBIC,NULL,NULL,NULL);
    /********************************************************/
    int len,got_frame,framecount = 0;
    //一段一段读取压缩的视频数据AVPacket
    while (av_read_frame(pFormatCtx,packet)>=0){
        //解码AVPacket ->AVFrame
        if (packet->stream_index == video_stream_index){
            //解码一帧视频压缩数据,得到视频像素数据

            len = avcodec_send_packet(pCodecCtx,packet);
            if (len != 0){
                LOG_I("%s","解码错误");
                return;
            }
            got_frame = avcodec_receive_frame(pCodecCtx,yuv_frame);
            //等于0.解码完成
            if(got_frame==0){
                LOG_I("解码%d帧",framecount++);
                /**                     方法一                         */
//                sws_scale(sws_ctx,yuv_frame->data,yuv_frame->linesize,
//                          0,pCodecCtx->height,rgb_frame->data,rgb_frame->linesize);
                /********************************************************/
                //上锁lock
                //int32_t ANativeWindow_setBuffersGeometry(ANativeWindow* window,
//                int32_t width, int32_t height, int32_t format);
                ANativeWindow_setBuffersGeometry(nativeWindow,pCodecCtx->width,pCodecCtx->height,WINDOW_FORMAT_RGBA_8888);
                ANativeWindow_lock(nativeWindow,&outBuffer,NULL);
                //初始化缓冲区
                //avpicture_fill((AVPicture *)rgb_frame, outBuffer.bits, AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height);
                av_image_fill_arrays(rgb_frame->data,rgb_frame->linesize,out_buffer,AV_PIX_FMT_RGBA,pCodecCtx->width,pCodecCtx->height,1);
                //YUV->RGBA_8888
                //int I420ToARGB(const uint8* src_y, int src_stride_y,
                //const uint8* src_u, int src_stride_u,
                //const uint8* src_v, int src_stride_v,
                //uint8* dst_argb, int dst_stride_argb,
                //int width, int height);
                /**                     方法二                         */
                I420ToARGB(yuv_frame->data[0],yuv_frame->linesize[0],
                           yuv_frame->data[2],yuv_frame->linesize[2],
                           yuv_frame->data[1],yuv_frame->linesize[1],
                           rgb_frame->data[0],rgb_frame->linesize[0],
                           pCodecCtx->width,pCodecCtx->height);
                /********************************************************/

                uint8_t *bits = (uint8_t *) outBuffer.bits;
                for (int h = 0; h < pCodecCtx->height; h++) {
                    memcpy(bits + h * outBuffer.stride * 4,out_buffer + h * rgb_frame->linesize[0], rgb_frame->linesize[0]);
                }
                //解锁
                //unlock
                ANativeWindow_unlockAndPost(nativeWindow);
                usleep(1000 * 16);
            }
        }
        av_packet_unref(packet);
    }
    ANativeWindow_release(nativeWindow);
//    sws_freeContext(sws_ctx);
    av_free(out_buffer);
    av_frame_free(&yuv_frame);
    av_frame_free(&rgb_frame);
    avcodec_close(pCodecCtx);
    avcodec_free_context(&pCodecCtx);
    avformat_free_context(pFormatCtx);
    (*env)->ReleaseStringUTFChars(env, inputString, inputString);
};
