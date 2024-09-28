/*
 * PvrtcImage.c
 *
 *  Created on: 2013/04/07
 */

#include    "support.h"

/**
 * PVRTCテクスチャヘッダ。
 */
typedef struct PVRTexHeader {
    uint32_t headerLength;
    uint32_t height;
    uint32_t width;
    uint32_t numMipmaps;
    uint32_t flags;
    uint32_t dataLength;
    uint32_t bpp;
    uint32_t bitmaskRed;
    uint32_t bitmaskGreen;
    uint32_t bitmaskBlue;
    uint32_t bitmaskAlpha;
    uint8_t pvrTag[4];
    uint32_t numSurfs;
} PVRTexHeader;

#define PVR_TEXTURE_FLAG_TYPE_MASK  0xff

/**
 * PVRTC圧縮画像を読み込む
 * 読み込んだ画像はes20_freePvrtcImage()で解放する
 */
PvrtcImage* PvrtcImage_load(GLApplication *app, const char *file_name) {

    enum {
        kPVRTextureFlagTypePVRTC_2 = 24,
        kPVRTextureFlagTypePVRTC_4
    };

    RawData *raw = RawData_loadFile(app, file_name);

    if (!raw) {
        // fail...
        return NULL;
    }

    PVRTexHeader *header = (PVRTexHeader*) RawData_getReadHeader(raw);

    // check tag
    if (header->pvrTag[0] != 'P' || header->pvrTag[1] != 'V' || header->pvrTag[2] != 'R' || header->pvrTag[3] != '!') {
        RawData_freeFile(app, raw);
        __logf("texture format error(%s)", file_name);
        return NULL;
    }

    __logf("pvrtc mipmaps(%d) surfs(%d)", header->numMipmaps, header->numSurfs);

    PvrtcImage *result = (PvrtcImage*) malloc(sizeof(PvrtcImage));

    // データコピー
    result->raw = raw;
    result->width = header->width;
    result->height = header->height;
    {
        // 詳細フォーマットのチェック
        switch (header->flags & PVR_TEXTURE_FLAG_TYPE_MASK) {
            case kPVRTextureFlagTypePVRTC_2:
                result->format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                result->bits_per_pixel = 2;
                break;
            case kPVRTextureFlagTypePVRTC_4:
                result->format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                result->bits_per_pixel = 4;
                break;
            default:
                // format error
                assert(0);
                break;
        }
    }

    // 各圧縮画像へのポインタを計算する
    // 画像は固定ビットレートで格納されるため、ポインタ位置を計算可能。
    {
        // mipmap対応チェック
        // 等倍テクスチャ+mipmap数を保持するため、例えば等倍テクスチャであればnumMpmapは0になる。
        // +1を行うことでfor文で回すことができる。
        result->mipmaps = header->numMipmaps + 1;
        result->image_table = (void**) malloc(sizeof(void*) * result->mipmaps);
        result->image_length_table = (int*) malloc(sizeof(void*) * result->mipmaps);

        int miplevel = 0;
        int texWidth = result->width;
        int texHeight = result->height;
        RawData_setHeaderPosition(raw, header->headerLength);
        uint8_t *image_header = RawData_getReadHeader(raw);
        for (miplevel = 0; miplevel < result->mipmaps; ++miplevel) {

            // 1ブロックのサイズは圧縮時オプションで変動する
            const int blockSize = header->flags == kPVRTextureFlagTypePVRTC_4 ? (4 * 4) : (8 * 4);
            int widthBlocks = texWidth / (header->flags == kPVRTextureFlagTypePVRTC_4 ? 4 : 8);
            int heightBlocks = texHeight / 4;
            const int bpp = header->flags == kPVRTextureFlagTypePVRTC_4 ? 4 : 2;

            // 最低限のブロック数は持たなければならない
            if (widthBlocks < 2) {
                widthBlocks = 2;
            }
            if (heightBlocks < 2) {
                heightBlocks = 2;
            }

            __logf("decode blockSize(%d) Xblock(%d) Yblocks(%d) bit par pix(%d)", blockSize, widthBlocks, heightBlocks, bpp);

            const int dataSize = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);

            // 次のmipmapを読む
            texWidth /= 2;
            texHeight /= 2;

            // テクスチャデータを保存する
            result->image_length_table[miplevel] = dataSize;
            result->image_table[miplevel] = image_header;

            image_header += dataSize;
        }
    }

    assert(result->width == result->height);
    assert(Texture_checkPowerOfTwo(result->width));
    assert(Texture_checkPowerOfTwo(result->height));

    __logf("PVRTC mipmaps(%d) tex size(%d x %d) format(%s)", result->mipmaps, result->width, result->height, result->format == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG ? "GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG" : "GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG");
    return result;
}

/**
 * PVRTC圧縮画像を解放する
 */
void PvrtcImage_free(GLApplication *app, PvrtcImage *pvrtc) {
    if (pvrtc) {
        RawData_freeFile(app, pvrtc->raw);

        free(pvrtc->image_table);
        free(pvrtc->image_length_table);

        free(pvrtc);
    }
}


/**
 * 画像をテクスチャとして読み込む。
 * 読み込んだ画像はes20_freeTexture()で解放する
 */
Texture* PvrtcImage_loadTexture(GLApplication *app, const char* file_name) {
    PvrtcImage *pvrtc = PvrtcImage_load(app, file_name);

    // error images
    if (!pvrtc) {
        return NULL;
    }

    Texture *texture = (Texture*) malloc(sizeof(Texture));

    {
        // 元画像から必要情報をコピーする
        texture->width = pvrtc->width;
        texture->height = pvrtc->height;
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
        int width = pvrtc->width;
        int height = pvrtc->height;
        int miplevel = 0;

        for (miplevel = 0; miplevel < pvrtc->mipmaps; ++miplevel) {
            glCompressedTexImage2D(GL_TEXTURE_2D, miplevel, pvrtc->format, width, height, 0, pvrtc->image_length_table[miplevel], pvrtc->image_table[miplevel]);

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
        if (pvrtc->mipmaps > 1) {
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
    PvrtcImage_free(app, pvrtc);
    return texture;
}
