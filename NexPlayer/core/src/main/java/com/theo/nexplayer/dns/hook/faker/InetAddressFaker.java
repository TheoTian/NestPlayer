package com.theo.nexplayer.dns.hook.faker;

import java.net.InetAddress;

/**
 * Created by tianxiya on 2018/4/30.
 */

public class InetAddressFaker {

    private InetAddress mInetAddress;

    public InetAddressFaker() {
        try {
            mInetAddress = InetAddress.class.newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
