package com.jni.ndk;

public class DecodeYUV {

    public  static native void decode(String inputString,String outputString);

    static {
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
        System.loadLibrary("jni_demo");
    }
}
