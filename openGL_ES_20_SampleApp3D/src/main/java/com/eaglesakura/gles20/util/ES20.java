package com.eaglesakura.gles20.util;

import java.nio.charset.Charset;

import android.annotation.SuppressLint;
import android.os.Build;

import com.eaglesakura.gles20.app.GLApplication;
import com.eaglesakura.gles20.data.AndroidPlatformData;

public class ES20 {

    static final String TAG = ES20.class.getSimpleName();

    /**
     * 関数が成功した
     */
    public static final int ES20_NO_ERROR = 0;

    /**
     * 関数が失敗した
     */
    public static final int ES20_ERROR = 1;

    /**
     * Native側の初期化を行う
     */
    public static native void initializeNative();

    /**
     * 描画結果を画面へ反映する。
     * この関数を呼び出すまで、画面には反映されない。
     * @param util
     */
    public static void postFrontBuffer(GLApplication app) {
        ((AndroidPlatformData) app.platform).windowDevice.postFrontBuffer();
    }

    /**
     * SJISをUTF-8へ変換する
     * API8は非対応のため文字コードをそのまま返す
     * @param sjis
     * @return
     */
    @SuppressLint("NewApi")
    public static byte[] sjis2utf8(byte[] sjis) {
        if (Build.VERSION.SDK_INT < 9) {
            return sjis;
        }

        try {
            return new String(sjis, Charset.forName("Shift-JIS")).getBytes(Charset.forName("UTF-8"));
        } catch (Exception e) {
            return sjis;
        }
    }
}
