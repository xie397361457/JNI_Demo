//
// Created by CQNNJ-003 on 2019/4/2.
//
#include <pthread.h>
#include "com_jni_ndk_DecodeYUV.h"
//编码
#include "ffmpeg/include/libavcodec/avcodec.h"
//封装格式处理
#include "ffmpeg/include/libavformat/avformat.h"
//像素处理
#include "ffmpeg/include/libswscale/swscale.h"
#include "ffmpeg/include/libavutil/avutil.h"
#include "ffmpeg/include/libavutil/frame.h"
#include "ffmpeg/include/libavutil/imgutils.h"

//PS 使用 YUV 播放器 播放时要根据视频实际宽高设置 播放器宽高

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
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            v_stream_idx = i;
            break;
        }
    }
    if (v_stream_idx == -1){
        LOG_I("%s","找不到视频流\n");
        return;
    }

    //只有知道视频的编码方式,才能根据编码方式去找到解码器
    //根据编码上下文中的编码id查找对应的解码
    const AVCodec *pCodec = avcodec_find_decoder(pFormatCtx->streams[i]->codecpar->codec_id);
    //创建视频流中的编解码上下文
    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    //将paramter转换成编码器上下文
    avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[i]->codecpar);
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
    LOG_I("视频时长：%lld", (pFormatCtx->duration)/1000000);
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
    uint8_t *out_buffer = av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height,1));
    //初始化缓冲区
    // * @param dst_data      data pointers to be filled in
    // * @param dst_linesize  linesizes for the image in dst_data to be filled in
    // * @param src           buffer which will contain or contains the actual image data, can be NULL
    // * @param pix_fmt       the pixel format of the image
    // * @param width         the width of the image in pixels
    // * @param height        the height of the image in pixels
    // * @param align         the value used in src for linesize alignment
    // * @return the size in bytes required for src, a negative error code
    av_image_fill_arrays(pFrameYUV->data,pFrameYUV->linesize,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height,1);
    //avpicture_fill((AVPicture*)pFrameYUV,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);

    //用于转码的参数,转之前得宽高,转之后的宽高,格式等
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
    pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

     FILE *fp_yuv = fopen(outputString,"wb+");
    int got_picture,ret;
    int frame_count = 0;
    //一帧一帧的读取压缩数据
    while(av_read_frame(pFormatCtx,packet)>=0){
        //只要视频压缩数据(根据流的索引位置判断)
        if (packet->stream_index == v_stream_idx){
            //解码一帧视频压缩数据,得到视频像素数据
            //int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt);
            ret = avcodec_send_packet(pCodecCtx,packet);
            // int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame);
            got_picture = avcodec_receive_frame(pCodecCtx,pFrame);
            // ret = avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,packet);
            if (ret != 0){
                LOG_I("%s","解码错误");
                return;
            }

            //为0说明解码完成,非0正在解码
            if(got_picture == 0){
                //AVFrame转为像素格式YUV420，宽高
                //2 6输入、输出数据
                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
                //4 输入数据第一列要转码的位置 从0开始
                //5 输入画面的高度
                //struct SwsContext *c, const uint8_t *const srcSlice[],
                //              const int srcStride[], int srcSliceY, int srcSliceH,
                //              uint8_t *const dst[], const int dstStride[]
                //uint8_t *data[AV_NUM_DATA_POINTERS];
                sws_scale(sws_ctx,pFrame->data,pFrame->linesize,
                          0,pCodecCtx->height,pFrameYUV->data,pFrameYUV->linesize);

                //输出到YUV文件
                //AVFrame像素帧写入文件
                //data解码后的图像像素数据（音频采样数据）
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
                //U V 个数是Y的1/4
                int y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);

                frame_count++;
                LOG_I("解码第%d帧",frame_count);
            }
        }
        //释放资源
        av_packet_unref(packet);
    }

    //释放资源
    fclose(fp_yuv);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_free_context(pFormatCtx);
    (*env)->ReleaseStringUTFChars(env, inputString_, inputString);
    (*env)->ReleaseStringUTFChars(env, outputString_, outputString);
}

