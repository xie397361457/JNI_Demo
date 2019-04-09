//
// Created by CQNNJ-003 on 2019/4/8.
//
#include <jni.h>
/* Header for class com_jni_ndk_FileNative */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#ifndef _Included_com_jni_ndk_FileNative
#define _Included_com_jni_ndk_FileNative
#define LOG_I(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"C_LOG",FORMAT,__VA_ARGS__)
#define LOG_E(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"C_LOG",FORMAT,__VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_jni_ndk_FFmpegPlayer_nicePlay(JNIEnv *env, jclass type, jstring input_, jobject surface);

#ifdef __cplusplus
}
#endif
#endif