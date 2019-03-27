
#include <pthread.h>
#include <unistd.h>
#include "com_jni_ndk_PosixThread.h"
JavaVM *javaVM ;
jclass uuid_class_global;
//android SDK 2.2之后 才有这个函数
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
    LOG_I("%s","JNI_OnLoad");
    javaVM = vm;
    return JNI_VERSION_1_4;
}
//初始化class 转换成全局变量
JNIEXPORT void JNICALL
Java_com_jni_ndk_PosixThread_init(JNIEnv *env, jclass jcls){
    LOG_I("%s","init");
    jclass j_class = (*env)->FindClass(env,"com/jni/ndk/UUIDUtils");
    //将class转换成全局变量
    uuid_class_global = (*env)->NewGlobalRef(env,j_class);
}
//销毁全局变量
JNIEXPORT void JNICALL
Java_com_jni_ndk_PosixThread_destroy(JNIEnv *env, jclass jcls){
    LOG_I("%s","destroy");
    (*env)->DeleteGlobalRef(env,uuid_class_global);
}
void* th_fun(void* arg){
    JNIEnv* env = NULL;
    //通过javaVm关联当前线程,获取当前线程的JNIEnv
    (*javaVM)->AttachCurrentThread(javaVM,&env,NULL);
    jmethodID get_mid = (*env)->GetStaticMethodID(env,uuid_class_global,"get","()Ljava/lang/String;");
    char* no = (char*)arg;
    int i;
    for (i = 0;i<5;i++){
        LOG_I("thread %s, i : %d",no,i);
        jstring uuid_str = (*env)->CallStaticObjectMethod(env,uuid_class_global,get_mid);
        char* uuid_cstr = (*env)->GetStringUTFChars(env,uuid_str,NULL);
        LOG_I("%s",uuid_cstr);
        if (i == 4){
            pthread_exit((void*)0);
        }
        (*env)->ReleaseStringUTFChars(env,uuid_str,uuid_cstr);
        sleep(1);
    }
    //解除关联
    (*javaVM)->DetachCurrentThread(javaVM);
}
//JavaVM代表是的java虚拟机,所有的工作都是从javaVm开始
//可以通过javaVm获取到每个线程关联的jniEnv
//如何获取JavaVM?
//1.再JNI_OnLoad函数中获取
//2.(*env)->GetJavaVM(env,&javaVM);
//每个线程都有独立得JNIEnv
JNIEXPORT void JNICALL Java_com_jni_ndk_PosixThread_posixThread
(JNIEnv *env, jclass jclass){

    //(*env)->GetJavaVM(env,&javaVM);
    LOG_I("%s","posixThread");
    //创建多线程
    pthread_t tid;
    pthread_create(&tid,NULL,th_fun,(void*)"NO1");
}