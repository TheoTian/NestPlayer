package com.theo.nexplayer.jni;

import android.view.Surface;

/**
 * Created by txy on 18-4-19.
 */

public class NativeMediaPlayer {

    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("nexplayer");
    }

    public static native String _test();

    public native int _testplay(String url, Surface surface);
}
