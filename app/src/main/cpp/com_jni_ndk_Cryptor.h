/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jni_ndk_Cryptor */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#ifndef _Included_com_jni_ndk_Cryptor
#define _Included_com_jni_ndk_Cryptor
#define LOG_I(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"C_LOG",FORMAT,__VA_ARGS__)
#ifdef __cplusplus
extern "C" {

#endif
/*
 * Class:     com_jni_ndk_Cryptor
 * Method:    crypt
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_jni_ndk_Cryptor_crypt
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_jni_ndk_Cryptor
 * Method:    decrypt
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_jni_ndk_Cryptor_decrypt
  (JNIEnv *, jclass, jstring, jstring);

JNIEXPORT jstring JNICALL
Java_com_jni_ndk_Cryptor_getString(JNIEnv *, jclass );



#ifdef __cplusplus
}
#endif
#endif
