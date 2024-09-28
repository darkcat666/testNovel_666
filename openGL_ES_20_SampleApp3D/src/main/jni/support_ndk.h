/*
 * support_ndk.h
 *
 *  Created on: 2013/02/12
 */

#ifndef SUPPORT_NDK_H_
#define SUPPORT_NDK_H_

#include    "support.h"
#include    "jni.h"
#include    "signatures.h"

/**
 * NDK側初期化を行う
 */
extern void ndk_support_initialize(JNIEnv *env);

/**
 * classを読み込む
 */
extern jclass ndk_loadClass(JNIEnv *env, const char* class_signeture, bool is_static);

/**
 * メソッドの読み込みを行う
 */
extern jmethodID ndk_loadMethod(JNIEnv *env, jclass clazz, const char* method_name, const char* method_signeture, bool is_static);

/**
 * IntのFieldIdを読み込む
 */
extern jfieldID ndk_loadIntField(JNIEnv *env, jclass clazz, const char* field_name);

/**
 * Object
 */
extern jfieldID ndk_loadObjectField(JNIEnv *env, jclass clazz, const char* field_name);

/**
 * Class
 */
extern jfieldID ndk_loadClassField(JNIEnv *env, jclass clazz, const char* class_sign, const char* field_name);

/**
 * BufferのFieldIdを読み込む
 */
extern jfieldID ndk_loadBufferField(JNIEnv *env, jclass clazz, const char* field_name);

/**
 * byte配列のFieldIdを読み込む
 */
extern jfieldID ndk_loadByteArrayField(JNIEnv *env, jclass clazz, const char* field_name);

/**
 * 現在のスレッドで有効なJNIEnvを取得する
 */
extern JNIEnv* ndk_current_JNIEnv();

/**
 * 管理用の構造体
 */
typedef struct NDKPlatform {
    /**
     * SDK側のplatform
     */
    jobject jPlatform;

    /**
     * SDK側のGLApplication
     */
    jobject jGLApplication;
} NDKPlatform;

#endif /* SUPPORT_NDK_H_ */
