package com.eaglesakura.gles20.egl;

import static javax.microedition.khronos.egl.EGL10.*;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.annotation.SuppressLint;
import android.opengl.GLES20;
import android.util.Log;
import android.view.Choreographer;

/**
 * 一つのThreadからアクセスされるEGL/EGLContext/EGLSurfaceを1グループにまとめたオブジェクトをDeviceとする
 */
public class EGLDevice {
    static final String TAG = EGLDevice.class.getSimpleName();
    final EGL10 egl;
    final EGLConfig config;

    private final int[] CONTEXT_ATTRIBUTES = {
            0x3098 /* EGL_CONTEXT_CLIENT_VERSION */, 2, EGL10.EGL_NONE
    };

    /**
     * 垂直同期信号
     */
    Object vsyncLock;

    /**
     * 操作対象のContext
     */
    EGLContext context;

    /**
     * 描画対象のサーフェイス
     */
    EGLSurface surface;

    /**
     * 描画対象のディスプレイ
     */
    EGLDisplay display;

    /**
     * サーフェイス幅ピクセル
     */
    int surfaceWidth;

    /**
     * サーフェイス高ピクセル
     */
    int surfaceHeight;

    /**
     * バインド済みのThread
     */
    Thread bindedThread;

    public EGLDevice(EGLManager eglManager) {
        this.egl = eglManager.getEGL();
        this.config = eglManager.getConfig();
        this.display = eglManager.getDisplay();
    }

    /**
     * サーフェイス幅を取得する
     * @return
     */
    public int getSurfaceWidth() {
        return surfaceWidth;
    }

    /**
     * サーフェイス高を取得する
     * @return
     */
    public int getSurfaceHeight() {
        return surfaceHeight;
    }

    /**
     * コンテキストを取得する
     * @return
     */
    public EGLContext getContext() {
        return context;
    }

    /**
     * サーフェイスの生成・サイズ変更がなされた
     * @param newWidth
     * @param newHeight
     */
    public void onSurfaceChanged(Object native_window, int newWidth, int newHeight) {

        // コンテキストが未生成であれば生成する
        if (context == null) {
            context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, CONTEXT_ATTRIBUTES);
            if (context == EGL10.EGL_NO_CONTEXT) {
                EGLManager.printEGLError(egl);
                throw new IllegalStateException("eglCreateContext fail...");
            }
        }

        this.surfaceWidth = newWidth;
        this.surfaceHeight = newHeight;

        // 既存サーフェイスを廃棄する
        // サーフェイス縦横サイズが変更された時の処理
        if (surface != null) {
            egl.eglDestroySurface(display, surface);
            surface = null;
        }

        // サーフェイスを生成する
        surface = egl.eglCreateWindowSurface(display, config, native_window, null);

        if (surface == EGL10.EGL_NO_SURFACE) {
            EGLManager.printEGLError(egl);
            throw new IllegalStateException("eglCreateWindowSurface fail..");
        }
    }

    /**
     * サーフェイスが廃棄された
     */
    public void onSurfaceDestroyed() {
        if (surface != null) {
            // 生存チェックに使っているため、ローカル変数に対比させてサーフェイスをnullにする
            final EGLSurface destroySurface = surface;
            surface = null;

            // 解放処理を行う
            // ここで解放するにはサーフェイスのみである
            egl.eglDestroySurface(display, destroySurface);
        }
    }

    /**
     * Deviceに関連付けられたThreadの場合はtrueを返す。
     * @return
     */
    public boolean isDeviceThread() {
        return Thread.currentThread().equals(bindedThread);
    }

    /**
     * OpenGL ESの呼び出しが行える状態の場合true
     * ただし、厳密なThreadチェックは行っていない。
     * @return
     */
    public boolean isBinded() {
        return bindedThread != null;
    }

    /**
     * 現在のThreadに対してバインドを行い、OpenGL ESの呼び出しが行える状態にする
     */
    public void bind() {
        if (egl.eglMakeCurrent(display, surface, surface, context)) {
            this.bindedThread = Thread.currentThread();
        }
    }

    /**
     * 現在のThreadからアンバインドを行い、EGLをフリーな状態にする
     */
    public void unbind() {
        // Finish待ちを行う
        GLES20.glFinish();

        if (egl.eglMakeCurrent(display, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT)) {
            // どのThreadにも関連付けられていない
            bindedThread = null;
        }
    }

    /**
     * フロントバッファへ反映を行う
     */
    public void postFrontBuffer() {
        if (bindedThread != Thread.currentThread()) {
            return;
        }

        if (!egl.eglSwapBuffers(display, surface)) {
            EGLManager.printEGLError(egl);
            throw new IllegalStateException("eglSwapBuffers failed...");
        }

        // 垂直同期待ちを行う
        waitVSync();
    }

    /**
     * 垂直同期待ちを行う。
     * Android 4.1以上
     */
    private void waitVSync() {
        if (vsyncLock != null) {
            synchronized (vsyncLock) {
                try {
                    // 16ms以上は60fpsを保てなくなるため、強制的にwaitを終了させて問題ない
                    vsyncLock.wait(16);
                } catch (Exception e) {

                }
            }
        }
    }

    /**
     * 垂直同期を開始する
     */
    @SuppressLint("NewApi")
    public void startVSync() {

        vsyncLock = new Object();

        // 垂直同期のコールバック設定
        Choreographer.getInstance().postFrameCallback(new Choreographer.FrameCallback() {
            @Override
            public void doFrame(long frameTimeNanos) {
                synchronized (vsyncLock) {
                    vsyncLock.notifyAll();

                    // 次の垂直同期設定を行う
                    if (isSurfaceAvailable()) {
                        Choreographer.getInstance().postFrameCallback(this);
                    }
                }
            }
        });
    }

    /**
     * サーフェイスが使用可能な状態にある場合true
     * @return
     */
    public boolean isSurfaceAvailable() {
        return surface != null && !surface.equals(EGL10.EGL_NO_SURFACE);
    }

    /**
     * デバイスの廃棄を行う
     */
    public void destroy() {
        Log.d(TAG, "destroy surface/context");

        if (surface != null) {
            final EGLSurface destroySurface = surface;
            surface = null;

            if (!egl.eglDestroySurface(display, destroySurface)) {
                EGLManager.printEGLError(egl);
                throw new IllegalStateException("eglDestroySurface fail...");
            }
        }

        if (context != null) {
            if (!egl.eglDestroyContext(display, context)) {
                EGLManager.printEGLError(egl);
                throw new IllegalStateException("eglDestroyContext fail...");
            }
            context = null;
        }
    }

    /**
     * 
     * @param ms
     */
    protected void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (Exception e) {

        }
    }

    /**
     * オフスクリーンレンダリング用の初期化を行う
     */
    public void initializeOffscreenDevice(EGLContext share_context) {
        context = egl.eglCreateContext(display, config, share_context, CONTEXT_ATTRIBUTES);
        if (context == EGL10.EGL_NO_CONTEXT) {
            EGLManager.printEGLError(egl);
            throw new IllegalStateException("eglCreateContext fail...");
        }

        // オフスクリーンレンダリングのため、1x1サーフェイスで十分
        final int[] OFFSCREEN_ATTRIBUTE = new int[] {
                //
                EGL_WIDTH, 1, EGL_HEIGHT, 1,
                //
                EGL_NONE,
        };
        surface = egl.eglCreatePbufferSurface(display, config, OFFSCREEN_ATTRIBUTE);
        if (surface == null) {
            throw new IllegalArgumentException("eglCreatePbufferSurface failed...");
        }
    }

}
