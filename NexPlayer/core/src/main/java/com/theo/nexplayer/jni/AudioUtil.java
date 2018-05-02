package com.theo.nexplayer.jni;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

/**
 * Created by tianxiya on 2018/5/3.
 * just for learning
 */

public class AudioUtil {

    public int write(byte[] audioData, int offsetInBytes, int sizeInBytes) {
        return 0;
    }

    /**
     * 创建一个AudioTrack对象，用于播放
     *
     * @return
     */
    public AudioTrack createAudioTrack(int sampleRateInHz, int nb_channels) {
        Log.i("AudioUtil", "nb_channels:" + nb_channels);
        Log.i("AudioUtil", "sampleRateInHz:" + sampleRateInHz);
        //固定格式的音频码流
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;

        //声道个数 影响声道的布局
        int channelConfig;
        if (nb_channels == 1) {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_MONO;
        } else if (nb_channels == 2) {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        } else {
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        }

        int bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat);
        AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                sampleRateInHz, channelConfig,
                audioFormat,
                bufferSizeInBytes, AudioTrack.MODE_STREAM);
        //audioTrack.write()
        return audioTrack;
    }

}
