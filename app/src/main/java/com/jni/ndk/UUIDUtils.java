package com.jni.ndk;

import java.util.UUID;

public class UUIDUtils {
    //()Ljava/lang/String;
    public static String get(){
        return UUID.randomUUID().toString();
    }

}
