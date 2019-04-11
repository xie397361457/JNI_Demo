/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jni_ndk_SoundUntil */
#include <android/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _Included_com_jni_ndk_SoundUntil
#define _Included_com_jni_ndk_SoundUntil

#define LOG_I(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"C_LOG",FORMAT,__VA_ARGS__)
#define LOG_E(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"C_LOG",FORMAT,__VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jni_ndk_SoundUntil
 * Method:    play_sound
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_play_1sound
  (JNIEnv *, jclass, jstring);
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_close_1sound
  (JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_destroy_1sound
  (JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_play_1effect
  (JNIEnv *, jclass, jstring,jint);
#ifdef __cplusplus
}
#endif
#endif