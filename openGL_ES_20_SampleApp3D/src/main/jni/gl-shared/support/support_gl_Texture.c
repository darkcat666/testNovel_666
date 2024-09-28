/*
 * support_gl_texture.c
 *
 *  Created on: 2013/02/19
 */

#include    "support.h"

extern Texture* PkmImage_loadTexture(GLApplication *app, const char* file_name);
extern Texture* PvrtcImage_loadTexture(GLApplication *app, const char* file_name);
extern Texture* KtxImage_loadTexture(GLApplication *app, const char* file_name);
extern Texture* RawPixelImage_loadTexture(GLApplication *app, const char* file_name, const int pixel_fotmat);

/**
 * サイズがpotならTEXTURE_POTを返す。npotなら、TEXTURE_NPOTを返す。
 */
bool Texture_checkPowerOfTwo(const int size) {
    int bit = 1;
    int i = 0;
    const int BIT_SIZE = sizeof(int) * 8;

    if (!size) {
        return false;
    }

    for (i = 0; i < BIT_SIZE; ++i) {
        if (size == bit) {
            return true;
        }
        bit <<= 1;
    }

    return false;
}

/**
 * 引数sizeがpotならTEXTURE_POTを返す。npotなら、TEXTURE_NPOTを返す。
 */
bool Texture_checkPowerOfTwoWH(const int width, const int height) {
    return Texture_checkPowerOfTwo(width) && Texture_checkPowerOfTwo(height);
}


/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
Texture* Texture_load(GLApplication *app, const char* file_name, const int pixel_fotmat) {
    if (pixel_fotmat == TEXTURE_COMPRESS_ETC1) {
        return PkmImage_loadTexture(app, file_name);
    } else if (pixel_fotmat == TEXTURE_COMPRESS_PVRTC) {
        return PvrtcImage_loadTexture(app, file_name);
    } else if (pixel_fotmat == TEXTURE_COMPRESS_KTX) {
        return KtxImage_loadTexture(app, file_name);
    } else {
        return RawPixelImage_loadTexture(app, file_name, pixel_fotmat);
    }
}

/**
 * テクスチャの縦横が2のn乗であればtrueを返す
 */
bool Texture_isPowerOfTwo(Texture *texture) {
    return Texture_checkPowerOfTwo(texture->width) && Texture_checkPowerOfTwo(texture->height);
}

/**
 * テクスチャを解放する。
 */
void Texture_free(Texture *texture) {
    glDeleteTextures(1, &texture->id);
    free((void*) texture);
}

