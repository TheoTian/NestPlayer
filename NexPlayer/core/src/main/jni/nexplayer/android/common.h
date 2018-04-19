#ifndef __COMMON_H__
#define __COMMON_H__

#include <jni.h>
#include <android/log.h>

#define RES_OK 0
#define RES_ERROR -1

#define IN
#define OUT

#define SAFE_DELETE_JOBJ_REF(env, jobj)    \
if(jobj != NULL) {   \
env->DeleteLocalRef(jobj);   \
jobj = NULL; \
}   \

#define SAFE_DELETE_ARRAY_JOBJ_REF(env, jarray_obj)   \
if (jarray_obj != NULL) {   \
for (int i = 0; i < env->GetArrayLength(jarray_obj); i++) { \
jobject j_element = env->GetObjectArrayElement(jarray_obj, i);  \
SAFE_DELETE_JOBJ_REF(env, j_element);   \
}   \
SAFE_DELETE_JOBJ_REF(env, jarray_obj);  \
}   \

//ffmpeg log
#define AV_LOG_QUIET    -8
#define AV_LOG_PANIC     0
#define AV_LOG_FATAL     8
#define AV_LOG_ERROR    16
#define AV_LOG_WARNING  24
#define AV_LOG_INFO     32
#define AV_LOG_VERBOSE  40
#define AV_LOG_DEBUG    48

#define MAX_STR_SIZE (2*1024)

#ifndef DEBUG
#define DEBUG 1
#endif

#ifdef DEBUG
#define TAG "nexplayer"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else
#define LOGV(...) {}
#define LOGD(...) {}
#define LOGI(...) {}
#define LOGW(...) {}
#define LOGE(...) {}
#endif
#endif /* __COMMON_H__ */
