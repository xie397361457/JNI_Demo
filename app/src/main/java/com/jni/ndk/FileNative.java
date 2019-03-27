package com.jni.ndk;

public class FileNative {

    public native static void diff(String diff_path,String pattern_path,int count);

    public native static void merge(String pattern_path,int count,String merge_path);

    static {
        System.loadLibrary("jni_demo");
    }
}
