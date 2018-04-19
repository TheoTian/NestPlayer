
ROOT_PATH := $(call my-dir)
LOCAL_PATH := $(ROOT_PATH)
include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -mfloat-abi=soft
endif
LOCAL_LDLIBS += -llog -landroid

LOCAL_LDFLAGS += -Wl,--hash-style=sysv
libsysv-hash-table-library_ldflags := -Wl,--hash-style=sysv

LOCAL_MODULE    := nexplayer

LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH))
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/../../ffmpeg/include)
LOCAL_C_INCLUDES += $(realpath $(MY_APP_FFMPEG_INCLUDE_PATH))

LOCAL_SRC_FILES := ../core/ffplay.c
LOCAL_SRC_FILES += ./nexplayer_jni.cpp

LOCAL_LDLIBS := -llog
##LOCAL_SHARED_LIBRARIES += ffmpeg

include $(BUILD_SHARED_LIBRARY)
