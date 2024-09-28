package com.eaglesakura.gles20.miracast;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Presentation;
import android.content.Context;
import android.hardware.display.DisplayManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;

import com.eaglesakura.gles20.app.GLApplication;
import com.eaglesakura.gles20.app.RenderingThread;
import com.eaglesakura.gles20.app.ndk.NDKApplication;

/**
 * Miracast制御クラス
 * Android 4.3(APIレベル18)以上で利用可能
 * 
 * 注意）サンプルプログラムとしては残すが、紙面の都合から解説は割愛する
 */
@SuppressLint("NewApi")
public class MiracastController {

    final DisplayManager displayManager;

    /**
     * 接続されているディスプレイ
     * 本体ディスプレイを含む
     */
    Display[] displays;

    GLPresentation presentation = null;

    final Activity activity;

    int chapterNumber;

    int sampleNumber;

    /**
     * TextiveViewモード
     */
    boolean textureView = false;

    /**
     * 垂直同期
     */
    boolean vsync = false;

    public MiracastController(Activity activity, int chapter, int sample) {
        this.activity = activity;
        this.chapterNumber = chapter;
        this.sampleNumber = sample;
        displayManager = (DisplayManager) activity.getSystemService(Context.DISPLAY_SERVICE);
        updateDisplays();
    }

    /**
     * ディスプレイを更新する
     */
    private void updateDisplays() {
        displays = displayManager.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION);
    }

    /**
     * TextureViewを有効化する
     */
    public void enableTextureView() {
        this.textureView = true;
    }

    /**
     * 垂直同期を有効化する
     */
    public void enableVsync() {
        this.vsync = true;
    }

    /**
     * リモートディスプレイに接続されている場合true
     * @return
     */
    public boolean isConnectedDisplay() {
        return displays != null && displays.length > 0;
    }

    public void onResume() {
        updateDisplays();

        if (isConnectedDisplay()) {
            log("connected Miracast");
            log("Connected Displays :: " + displays.length);
            presentation = new GLPresentation(activity, displays[0]);
            presentation.show();
        } else {
            log("not connect...");
        }
    }

    public void onPause() {
        if (presentation != null) {
            presentation.dismiss();
            presentation = null;
        }
    }

    private static void log(String msg) {
        Log.i("miracast", msg);
    }

    class GLPresentation extends Presentation {
        RenderingThread renderingThread;

        public GLPresentation(Context context, Display display) {
            super(context, display);
        }

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            View surface = null;
            if (textureView) {
                surface = new TextureView(getContext());
            } else {
                surface = new SurfaceView(getContext());
            }
            setContentView(surface);

            // レンダリングスレッドを起動する
            GLApplication app = new NDKApplication(chapterNumber, sampleNumber);
            app.platform.context = activity;
            renderingThread = new RenderingThread(app);
            renderingThread.initialize(surface);
            renderingThread.start();

            log("Presentation onCreated");
        }

        @Override
        protected void onStop() {
            super.onStop();
            renderingThread.onPause();
            renderingThread.onDestroy();
            log("Presentation onStop");
        }
    }

}
