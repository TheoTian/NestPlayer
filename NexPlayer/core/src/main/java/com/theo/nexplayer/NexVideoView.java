package com.theo.nexplayer;

import android.content.Context;
import android.os.Environment;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.theo.nexplayer.jni.NativeMediaPlayer;

/**
 * Created by txy on 18-4-20.
 */

public class NexVideoView extends SurfaceView {

    private String URL = "http://mp4.res.hunantv.com/video/1155/79c71f27a58042b23776691d206d23bf.mp4";

    private NexMediaPlayer mPlayer = new NexMediaPlayer();

    public NexVideoView(Context context) {
        super(context);
        initView(context);
    }

    public NexVideoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView(context);
    }

    public NexVideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView(context);
    }

    private void initView(Context context) {
        URL = context.getExternalCacheDir().getAbsolutePath() + "/test.mp4";
        getHolder().addCallback(mSHCallback);

    }

    SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder, int format,
                                   int w, int h) {

        }

        public void surfaceCreated(final SurfaceHolder holder) {
            mPlayer.testPlay(URL, holder.getSurface());
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
        }
    };


}
