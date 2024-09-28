/*
 * es20app.h
 *
 *  Created on: 2013/02/19
 */

#ifndef ES20APP_H_
#define ES20APP_H_

/**
 * サンプルの関数と名称を保持する
 */
typedef struct SampleInfo {
    // サンプル名
    char *name;


    /**
     * 初期化関数
     */
    GLApplication_initialize func_initialize;

    /**
     * サーフェイスサイズ変更関数
     */
    GLApplication_surface_changed func_resized;

    /**
     * レンダリング関数
     */
    GLApplication_rendering func_rendering;

    /**
     * 解放関数
     */
    GLApplication_destroy func_destroy;
} SampleInfo;

/**
 * 各チャプターを保持する
 */
typedef struct ChapterInfo {
    // チャプター名
    char *name;

    // 章番号
    int book_chapternumber;

    // サンプルの配列
    SampleInfo *sample_info;
} ChapterInfo;

/**
 * アプリで解説するチャプター数を取得する
 */
extern int SampleList_getChapterNum();

/**
 * 任意の番号のチャプター番号を返す。
 */
extern ChapterInfo* SampleList_getChapter(int chapter_num);

/**
 * チャプターに所属するサンプル数を取得する。
 */
extern int SamplesList_getChapterSampleNum(int chapter_num);

/**
 * チャプターサンプルの情報を取得する
 */
extern SampleInfo* SampleList_getSample(int chapter_num, int sample_num);

#endif /* ES20APP_H_ */
