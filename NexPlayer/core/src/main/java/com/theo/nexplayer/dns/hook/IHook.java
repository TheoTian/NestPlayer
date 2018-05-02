package com.theo.nexplayer.dns.hook;

import java.lang.reflect.Method;

/**
 * Created by tianxiya on 2018/4/27.
 */

public interface IHook {

    /**
     * hook invoke
     *
     * @param proxy
     * @param method
     * @param args
     * @return
     * @throws Throwable
     */
    Object invoke(Object proxy, Method method, Object[] args) throws Throwable;

    /**
     * hook the class
     */
    void hook();
}
