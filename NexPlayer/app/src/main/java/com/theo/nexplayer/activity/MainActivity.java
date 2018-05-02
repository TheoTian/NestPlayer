package com.theo.nexplayer.activity;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Display;
import android.widget.RelativeLayout;

import com.theo.nexplayer.NexVideoView;
import com.theo.nexplayer.jni.NativeMediaPlayer;
import com.theo.nexplayer.net.Requester;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    private final static String URL = "http://mp4.res.hunantv.com/video/1155/79c71f27a58042b23776691d206d23bf.mp4";

    private class InetAddressImplHandler implements InvocationHandler {

        private Object host;

        public InetAddressImplHandler(Object host) {
            this.host = host;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            Log.i("test", "invoke:" + method.getName());
            if(method.getName().equals("lookupAllHostAddr")) {
                Log.i("test", "lookupAllHostAddr:" + args[0] + "," + args[1]);
            }
            return method.invoke(host, args);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Display display = this.getWindowManager().getDefaultDisplay();

        NexVideoView videoView = new NexVideoView(this);
        ((RelativeLayout) findViewById(R.id.container)).addView(videoView,
                new RelativeLayout.LayoutParams(display.getWidth(), display.getWidth() * 9 / 16));
//        videoView.testPlay(URL);
        Log.d("test", NativeMediaPlayer._test());

        Requester.get("http://www.baidu.com", null, new Requester.Listener() {
            @Override
            public void onSuccess(int code, String response) {
                Log.d("test", response);
            }

            @Override
            public void onFailed(int status) {
            }
        });

    }
}