//JNIEXPORT void JNICALL
//Java_com_jni_ndk_DecodeYUV_decode(JNIEnv *env, jclass jcls, jstring input_jstr,
//                                  jstring output_jstr) {
//    //需要转码的视频文件(输入的视频文件)
//    const char* input_cstr = (*env)->GetStringUTFChars(env,input_jstr,NULL);
//    const char* output_cstr = (*env)->GetStringUTFChars(env,output_jstr,NULL);
//
//    //1.注册所有组件
//    av_register_all();
//
//    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
//    AVFormatContext *pFormatCtx = avformat_alloc_context();
//
//    //2.打开输入视频文件
//    if (avformat_open_input(&pFormatCtx, input_cstr, NULL, NULL) != 0)
//    {
//        LOG_I("%s","无法打开输入视频文件");
//        return;
//    }
//
//    //3.获取视频文件信息
//    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
//    {
//        LOG_I("%s","无法获取视频文件信息");
//        return;
//    }
//
//    //获取视频流的索引位置
//    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
//    int v_stream_idx = -1;
//    int i = 0;
//    //number of streams
//    for (; i < pFormatCtx->nb_streams; i++)
//    {
//        //流的类型
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
//        {
//            v_stream_idx = i;
//            break;
//        }
//    }
//
//    if (v_stream_idx == -1)
//    {
//        LOG_I("%s","找不到视频流\n");
//        return;
//    }
//
//    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
//    //获取视频流中的编解码上下文
//    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;
//    //4.根据编解码上下文中的编码id查找对应的解码
//    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//    //（迅雷看看，找不到解码器，临时下载一个解码器）
//    if (pCodec == NULL)
//    {
//        LOG_I("%s","找不到解码器\n");
//        return;
//    }
//
//    //5.打开解码器
//    if (avcodec_open2(pCodecCtx,pCodec,NULL)<0)
//    {
//        LOG_I("%s","解码器无法打开\n");
//        return;
//    }
//
//    //输出视频信息
//    LOG_I("视频的文件格式：%s",pFormatCtx->iformat->name);
//    LOG_I("视频时长：%lld", (pFormatCtx->duration)/1000000);
//    LOG_I("视频的宽高：%d,%d",pCodecCtx->width,pCodecCtx->height);
//    LOG_I("解码器的名称：%s",pCodec->name);
//
//    //准备读取
//    //AVPacket用于存储一帧一帧的压缩数据（H264）
//    //缓冲区，开辟空间
//    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));
//
//    //AVFrame用于存储解码后的像素数据(YUV)
//    //内存分配
//    AVFrame *pFrame = av_frame_alloc();
//    //YUV420
//    AVFrame *pFrameYUV = av_frame_alloc();
//    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
//    //缓冲区分配内存
//    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
//    //初始化缓冲区
//    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
//
//    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
//    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
//                                                pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
//                                                SWS_BICUBIC, NULL, NULL, NULL);
//
//
//    int got_picture, ret;
//
//    FILE *fp_yuv = fopen(output_cstr, "wb+");
//
//    int frame_count = 0;
//
//    //6.一帧一帧的读取压缩数据
//    while (av_read_frame(pFormatCtx, packet) >= 0)
//    {
//        //只要视频压缩数据（根据流的索引位置判断）
//        if (packet->stream_index == v_stream_idx)
//        {
//            //7.解码一帧视频压缩数据，得到视频像素数据
//            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
//            if (ret < 0)
//            {
//                LOG_I("%s","解码错误");
//                return;
//            }
//
//            //为0说明解码完成，非0正在解码
//            if (got_picture)
//            {
//                //AVFrame转为像素格式YUV420，宽高
//                //2 6输入、输出数据
//                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
//                //4 输入数据第一列要转码的位置 从0开始
//                //5 输入画面的高度
//                sws_scale(sws_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
//                          pFrameYUV->data, pFrameYUV->linesize);
//
//                //输出到YUV文件
//                //AVFrame像素帧写入文件
//                //data解码后的图像像素数据（音频采样数据）
//                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
//                //U V 个数是Y的1/4
//                int y_size = pCodecCtx->width * pCodecCtx->height;
//                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
//                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
//                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);
//
//                frame_count++;
//                LOG_I("解码第%d帧",frame_count);
//            }
//        }
//
//        //释放资源
//        av_free_packet(packet);
//    }
//
//    fclose(fp_yuv);
//
//    (*env)->ReleaseStringUTFChars(env,input_jstr,input_cstr);
//    (*env)->ReleaseStringUTFChars(env,output_jstr,output_cstr);
//
//    av_frame_free(&pFrame);
//
//    avcodec_close(pCodecCtx);
//
//    avformat_free_context(pFormatCtx);
//}