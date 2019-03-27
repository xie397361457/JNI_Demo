
#include "com_jni_ndk_Cryptor.h"
//
// Created by CQNNJ-003 on 2019/3/5.
//
char password[] = "ilovexyp";//可能会被找到密码,可以选择应用签名来MDD 在 C里面获取
        //加密
JNIEXPORT void JNICALL Java_com_jni_ndk_Cryptor_crypt
        (JNIEnv *env, jclass jclss, jstring jni_normal_path, jstring jni_crypt_path){
    //jstring ->char*
    const char* normal_path = (*env)->GetStringUTFChars(env,jni_normal_path,NULL);
    const char* crypt_path = (*env)->GetStringUTFChars(env,jni_crypt_path,NULL);

    //打开文件
    FILE *normal_fp = fopen(normal_path, "rb");
    FILE *crypt_fp = fopen(crypt_path, "wb");

    //一次读取一个字符
    int ch;
    int i = 0;//循环使用密码中的字母进行异或运算
    int pwd_len = strlen(password);//密码的长度

    while ((ch = fgetc(normal_fp)) != EOF) //End of File
    {
        //写入(异或运算)
        fputc(ch ^ password[i%pwd_len], crypt_fp);
        i++;
    }

    fclose(normal_fp);
    fclose(crypt_fp);
    //释放
    (*env)->ReleaseStringUTFChars(env,jni_normal_path,normal_path);
    (*env)->ReleaseStringUTFChars(env,jni_crypt_path,crypt_path);
}
//解密
JNIEXPORT void JNICALL Java_com_jni_ndk_Cryptor_decrypt
        (JNIEnv *env, jclass jclss, jstring jni_crypt_path, jstring jni_decrypt_path){

    //jstring ->char*
    const char* crypt_path = (*env)->GetStringUTFChars(env,jni_crypt_path,NULL);
    const char* decrypt_path = (*env)->GetStringUTFChars(env,jni_decrypt_path,NULL);

    //打开文件
    FILE *crypt_fp = fopen(crypt_path, "rb");
    FILE *decrypt_fp = fopen(decrypt_path, "wb");
    //一次读取一个字符
    int ch;
    int i = 0;//循环使用密码中的字母进行异或运算
    int pwd_len = strlen(password);
    while ((ch = fgetc(crypt_fp)) != EOF) //End of File
    {
        //写入(异或运算)
        fputc(ch ^ password[i%pwd_len], decrypt_fp);
        i++;
    }
    fclose(crypt_fp);
    fclose(decrypt_fp);


    //释放
    (*env)->ReleaseStringUTFChars(env,jni_crypt_path,crypt_path);
    (*env)->ReleaseStringUTFChars(env,jni_decrypt_path,decrypt_path);
}


JNIEXPORT jstring JNICALL
Java_com_jni_ndk_Cryptor_getString(JNIEnv *env, jclass jclss){

    const char *str = "你好嘛";
    jstring jstring_str = (*env)->NewStringUTF(env,str);
    LOG_I("%s",str);
    return jstring_str;

}