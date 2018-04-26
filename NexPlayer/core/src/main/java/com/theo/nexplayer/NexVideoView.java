package com.theo.nexplayer;

import android.content.Context;
import android.net.Uri;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * The simple videoview just for learning
 * Created by txy on 18-4-20.
 */
public class NexVideoView extends SurfaceView {

    private static final String TAG = "VideoView";

    // all possible internal states
    private static final int STATE_ERROR = -1;
    private static final int STATE_IDLE = 0;
    private static final int STATE_PREPARING = 1;
    private static final int STATE_PREPARED = 2;
    private static final int STATE_PLAYING = 3;
    private static final int STATE_PAUSED = 4;
    private static final int STATE_PLAYBACK_COMPLETED = 5;


    private NexMediaPlayer mMediaPlayer = new NexMediaPlayer();

    // mCurrentState is a VideoView object's current state.
    // mTargetState is the state that a method caller intends to reach.
    // For instance, regardless the VideoView object's current state,
    // calling pause() intends to bring the object to a target state
    // of STATE_PAUSED.
    private int mCurrentState = STATE_IDLE;
    private int mTargetState = STATE_IDLE;

    private int mVideoWidth;
    private int mVideoHeight;


    private int mSurfaceWidth;
    private int mSurfaceHeight;

    // settable by the client
    private Uri mUri;
    private String mUrl;


    // All the stuff we need for playing and showing a video
    private SurfaceHolder mSurfaceHolder = null;


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
        mVideoWidth = 0;
        mVideoHeight = 0;

        mSurfaceWidth = 0;
        mSurfaceHeight = 0;

        getHolder().addCallback(mSHCallback);

        mCurrentState = STATE_IDLE;
        mTargetState = STATE_IDLE;

    }


    SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder, int format,
                                   int w, int h) {
        }

        public void surfaceCreated(final SurfaceHolder holder) {
            mSurfaceHolder = holder;
            openVideo();
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
        }
    };

    public void testPlay(String url) {
        mUri = Uri.parse(url);
        mUrl = url;
    }

    private void openVideo() {
        if (mUri == null || mSurfaceHolder == null) {
            // not ready for playback just yet, will try again later
            return;
        }

        mMediaPlayer.testPlay(mUrl, mSurfaceHolder.getSurface());
    }
}
