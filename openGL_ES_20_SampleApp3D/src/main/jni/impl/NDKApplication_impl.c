/*
 * NDKApplication_impl.c
 *
 *  Created on: 2013/04/07
 */

#include    "support_ndk.h"
#include    "SampleList.h"

static jclass class_NDKApplication = NULL;

static jclass class_GLApplication = NULL;

/**
 * NDK側のGLApplicationポインタ
 */

static jfieldID field_GLApplication_ptr = NULL;

/**
 * View（レンダリングターゲット）の幅
 */
static jfieldID field_surface_width = NULL;

/**
 * View（レンダリングターゲット）の高さ
 */
static jfieldID field_surface_height = NULL;

/**
 * アプリ実行フラグ
 */
static jfieldID field_flags = NULL;

/**
 * プラットフォームごとに保持している拡張データ
 * サンプルアプリから扱う必要はない。
 */
static jfieldID field_platform = NULL;

/**
 * アプリを強制終了させるメソッド
 */
static jmethodID method_abortWithMessage = NULL;

/**
 * NDKの情報をSDKにコピーする
 */
static void NDKApplication_syncNDK2SDK(JNIEnv *env, jobject _this, GLApplication *app) {
    (*env)->SetIntField(env, _this, field_surface_width, app->surface_width);
    (*env)->SetIntField(env, _this, field_surface_height, app->surface_height);
    (*env)->SetIntField(env, _this, field_flags, app->flags);
}

JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_initialize(JNIEnv *env, jobject _this) {

    // 必要なリフレクションを得る
    if (!class_NDKApplication) {
        class_NDKApplication = (jclass) (*env)->NewGlobalRef(env, (*env)->FindClass(env, NDKApplication_CLASS_SIGNATURE));
        class_GLApplication = (*env)->NewGlobalRef(env, (*env)->GetSuperclass(env, class_NDKApplication));

        field_surface_width = ndk_loadIntField(env, class_GLApplication, "surface_width");
        field_surface_height = ndk_loadIntField(env, class_GLApplication, "surface_height");
        field_flags = ndk_loadIntField(env, class_GLApplication, "flags");
        field_platform = ndk_loadClassField(env, class_GLApplication, "L" AndroidPlatformData_CLASS_SIGNATURE ";", "platform");

        field_GLApplication_ptr = ndk_loadIntField(env, class_NDKApplication, "GLApplication_ptr");

        method_abortWithMessage = ndk_loadMethod(env, class_GLApplication, "abortWithMessage", "(L" GLApplication_CLASS_SIGNATURE ";Ljava/lang/String;)V", true);
    }

    // アプリ構造体を作成する
    GLApplication *app = (GLApplication*) calloc(1, sizeof(GLApplication));
    {
        // platform固有データを設定する
        {
            NDKPlatform *platform = (NDKPlatform*) malloc(sizeof(NDKPlatform));
            platform->jGLApplication = (*env)->NewGlobalRef(env, _this);
            platform->jPlatform = (*env)->NewGlobalRef(env, (*env)->GetObjectField(env, _this, field_platform));
            app->platform = (void*) platform;
        }

        // 関数ポインタを設定する
        {
            jfieldID field_chapterNum = ndk_loadIntField(env, class_NDKApplication, "chapterNum");
            jfieldID field_sampleNum = ndk_loadIntField(env, class_NDKApplication, "sampleNum");

            const int chapter_num = (*env)->GetIntField(env, _this, field_chapterNum);
            const int sample_num = (*env)->GetIntField(env, _this, field_sampleNum);
            SampleInfo *sampleInfo = SampleList_getSample(chapter_num, sample_num);
            app->initialize = sampleInfo->func_initialize;
            app->resized = sampleInfo->func_resized;
            app->rendering = sampleInfo->func_rendering;
            app->destroy = sampleInfo->func_destroy;
        }
    }

    // 構造体を保存する
    (*env)->SetIntField(env, _this, field_GLApplication_ptr, (jint) app);

    // サンプル関数に処理を行わせる
    (*app->initialize)(app);
}

JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_resized(JNIEnv *env, jobject _this) {
    GLApplication *app = (GLApplication*) (*env)->GetIntField(env, _this, field_GLApplication_ptr);
    assert(app != NULL);

    // サーフェイスの幅と高さを同期する
    {
        app->surface_width = (*env)->GetIntField(env, _this, field_surface_width);
        app->surface_height = (*env)->GetIntField(env, _this, field_surface_height);
    }

    // サンプル関数に処理を行わせる
    (*app->resized)(app);

    // 変数情報をNDK -> SDKにコピーする
    NDKApplication_syncNDK2SDK(env, _this, app);
}

JNIEXPORT jboolean JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_hasAsync(JNIEnv *env, jobject _this) {
    GLApplication *app = (GLApplication*) (*env)->GetIntField(env, _this, field_GLApplication_ptr);
    assert(app != NULL);

    return app->async != NULL;
}

JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_async(JNIEnv *env, jobject _this) {
    GLApplication *app = (GLApplication*) (*env)->GetIntField(env, _this, field_GLApplication_ptr);
    assert(app != NULL);
    // サンプル関数に処理を行わせる
    if (app->async) {
        (*app->async)(app);
    }
//    (*app->rendering)(app);
}

JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_rendering(JNIEnv *env, jobject _this) {
    GLApplication *app = (GLApplication*) (*env)->GetIntField(env, _this, field_GLApplication_ptr);
    assert(app != NULL);
    // サンプル関数に処理を行わせる
    (*app->rendering)(app);

    // 変数情報をNDK -> SDKにコピーする
    NDKApplication_syncNDK2SDK(env, _this, app);
}

JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_app_ndk_NDKApplication_destroy(JNIEnv *env, jobject _this) {
    GLApplication *app = (GLApplication*) (*env)->GetIntField(env, _this, field_GLApplication_ptr);
    assert(app != NULL);
    (*app->destroy)(app);

    // 参照削除
    {
        NDKPlatform *platform = (NDKPlatform*) app->platform;
        (*env)->DeleteGlobalRef(env, platform->jGLApplication);
        (*env)->DeleteGlobalRef(env, platform->jPlatform);

    }
    // GLApplicationを廃棄する
    free(app->platform);
    free(app);
}

/**
 * ダイアログを出して実行を停止する
 */
void GLApplication_abortWithMessage(GLApplication *app, const char* message) {
    NDKPlatform *platform = (NDKPlatform*) app->platform;
    JNIEnv *env = ndk_current_JNIEnv();

    jstring jMessage = (*env)->NewStringUTF(env, message);
    // SDK側でダイアログを出してシャットダウン

    (*env)->CallStaticVoidMethod(env, class_GLApplication, method_abortWithMessage, platform->jGLApplication, jMessage);
    // destroy
    (*env)->DeleteLocalRef(env, jMessage);

    app->flags |= GLAPP_FLAG_ABORT;
}
