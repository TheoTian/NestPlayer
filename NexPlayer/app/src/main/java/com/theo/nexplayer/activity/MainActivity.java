package com.theo.nexplayer.activity;

import android.graphics.Point;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

import com.theo.nexplayer.NexVideoView;
import com.theo.nexplayer.jni.NativeMediaPlayer;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Display display = this.getWindowManager().getDefaultDisplay();

        NexVideoView videoView = new NexVideoView(this);
        ((RelativeLayout)findViewById(R.id.container)).addView(videoView,
                new RelativeLayout.LayoutParams(display.getWidth(), display.getWidth() * 9 / 16));
        Log.d("test", NativeMediaPlayer._test());

    }
}
