package com.eaglesakura.gles20;

import android.os.Looper;

public class Util {

    public static void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (Exception e) {

        }
    }

    /**
     * UIスレッドで動いていたらtrueを返す
     * @return
     */
    public static boolean isUIThread() {
        return Thread.currentThread().equals(Looper.getMainLooper().getThread());
    }
}
