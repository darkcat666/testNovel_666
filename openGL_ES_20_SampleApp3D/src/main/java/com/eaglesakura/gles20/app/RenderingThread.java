package com.eaglesakura.gles20.app;

import android.annotation.SuppressLint;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.TextureView.SurfaceTextureListener;

import com.eaglesakura.gles20.Util;
import com.eaglesakura.gles20.egl.EGLDevice;
import com.eaglesakura.gles20.egl.EGLManager;

@SuppressLint("NewApi")
public class RenderingThread extends Thread {

    static final String TAG = RenderingThread.class.getSimpleName();

    private enum Status {
        /**
         * 通常実行中
         */
        RUN,

        /**
         * レンダリング停止中
         */
        PAUSE,

        /**
         * 廃棄を行う
         */
        DESTROY,
    }

    /**
     * 通常実行中
     */
    private Status status = Status.RUN;

    /**
     * サンプルアプリ
     */
    private final GLApplication app;

    /**
     * 垂直同期を行う場合true
     */
    private boolean vsyncEnable = false;

    /**
     * 
     * @param activity
     */
    public RenderingThread(GLApplication sampleApp) {
        this.app = sampleApp;
    }

    /**
     * 垂直同期を行う場合true
     * @param vsyncEnable
     */
    public void setVsyncEnable(boolean vsyncEnable) {
        this.vsyncEnable = vsyncEnable;
    }

    /**
     * 初期化処理を行う
     * @param surfaceView
     */
    public void initialize(Object surface) {
        if (surface instanceof SurfaceView) {
            // SurfaceViewであれば、中身のSurfaceHolderだけ取り出せればいい
            surface = ((SurfaceView) surface).getHolder();
        }

        if (surface instanceof SurfaceHolder) {
            // SurfaceHolderとして初期化を行う
            SurfaceHolder holder = (SurfaceHolder) surface;

            // ピクセルフォーマットの指定と、リスナの登録
            holder.setFormat(PixelFormat.RGBA_8888);
            holder.addCallback(new SurfaceHolderCallbackImpl());
        } else if (surface instanceof TextureView) {
            // SurfaceTextureとして初期化を行う

            // リスナを登録
            ((TextureView) surface).setSurfaceTextureListener(new SurfaceTextureListenerImpl());
        } else {
            // 非対応のサーフェイス
            throw new IllegalArgumentException("EGLの初期化対象に出来ないオブジェクトです");
        }

        // EGL管理クラス生成と初期化
        app.platform.eglManager = new EGLManager();
        app.platform.eglManager.initialize(EGLManager.ColorSpec.RGBA8);
    }

    /**
     * 一時停止を行う
     */
    public void onPause() {
        status = Status.PAUSE;
    }

    /**
     * レジュームを行う
     */
    public void onResume() {
        status = Status.RUN;
    }

    /**
     * スレッドの廃棄・終了を行う
     */
    public void onDestroy() {
        if (status == Status.DESTROY) {
            // 既に廃棄済みのため何もしない
            return;
        }

        status = Status.DESTROY;

        try {
            // レンダリング停止待ちを行う
            join();
        } catch (Exception e) {
        }
    }

    /**
     * スレッドの廃棄命令が出ていなければtrue
     * @return
     */
    private boolean isAvailable() {
        return status != Status.DESTROY;
    }

    /**
     * 毎フレームの描画処理を行う
     */
    @Override
    public void run() {

        // 描画用デバイスが生成されるまで待つ
        while (app.platform.windowDevice == null) {
            Util.sleep(1);
        }

        final EGLDevice windowDevice = app.platform.windowDevice;

        // サーフェイスが使用可能な状態になるまで待つ
        while (!windowDevice.isSurfaceAvailable() && isAvailable()) {
            Util.sleep(1);
        }

        // バインドを行う
        windowDevice.bind();

        // 初期化処理を行う
        {
            Log.d(TAG, "app initialize");
            app.initialize();
            Log.d(TAG, "app initialize finish");
        }

        if (app.hasAsync()) {
            // 非同期処理を開始する
            new WorkerThread(app).start();
        }

        // アプリの停止命令が出るか、終了命令が出るまで描画を繰り返す
        while (isAvailable() && !app.isAbort()) {
            if (status == Status.RUN && windowDevice.isSurfaceAvailable()) {
                // 実行が行える状態で、かつサーフェイスが生きているならば実行を続ける

                // バインドが行われていなければ、バインドを行う
                if (!windowDevice.isBinded()) {
                    windowDevice.bind();
                }

                // サーフェイスサイズが変わっていたら通知する
                if (app.surface_width != windowDevice.getSurfaceWidth() || app.surface_height != windowDevice.getSurfaceHeight()) {

                    // アプリに通知済みのサーフェイスサイズを変更する
                    app.surface_width = windowDevice.getSurfaceWidth();
                    app.surface_height = windowDevice.getSurfaceHeight();

                    app.resized();
                }

                // 実行中
                app.rendering();
            } else {
                // バインドが行われていたら、解除をしなければならない
                if (windowDevice.isBinded()) {
                    windowDevice.unbind();
                }

                // 一時停止
                Util.sleep(1);
            }
        }

        // 終了処理を行う
        Log.d(TAG, "app destroy");
        app.destroy();
        Log.d(TAG, "app destroy finish");

        // バインド解除を行う
        windowDevice.unbind();
    }

