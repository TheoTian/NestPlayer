//
// Created by theotian on 17/12/22.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "common.h"

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

/**
 * TEST ZONE
 */
JNIEXPORT jstring JNICALL Native_Test(JNIEnv *env, jobject jobj) {
    char *str = "Native Test";
//    test(0,NULL);
    return env->NewStringUTF(str);
}

//
///* Called from the main */
//static int test(int argc, char **argv)
//{
//    int flags;
//    VideoState *is;
//
//    init_dynload();
//
//    av_log_set_flags(AV_LOG_SKIP_REPEATED);
//    parse_loglevel(argc, argv, options);
//
//    /* register all codecs, demux and protocols */
//#if CONFIG_AVDEVICE
//    avdevice_register_all();
//#endif
//    avformat_network_init();
//
//    init_opts();
//
//    signal(SIGINT , sigterm_handler); /* Interrupt (ANSI).    */
//    signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */
//
//    show_banner(argc, argv, options);
//
//    parse_options(NULL, argc, argv, options, opt_input_file);
//
//    if (!input_filename) {
//        show_usage();
//        av_log(NULL, AV_LOG_FATAL, "An input file must be specified\n");
//        av_log(NULL, AV_LOG_FATAL,
//               "Use -h to get full help or, even better, run 'man %s'\n", program_name);
//        exit(1);
//    }
//
//    if (display_disable) {
//        video_disable = 1;
//    }
//    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
//    if (audio_disable)
//        flags &= ~SDL_INIT_AUDIO;
//    else {
//        /* Try to work around an occasional ALSA buffer underflow issue when the
//         * period size is NPOT due to ALSA resampling by forcing the buffer size. */
//        if (!SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE"))
//            SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE","1", 1);
//    }
//    if (display_disable)
//        flags &= ~SDL_INIT_VIDEO;
//    if (SDL_Init (flags)) {
//        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
//        av_log(NULL, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
//        exit(1);
//    }
//
//    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
//    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
//
//    av_init_packet(&flush_pkt);
//    flush_pkt.data = (uint8_t *)&flush_pkt;
//
//    if (!display_disable) {
//        int flags = SDL_WINDOW_HIDDEN;
//        if (borderless)
//            flags |= SDL_WINDOW_BORDERLESS;
//        else
//            flags |= SDL_WINDOW_RESIZABLE;
//        window = SDL_CreateWindow(program_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, default_width, default_height, flags);
//        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
//        if (window) {
//            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//            if (!renderer) {
//                av_log(NULL, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n", SDL_GetError());
//                renderer = SDL_CreateRenderer(window, -1, 0);
//            }
//            if (renderer) {
//                if (!SDL_GetRendererInfo(renderer, &renderer_info))
//                    av_log(NULL, AV_LOG_VERBOSE, "Initialized %s renderer.\n", renderer_info.name);
//            }
//        }
//        if (!window || !renderer || !renderer_info.num_texture_formats) {
//            av_log(NULL, AV_LOG_FATAL, "Failed to create window or renderer: %s", SDL_GetError());
//            do_exit(NULL);
//        }
//    }
//
//    is = stream_open(input_filename, file_iformat);
//    if (!is) {
//        av_log(NULL, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
//        do_exit(NULL);
//    }
//
//    event_loop(is);
//
//    /* never returns */
//
//    return 0;
//}


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
        {"_test", "()Ljava/lang/String;", (void *) Native_Test},
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



