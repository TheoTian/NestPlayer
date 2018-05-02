package com.theo.nexplayer.utils;

import java.io.Closeable;

/**
 * Created by tianxiya on 2017/5/21.
 */

public class SafeUtil {
    /**
     * safe close
     * @param closeable
     */
    public static void safeClose(Closeable closeable) {
        try {
            if (closeable != null) {
                closeable.close();
            }
        } catch (Exception e) {
        }
    }

    /**
     * safe equal contents, return false when null
     *
     * @return
     */
    public static boolean safeEqualIgnoreCase(String str1, String str2) {
        if (str1 == null || str2 == null) {
            return false;
        }
        if (str1.equalsIgnoreCase(str2)) {
            return true;
        }
        return false;
    }
}
