package com.theo.nexplayer.dns.hook;

import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * Created by tianxiya on 2018/4/27.
 */

public class InetAddressImplHook extends BaseHook implements IHook {

    private final static String TAG = "InetAddressImplHook";
    private final static String HOOK_METHOD = "lookupAllHostAddr";

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        if (method.getName().equals(HOOK_METHOD)) {
            return invokeLookupAllHostAddr(proxy, method, args);
        }
        return method.invoke(getHost(), args);
    }

    private Object invokeLookupAllHostAddr(Object proxy, Method method, Object[] args) throws Throwable {
        Log.i(TAG, "invokeLookupAllHostAddr");

        InetAddress[] addresses;

        if (args != null && args.length == 2 && args[0] instanceof String && args[1] instanceof Integer) {
            addresses = lookupAllHostAddr((String) args[0], (int) args[1]);
            if (addresses != null && addresses.length > 0) {
                return addresses;
            }
        }

        addresses = (InetAddress[]) method.invoke(getHost(), args);

        if (addresses != null) {
            for (InetAddress address : addresses) {
                Log.i("test", "address:" + address.getHostName() + "[" + address.getHostAddress() + "]");
            }
        }

        return addresses;
    }

    private InetAddress[] lookupAllHostAddr(String host, int netId) throws UnknownHostException {
        if (host == null || host.isEmpty()) {
            //return null to use system method
            return null;
        }
//        // Is it a numeric address?
//        InetAddress result = InetAddress.parseNumericAddressNoThrow(host);
//        if (result != null) {
//            result = InetAddress.disallowDeprecatedFormats(host, result);
//            if (result == null) {
//                throw new UnknownHostException("Deprecated IPv4 address format: " + host);
//            }
//            return new InetAddress[]{result};
//        }

        return new InetAddress[]{};
    }

    @Override
    public void hook() {
        try {
            Field implField = InetAddress.class.getDeclaredField("impl");
            implField.setAccessible(true);

            Object inetAddressImplObj = implField.get(null);
            setHost(inetAddressImplObj);

            Object newInetAddressImpl = Proxy.newProxyInstance(inetAddressImplObj.getClass().getClassLoader(),
                    inetAddressImplObj.getClass().getInterfaces(), new HookInvocationHandler(this));

            implField.set(null, newInetAddressImpl);
            implField.setAccessible(false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
