//
// Created by theotian on 17/12/22.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "common.h"
#include "ffplay.h"

static char *class_NativeMediaPlayer = "com/theo/nexplayer/jni/NativeMediaPlayer";

/**
 * global params
 */
static JavaVM *g_VM = NULL;

struct g_classes_t {
};

struct g_fields_t {
};

struct g_methods_t {
};

static g_classes_t g_classes;
static g_fields_t g_fields;
static g_methods_t g_methods;

/**
 * API
 */

/**
 * callback to java
 * @param ret
 * @param vid
 * @param stream_id
 */
void callback(int ret, void *p_data) {
    LOGD("cache_callback ret:%d, p_data:%p", ret, p_data);
    if (g_VM == NULL) {
        return;
    }
    /**
     * attach thread
     */
    JNIEnv *env = NULL;
    int need_detach_at_last = false;
    int env_status = g_VM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    /**
     * 如果外部已经将env attach to vm,则由外部对该env detach
     * 否则,将主动attach当前thread env,并在结束的时候进行detach
     */
    if (env_status != JNI_OK
        && g_VM->AttachCurrentThread(&env, NULL) == JNI_OK) {
        need_detach_at_last = true;
    }

    if (env == NULL) {
        goto EXIT;
    } else {

    }
    EXIT:
    if (need_detach_at_last) {
        g_VM->DetachCurrentThread();
    }

    return;
}


JNIEXPORT jint JNICALL Native_TestPlay(JNIEnv *env, jobject jobj, jstring url, jobject surface) {
    return TestPlay(env, jobj, url, surface);
}

/**
 * TEST ZONE
 */
JNIEXPORT jstring JNICALL Native_Test(JNIEnv *env, jobject jobj) {
    char *str = "Native Test";
    return env->NewStringUTF(str);
}

/**
 * 初始化全局参数
 * @param env
 * @param javaClass
 */
static void initGlobalParams(JNIEnv *env, jclass javaClass) {
    /**
    * new global ref.
    * 使用全局引用保存，会占用引用表数量，直到进程结束
    */

}


static JNINativeMethod nativeMethods[] = {
        {"_test",     "()Ljava/lang/String;",                        (void *) Native_Test},
        {"_testplay", "(Ljava/lang/String;Landroid/view/Surface;)I", (void *) Native_TestPlay},
};

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    g_VM = vm;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass(class_NativeMediaPlayer);
    if (javaClass == NULL) {
        return JNI_ERR;
    }
    if (env->RegisterNatives(javaClass, nativeMethods,
                             sizeof(nativeMethods) / sizeof(nativeMethods[0])) < 0) {
        return JNI_ERR;
    }

    initGlobalParams(env, javaClass);

    return JNI_VERSION_1_6;
}



