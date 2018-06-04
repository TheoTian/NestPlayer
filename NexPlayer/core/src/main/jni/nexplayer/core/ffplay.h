
#ifndef FFPLAY_H
#define FFPLAY_H

extern "C" {
#include "common.h"

#include <stdio.h>
#include <assert.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>

#include "ffplay_def.h"

jint TestPlay(JNIEnv *env, jobject jobj, jstring url, jobject surface);
}
#endif