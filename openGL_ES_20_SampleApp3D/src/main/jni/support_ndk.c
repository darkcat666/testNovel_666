/*
 * JNI_app.c
 *
 *  Created on: 2013/02/12
 */
#include    "support_ndk.h"

static JavaVM *g_javavm = NULL;

/**
 * NDK側サポート関数の初期化を行う
 */
void ndk_support_initialize(JNIEnv *env) {
    if (!g_javavm) {
        (*env)->GetJavaVM(env, &g_javavm);
    }
}

/**
 * classを読み込む
 */
jclass ndk_loadClass(JNIEnv *env, const char* class_signeture, bool is_static) {
    jclass result = (*env)->FindClass(env, class_signeture);
    if (!result) {
        __logf("error FineClass(%s)", class_signeture);
    }
    assert(result != NULL);

    if (is_static) {
        result = (*env)->NewGlobalRef(env, result);
    }

    return result;
}

/**
 * メソッドを読み込む。
 */
jmethodID ndk_loadMethod(JNIEnv *env, jclass clazz, const char* method_name, const char* method_signeture, bool is_static) {
    jmethodID result = NULL;
    if (is_static) {
        result = (*env)->GetStaticMethodID(env, clazz, method_name, method_signeture);
    } else {
        result = (*env)->GetMethodID(env, clazz, method_name, method_signeture);
    }

    if (!result) {
        __logf("error GetMethod(%s(%s))", method_name, method_signeture);
    }
    assert(result != NULL);
    return result;
}

/**
 * IntのFieldIdを読み込む
 */
jfieldID ndk_loadIntField(JNIEnv *env, jclass clazz, const char* field_name) {
    assert(clazz != NULL);
    jfieldID result = (*env)->GetFieldID(env, clazz, field_name, "I");
    assert(result != NULL);
    return result;
}

/**
 * ObjectのFieldIdを読み込む
 */
jfieldID ndk_loadObjectField(JNIEnv *env, jclass clazz, const char* field_name) {
    assert(clazz != NULL);
    jfieldID result = (*env)->GetFieldID(env, clazz, field_name, "Ljava/lang/Object;");
    assert(result != NULL);
    return result;

}

/**
 * ObjectのFieldIdを読み込む
 */
jfieldID ndk_loadClassField(JNIEnv *env, jclass clazz, const char* class_sign, const char* field_name) {
    assert(clazz != NULL);
    jfieldID result = (*env)->GetFieldID(env, clazz, field_name, class_sign);
    assert(result != NULL);
    return result;

}

/**
 * BufferのFieldIdを読み込む
 */
jfieldID ndk_loadBufferField(JNIEnv *env, jclass clazz, const char* field_name) {
    assert(clazz != NULL);
    jfieldID result = (*env)->GetFieldID(env, clazz, field_name, "Ljava/nio/Buffer;");
    assert(result != NULL);
    return result;

}

/**
 * byte配列のFieldIdを読み込む
 */
jfieldID ndk_loadByteArrayField(JNIEnv *env, jclass clazz, const char* field_name) {
    assert(clazz != NULL);
    jfieldID result = (*env)->GetFieldID(env, clazz, field_name, "[B");
    assert(result != NULL);
    return result;
}

/**
 * 実行中スレッドに合ったJNIEnv*を取得する
 */
JNIEnv* ndk_current_JNIEnv() {
    JNIEnv *result = NULL;
    assert(g_javavm);
    (*g_javavm)->GetEnv(g_javavm, (void**) &result, JNI_VERSION_1_6);
    return result;
}
