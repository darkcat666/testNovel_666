package com.eaglesakura.gles20.egl;

import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import android.util.Log;

public class EGLManager {
    /**
     * EGLオブジェクト
     */
    EGL10 egl = null;

    /**
     * レンダリング用ディスプレイ
     */
    EGLDisplay eglDisplay = null;

    /**
     * config情報
     */
    EGLConfig eglConfig = null;

    public EGLManager() {
    }

    /**
     * EGLで利用する色バッファ情報
     */
    public enum ColorSpec {
        /**
         * RGBA各8bit
         */
        RGBA8,

        /**
         * RGB各8bit
         */
        RGB8,

        /**
         * RGB各5/6/5bit
         */
        RGB565,
    }

    /**
     * 初期化を行う
     */
    public void initialize(ColorSpec spec) {
        if (egl != null) {
            throw new RuntimeException("initialized");
        }

        egl = (EGL10) EGLContext.getEGL();

        eglDisplay = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

        {
            int[] versions = new int[2];
            if (!egl.eglInitialize(eglDisplay, versions)) {
                printEGLError(egl);
                throw new RuntimeException("eglInitialize");
            }

            Log.i("EGL", String.format("EGL Version(%d.%d)", versions[0], versions[1]));
        }

        // コンフィグ取得
        {
            eglConfig = chooseConfig(spec, 16, 8);
        }
    }


    /**
     * RGB各色、深度、ステンシルそれぞれが指定に近いconfigを取り出す
     * @param color
     * @param depth_bits
     * @param stencil_bits
     * @return
     */
    private EGLConfig chooseConfig(ColorSpec color, int depth_bits, int stencil_bits) {
        //! コンフィグを全て取得する
        EGLConfig[] configs = new EGLConfig[32];
        // コンフィグ数がeglChooseConfigから返される
        int[] config_num = new int[1];
        if (!egl.eglChooseConfig(eglDisplay, getConfigSpec(color, depth_bits, stencil_bits), configs, configs.length,
                config_num)) {
            throw new RuntimeException("eglChooseConfig");
        }
        final int CONFIG_NUM = config_num[0];

        int r_bits = 0;
        int g_bits = 0;
        int b_bits = 0;
        int a_bits = 0;

        switch (color) {
            case RGBA8:
                r_bits = g_bits = b_bits = a_bits = 8;
                break;
            case RGB8:
                r_bits = g_bits = b_bits = 8;
                break;
            case RGB565:
                r_bits = 5;
                g_bits = 6;
                b_bits = 5;
                break;
            default:
                throw new UnsupportedOperationException(color.toString());
        }

        // 指定したジャストサイズのconfigを探す
        for (int i = 0; i < CONFIG_NUM; ++i) {
            final EGLConfig checkConfig = configs[i];

            final int config_r = getConfigAttrib(checkConfig, EGL10.EGL_RED_SIZE);
            final int config_g = getConfigAttrib(checkConfig, EGL10.EGL_GREEN_SIZE);
            final int config_b = getConfigAttrib(checkConfig, EGL10.EGL_BLUE_SIZE);
            final int config_a = getConfigAttrib(checkConfig, EGL10.EGL_ALPHA_SIZE);
            final int config_d = getConfigAttrib(checkConfig, EGL10.EGL_DEPTH_SIZE);
            final int config_s = getConfigAttrib(checkConfig, EGL10.EGL_STENCIL_SIZE);

            // RGBが指定サイズジャスト、ADSが指定サイズ以上あれば合格とする
            if (config_r == r_bits && config_g == g_bits && config_b == b_bits && config_a >= a_bits
                    && config_d >= depth_bits && config_s >= stencil_bits) {

                Log.i("EGLConfig", String.format("R(%d) G(%d) B(%d) A(%d) D(%d) S(%d)", config_r, config_g, config_b,
                        config_a, config_d, config_s));
                return checkConfig;
            }
        }

        // ジャストサイズが見つからなければ先頭のコンフィグを返す
        return configs[0];
    }

    private int[] getConfigSpec(ColorSpec color, int depth_bits, int stencil_bits) {
        List<Integer> result = new ArrayList<Integer>();
        // レンダラーをES2.0に設定
        {
            result.add(EGL10.EGL_RENDERABLE_TYPE);
            result.add(4); /* EGL_OPENGL_ES2_BIT */
        }

        switch (color) {
            case RGBA8:
                result.add(EGL10.EGL_RED_SIZE);
                result.add(8);
                result.add(EGL10.EGL_GREEN_SIZE);
                result.add(8);
                result.add(EGL10.EGL_BLUE_SIZE);
                result.add(8);
                result.add(EGL10.EGL_ALPHA_SIZE);
                result.add(8);
                break;
            case RGB8:
                result.add(EGL10.EGL_RED_SIZE);
                result.add(8);
                result.add(EGL10.EGL_GREEN_SIZE);
                result.add(8);
                result.add(EGL10.EGL_BLUE_SIZE);
                result.add(8);
                break;
            case RGB565:
                result.add(EGL10.EGL_RED_SIZE);
                result.add(5);
                result.add(EGL10.EGL_GREEN_SIZE);
                result.add(6);
                result.add(EGL10.EGL_BLUE_SIZE);
                result.add(5);
                break;
            default:
                throw new UnsupportedOperationException(color.toString());
        }

        if (depth_bits > 0) {
            result.add(EGL10.EGL_DEPTH_SIZE);
            result.add(depth_bits);
        }

        if (stencil_bits > 0) {
            result.add(EGL10.EGL_STENCIL_SIZE);
            result.add(stencil_bits);
        }

        // 終端
        result.add(EGL10.EGL_NONE);

        int[] result_array = new int[result.size()];
        for (int i = 0; i < result.size(); ++i) {
            result_array[i] = result.get(i);
        }
        return result_array;
    }

    private int getConfigAttrib(EGLConfig eglConfig, int attr) {
        int[] value = new int[1];
        egl.eglGetConfigAttrib(eglDisplay, eglConfig, attr, value);
        return value[0];
    }
    public static void printEGLError(EGL10 egl) {
        int error = egl.eglGetError();
        if (error == EGL10.EGL_SUCCESS) {
            return;
        }

        String message = String.format("EGLError(%x)", egl.eglGetError());
        switch (error) {
            case EGL10.EGL_BAD_ALLOC:
                message = "EGL_BAD_ALLOC";
                break;
            case EGL10.EGL_BAD_CONFIG:
                message = "EGL_BAD_CONFIG";
                break;
            case EGL10.EGL_BAD_ACCESS:
                message = "EGL_BAD_ACCESS";
                break;
            case EGL10.EGL_BAD_ATTRIBUTE:
                message = "EGL_BAD_ATTRIBUTE";
                break;
            case EGL10.EGL_BAD_DISPLAY:
                message = "EGL_BAD_DISPLAY";
                break;
        }

        Log.i("EGL", message);
    }

    public EGL10 getEGL() {
        return egl;
    }

    public EGLDisplay getDisplay() {
        return eglDisplay;
    }

    public EGLConfig getConfig() {
        return eglConfig;
    }

    /**
     * 解放処理を行う
     */
    public void destroy() {
        if (egl == null) {
            return;
        }

        egl.eglTerminate(eglDisplay);

        eglDisplay = null;
        eglConfig = null;
        egl = null;
    }

}
