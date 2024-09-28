/*
 * GLAppapp_impl.c
 *
 *  Created on: 2013/02/11
 */

#include    "jni.h"
#include    "support.h"
#include    "SampleList.h"

/**
 * チャプター数を取得する
 */
JNIEXPORT jint JNICALL Java_com_eaglesakura_gles20_SampleList_getChapterNum(JNIEnv *env, jobject _this) {
    return (jint) SampleList_getChapterNum();
}

/**
 * チャプター名を取得する
 */
JNIEXPORT jint JNICALL Java_com_eaglesakura_gles20_SampleList_getBookChapterNumber(JNIEnv *env, jobject _this, jint chapter_num) {
    ChapterInfo *info = SampleList_getChapter(chapter_num);
    return info->book_chapternumber;
}

/**
 * チャプター名を取得する
 */
JNIEXPORT jstring JNICALL Java_com_eaglesakura_gles20_SampleList_getChapterName(JNIEnv *env, jobject _this, jint chapter_num) {
    ChapterInfo *info = SampleList_getChapter(chapter_num);
    return (jstring) (*env)->NewStringUTF(env, info->name);
}

/**
 * チャプターに所属するサンプル数を取得する
 */
JNIEXPORT jint JNICALL Java_com_eaglesakura_gles20_SampleList_getChapterSampleNum(JNIEnv *env, jobject _this, jint chapter_num) {
    return (jint) SamplesList_getChapterSampleNum(chapter_num);
}

/**
 * チャプターに所属するサンプル名を取得する
 */
JNIEXPORT jstring JNICALL Java_com_eaglesakura_gles20_SampleList_getChapterSampleName(JNIEnv *env, jobject _this, jint chapter_num, jint sample_num) {
    SampleInfo *info = SampleList_getSample(chapter_num, sample_num);
    return (jstring) (*env)->NewStringUTF(env, info->name);
}
