/*
 * CompressedTexture.h
 *
 *  Created on: 2013/04/07
 */

#ifndef COMPRESSEDTEXTURE_H_
#define COMPRESSEDTEXTURE_H_

/**
 * 圧縮テクスチャ ETC1-PKM形式
 * Android標準サポート
 */
#define TEXTURE_COMPRESS_ETC1         10

/**
 * 圧縮テクスチャ PVRTC形式
 * PowerVR系GPUがサポート
 */
#define TEXTURE_COMPRESS_PVRTC        11

/**
 * 圧縮テクスチャ Khronos Texture形式
 * Tegra(DXT)/Adreno(ATC)がサポート
 */
#define TEXTURE_COMPRESS_KTX          12


/**
 * PKMフォーマット画像
 * Android標準ツールで作成可能
 */
typedef struct PkmImage {
    /**
     * origin data
     */
    RawData *raw;

    /**
     * 解像度が2のn乗であるかのチェック。
     * TEXTURE_POT | TEXTURE_NPOT
     */
    int power_of_two;

    /**
     * 画像幅
     */
    int width;

    /**
     * 画像高さ
     */
    int height;

    /**
     * 圧縮前の画像幅
     */
    int origin_width;

    /**
     * 圧縮前の画像高さ
     */
    int origin_height;

    /**
     * ETC1 = ETC1_RGB_NO_MIPMAPS = 0
     */
    int16_t data_type;

    /**
     * 圧縮テクスチャへのポインタ
     */
    void* image;

    /**
     * 圧縮テクスチャ本体の長さ
     */
    int image_bytes;
} PkmImage;


/**
 * PKM圧縮画像を読み込む。
 * 読み込んだ画像はPkmImage_free()で解放する
 */
extern PkmImage* PkmImage_load(GLApplication *app, const char* file_name);

/**
 * PKM圧縮画像を解放する
 */
extern void PkmImage_free(GLApplication *app, PkmImage *pkm);


/**
 * PVRTCフォーマット画像
 * iOS標準ツールで作成可能
 */
typedef struct PvrtcImage {
    /**
     * 読み込み元のデータ
     */
    RawData *raw;

    /**
     * PVRTCフォーマット。下記のどれかが設定される。
     * GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
     * GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
     */
    GLenum format;

    /**
     * 1pixelごとのビット数
     */
    int bits_per_pixel;

    /**
     * 解像度が2のn乗であるかのチェック。
     * PVRTCは仕様上、常にES20_POTとなる。
     */
    int power_of_two;

    /**
     * 画像幅
     * mipmapを含む場合、最大（miplevel 0）の値が格納される
     * PVRTCは仕様上、常にwidth == heightとなる。
     */
    int width;

    /**
     * 画像高さ
     * mipmapを含む場合、最大（miplevel 0）の値が格納される
     * PVRTCは仕様上、常にwidth == heightとなる。
     */
    int height;

    /**
     * 画像のmipmap数
     */
    int mipmaps;

    /**
     * 画像の長さ
     * mipmap数だけ格納されている
     */
    int* image_length_table;

    /**
     * 各画像へのポインタ
     * mipmap数だけ格納されている
     */
    void** image_table;
} PvrtcImage;

/**
 * PVRTC圧縮画像を読み込む
 * 読み込んだ画像はes20_freePvrtcImage()で解放する
 */
extern PvrtcImage* PvrtcImage_load(GLApplication *app, const char *file_name);

/**
 * PVRTC圧縮画像を解放する
 */
extern void PvrtcImage_free(GLApplication *app, PvrtcImage *pvrtc);


/**
 * KTXフォーマットデータ
 * 詳細：http://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/
 * Qualcomm Texture Tool(Windows専用）で作成可能
 */
typedef struct KtxImage {
    /**
     * origin data
     */
    RawData *raw;

    /**
     * 格納されたテクスチャのフォーマット
     * RGB, RGBA, BGRA, etc.
     */
    GLenum format;

    /**
     * 格納されたテクスチャのピクセルタイプ
     * UNSIGNED_BYTE, UNSIGNED_SHORT_5_6_5, etc.
     * 必ず0以外の値が格納される
     */
    GLenum type;

    /**
     * 画像幅
     * mipmapを含む場合、最大（miplevel 0）の値が格納される
     */
    int width;

    /**
     * 画像高さ
     * mipmapを含む場合、最大（miplevel 0）の値が格納される
     */
    int height;

    /**
     * 画像のmipmap数
     */
    int mipmaps;

    /**
     * 画像の長さ
     * mipmap数だけ格納されている
     */
    int* image_length_table;

    /**
     * 各画像へのポインタ
     * mipmap数だけ格納されている
     */
    void** image_table;
} KtxImage;


/**
 * Qualcomm Texture Toolを利用して作成したKTXファイルを読み込む
 */
extern KtxImage* KtxImage_load(GLApplication *app, const char* file_name);

/**
 * KTXファイルを解放する
 */
extern void KtxImage_free(GLApplication *app, KtxImage *image);



#endif /* COMPRESSEDTEXTURE_H_ */
