/*
 * support_gl_texture.h
 *
 *  Created on: 2013/02/19
 */

#ifndef SUPPORT_GL_TEXTURE_H_
#define SUPPORT_GL_TEXTURE_H_

/**
 * RGBA各8bit色
 *
 * 32bit / pixel
 */
#define TEXTURE_RAW_RGBA8         0

/**
 * RGB各8bit色
 *
 * 24bit / pixel
 */
#define TEXTURE_RAW_RGB8          1

/**
 * RGB各5bit
 * A 1bit
 *
 * 16bit / pixel
 */
#define TEXTURE_RAW_RGBA5551     2

/**
 * R 5bit
 * G 6bit
 * B 5bit
 *
 * 16bit/ pixel
 */
#define TEXTURE_RAW_RGB565       3

/**
 * 読み込んだ画像のピクセル情報をそのまま保存する構造体
 */
typedef struct RawPixelImage {
    /**
     * ピクセル配列
     */
    void* pixel_data;

    /**
     * 画像幅
     */
    int width;

    /**
     * 画像高さ
     */
    int height;

    /**
     * 画像フォーマット（下記のいずれか）
     * TEXTURE_RAW_RGBA8
     * TEXTURE_RAW_RGB8
     * TEXTURE_RAW_RGBA5551
     * TEXTURE_RAW_RGB565
     */
    int format;
} RawPixelImage;
/**
 * 画像を読み込む。
 * 読み込んだ画像はes20_freeImage()で解放する
 */
extern RawPixelImage* RawPixelImage_load(GLApplication *app, const char* file_name, const int pixel_format);

/**
 * es20_loadImage()関数から読み込んだ画像を解放する
 */
extern void RawPixelImage_free(GLApplication *app, RawPixelImage *image);

/**
 * RGB888のポインタをdst_pixelsへピクセル情報をコピーする。
 */
extern void RawPixelImage_convertColorRGB(const void *rgb888_pixels, const int pixel_format, void *dst_pixels, const int pixel_num);

/**
 * RGBA8888のポインタをdst_pixelsへピクセル情報をコピーする。
 */
extern void RawPixelImage_convertColorRGBA(const void *rgba8888_pixels, const int pixel_format, void *dst_pixels, const int pixel_num);

/**
 * テクスチャ用構造体
 */
typedef struct Texture {
    /**
     * 画像幅
     */
    int width;

    /**
     * 画像高さ
     */
    int height;

    /**
     * GL側のテクスチャID
     */
    GLuint id;
} Texture;

/**
 * 引数sizeがpotならtrueを返す。
 */
extern bool Texture_checkPowerOfTwo(const int size);

/**
 * 引数width/height共にがpotならtrueを返す。
 */
extern bool Texture_checkPowerOfTwoWH(const int width, const int height);

/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
extern Texture* Texture_load(GLApplication *app, const char* file_name, const int pixel_fotmat);

/**
 * テクスチャの縦横が2のn乗であればtrueを返す
 */
extern bool Texture_isPowerOfTwo(Texture *texture);

/**
 * テクスチャを解放する。
 */
extern void Texture_free(Texture *texture);

#endif /* SUPPORT_GL_TEXTURE_H_ */
