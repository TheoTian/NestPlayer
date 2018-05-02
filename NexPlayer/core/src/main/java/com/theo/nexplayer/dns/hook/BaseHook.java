package com.theo.nexplayer.dns.hook;

/**
 * Created by tianxiya on 2018/4/27.
 */

public abstract class BaseHook {

    private Object host;

    public Object getHost() {
        return host;
    }

    public void setHost(Object host) {
        this.host = host;
    }
}
