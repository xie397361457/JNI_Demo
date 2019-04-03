package com.jni.ndk;

public class DecodeYUV {

    public  static native void decode(String inputString,String outputString);

    static {
        System.loadLibrary("libavcodec");
        System.loadLibrary("libavdevice");
        System.loadLibrary("libavfilter");
        System.loadLibrary("libavformat");
        System.loadLibrary("libavutil");
        System.loadLibrary("libswresample");
        System.loadLibrary("libswscale");
        System.loadLibrary("jni_demo");
    }
}
