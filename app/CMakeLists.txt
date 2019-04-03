# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(ffmpeg-include ${CMAKE_SOURCE_DIR}/src/main/cpp/include)
set(ffmpeg-so-path ${CMAKE_SOURCE_DIR}/src/main/jniLibs)
#FFmpeg头文件
include_directories(${ffmpeg-include})
#FFmpeg动态库
add_library(libavcodec SHARED IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libavcodec.so)
add_library(libavdevice SHARED IMPORTED)
set_target_properties(libavdevice PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libavdevice.so)
add_library(libavfilter SHARED IMPORTED)
set_target_properties(libavfilter PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libavfilter.so)
add_library(libavformat SHARED IMPORTED)
set_target_properties(libavformat PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libavformat.so)
add_library(libavutil SHARED IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libavutil.so)
add_library(libswresample SHARED IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libswresample.so)
add_library(libswscale SHARED IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORT_LOCATION ${ffmpeg-so-path}/${ANDROID_ABI}/libswscale.so)
#本地代码
add_library(jni_demo SHARED src/main/cpp/cryptor.c src/main/cpp/file.c src/main/cpp/posix_thread.c src/main/cpp/decode_yuv.c)
#log库
find_library(log-lib log )
#链接各个库
target_link_libraries(jni_demo  ${Lffmpeg} ${log-lib} )