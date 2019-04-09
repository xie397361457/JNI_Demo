package com.jni.ndk;

import android.view.Surface;

/**
 * @auth NiceXjj
 * @date 2019/4/8
 * @time 15:48
 */

public class FFmpegPlayer {

    public static native void nicePlay(String input, Surface surface);

    static {
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
        System.loadLibrary("yuv");
        System.loadLibrary("jni_demo");
    }
}
