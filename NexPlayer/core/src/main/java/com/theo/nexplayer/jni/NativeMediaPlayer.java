package com.theo.nexplayer.jni;

/**
 * Created by txy on 18-4-19.
 */

public class NativeMediaPlayer {
    static {
        System.loadLibrary("nexplayer");
    }
    public static native String _test();
}
