# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#--------------------------- ffmpeg ----------------------------- #
add_library(avcodec SHARED IMPORTED)

set_target_properties(avcodec
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavcodec.so)
add_library(avdevice SHARED IMPORTED)

set_target_properties(avdevice
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavdevice.so)
add_library(avfilter SHARED IMPORTED)

set_target_properties(avfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavfilter.so)
 add_library(avformat SHARED IMPORTED)

 set_target_properties(avformat
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavformat.so)
 add_library(avutil SHARED IMPORTED)

 set_target_properties(avutil
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavutil.so)
  add_library(swresample SHARED IMPORTED)

  set_target_properties(swresample
                         PROPERTIES IMPORTED_LOCATION
                         ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswresample.so)
  add_library(swscale SHARED IMPORTED)

  set_target_properties(swscale
                         PROPERTIES IMPORTED_LOCATION
                         ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswscale.so)
  #-----------------------------------    yuv     ------------------------------------- #
  add_library(yuv SHARED IMPORTED)

  set_target_properties(yuv
                            PROPERTIES IMPORTED_LOCATION
                            ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libyuv.so)
  #-----------------------------------    fmod   ------------------------------------- #
  add_library(fmod SHARED IMPORTED)

  set_target_properties(fmod
                              PROPERTIES IMPORTED_LOCATION
                              ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libfmod.so)
  add_library(fmodL SHARED IMPORTED)

  set_target_properties(fmodL
                                PROPERTIES IMPORTED_LOCATION
                                ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libfmodL.so)

add_library( # Sets the name of the library.
             jni_demo

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
              src/main/cpp/cryptor.c
              src/main/cpp/decode_yuv.c
              src/main/cpp/file.c
              src/main/cpp/posix_thread.c
              src/main/cpp/nice_ffmpeg_play.c
              src/main/cpp/fmod_local.cpp
              src/main/cpp/fmod/common.cpp
              src/main/cpp/fmod/common_platform.cpp
              src/main/cpp/fmod/play_sound.cpp
              )
#-----------------------------------    fmod_local   ------------------------------------- #
#add_library( # Sets the name of the library.
             #fmod_local

             # Sets the library as a shared library.
             #SHARED

             # Provides a relative path to your source file(s).

             # )
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
include_directories(src/main/cpp/ffmpeg/include)
include_directories(src/main/cpp/libyuv/libyuv)
include_directories(src/main/cpp/libyuv/libyuv.h)
include_directories(src/main/cpp/fmod/inc)
target_link_libraries( # Specifies the target library.
                       jni_demo
                       #fmod_local
                       avcodec
                       avdevice
                       avfilter
                       avformat
                       avutil
                       swresample
                       swscale
                       yuv
                       fmod
                       fmodL
                       android
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       )