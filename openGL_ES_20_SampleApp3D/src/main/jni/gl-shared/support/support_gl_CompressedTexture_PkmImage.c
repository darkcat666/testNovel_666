/*
 * Compress_PkmImage.c
 *
 *  Created on: 2013/04/07
 */
#include    "support.h"

/**
 * 圧縮画像を読み込む。
 * 読み込んだ画像はes20_freePkmImage()で解放する
 */
PkmImage* PkmImage_load(GLApplication *app, const char* file_name) {
    RawData *raw = RawData_loadFile(app, file_name);

    if (!raw) {
        // fail...
        return NULL;
    }

    PkmImage *image = (PkmImage*) malloc(sizeof(PkmImage));

    image->raw = raw;
    image->image_bytes = RawData_getLength(raw);

    {
        // check header
        char magic[4] = { 0 };
        magic[0] = RawData_read8(raw);
        magic[1] = RawData_read8(raw);
        magic[2] = RawData_read8(raw);
        magic[3] = RawData_read8(raw);

        assert(magic[0] == 'P');
        assert(magic[1] == 'K');
        assert(magic[2] == 'M');
    }
    {
        // check version
        char version[2] = { 0 };
        version[0] = RawData_read8(raw);
        version[1] = RawData_read8(raw);

        assert(version[0] == '1');
        assert(version[1] == '0');
    }
    // read data type
    image->data_type = RawData_readBE16(raw);
    // 圧縮後の幅と高さを読み込む
    image->width = RawData_readBE16(raw);
    image->height = RawData_readBE16(raw);
    // 圧縮前の幅と高さを読み込む
    image->origin_width = RawData_readBE16(raw);
    image->origin_height = RawData_readBE16(raw);

    image->image = RawData_getReadHeader(raw);
    image->image_bytes = RawData_getAvailableBytes(raw);

// texture size check
    assert(image->width >= image->origin_width);
    assert(image->height >= image->origin_height);

    __logf("ETC1 bytes(%d) tex size(%d x %d) origin size(%d x %d)", image->image_bytes, image->width, image->height, image->origin_width, image->origin_height);
    return image;
}

/**
 * 圧縮画像を解放する
 */
void PkmImage_free(GLApplication *app, PkmImage *pkm) {
    if (pkm) {
        RawData_freeFile(app, pkm->raw);
        free(pkm);
    }
}

/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
Texture* PkmImage_loadTexture(GLApplication *app, const char* file_name) {
    // GL_OES_compressed_ETC1_RGB8_textureがサポートされていないプラットフォームではifdefで切る
#ifndef GL_OES_compressed_ETC1_RGB8_texture
    __log("サポート外のテクスチャ形式(GL_OES_compressed_ETC1_RGB8_texture)");
    return NULL;
#else
    PkmImage *pkm = PkmImage_load(app, file_name);

    // error images
    if (!pkm) {
        return NULL;
    }

    Texture *texture = (Texture*) malloc(sizeof(Texture));

    {
        // 元画像から必要情報をコピーする
        texture->width = pkm->width;
        texture->height = pkm->height;
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
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, pkm->width, pkm->height, 0, pkm->image_bytes, pkm->image);
        assert(glGetError() == GL_NO_ERROR);
    }

    {
        // wrapの初期設定
        // 互換性のため、初期は常にGL_CLAMP_TO_EDGE
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

// 元画像を解放
    PkmImage_free(app, pkm);
    return texture;
#endif
}
