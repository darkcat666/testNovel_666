package com.eaglesakura.gles20;

import java.util.Locale;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.TextureView;
import android.widget.Toast;

import com.eaglesakura.gles20.app.GLApplication;
import com.eaglesakura.gles20.app.RenderingThread;
import com.eaglesakura.gles20.app.ndk.NDKApplication;

public class RenderingActivity extends Activity {

    static final String TAG = RenderingActivity.class.getSimpleName();

    /**
     * 起動するチャプター番号
     */
    public static final String EXTRA_CHAPTER_NUMBER = "EXTRA_CHAPTER_NUMBER";

    /**
     * 起動するサンプル番号
     */
    public static final String EXTRA_SAMPLE_NUMBER = "EXTRA_SAMPLE_NUMBER";

    /**
     * TextureViewを使用する
     */
    public static final String EXTRA_MODE_TEXTUREVIEW = "EXTRA_MODE_TEXTUREVIEW";

    /**
     * 垂直同期を行う
     */
    public static final String EXTRA_MODE_VSYNC = "EXTRA_MODE_VSYNC";

    /**
     * 起動するチャプター番号
     */
    int chapterNumber = -1;

    /**
     * 起動するサンプル番号
     */
    int sampleNumber = -1;

    /**
     * サンプル向けのレンダリングスレッド
     */
    RenderingThread renderThread;

    @SuppressLint("NewApi")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate");

        chapterNumber = getIntent().getIntExtra(EXTRA_CHAPTER_NUMBER, -1);
        sampleNumber = getIntent().getIntExtra(EXTRA_SAMPLE_NUMBER, -1);
        boolean use_textureview = getIntent().getBooleanExtra(EXTRA_MODE_TEXTUREVIEW, false);
        boolean vsync_enable = getIntent().getBooleanExtra(EXTRA_MODE_VSYNC, false);

        if (chapterNumber < 0 || sampleNumber < 0) {
            Toast.makeText(this, String.format(Locale.getDefault(), "Intentが不正です(%d - %d)", chapterNumber, sampleNumber), Toast.LENGTH_LONG).show();
            finish();
        }

        Log.d(TAG, String.format("sample app(%02d-%02d) start", chapterNumber + 1, sampleNumber + 1));

        // タイトル設定
        setTitle(SampleList.getChapterSampleName(chapterNumber, sampleNumber));

        GLApplication app = new NDKApplication(chapterNumber, sampleNumber);
        app.platform.context = this;
        renderThread = new RenderingThread(app);

        if (use_textureview) {
            Log.i(TAG, "Rendering TextureView");
            TextureView view = new TextureView(this);
            setContentView(view);
            renderThread.initialize(view);

            // TextureViewのみ、垂直同期を行う
            renderThread.setVsyncEnable(vsync_enable);
        } else {
            Log.i(TAG, "Rendering SurfaceView");
            SurfaceView view = new SurfaceView(this);
            setContentView(view);
            renderThread.initialize(view);
        }

        // 描画スレッドを開始する
        renderThread.start();
    }

    @Override
    protected void onPause() {
        Log.i(TAG, "onPause");
        super.onPause();

        renderThread.onPause();
        if (isFinishing()) {
            renderThread.onDestroy();
        }
    }

    @Override
    protected void onResume() {
        Log.i(TAG, "onResume");
        super.onResume();

        renderThread.onResume();
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "onDestroy");
        super.onDestroy();
        // pause状態のままActivityが廃棄されることに備え、こちらでも廃棄を行う
        renderThread.onDestroy();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.i(TAG, "onConfigurationChanged");
    }
}
