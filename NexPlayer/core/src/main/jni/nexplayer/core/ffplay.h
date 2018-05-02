
#ifndef FFPLAY_H
#define FFPLAY_H

extern "C" {
#include "common.h"

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"

#include <stdio.h>
#include <assert.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>


jint TestPlay(JNIEnv *env, jobject jobj, jstring url, jobject surface);
}
#endif