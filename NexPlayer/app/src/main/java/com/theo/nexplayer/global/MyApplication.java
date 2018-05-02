package com.theo.nexplayer.global;

import android.app.Application;

import com.theo.nexplayer.dns.hook.IHook;
import com.theo.nexplayer.dns.hook.InetAddressImplHook;

/**
 * Created by tianxiya on 2018/4/26.
 */

public class MyApplication extends Application {

    private IHook mInetAddressHook = new InetAddressImplHook();

    @Override
    public void onCreate() {
        super.onCreate();
        mInetAddressHook.hook();
    }
}
