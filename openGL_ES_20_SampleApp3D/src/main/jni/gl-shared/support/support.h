#ifndef OpenGL_ES_20_support_h
#define OpenGL_ES_20_support_h

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <assert.h>
#include    <string.h>
#include    <math.h>

#ifdef __APPLE__
#ifdef __OBJC__
#define CSTR2NSSTRING(c_str)       [[NSString alloc] initWithUTF8String:c_str]

// Objective-C source
#define     __log(message)        NSLog(@message)
#define     __logf(fmt, ...)      NSLog(@fmt, __VA_ARGS__)
#else
// C Source
#define     __log(message)        printf("%s\n", message)
#define     __logf(fmt, ...)      printf(fmt "\n", __VA_ARGS__)

#endif

#else // ANDROID
#include    <android/log.h>
#define __LOG_TAG   "GLES20"
#define __log(msg)       __android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG, "%s", msg)
#define __logf(...)      __android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG, __VA_ARGS__)

#endif

/**
 * アプリを強制停止させる（対応してないextension解説等）
 */
#define GLAPP_FLAG_ABORT (0x1 << 2)

struct GLApplication;

/**
 * アプリの初期化を行う
 */
typedef void (*GLApplication_initialize)(struct GLApplication *app);

/**
 * アプリの非同期処理を行う
 */
typedef void (*GLApplication_async)(struct GLApplication *app);

/**
 * レンダリングエリアが変更された
 */
typedef void (*GLApplication_surface_changed)(struct GLApplication *app);

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
typedef void (*GLApplication_rendering)(struct GLApplication *app);

/**
 * アプリのデータ削除を行う
 */
typedef void (*GLApplication_destroy)(struct GLApplication *app);

/**
 * アプリに与えられるデータ
 * プラットフォームごとに適当な拡張を行なっている。
 */
typedef struct GLApplication {
    /**
     * View（レンダリングターゲット）の幅
     */
    int surface_width;

    /**
     * View（レンダリングターゲット）の高さ
     */
    int surface_height;

    /**
     * アプリ実行フラグ
     */
    int flags;

    /**
     * プラットフォーム固有データを格納する。
     * サンプルアプリから扱う必要はない。
     */
    void* platform;

    /**
     * 初期化関数
     */
    GLApplication_initialize initialize;

    /**
     * 非同期処理
     */
    GLApplication_async async;

    /**
     * サーフェイスサイズ変更関数
     */
    GLApplication_surface_changed resized;

    /**
     * レンダリング関数
     */
    GLApplication_rendering rendering;

    /**
     * 解放関数
     */
    GLApplication_destroy destroy;

    /**
     * サンプルごとの拡張データ保持用変数。
     * 自由に構造体等をallocして利用する。
     * XXX_destroy時に必ずfreeを行うこと。
     */
    void* extension;
} GLApplication;

/**
 * ファイルのフルパス -> ファイル名に変換する('/'区切り専用)
 */
extern char* util_getFileName(char* __file__);

/**
 * ダイアログを出して実行を停止する
 */
extern void GLApplication_abortWithMessage(GLApplication *app, const char* message);

/**
 * アプリ実行を停止している場合はapp_TRUE
 */
extern bool GLApplication_isAbort(GLApplication *app);

/**
 * ファイル系サポート関数
 */
#include    "support_RawData.h"

/**
 * GL系サポート関数宣言
 */
#include    "support_gl.h"

#endif
