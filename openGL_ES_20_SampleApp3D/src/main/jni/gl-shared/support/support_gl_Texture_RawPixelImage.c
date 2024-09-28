/*
 * RawPixelImage.c
 *
 *  Created on: 2013/04/07
 */

#include    "support.h"

/**
 * RGB888のポインタをdst_pixelsへピクセル情報をコピーする。
 */
void RawPixelImage_convertColorRGB(const void *rgb888_pixels, const int pixel_format, void *dst_pixels, const int pixel_num) {
    // 残ピクセル数
    int pixels = pixel_num;
    unsigned char *src_rgb888 = (unsigned char *) rgb888_pixels;

    switch (pixel_format) {
        case TEXTURE_RAW_RGB565: {
            unsigned short *p = (unsigned short*) dst_pixels;
            while (pixels) {

                const int r = src_rgb888[0] & 0xff;
                const int g = src_rgb888[1] & 0xff;
                const int b = src_rgb888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case TEXTURE_RAW_RGBA5551: {
            unsigned short *p = (unsigned short*) dst_pixels;
            while (pixels) {

                const int r = src_rgb888[0] & 0xff;
                const int g = src_rgb888[1] & 0xff;
                const int b = src_rgb888[2] & 0xff;
                const int a = 1;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case TEXTURE_RAW_RGB8: {
            memcpy(dst_pixels, src_rgb888, pixels * 3);
        }
            break;
        case TEXTURE_RAW_RGBA8: {
            unsigned char *dst = (unsigned char*) dst_pixels;
            while (pixels) {

                dst[0] = src_rgb888[0];
                dst[1] = src_rgb888[1];
                dst[2] = src_rgb888[2];
                dst[3] = 0xFF;

                src_rgb888 += 3;
                dst += 4;
                --pixels;
            }
        }
            break;
    }
}
/**
 * RGBA8888のポインタをdst_pixelsへピクセル情報をコピーする。
 */
void RawPixelImage_convertColorRGBA(const void *rgba8888_pixels, const int pixel_format, void *dst_pixels, const int pixel_num) {
    // 残ピクセル数
    int pixels = pixel_num;
    unsigned char *src_rgba8888 = (unsigned char *) rgba8888_pixels;

    switch (pixel_format) {
        case TEXTURE_RAW_RGB565: {
            unsigned short *p = (unsigned short*) dst_pixels;
            while (pixels) {

                const int r = src_rgba8888[0] & 0xff;
                const int g = src_rgba8888[1] & 0xff;
                const int b = src_rgba8888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgba8888 += 4;
                ++p;
                --pixels;
            }
        }
            break;
        case TEXTURE_RAW_RGBA5551: {
            unsigned short *p = (unsigned short*) dst_pixels;
            while (pixels) {

                const int r = src_rgba8888[0] & 0xff;
                const int g = src_rgba8888[1] & 0xff;
                const int b = src_rgba8888[2] & 0xff;
                const int a = (src_rgba8888[3] & 0xff) > 0 ? 1 : 0;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_rgba8888 += 4;
                ++p;
                --pixels;
            }
        }
            break;
        case TEXTURE_RAW_RGB8: {
            unsigned char *dst = (unsigned char*) dst_pixels;
            while (pixels) {

                dst[0] = src_rgba8888[0];
                dst[1] = src_rgba8888[1];
                dst[2] = src_rgba8888[2];

                src_rgba8888 += 4;
                dst += 3;
                --pixels;
            }
        }
            break;
        case TEXTURE_RAW_RGBA8: {
            memcpy(dst_pixels, src_rgba8888, pixels * 4);
        }
            break;
    }
}

/**
 * 画像用に確保したメモリを解放する
 */
void RawPixelImage_free(GLApplication *app, RawPixelImage *image) {
    if (image) {
        if (image->pixel_data) {
            free(image->pixel_data);
            image->pixel_data = NULL;
        }
        free(image);
    }
}

/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
Texture* RawPixelImage_loadTexture(GLApplication *app, const char* file_name, const int pixel_fotmat) {
    RawPixelImage *image = RawPixelImage_load(app, file_name, pixel_fotmat);

// error images
    if (!image) {
        return NULL;
    }

    Texture *texture = (Texture*) malloc(sizeof(Texture));

    {
        // 元画像から必要情報をコピーする
        texture->width = image->width;
        texture->height = image->height;
    }

    {
        // 領域確保
        glGenTextures(1, &texture->id);
        assert(texture->id > 0);
        assert(glGetError() == GL_NO_ERROR);
    }

    glBindTexture(GL_TEXTURE_2D, texture->id);

    {
        // VRAMへピクセル情報をコピーする
        static const GLenum FORMAT[] = { GL_RGBA, GL_RGB, GL_RGBA, GL_RGB };
        static const GLenum TYPE[] = { GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_5_6_5 };
        glTexImage2D(GL_TEXTURE_2D, 0, FORMAT[pixel_fotmat], image->width, image->height, 0, FORMAT[pixel_fotmat], TYPE[pixel_fotmat], image->pixel_data);

        assert(glGetError() == GL_NO_ERROR);
    }

    {
        // wrapの初期設定
        // 互換性のため、デフォルトはGL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        assert(glGetError() == GL_NO_ERROR);
    }

    {
        // filterの初期設定
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        assert(glGetError() == GL_NO_ERROR);
    }

// unbindする
    glBindTexture(GL_TEXTURE_2D, 0);
    assert(glGetError() == GL_NO_ERROR);

// 元画像を廃棄する
    RawPixelImage_free(app, image);
    return texture;
}
