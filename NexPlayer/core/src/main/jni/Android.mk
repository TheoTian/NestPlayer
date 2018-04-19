LOCAL_PATH := $(call my-dir)

MY_APP_JNI_ROOT := $(realpath $(LOCAL_PATH))
MY_APP_PRJ_ROOT := $(realpath $(MY_APP_JNI_ROOT)/../../../..)
MY_APP_ANDROID_ROOT := $(realpath $(MY_APP_PRJ_ROOT)/..)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(MY_APP_ANDROID_ROOT)/build/ffmpeg-armv7a/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)
endif
ifeq ($(TARGET_ARCH_ABI),armeabi)
MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(MY_APP_ANDROID_ROOT)/build/ffmpeg-armv5/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)
endif
ifeq ($(TARGET_ARCH_ABI),x86)
MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(MY_APP_ANDROID_ROOT)/build/ffmpeg-x86/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
MY_APP_FFMPEG_OUTPUT_PATH := $(realpath $(MY_APP_ANDROID_ROOT)/build/ffmpeg-arm64-v8a/output)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(MY_APP_FFMPEG_OUTPUT_PATH)/include)
endif

include $(call all-subdir-makefiles)
