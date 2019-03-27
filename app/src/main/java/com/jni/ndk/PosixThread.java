package com.jni.ndk;

public class PosixThread {

    public static native void posixThread();
    //初始化 class
    public static native void init();
    //销毁
    public static native void destroy();
    static {
        System.loadLibrary("jni_demo");
    }
}
