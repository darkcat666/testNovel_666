package com.eaglesakura.gles20.app.ndk;

import com.eaglesakura.gles20.app.GLApplication;

public class NDKApplication extends GLApplication {
    /**
     * NDK側のGLApplicationポインタ
     */
    int GLApplication_ptr;

    /**
     * 選択されたチャプター番号
     */
    int chapterNum;

    /**
     * 選択されたサンプル番号
     */
    int sampleNum;

    public NDKApplication(int chapterNum, int sampleNum) {
        this.chapterNum = chapterNum;
        this.sampleNum = sampleNum;
    }

    @Override
    public native void initialize();

    @Override
    public native void async();

    @Override
    public native void resized();

    @Override
    public native void rendering();

    @Override
    public native void destroy();

    @Override
    public native boolean hasAsync();
}
