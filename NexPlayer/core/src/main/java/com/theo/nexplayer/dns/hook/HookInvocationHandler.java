package com.theo.nexplayer.dns.hook;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

/**
 * Created by tianxiya on 2018/4/27.
 */

public class HookInvocationHandler implements InvocationHandler {

    private IHook hook;

    public HookInvocationHandler(IHook hook) {
        this.hook = hook;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        if(hook != null) {
            return hook.invoke(proxy, method, args);
        }
        return null;
    }
}
