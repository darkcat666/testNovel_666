package com.eaglesakura.gles20.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;

import com.eaglesakura.gles20.data.AndroidPlatformData;

/**
 * Application
 */
public abstract class GLApplication {
    /**
     * アプリを強制停止させる（対応してないextension解説等）
     */
    public static final int GLAPP_FLAG_ABORT = (0x1 << 2);

    /**
     * View（レンダリングターゲット）の幅
     */
    public int surface_width = 0;

    /**
     * View（レンダリングターゲット）の高さ
     */
    public int surface_height = 0;

    /**
     * アプリ実行フラグ
     */
    public int flags = 0;

    /**
     * プラットフォームごとに保持している拡張データ
     * サンプルアプリから扱う必要はない。
     */
    public AndroidPlatformData platform = new AndroidPlatformData();

    /**
     * 初期化を行う
     * @param surface
     * @param eglManager
     */
    public abstract void initialize();

    /**
     * 初期化が終わった後、非同期関数が設定されていれば非同期呼び出しを行う。
     */
    public abstract void async();

    /**
     * 解像度変更に対応する
     * @param utilData
     * @param appData
     * @param newWidth
     * @param newHeight
     */
    public abstract void resized();

    /**
     * レンダリングを行う
     * @param utilData
     * @param appData
     */
    public abstract void rendering();

    /**
     * 廃棄を行う
     * @param utilData
     * @param appData
     */
    public abstract void destroy();

    /**
     * メッセージを表示して処理を中断する。
     * @param util
     * @param message
     */
    public static void abortWithMessage(GLApplication app, final String message) {

        final AndroidPlatformData platform = (AndroidPlatformData) app.platform;

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder builder = new AlertDialog.Builder(platform.context);

                builder.setTitle("処理を中断します");
                builder.setMessage(message);
                builder.setCancelable(false);
                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        ((Activity) platform.context).finish();
                    }
                });

                builder.show();

            }
        };

        ((Activity) platform.context).runOnUiThread(runnable);

        // abortフラグを立てる
        app.flags |= GLApplication.GLAPP_FLAG_ABORT;
    }

    /**
     * 処理中断済みの場合はtrueを返す
     * @param app
     * @return
     */
    public boolean isAbort() {
        return (flags & GLAPP_FLAG_ABORT) != 0;
    }

    /**
     * 非同期関数が登録されている場合true
     * @return
     */
    public boolean hasAsync() {
        return false;
    }
}
