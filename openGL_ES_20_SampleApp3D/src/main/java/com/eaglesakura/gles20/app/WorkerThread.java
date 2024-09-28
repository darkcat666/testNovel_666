package com.eaglesakura.gles20.app;

import com.eaglesakura.gles20.egl.EGLDevice;

public class WorkerThread extends Thread {

    /**
     * サンプルアプリ
     */
    private final GLApplication app;

    public WorkerThread(GLApplication app) {
        this.app = app;
    }

    @Override
    public void run() {
        // Windowデバイスとの共有コンテキストを生成する
        EGLDevice windowDevice = app.platform.windowDevice;

        // 非同期デバイスを生成する
        EGLDevice workerDevice = new EGLDevice(app.platform.eglManager);
        workerDevice.initializeOffscreenDevice(windowDevice.getContext());

        // バインドして処理を行う
        workerDevice.bind();
        {
            app.async();
        }
        workerDevice.unbind();

        // 解放を行う
        workerDevice.destroy();
    }
}
