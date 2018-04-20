package com.theo.nexplayer;

import android.view.Surface;

import com.theo.nexplayer.jni.NativeMediaPlayer;

/**
 * Created by txy on 18-4-19.
 */

public class NexMediaPlayer {

    NativeMediaPlayer mNativePlayer;

    public NexMediaPlayer() {
        mNativePlayer = new NativeMediaPlayer();
    }

    /**
     * for test
     *
     * @param url
     * @param surface
     * @return
     */
    public void testPlay(final String url, final Surface surface) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                mNativePlayer._testplay(url, surface);
            }
        }).start();
    }
}
