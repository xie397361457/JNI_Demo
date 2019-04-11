package com.jni.ndk;

/**
 * @auth NiceXjj
 * @date 2019/4/10
 * @time 16:59
 */

public class SoundUntil {

    public static final int MY_FMOD_DSP_TYPE_LUO_LI = 1;
    public static final int MY_FMOD_DSP_TYPE_DA_SHU = 2;
    public static final int MY_FMOD_DSP_TYPE_JINGSONG = 3;
    public static final int MY_FMOD_DSP_TYPE_HUISHENG = 4;
    public static final int MY_FMOD_DSP_TYPE_KONGLING = 5;
    public static final int MY_FMOD_DSP_TYPE_GUAOGUAI = 6;
    public static final int MY_FMOD_DSP_TYPE_XIAOHUANGREN = 7;

    public  static native void play_sound(String path);
    public  static native void close_sound();
    public  static native void destroy_sound();
    public  static native void play_effect(String path,int type);
    static {
        System.loadLibrary("fmodL");
        System.loadLibrary("fmod");
        System.loadLibrary("jni_demo");
    }
}
