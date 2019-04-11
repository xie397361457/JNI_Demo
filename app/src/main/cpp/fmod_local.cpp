//
// Created by CQNNJ-003 on 2019/4/10.
//
#include "com_jni_ndk_SoundUntil.h"
#include <jni.h>
#include <unistd.h>
#include "fmod.hpp"
#include "fmod/common.h"

using namespace FMOD;
System     *systems;
Sound      *sound1;
Channel    *channel = 0;
FMOD::DSP          *dsp    = 0;
FMOD_RESULT       result;
float frequency = 0;
unsigned int      version;
void             *extradriverdata = 0;
#define MY_FMOD_DSP_TYPE_LUO_LI 1
#define MY_FMOD_DSP_TYPE_DA_SHU 2
#define MY_FMOD_DSP_TYPE_JINGSONG 3
#define MY_FMOD_DSP_TYPE_HUISHENG 4
#define MY_FMOD_DSP_TYPE_KONGLING 5
#define MY_FMOD_DSP_TYPE_GUAOGUAI 6
#define MY_FMOD_DSP_TYPE_XIAOHUANGREN 7
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_play_1sound
        (JNIEnv *env, jclass cls, jstring sound_path){
    const char *path = env->GetStringUTFChars(sound_path, 0);

    result = System_Create(&systems);
    LOG_I("%s","System_Create");

    result = systems->getVersion(&version);
    LOG_I("%s","getVersion");

    //初始化系统
    result = systems->init(32, FMOD_INIT_NORMAL, extradriverdata);
    LOG_I("%s","init");
    //创建音频
    result = systems->createSound(path, FMOD_DEFAULT, 0, &sound1);
    LOG_I("%s","createSound");
        //播放音频
    result = systems->playSound(sound1, 0, false, &channel);
    LOG_I("%s","playSound");
    result = systems->update();
    LOG_I("%s","update");
    env->ReleaseStringUTFChars(sound_path, path);
}

JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_close_1sound
        (JNIEnv *env, jclass cls) {
    //释放资源
    result = sound1->release();
    LOG_I("%s","release");
    result = systems->close();
    LOG_I("%s","close");
    result = systems->release();
    LOG_I("%s","release");
}
JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_destroy_1sound
        (JNIEnv *env, jclass cls){

}

JNIEXPORT void JNICALL Java_com_jni_ndk_SoundUntil_play_1effect
        (JNIEnv *env, jclass cls, jstring jsr_path,jint type){
    const char *c_path = env->GetStringUTFChars(jsr_path, 0);

    System_Create(&systems);
    systems->init(32, FMOD_INIT_NORMAL, extradriverdata);
    switch (type){
        case MY_FMOD_DSP_TYPE_LUO_LI:
            systems->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
            //设置音调参数
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,2.5);
            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            //播放音频
            systems->playSound(sound1, 0, false, &channel);
            channel->addDSP(0,dsp);
            break;
        case MY_FMOD_DSP_TYPE_DA_SHU:
            systems->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
            //设置音调参数
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,0.8);
            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            //播放音频
            systems->playSound(sound1, 0, false, &channel);
            channel->addDSP(0,dsp);
            break;
        case MY_FMOD_DSP_TYPE_JINGSONG:
            systems->createDSPByType(FMOD_DSP_TYPE_TREMOLO,&dsp);
            //设置音调参数
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.5);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20);

            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            //播放音频
            systems->playSound(sound1, 0, false, &channel);
            channel->addDSP(0,dsp);
            break;
        case MY_FMOD_DSP_TYPE_HUISHENG:
            systems->createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp);

            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 500);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 22);
            dsp->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, -15);

            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            //播放音频
            systems->playSound(sound1, 0, false, &channel);
            channel->addDSP(0,dsp);
            break;
        case MY_FMOD_DSP_TYPE_KONGLING:
            //空灵
            systems->createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,300);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,20);
            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            systems->playSound(sound1, 0, false, &channel);
            channel->addDSP(0,dsp);
            break;
        case MY_FMOD_DSP_TYPE_GUAOGUAI:
            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            systems->playSound(sound1, 0, false, &channel);
            channel->getFrequency(&frequency);
            frequency = frequency * 1.6;
            channel->setFrequency(frequency);
            break;
        case MY_FMOD_DSP_TYPE_XIAOHUANGREN:
            systems->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
            //设置音调参数
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,2.5);
            //创建音频
            systems->createSound(c_path, FMOD_DEFAULT, 0, &sound1);
            //播放音频
            systems->playSound(sound1, 0, false, &channel);
            channel->getFrequency(&frequency);
            frequency = frequency * 1.2;
            channel->setFrequency(frequency);
            channel->addDSP(0,dsp);
            break;
        default:
            break;
    }
    systems->update();
    env->ReleaseStringUTFChars(jsr_path, c_path);
}