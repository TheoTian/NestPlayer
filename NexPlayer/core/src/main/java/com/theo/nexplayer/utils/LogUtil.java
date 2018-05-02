package com.theo.nexplayer.utils;

import android.util.Log;

import java.util.Locale;

/**
 * Created by tianxiya on 2017/5/21.
 */
public class LogUtil {
    public final static String DEFAULT_TAG = "theo";

    private static boolean mSwitch = true;

    /**
     * 设置日志记录开关
     *
     * @param isSwitch 此开关与BuildConfig.DEBUG绑定一致，开发环境为true，release版本为false
     */
    public static void setSwitch(boolean isSwitch) {
        mSwitch = isSwitch;
    }

    public static void d(String tag, String msg) {
        if (mSwitch) {
            Log.d(tag, msg);
        }
    }

    public static void e(String tag, String msg) {
        if (mSwitch) {
            Log.e(tag, msg);
        }
    }

    public static void i(String tag, String msg) {
        if (mSwitch) {
            Log.i(tag, msg);
        }
    }

    public static void ifmt(String tag, String fmt, Object... args) {
        if (mSwitch) {
            String msg = String.format(Locale.US, fmt, args);
            Log.i(tag, msg);
        }

    }

    public static void w(String tag, String msg) {
        if (mSwitch) {
            Log.w(tag, msg);
        }
    }

    public static void v(String tag, String msg) {
        if (mSwitch) {
            Log.v(tag, msg);
        }
    }

    public static void e(String tag, String msg, Exception e) {
        if (mSwitch) {
            Log.v(tag, msg, e);
        }
    }

    public static void e(String tag, String msg, Throwable e) {
        if (mSwitch) {
            Log.v(tag, msg, e);
        }
    }

    // -------------------------
    public static void d(Class tag, String msg) {
        if (mSwitch) {
            Log.d(tag.getSimpleName(), msg);
        }
    }

    public static void e(Object tag, String msg) {
        if (mSwitch) {
            Log.e(tag.getClass().getSimpleName(), msg);
        }
    }

    public static void i(Class tag, String msg) {
        if (mSwitch) {
            Log.i(tag.getSimpleName(), msg);
        }
    }

    public static void w(Object tag, String msg) {
        if (mSwitch) {
            Log.w(tag.getClass().getSimpleName(), msg);
        }
    }

    public static void d(Object tag, String msg) {
        if (mSwitch) {
            Log.d(tag.getClass().getSimpleName(), msg);
        }
    }

    public static void i(Object tag, String msg) {
        if (mSwitch) {
            Log.i(tag.getClass().getSimpleName(), msg);
        }
    }

    public static void v(Object tag, String msg) {
        if (mSwitch) {
            Log.v(tag.getClass().getSimpleName(), msg);
        }
    }

    public static void e(Object tag, String msg, Exception e) {
        if (mSwitch) {
            Log.v(tag.getClass().getSimpleName(), msg, e);
        }
    }

    public static void e(Object tag, String msg, Throwable e) {
        if (mSwitch) {
            Log.v(tag.getClass().getSimpleName(), msg, e);
        }
    }

}
