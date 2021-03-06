
extern "C" {
#include "ffplay.h"

#define MAX_AUDIO_FRME_SIZE  2 * 44100

/**
 * flush pkt
 */
static AVPacket flush_pkt;

static int av_sync_type = AV_SYNC_AUDIO_MASTER;

void global_init() {
    /**
     * register
     */
    av_register_all();

    avformat_network_init();

    /**
     * init flush pkt
     */
    av_init_packet(&flush_pkt);

    flush_pkt.data = (uint8_t *) &flush_pkt;
}

static void packet_queue_flush(PacketQueue *q) {
    MyAVPacketList *pkt, *pkt1;

    SDL_LockMutex(q->mutex);
    for (pkt = q->first_pkt; pkt; pkt = pkt1) {
        pkt1 = pkt->next;
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);
    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}

static void packet_queue_destroy(PacketQueue *q) {
    packet_queue_flush(q);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

static void frame_queue_unref_item(Frame *vp) {
    av_frame_unref(vp->frame);
    avsubtitle_free(&vp->sub);
}

static void free_picture(Frame *vp) {
    if (vp->bmp) {
//        SDL_FreeYUVOverlay(vp->bmp);
        vp->bmp = NULL;
    }
}

static void frame_queue_destory(FrameQueue *f) {
    int i;
    for (i = 0; i < f->max_size; i++) {
        Frame *vp = &f->queue[i];
        frame_queue_unref_item(vp);
        av_frame_free(&vp->frame);
        free_picture(vp);
    }
    SDL_DestroyMutex(f->mutex);
    SDL_DestroyCond(f->cond);
}

static int frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last) {
    int i;
    memset(f, 0, sizeof(FrameQueue));
    if (!(f->mutex = SDL_CreateMutex()))
        return AVERROR(ENOMEM);
    if (!(f->cond = SDL_CreateCond()))
        return AVERROR(ENOMEM);
    f->pktq = pktq;
    f->max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
    f->keep_last = !!keep_last;
    for (i = 0; i < f->max_size; i++)
        if (!(f->queue[i].frame = av_frame_alloc()))
            return AVERROR(ENOMEM);
    return 0;
}

/* packet queue handling */
static void packet_queue_init(PacketQueue *q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
    q->abort_request = 1;
}

static void stream_close(VideoState *is) {
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    is->abort_request = 1;
    SDL_WaitThread(is->read_tid, NULL);
    packet_queue_destroy(&is->videoq);
    packet_queue_destroy(&is->audioq);
    packet_queue_destroy(&is->subtitleq);

    /* free all pictures */
    frame_queue_destory(&is->pictq);
    frame_queue_destory(&is->sampq);
    frame_queue_destory(&is->subpq);
    SDL_DestroyCond(is->continue_read_thread);
#if !CONFIG_AVFILTER
    sws_freeContext(is->img_convert_ctx);
#endif
    av_free(is);
}

static void set_clock_at(Clock *c, double pts, int serial, double time) {
    c->pts = pts;
    c->last_updated = time;
    c->pts_drift = c->pts - time;
    c->serial = serial;
}

static void set_clock(Clock *c, double pts, int serial) {
    double time = av_gettime_relative() / 1000000.0;
    set_clock_at(c, pts, serial, time);
}

static void init_clock(Clock *c, int *queue_serial) {
    c->speed = 1.0;
    c->paused = 0;
    c->queue_serial = queue_serial;
    set_clock(c, NAN, -1);
}

/* this thread gets the stream from the disk or the network */
static int read_thread(void *arg) {

}

static VideoState *stream_open(const char *filename, AVInputFormat *iformat) {
    VideoState *is;
    is = (VideoState *) av_mallocz(sizeof(VideoState));
    if (!is)
        return NULL;
    av_strlcpy(is->filename, filename, sizeof(is->filename));
    is->iformat = iformat;
    is->ytop = 0;
    is->xleft = 0;

    /* start video display */
    if (frame_queue_init(&is->pictq, &is->videoq, VIDEO_PICTURE_QUEUE_SIZE, 1) < 0)
        goto fail;
    if (frame_queue_init(&is->subpq, &is->subtitleq, SUBPICTURE_QUEUE_SIZE, 0) < 0)
        goto fail;
    if (frame_queue_init(&is->sampq, &is->audioq, SAMPLE_QUEUE_SIZE, 1) < 0)
        goto fail;

    packet_queue_init(&is->videoq);
    packet_queue_init(&is->audioq);
    packet_queue_init(&is->subtitleq);

    is->continue_read_thread = SDL_CreateCond();

    init_clock(&is->vidclk, &is->videoq.serial);
    init_clock(&is->audclk, &is->audioq.serial);
    init_clock(&is->extclk, &is->extclk.serial);
    is->audio_clock_serial = -1;
    is->av_sync_type = av_sync_type;
    is->read_tid = SDL_CreateThreadEx(&is->_read_tid, read_thread, is, "ff_read");

    if (!is->read_tid) {
        fail:
        stream_close(is);
        return NULL;
    }
    return is;
}

/**
 * just for learning
 *
 * @param env
 * @param jobj
 * @param url
 * @param surface
 * @return
 */
jint TestPlay(JNIEnv *env, jobject jobj, jstring url, jobject surface) {
    int i;

    AVCodecContext *vCodecCtx;
    AVCodecContext *aCodecCtx;

    AVPacket *avPacket;
    AVFrame *avFrame, *pFrameRGBA;
    SwsContext *img_convert_ctx;
    AVFormatContext *pFormatCtx;

    ANativeWindow *nativeWindow;
    ANativeWindow_Buffer windowBuffer;
    uint8_t *v_out_buffer;

    AVCodec *vCodec;
    AVCodec *aCodec;

    char input_str[3000] = {0};
    //读取输入的视频频文件地址
    sprintf(input_str, "%s", env->GetStringUTFChars(url, NULL));
    //初始化
    av_register_all();
    //分配一个AVFormatContext结构
    pFormatCtx = avformat_alloc_context();
    //打开文件
    LOGI("TestPlay input_str:%s", input_str);
    if (avformat_open_input(&pFormatCtx, input_str, NULL, NULL) != 0) {
        LOGD("Couldn't open input stream.\n");
        return -1;
    }
    //查找文件的流信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGD("Couldn't find stream information.\n");
        return -1;
    }
    //在流信息中找到视频流
    int videoindex = -1;
    int audioindex = -1;

    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            LOGD("find videoindex %d.\n", videoindex);
        }
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
            LOGD("find audioindex %d.\n", audioindex);
        }
    }

    if (videoindex == -1) {
        LOGD("Couldn't find a video stream.\n");
        return -1;
    }

    if (audioindex == -1) {
        LOGD("Couldn't find a audio stream.\n");
        return -1;
    }

    //获取相应视频流的解码器
    vCodecCtx = pFormatCtx->streams[videoindex]->codec;
    vCodec = avcodec_find_decoder(vCodecCtx->codec_id);

    aCodecCtx = pFormatCtx->streams[audioindex]->codec;
    aCodec = avcodec_find_decoder(aCodecCtx->codec_id);

    assert(vCodec != NULL);
    assert(aCodec != NULL);

    LOGD("avcodec_find_decoder vCodec name:%s\n", vCodec->name);
    LOGD("avcodec_find_decoder aCodec name:%s\n", aCodec->name);

    LOGD("read container width-height:%d-%d\n", vCodecCtx->width, vCodecCtx->height);

    //打开解码器
    if (avcodec_open2(vCodecCtx, vCodec, NULL) < 0) {
        LOGE("Couldn't open codec.\n");
        return -1;
    }

    //打开解码器
    if (avcodec_open2(aCodecCtx, aCodec, NULL) < 0) {
        LOGE("Couldn't open codec.\n");
        return -1;
    }

    //获取界面传下来的surface
    nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (0 == nativeWindow) {
        LOGE("Couldn't get native window from surface.\n");
        return -1;
    }

    int width = vCodecCtx->width;
    int height = vCodecCtx->height;

    LOGD("width-height:%d-%d\n", width, height);
    //分配一个帧指针，指向解码后的原始帧
    avFrame = av_frame_alloc();
    avPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    pFrameRGBA = av_frame_alloc();
    //绑定输出buffer
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
    v_out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, v_out_buffer, AV_PIX_FMT_RGBA,
                         width, height, 1);
    img_convert_ctx = sws_getContext(width, height, vCodecCtx->pix_fmt,
                                     width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
    if (0 >
        ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBA_8888)) {
        LOGE("Couldn't set buffers geometry.\n");
        ANativeWindow_release(nativeWindow);
        return -1;
    }

    //frame->16bit  44100 PCM 统一音频采样格式与采样率
    SwrContext *swrCtx = swr_alloc();

    //输入采样率格式
    enum AVSampleFormat in_sample_fmt = aCodecCtx->sample_fmt;
    //输出采样率格式16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入采样率
    int in_sample_rate = aCodecCtx->sample_rate;
    //输出采样率
    int out_sample_rate = 44100;
    //获取输入的声道布局
    //根据声道个数获取默认的声道布局(2个声道，默认立体声)
    //av_get_default_channel_layout(pCodeCtx->channels);
    uint64_t in_ch_layout = aCodecCtx->channel_layout;
    //输出的声道布局
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;


    swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout,
                       in_sample_fmt, in_sample_rate, 0, NULL);


    swr_init(swrCtx);

    //获取输入输出的声道个数
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    LOGI("out_count:%d", out_channel_nb);

    jclass cls = env->FindClass("com/theo/nexplayer/jni/AudioUtil");
    //jmethodID
    jmethodID constructor_mid = env->GetMethodID(cls, "<init>", "()V");

    //实例化一个AudioUtil对象(可以在constructor_mid后加参)
    jobject audioutil_obj = env->NewObject(cls,
                                           constructor_mid);   //类似于AudioUtil audioutil =new AudioUtil();

    //AudioTrack对象
    jmethodID create_audio_track_mid = env->GetMethodID(cls, "createAudioTrack",
                                                        "(II)Landroid/media/AudioTrack;");
    jobject audio_track = env->CallObjectMethod(audioutil_obj, create_audio_track_mid,
                                                out_sample_rate, out_channel_nb);

    //调用AudioTrack.play方法
    jclass audio_track_class = env->GetObjectClass(audio_track);
    jmethodID audio_track_play_mid = env->GetMethodID(audio_track_class, "play", "()V");
    env->CallVoidMethod(audio_track, audio_track_play_mid);

    //AudioTrack.write
    jmethodID audio_track_write_mid = env->GetMethodID(audio_track_class, "write", "([BII)I");

    //16bit 44100 PCM 数据
    uint8_t *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRME_SIZE);

    int got_frame = 0, framecount = 0, ret;
    //读取帧
    while (av_read_frame(pFormatCtx, avPacket) >= 0) {
        LOGD("av_read_frame");
        //output audio
        if (avPacket->stream_index == audioindex) {
//解码
            ret = avcodec_decode_audio4(aCodecCtx, avFrame, &got_frame, avPacket);

            if (ret < 0) {
                LOGI("%s", "解码完成");
                break;
            }
            //非0，正在解码
            if (got_frame > 0) {
                LOGI("解码：%d", framecount++);
                swr_convert(swrCtx, &out_buffer, MAX_AUDIO_FRME_SIZE,
                            (const uint8_t **) avFrame->data, avFrame->nb_samples);
                //获取sample的size
                int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb,
                                                                 avFrame->nb_samples,
                                                                 out_sample_fmt, 1);
                //out_buffer 缓冲区数据，转换成byte数组
                jbyteArray audio_sample_array = env->NewByteArray(out_buffer_size);

                jbyte *sample_byte = env->GetByteArrayElements(audio_sample_array, NULL);

                //将out_buffer的数据复制到sample_byte
                memcpy(sample_byte, out_buffer, out_buffer_size);

                //同步数据 同时释放sample_byte
                env->ReleaseByteArrayElements(audio_sample_array, sample_byte, 0);

                //AudioTrack.write PCM数据
                env->CallIntMethod(audio_track, audio_track_write_mid, audio_sample_array, 0,
                                   out_buffer_size);

                //释放局部引用  否则报错JNI ERROR (app bug): local reference table overflow (max=512)
                env->DeleteLocalRef(audio_sample_array);
            }

        }

        //output video
        if (avPacket->stream_index == videoindex) {
            //视频解码
            int ret = avcodec_send_packet(vCodecCtx, avPacket);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                LOGD("avcodec_send_packet failed");
                return -1;
            }
            ret = avcodec_receive_frame(vCodecCtx, avFrame);
            if (ret < 0 && ret != AVERROR_EOF) {
                if (ret == AVERROR(EAGAIN)) {
                    continue;
                }
                LOGD("avcodec_receive_frame failed ret:%d,%d,%d", ret, AVERROR_EOF,
                     AVERROR(EAGAIN));
                return -1;
            }
            //转化格式
            sws_scale(img_convert_ctx, (const uint8_t *const *) avFrame->data, avFrame->linesize, 0,
                      vCodecCtx->height,
                      pFrameRGBA->data, pFrameRGBA->linesize);
            if (ANativeWindow_lock(nativeWindow, &windowBuffer, NULL) < 0) {
                LOGD("cannot lock window");
            } else {
                //将图像绘制到界面上，注意这里pFrameRGBA一行的像素和windowBuffer一行的像素长度可能不一致
                //需要转换好，否则可能花屏
                uint8_t *dst = (uint8_t *) windowBuffer.bits;
                for (int h = 0; h < height; h++) {
                    memcpy(dst + h * windowBuffer.stride * 4,
                           v_out_buffer + h * pFrameRGBA->linesize[0],
                           pFrameRGBA->linesize[0]);
                }
                ANativeWindow_unlockAndPost(nativeWindow);

            }
        }
        av_packet_unref(avPacket);
    }
    //释放内存
    sws_freeContext(img_convert_ctx);
    av_free(avPacket);
    av_free(avFrame);
    av_free(pFrameRGBA);
    avcodec_close(vCodecCtx);
    avcodec_close(aCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

}