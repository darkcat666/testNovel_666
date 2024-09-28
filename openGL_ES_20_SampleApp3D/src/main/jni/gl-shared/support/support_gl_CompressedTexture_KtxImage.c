/*
 * Compress_KtxImage.c
 *
 *  Created on: 2013/04/07
 */

#include    "support.h"


/**
 * Qualcomm Texture Toolを利用して作成したKTXファイルを読み込む
 */
KtxImage* KtxImage_load(GLApplication *app, const char* file_name) {
    RawData *rawData = RawData_loadFile(app, file_name);

    if (rawData == NULL) {
        return NULL;
    }
    // ファイルの識別子を確認する
    // サンプルでは詳細なエラー処理を行わない。
    {
        const uint8_t KTXFileIdentifier[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

        const uint8_t *header = (uint8_t*) RawData_getReadHeader(rawData);

        int index = 0;
        for (index = 0; index < 12; ++index) {
            if (header[index] != KTXFileIdentifier[index]) {
                __logf("header error(%s)", file_name);
                RawData_freeFile(app, rawData);
                return NULL;
            }
        }

        RawData_offsetHeader(rawData, 12);
    }

    //
    {
        // エンディアンのチェックを行う
        // サンプルではビッグエンディアンのみに対応する
        int32_t check_endian = RawData_readLE32(rawData);

        // エンディアンチェックの値は0x04030201でり、それ以外であればエンディアン変換を行う必要がある
        // サンプルのため、エンディアンは固定であると想定する。
        if (check_endian == 0x01020304) {
            // エンディアンが想定と違うため、読み込まない
            __logf("endian error(%s)", file_name);
            RawData_freeFile(app, rawData);
            return NULL;
        }
    }

    // 読み込みできるデータだった

    KtxImage *result = (KtxImage*) malloc(sizeof(KtxImage));
    result->raw = rawData;

    // ヘッダからデータを読み取る
    typedef struct KTXImageHeader {
        uint32_t glType;
        uint32_t glTypeSize;
        uint32_t glFormat;
        uint32_t glInternalFormat;
        uint32_t glBaseInternalFormat;
        uint32_t pixelWidth;
        uint32_t pixelHeight;
        uint32_t pixelDepth;
        uint32_t numberOfArrayElements;
        uint32_t numberOfFaces;
        uint32_t numberOfMipmapLevels;
        uint32_t bytesOfKeyValueData;
    } KTXImageHeader;

    // ヘッダとしてチェックする
    KTXImageHeader *pImageHeader = (KTXImageHeader*) RawData_getReadHeader(rawData);

    __logf("image format(%x)", pImageHeader->glBaseInternalFormat);
    __logf("image size(%d x %d) depth(%d)", pImageHeader->pixelWidth, pImageHeader->pixelHeight, pImageHeader->pixelDepth);
    __logf("image mipmaps(%d)", pImageHeader->numberOfMipmapLevels);
    __logf("image key value data (%d bytes)", pImageHeader->bytesOfKeyValueData);

    {
        result->format = pImageHeader->glBaseInternalFormat;
        result->width = pImageHeader->pixelWidth;
        result->height = pImageHeader->pixelHeight;
        result->mipmaps = pImageHeader->numberOfMipmapLevels;

        result->image_length_table = (int*) malloc(sizeof(int) * pImageHeader->numberOfMipmapLevels);
        result->image_table = (void**) malloc(sizeof(void*) * pImageHeader->numberOfMipmapLevels);
    }

    // ヘッダ位置を動かす
    RawData_offsetHeader(rawData, sizeof(KTXImageHeader));
    // Key Value Dataは無視する
    RawData_offsetHeader(rawData, pImageHeader->bytesOfKeyValueData);

    {
        // mipmapごとの画像データを読み込む
        int mip_level = 0;
        for (mip_level = 0; mip_level < pImageHeader->numberOfMipmapLevels; ++mip_level) {
            uint32_t image_size = RawData_readLE32(rawData);
            __logf("level(%d) image size(%d bytes)", mip_level, image_size);
            result->image_length_table[mip_level] = image_size;
            result->image_table[mip_level] = RawData_getReadHeader(rawData);

            RawData_offsetHeader(rawData, image_size);
        }
    }

    return result;
}

void KtxImage_free(GLApplication *app, KtxImage *image) {
    if (image) {
        RawData_freeFile(app, image->raw);
        free(image->image_table);
        free(image->image_length_table);
        free(image);
    }
}



/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
Texture* KtxImage_loadTexture(GLApplication *app, const char* file_name) {
    KtxImage *ktx = KtxImage_load(app, file_name);

    // error images
    if (!ktx) {
        return NULL;
    }

    Texture *texture = (Texture*) malloc(sizeof(Texture));

    {
        // 元画像から必要情報をコピーする
        texture->width = ktx->width;
        texture->height = ktx->height;
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
        int width = ktx->width;
        int height = ktx->height;
        int miplevel = 0;

        for (miplevel = 0; miplevel < ktx->mipmaps; ++miplevel) {
            glCompressedTexImage2D(GL_TEXTURE_2D, miplevel, ktx->format, width, height, 0, ktx->image_length_table[miplevel], ktx->image_table[miplevel]);

            width /= 2;
            height /= 2;
            assert(glGetError() == GL_NO_ERROR);
        }
    }

    {
        // wrapの初期設定
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        assert(glGetError() == GL_NO_ERROR);
    }

    {
        // filterの初期設定
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (ktx->mipmaps > 1) {
            // mipmapを保持している場合
            // texturetoolへ -m のオプションを与えると生成できる
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        } else {
            // mipmapを保持していない場合
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        assert(glGetError() == GL_NO_ERROR);
    }

// unbindする
    glBindTexture(GL_TEXTURE_2D, 0);
    assert(glGetError() == GL_NO_ERROR);

    // 元画像を解放
    KtxImage_free(app, ktx);
    return texture;
}
