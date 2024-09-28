/*
 * RawPixelImage_impl.c
 *
 *  Created on: 2014/03/09
 */
#include    "support_ndk.h"

static jclass RawPixelImage_class = NULL;

static jmethodID method_loadImage = NULL;

/**
 * 画像を読み込む。
 * 読み込んだ画像はes20_freeImage()で解放する
 */
RawPixelImage* RawPixelImage_load(GLApplication *app, const char* file_name, const int pixel_format) {
    JNIEnv *env = ndk_current_JNIEnv();

    if (!RawPixelImage_class) {
        RawPixelImage_class = ndk_loadClass(env, RawPixelImage_CLASS_SIGNATURE, true);
        method_loadImage = ndk_loadMethod(env, RawPixelImage_class, "loadImage", "(L"GLApplication_CLASS_SIGNATURE";Ljava/lang/String;I)L"RawPixelImage_CLASS_SIGNATURE";", true);
    }

    int pixelsize = 0;

    /**
     * 1ピクセルの深度を指定する
     */
    switch (pixel_format) {
        case TEXTURE_RAW_RGBA8:
            pixelsize = 4;
            break;
        case TEXTURE_RAW_RGB8:
            pixelsize = 3;
            break;
        case TEXTURE_RAW_RGB565:
        case TEXTURE_RAW_RGBA5551:
            pixelsize = 2;
            break;
    }

    assert(pixelsize > 0);

    NDKPlatform *platform = (NDKPlatform*) app->platform;

    jstring jFileName = (*env)->NewStringUTF(env, file_name);

    jobject jRawImage = (*env)->CallStaticObjectMethod(env, RawPixelImage_class, method_loadImage, platform->jGLApplication, jFileName, pixel_format);

// 読み込み失敗した
    if (!jRawImage) {
        __logf("image(%s) load fail...", file_name);
        return NULL;
    }

    static jfieldID field_width = NULL;
    static jfieldID field_height = NULL;
    static jfieldID field_pixel_data = NULL;

    if (!field_pixel_data) {
        jclass clazz = (*env)->GetObjectClass(env, jRawImage);
        field_width = ndk_loadIntField(env, clazz, "width");
        field_height = ndk_loadIntField(env, clazz, "height");
        field_pixel_data = ndk_loadBufferField(env, clazz, "pixel_data");

        assert(field_width != NULL);
        assert(field_height != NULL);
        assert(field_pixel_data != NULL);

        // 参照削除
        (*env)->DeleteLocalRef(env, clazz);
    }

// 返却用をalloc
    RawPixelImage *image = (RawPixelImage*) malloc(sizeof(RawPixelImage));

    image->format = pixel_format;
    image->width = (*env)->GetIntField(env, jRawImage, field_width);
    image->height = (*env)->GetIntField(env, jRawImage, field_height);
    __logf("image size(%d x %d) format(%x) pot(%s)", image->width, image->height, image->format, Texture_checkPowerOfTwoWH(image->width, image->height) ? "POT" : "NPOT");

// 画像用のメモリを確保
    image->pixel_data = (void*) malloc(image->width * image->height * pixelsize);

// メモリをコピーする
    {
        // SDK側のバッファを取得
        jobject jpixel_data = (*env)->GetObjectField(env, jRawImage, field_pixel_data);

        // メモリアドレスを取得
        void* pixelbuffer = (*env)->GetDirectBufferAddress(env, jpixel_data);

        // ピクセルフォーマット変換
        RawPixelImage_convertColorRGBA(pixelbuffer, pixel_format, image->pixel_data, image->width * image->height);

        // 参照削除
        (*env)->DeleteLocalRef(env, jpixel_data);
    }

// 参照削除
    (*env)->DeleteLocalRef(env, jFileName);
    (*env)->DeleteLocalRef(env, jRawImage);
    return image;
}
