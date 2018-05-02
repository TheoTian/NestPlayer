package com.theo.nexplayer.net;

import android.net.Uri;

import com.theo.nexplayer.utils.LogUtil;
import com.theo.nexplayer.utils.SafeUtil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.InterruptedIOException;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Map;
import java.util.Set;

/**
 * Author: theotian
 * Date: 17/5/19
 * Describe:
 * Http requester
 */

public class Requester {

    private final static int CONNECTION_TIMEOUT = 5000; //建立连接超时时间 5s
    private final static int READ_TIMEOUT = 5000; //数据传输超时时间 5s

    /**
     * 请求方式
     */
    enum Method {
        GET, POST
    }

    public interface Listener {
        /**
         * response from server and code is 2XX
         *
         * @param code
         * @param response
         */
        void onSuccess(int code, String response);

        final static int ERROR_DEFAULT = -1;

        /**
         * request error and code is not 2XX
         *
         * @param status
         */
        void onFailed(int status);
    }

    /**
     * get way
     *
     * @param urlStr
     * @param params
     * @param listener
     */
    public static void get(final String urlStr, final Map<String, String> params, final Listener listener) {
        request(Method.GET, urlStr, params, listener);
    }

    /**
     * post way
     *
     * @param urlStr
     * @param params
     * @param listener
     */
    public static void post(final String urlStr, final Map<String, String> params, final Listener listener) {
        request(Method.POST, urlStr, params, listener);
    }

    public static void request(final Method type, final String urlStr, final Map<String, String> params, final Listener listener) {
        new Thread(new Runnable() {
            @Override
            public void run() {

                URL url = null;
                HttpURLConnection connection = null;

                try {
                    String requestUrl = urlStr;
                    if (type == Method.GET && params != null && params.size() > 0) {
                        requestUrl += "?" + getParams(params);
                    }
                    url = new URL(requestUrl);
                    connection = (HttpURLConnection) (url.openConnection());
                    if (connection == null) {
                        if (listener != null) {
                            listener.onFailed(Listener.ERROR_DEFAULT);
                            return;
                        }
                    }
                    // set  http  configure
                    connection.setConnectTimeout(CONNECTION_TIMEOUT);// 建立连接超时时间
                    connection.setReadTimeout(READ_TIMEOUT);//数据传输超时时间，很重要，必须设置。
                    connection.setDoInput(true); // 向连接中写入数据
                    connection.setUseCaches(false); // 禁止缓存
                    connection.setInstanceFollowRedirects(true);
                    connection.setRequestProperty("Charset", "UTF-8");

                    switch (type) {
                        case GET:
                        default:
                            connection.setRequestMethod("GET");// 设置请求类型为
                            break;
                        case POST:
                            connection.setRequestMethod("POST");// 设置请求类型为
                            connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                            connection.setDoOutput(true); // 从连接中读取数据

                            //post data to server
                            writeParams(connection, params);

                            break;
                    }

                    connection.connect();

                    int status = connection.getResponseCode();

                    if (status == HttpURLConnection.HTTP_MOVED_TEMP) { //302跳转
                        String location = connection.getHeaderField("Location");
                        switch (type) {
                            case GET:
                            default:
                                request(type, location, null, listener);
                                break;
                            case POST:
                                request(type, location, params, listener);
                                break;
                        }
                        return;
                    }

                    if (status >= HttpURLConnection.HTTP_OK && status <= HttpURLConnection.HTTP_PARTIAL) {
                        if (listener != null) {
                            listener.onSuccess(status, readResponse(connection));
                            return;
                        }
                    } else {
                        if (listener != null) {
                            listener.onFailed(status);
                            return;
                        }
                    }
                } catch (InterruptedIOException e) {
                    e.printStackTrace();
                    if (listener != null) {
                        listener.onFailed(Listener.ERROR_DEFAULT);
                        return;
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    if (listener != null) {
                        listener.onFailed(Listener.ERROR_DEFAULT);
                        return;
                    }
                } finally {
                    if (connection != null) {
                        connection.disconnect();
                    }
                }
            }
        }).start();
    }

    /**
     * read response from server
     *
     * @param connection
     * @return
     */
    private static String readResponse(HttpURLConnection connection) {
        String result = "";
        if(connection == null) {
            return result;
        }
        InputStreamReader in = null;
        try {
            in = new InputStreamReader(connection.getInputStream()); // 获得读取的内容
            BufferedReader buffer = new BufferedReader(in); // 获取输入流对象
            String inputLine = "";
            while ((inputLine = buffer.readLine()) != null) {
                result += inputLine + "\n";
            }
        } catch (Exception e) {
        } finally {
            SafeUtil.safeClose(in);
        }
        return result;
    }
    private static void writeParams(final HttpURLConnection connection, final Map<String, String> params) {
        if (connection == null || params == null || params.size() <= 0) {
            return;
        }
        String paramStr = getParams(params);//get params
        byte[] data = null;
        try {
            data = paramStr.getBytes("UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        writeData(connection, data);
    }

    /**
     * get Map params to String params
     *
     * @param params
     * @return
     */
    private static String getParams(final Map<String, String> params) {
        String paramStr = "";
        try {
            //Map.entry<Integer,String> 映射项（键-值对）  有几个方法：用上面的名字entry
            //entry.getKey() ;entry.getValue(); entry.setValue();
            //map.entrySet()  返回此映射中包含的映射关系的 Set视图。
            int index = 0;
            Set<Map.Entry<String, String>> entrySet = params.entrySet();
            int lastIndex = entrySet.size() - 1;
            for (Map.Entry<String, String> entry : entrySet) {
                LogUtil.d(LogUtil.DEFAULT_TAG, "key= " + entry.getKey() + " and value= "
                        + entry.getValue());
                String tempStr = "";
                try {
                    tempStr = Uri.encode(entry.getKey(), "UTF-8") + "=" + Uri.encode(entry.getValue(), "UTF-8");
                    if (index < lastIndex) {
                        tempStr += "&";
                    }
                } catch (Exception e) {
                    tempStr = "";
                }
                index++;
                paramStr += tempStr;
            }
        } catch (Exception e) {

        }
        return paramStr;
    }

    /**
     * write data to connection
     *
     * @param connection
     * @param data
     */
    private static void writeData(HttpURLConnection connection, byte[] data) {
        if (connection == null || data == null || data.length <= 0) {
            return;
        }
        DataOutputStream out = null;
        try {
            out = new DataOutputStream(connection.getOutputStream()); // 获取输出流
            out.write(data);// 将要传递的数据写入数据输出流,不要使用out.writeBytes(param); 否则中文时会出错
            out.flush(); // 输出缓存
            SafeUtil.safeClose(out); // 关闭数据输出流
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            SafeUtil.safeClose(out);
        }
    }
}