    /**
     * デバイスがバインド解除されるまで待つ
     */
    private void waitUnbindDevice() {
        EGLDevice windowDevice = app.platform.windowDevice;
        if (windowDevice.isDeviceThread()) {
            // 現在のスレッドにバインドされていたら解除する
            windowDevice.unbind();
        } else {
            // バインドが解除されるまで待つ
            while (windowDevice.isBinded()) {
                Util.sleep(1);
            }
        }
    }

    /**
     * SurfaceHolder ≒ SurfaceView向けのリスナ実装
     */
    private class SurfaceHolderCallbackImpl implements SurfaceHolder.Callback {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            Log.d(TAG, "surfaceCreated");

            if (app.platform.windowDevice == null) {
                EGLDevice device = new EGLDevice(app.platform.eglManager);
                app.platform.windowDevice = device;
            }
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.d(TAG, String.format("surfaceChanged fmt(%d) (%d x %d)", format, width, height));

            EGLDevice windowDevice = app.platform.windowDevice;

            // 一度描画を停止させる
            final Status temp = status;
            status = Status.PAUSE;
            waitUnbindDevice();

            // EGLSurfaceの再構築を行わせる
            windowDevice.onSurfaceChanged(holder, width, height);

            // 状態を元に戻す
            status = temp;
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "surfaceDestroyed");

            EGLDevice windowDevice = app.platform.windowDevice;

            // 一度描画を停止させる
            if (status != Status.DESTROY) {
                status = Status.PAUSE;
            }
            // 描画の停止待ちを行う
            waitUnbindDevice();

            // EGLSurfaceを廃棄させる
            windowDevice.onSurfaceDestroyed();

            if (status == Status.DESTROY) {
                Log.d(TAG, "Status.DESTROY");
                // アプリが終了状態であればEGLの解放も行う
                windowDevice.destroy();
                app.platform.eglManager.destroy();
            }
        }

    }

    /**
     * SurfaceTexture ≒ TextureView向けのリスナ実装
     * API14以上でしか利用できないため、古い端末ではClassをロードしないようにする
     */
    private class SurfaceTextureListenerImpl implements SurfaceTextureListener {
        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            Log.d(TAG, String.format("onSurfaceTextureAvailable(%d x %d)", width, height));

            if (app.platform.windowDevice == null) {
                EGLDevice device = new EGLDevice(app.platform.eglManager);
                if (vsyncEnable) {
                    // 垂直同期開始
                    device.startVSync();
                }
                app.platform.windowDevice = device;
            }

            // changedと同じ処理で問題ない
            onSurfaceTextureSizeChanged(surface, width, height);
        }

        @Override
        public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
            Log.d(TAG, String.format("onRenderingSurfaceResized(%d x %d)", width, height));

            EGLDevice windowDevice = app.platform.windowDevice;

            // 一度描画を停止させる
            final Status temp = status;
            status = Status.PAUSE;
            waitUnbindDevice();

            // EGLSurfaceの再構築を行わせる
            windowDevice.onSurfaceChanged(surface, width, height);

            // 状態を元に戻す
            status = temp;
        }

        @Override
        public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
            Log.d(TAG, "onSurfaceTextureDestroyed");

            EGLDevice windowDevice = app.platform.windowDevice;

            // 一度描画を停止させる
            if (status != Status.DESTROY) {
                status = Status.PAUSE;
            }

            // 描画の停止待ちを行う
            waitUnbindDevice();

            // EGLSurfaceを廃棄させる
            windowDevice.onSurfaceDestroyed();

            if (status == Status.DESTROY) {
                Log.d(TAG, "Status.DESTROY");
                // アプリが終了状態であればEGLの解放も行う
                windowDevice.destroy();
                app.platform.eglManager.destroy();
            }
            // trueを返すことで自動的にSurfaceTexture内のリソースを解放する
            return true;
        }

        @Override
        public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        }
    }

}
