package com.eaglesakura.gles20;


public class SampleList {

    /**
     * 全チャプター数を取得する
     * @return
     */
    public static native int getChapterNum();

    /**
     * 書籍での章番号を取得する
     * @param chapter_num
     * @return
     */
    public static native int getBookChapterNumber(int chapter_num);

    /**
     * チャプター名を取得する
     * @param chapter_num
     * @return
     */
    public static native String getChapterName(int chapter_num);

    /**
     * チャプターに所属するサンプル数を取得する
     * @param chapter_num
     * @return
     */
    public static native int getChapterSampleNum(int chapter_num);

    /**
     * チャプターに所属するサンプル名を取得する
     * @param chapter_num
     * @param sample_num
     * @return
     */
    public static native String getChapterSampleName(int chapter_num, int sample_num);
}
