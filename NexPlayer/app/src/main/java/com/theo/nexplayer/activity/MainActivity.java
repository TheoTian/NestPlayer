package com.theo.nexplayer.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.widget.RelativeLayout;

import com.theo.nexplayer.NexVideoView;
import com.theo.nexplayer.jni.NativeMediaPlayer;

public class MainActivity extends AppCompatActivity {

    private final static String URL = "http://mp4.res.hunantv.com/video/1155/79c71f27a58042b23776691d206d23bf.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Display display = this.getWindowManager().getDefaultDisplay();

        NexVideoView videoView = new NexVideoView(this);
        ((RelativeLayout) findViewById(R.id.container)).addView(videoView,
                new RelativeLayout.LayoutParams(display.getWidth(), display.getWidth() * 9 / 16));
        videoView.testPlay(URL);
        Log.d("test", NativeMediaPlayer._test());

    }
}
