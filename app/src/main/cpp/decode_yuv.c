//
// Created by CQNNJ-003 on 2019/4/2.
//
#include "com_jni_ndk_DecodeYUV.h"
//编码
#include "include/libavcodec/avcodec.h"
//封装格式处理
#include "include/libavformat/avformat.h"
//像素处理
#include "include/libswscale/swscale.h"

JNIEXPORT void JNICALL
Java_com_jni_ndk_DecodeYUV_decode(JNIEnv *env, jclass jcls, jstring inputString_,
                                  jstring outputString_) {
    const char *inputString = (*env)->GetStringUTFChars(env, inputString_, 0);
    const char *outputString = (*env)->GetStringUTFChars(env, outputString_, 0);

    //1.注册所有组件 方法过时 已经不需要了
    //av_register_all()
    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信
    //需要释放资源
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    if(avformat_open_input(&pFormatCtx,inputString,NULL,NULL) != 0){
        LOG_I("%s","无法打开输入视频文件");
        return;
    }
    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOG_I("%s","无法打开输入视频文件");
        return;
    }
    //获取视频流的索引位置
    //遍历索引类型的流(音视频,视频流,字幕流),找到视频流
    int v_stream_idx = -1;
    int i = 0;
    for (; i < pFormatCtx->nb_streams; i++) {
        //流的类型
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            v_stream_idx = i;
            break;
        }
    }
    if (v_stream_idx == -1){
        LOG_I("%s","找不到视频流\n");
        return;
    }

    //只有知道视频的编码方式,才能根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;
    //根据编码上下文中的编码id查找对应的解码
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL){
        LOG_I("%s","找不到解码器\n");
        return;
    }
    //打开解码器
    if (avcodec_open2(pCodecCtx,pCodec,NULL)<0){
        LOG_I("%s","解码器无法打开\n");
        return;
    }
    //输出视频信息
    LOG_I("视频的文件格式：%s",pFormatCtx->iformat->name);
    LOG_I("视频时长：%d", (pFormatCtx->duration)/1000000);
    LOG_I("视频的宽高：%d,%d",pCodecCtx->width,pCodecCtx->height);
    LOG_I("解码器的名称：%s",pCodec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据(eg:H264)
    //缓冲区 开辟空间
    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *pFrame = av_frame_alloc();
    //YUV420
    AVFrame *pFrameYUV = av_frame_alloc();
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height));
    //初始化缓冲区
    avpicture_fill((AVPicture*)pFrameYUV,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);

    //用于转码的参数,转之前得宽高,转之后的宽高,格式等
    struct SwsContext *sws_ctx = (pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
    pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

    //释放资源
    avformat_free_context(pFormatCtx);
    (*env)->ReleaseStringUTFChars(env, inputString_, inputString);
    (*env)->ReleaseStringUTFChars(env, outputString_, outputString);
}