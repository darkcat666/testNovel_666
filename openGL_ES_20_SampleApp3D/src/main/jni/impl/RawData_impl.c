/*
 * RawData_impl.c
 *
 *  Created on: 2013/04/08
 */
#include    "support_ndk.h"

/**
 * class
 */
static jclass class_RawData = NULL;

/**
 * loadFileメソッド
 */
static jmethodID method_loadFile = NULL;

/**
 * assets配下からファイルを読み込む
 */
RawData* RawData_loadFile(GLApplication *app, const char* file_name) {

    NDKPlatform *platform = (NDKPlatform*) app->platform;
    JNIEnv *env = ndk_current_JNIEnv();

    if (!class_RawData) {
        class_RawData = ndk_loadClass(env, RawData_CLASS_SIGNATURE, true);
        method_loadFile = ndk_loadMethod(env, class_RawData, "loadFile", "(L" GLApplication_CLASS_SIGNATURE ";Ljava/lang/String;)L" RawData_CLASS_SIGNATURE ";", true);
    }

    jstring jFileName = (*env)->NewStringUTF(env, file_name);
    // SDK側で読み込ませる
    jobject jRawData = (*env)->CallStaticObjectMethod(env, class_RawData, method_loadFile, platform->jGLApplication, jFileName);

    // destroy
    (*env)->DeleteLocalRef(env, jFileName);

    // 読込チェック
    if (!jRawData) {
        // 読込エラー
        return NULL;
    }

    static jfieldID field_buffer = NULL;

    if (!field_buffer) {
        // get field
        jclass clazz = (*env)->GetObjectClass(env, jRawData);
        assert(clazz != NULL);
        field_buffer = ndk_loadBufferField(env, clazz, "head");
        assert(field_buffer != NULL);
        // destroy
        (*env)->DeleteLocalRef(env, clazz);
    }

    RawData *result = (RawData*) malloc(sizeof(RawData));
    jobject sdk_buffer = (*env)->GetObjectField(env, jRawData, field_buffer);
    // destroy
    (*env)->DeleteLocalRef(env, jRawData);

    {
        // bufferアドレスを取り出す
        void* buffer_addr = (*env)->GetDirectBufferAddress(env, sdk_buffer);
        // bufferの容量を取り出す
        result->length = (*env)->GetDirectBufferCapacity(env, sdk_buffer);

        // メモリをコピーする
        result->head = (void*) malloc(result->length);
        memcpy(result->head, buffer_addr, result->length);
    }

    // destroy
    (*env)->DeleteLocalRef(env, sdk_buffer);

    result->read_head = (uint8_t*) result->head;
    return result;
}

