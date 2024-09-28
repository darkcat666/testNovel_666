package com.eaglesakura.gles20.wallpaper;

import android.service.wallpaper.WallpaperService;
import android.view.SurfaceHolder;

import com.eaglesakura.gles20.app.GLApplication;
import com.eaglesakura.gles20.app.RenderingThread;
import com.eaglesakura.gles20.app.ndk.NDKApplication;
import com.eaglesakura.gles20.util.ES20;

public class GLWallpaperService extends WallpaperService {

    static {
        // Native側の初期化を行う
        System.loadLibrary("sample");
        ES20.initializeNative();
    }

    int chapterNumber = 8;
    int sampleNumer = 4;

    @Override
    public Engine onCreateEngine() {
        return new GLEngine();
    }

    class GLEngine extends Engine {

        GLApplication app;
        RenderingThread renderThread;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);
            app = new NDKApplication(chapterNumber, sampleNumer);

            // Activityがないため、Application Contextを利用する
            app.platform.context = getApplicationContext();

            renderThread = new RenderingThread(app);

            // SurfaceHolderとして初期化する
            renderThread.initialize(surfaceHolder);

            renderThread.start();
        }

        @Override
        public void onVisibilityChanged(boolean visible) {
            super.onVisibilityChanged(visible);

            if (visible) {
                renderThread.onResume();
            } else {
                renderThread.onPause();
            }
        }

        @Override
        public void onDestroy() {
            super.onDestroy();
            renderThread.onDestroy();
        }
    }
}
