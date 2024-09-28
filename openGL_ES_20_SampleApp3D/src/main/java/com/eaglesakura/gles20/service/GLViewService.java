package com.eaglesakura.gles20.service;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.view.Gravity;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;
import android.view.WindowManager;

import com.eaglesakura.gles20.app.GLApplication;
import com.eaglesakura.gles20.app.RenderingThread;
import com.eaglesakura.gles20.app.ndk.NDKApplication;
import com.eaglesakura.gles20.util.ES20;

public class GLViewService extends Service {

    static {
        // Native側の初期化を行う
        System.loadLibrary("sample");
        ES20.initializeNative();
    }

    /**
     * TextureViewを使用する
     */
    public static final String EXTRA_MODE_TEXTUREVIEW = "EXTRA_MODE_TEXTUREVIEW";

    /**
     * 垂直同期を行う
     */
    public static final String EXTRA_MODE_VSYNC = "EXTRA_MODE_VSYNC";

    /**
     * 表示用のView
     */
    View surface;

    /**
     * Window操作用マネージャ<BR>
     * ここへViewを登録することで、常にViewを表示することができる。
     */
    WindowManager windowManager;

    RenderingThread renderThread;

    GLApplication app = null;

    @SuppressWarnings("deprecation")
    @SuppressLint("NewApi")
    @Override
    public void onStart(Intent intent, int startId) {
        super.onStart(intent, startId);

        if (renderThread != null) {
            return;
        }

        app = new NDKApplication(8, 4);
        // Activityが使えないため、Application Contextを利用する
        app.platform.context = getApplicationContext();
        renderThread = new RenderingThread(app);

        // Viewの生成と初期化
        if (intent.getBooleanExtra(EXTRA_MODE_TEXTUREVIEW, false)) {
            surface = new TextureView(this);
            // 垂直同期を設定
            renderThread.setVsyncEnable(intent.getBooleanExtra(EXTRA_MODE_VSYNC, false));
        } else {
            surface = new SurfaceView(this);
        }

        // レンダリングスレッドを起動
        renderThread.initialize(surface);
        renderThread.start();

        // WindowManagerを取得する
        windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        {

            WindowManager.LayoutParams params = new WindowManager.LayoutParams(
            // レイアウトの幅 / 高さ設定
            // 適当なサイズで設定する
                    512, 512,
                    // レイアウトの挿入位置設定
                    // TYPE_SYSTEM_OVERLAYはほぼ最上位に位置して、ロック画面よりも上に表示される。
                    // ただし、タッチを拾うことはできない。
                    WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
                    // ウィンドウ属性
                    // TextureViewを利用するには、FLAG_HARDWARE_ACCELERATED が必至となる。
                    WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED,
                    // TRANSLUCENTを利用すると、最終的なピクセルの透過を反映させられる
                    PixelFormat.TRANSLUCENT);

            // Gravityを操作することで、画面の適当な位置に寄せる事ができる
            params.gravity = Gravity.LEFT | Gravity.TOP;

            // Viewを画面上に重ね合わせする
            windowManager.addView(surface, params);
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        if (renderThread != null) {
            // レンダリングの停止を行わせる
            renderThread.onDestroy();
            renderThread = null;

            // Windowからサーフェイスを排除する
            windowManager.removeView(surface);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    /**
     * 
     * @param context
     * @param useTextureView
     * @param useVSync
     */
    public static void start(Context context, boolean useTextureView, boolean useVSync) {
        Intent intent = new Intent(context, GLViewService.class);
        intent.putExtra(EXTRA_MODE_TEXTUREVIEW, useTextureView);
        intent.putExtra(EXTRA_MODE_VSYNC, useVSync);
        context.startService(intent);
    }

    /**
     * 
     * @param context
     */
    public static void stop(Context context) {
        Intent intent = new Intent(context, GLViewService.class);
        context.stopService(intent);
    }
}
