package com.theo.nexplayer.dns.hook.util;

import java.lang.reflect.Method;
import java.net.InetAddress;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by tianxiya on 2018/4/30.
 */

public class InetAddressUtil {

    /**
     * Returns true if the string is a valid numeric IPv4 or IPv6 address (such as "192.168.0.1").
     * This copes with all forms of address that Java supports, detailed in the {@link InetAddress}
     * class documentation.
     *
     * @param address
     * @return
     */
    public static boolean isNumeric(String address) {
        try {
            Method method = InetAddress.class.getDeclaredMethod("isNumeric", String.class);
            return (boolean) method.invoke(null, address);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return isNumeric2(address);
    }

    /**
     * regex way to detect
     *
     * @param address
     * @return
     */
    private static boolean isNumeric2(String address) {
        if (isIPv4(address) || isIPv6(address)) {
            return true;
        }
        return false;
    }

    /**
     * regex way to detect IPV4&IPV6
     */
    public final static String REGEX_IPV4 =
            "^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$";

    public static boolean isIPv4(String address) {
        try {
            Pattern pattern = Pattern.compile(REGEX_IPV4);
            Matcher matcher = pattern.matcher(address);
            return matcher.matches();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public final static String REGEX_IPV6 =
            "(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|" +
                    "([0-9a-fA-F]{1,4}:){1,7}:|" +
                    "([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|" +
                    "([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|" +
                    "([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|" +
                    "([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|" +
                    "([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|" +
                    "[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|" +
                    ":((:[0-9a-fA-F]{1,4}){1,7}|:)|" +
                    "fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|" +
                    "::(ffff(:0{1,4}){0,1}:){0,1}" +
                    "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}" +
                    "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|" +
                    "([0-9a-fA-F]{1,4}:){1,4}:" +
                    "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}" +
                    "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))";

    public static boolean isIPv6(String address) {
        try {
            Pattern pattern = Pattern.compile(REGEX_IPV6);
            Matcher matcher = pattern.matcher(address);
            return matcher.matches();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

}
