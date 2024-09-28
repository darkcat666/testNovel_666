#include    "support.h"
#include    "SampleList.h"

// プロトタイプ宣言用のマクロ
#define SAMPLE_PROTOTYPES(name) extern void sample_##name##_initialize(GLApplication *); \
                                extern void sample_##name##_resized(GLApplication *); \
                                extern void sample_##name##_rendering(GLApplication *); \
                                extern void sample_##name##_destroy(GLApplication *)

/*    CHAPTER    */
SAMPLE_PROTOTYPES(Skelton);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(DepthOrder);
SAMPLE_PROTOTYPES(DepthOrderRev);
SAMPLE_PROTOTYPES(DepthBufferEnable);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(CameraSetup);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(CameraCube);
SAMPLE_PROTOTYPES(CameraCubeDatastruct);
SAMPLE_PROTOTYPES(CameraCubeVertexstruct);
SAMPLE_PROTOTYPES(CameraCubeVertexstructAlignment);
SAMPLE_PROTOTYPES(CameraCubeVertexstructAlignmentEx);
SAMPLE_PROTOTYPES(CameraCubeVertexstructIndices);
SAMPLE_PROTOTYPES(CameraCubeVertexstructIndicesDegenerate);
SAMPLE_PROTOTYPES(RectangleIndicesDegenerate);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(PmdLoad);
SAMPLE_PROTOTYPES(PmdRenderingHighp);
SAMPLE_PROTOTYPES(PmdFacechange);
SAMPLE_PROTOTYPES(RenderAlpha);
SAMPLE_PROTOTYPES(RenderAlpha2Pass);
SAMPLE_PROTOTYPES(PmdEdge);
SAMPLE_PROTOTYPES(PmdMultirender);
SAMPLE_PROTOTYPES(PmdMultirenderVBO);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(BlendOrder);
SAMPLE_PROTOTYPES(BlendOrderRev);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(PmdGlFlush);
SAMPLE_PROTOTYPES(PmdGlFinish);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(PmdFramebuffer);
SAMPLE_PROTOTYPES(PmdFramebufferAlpha);
SAMPLE_PROTOTYPES(PmdFramebufferDepth);
SAMPLE_PROTOTYPES(PmdFramebufferDepthNotSupport);
SAMPLE_PROTOTYPES(DepthShadow);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(AsyncLoad);

#define SAMPLE_FUNCTIONS(name)  sample_##name##_initialize, sample_##name##_resized, sample_##name##_rendering, sample_##name##_destroy

static SampleInfo g_sample_initializes[] = {
//
        { "スケルトンプログラム", SAMPLE_FUNCTIONS(Skelton) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_depth[] = {
//
        { "Zの付いた図形を描画する(手前→奥)", SAMPLE_FUNCTIONS(DepthOrder) },
        //
        { "Zの付いた図形を描画する(奥→手前)", SAMPLE_FUNCTIONS(DepthOrderRev) },
        //
        { "深度テストを有効にする", SAMPLE_FUNCTIONS(DepthBufferEnable) },
        //
        { "3D描画でのブレンド(手前→奥)", SAMPLE_FUNCTIONS(BlendOrder) },
        //
        { "3D描画でのブレンド(奥→手前)", SAMPLE_FUNCTIONS(BlendOrderRev) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_camera[] = {
//
        { "カメラ情報をセットアップする", SAMPLE_FUNCTIONS(CameraSetup) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_struct[] = {
//
        { "立方体を描画する", SAMPLE_FUNCTIONS(CameraCube) },
        //
        { "立方体を描画する(構造体)", SAMPLE_FUNCTIONS(CameraCubeDatastruct) },
        //
        { "立方体を描画する(頂点構造体)", SAMPLE_FUNCTIONS(CameraCubeVertexstruct) },
        //
        { "立方体を描画する(Alignmentあり)", SAMPLE_FUNCTIONS(CameraCubeVertexstructAlignment) },
        //
        { "立方体を描画する(Alignment回避)", SAMPLE_FUNCTIONS(CameraCubeVertexstructAlignmentEx) },
        //
        { "立方体を描画する(インデックス)", SAMPLE_FUNCTIONS(CameraCubeVertexstructIndices) },
        //
        { "立方体を描画する(縮退三角形)", SAMPLE_FUNCTIONS(CameraCubeVertexstructIndicesDegenerate) },
        //
        { "四角形を描画する(縮退三角形)", SAMPLE_FUNCTIONS(RectangleIndicesDegenerate) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_advance3d[] = {
//
        { "PMDファイルを読み込む", SAMPLE_FUNCTIONS(PmdLoad) },
        //
        { "highp精度で描画する", SAMPLE_FUNCTIONS(PmdRenderingHighp) },
        //
        { "テクスチャを工夫して演出を行う", SAMPLE_FUNCTIONS(PmdFacechange) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_multipass[] = {
//
        { "3Dモデルを半透明に描画する", SAMPLE_FUNCTIONS(RenderAlpha) },
        //
        { "3Dモデルを半透明に描画する(2Pass)", SAMPLE_FUNCTIONS(RenderAlpha2Pass) },
        //
        { "アニメ風のエッジを描画する", SAMPLE_FUNCTIONS(PmdEdge) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_bufferobject[] = {
//
        { "複数のモデルを描画する", SAMPLE_FUNCTIONS(PmdMultirender) },
        //
        { "VBOでレンダリングを高速化する", SAMPLE_FUNCTIONS(PmdMultirenderVBO) }, // 終端
        { "", NULL } };

static SampleInfo g_sample_flushfinish[] = {
//
        { "頻繁なglFlushによる性能劣化", SAMPLE_FUNCTIONS(PmdGlFlush) },
        //
        { "頻繁なglFinishによる性能劣化", SAMPLE_FUNCTIONS(PmdGlFinish) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_framebuffer[] = {
//
        { "テクスチャへのレンダリング", SAMPLE_FUNCTIONS(PmdFramebuffer) },
        //
        { "テクスチャへのレンダリング(透過)", SAMPLE_FUNCTIONS(PmdFramebufferAlpha) },
        //
        { "深度レンダリング", SAMPLE_FUNCTIONS(PmdFramebufferDepth) },
        //
        { "深度レンダリング(非対応端末)", SAMPLE_FUNCTIONS(PmdFramebufferDepthNotSupport) },
        //
        { "デプスシャドウ", SAMPLE_FUNCTIONS(DepthShadow) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_async[] = {
//
        { "非同期でリソース読み込み", SAMPLE_FUNCTIONS(AsyncLoad) },
        // 終端
        { "", NULL } };

static ChapterInfo g_chapter_info[] = {
//
        { "サンプルプログラムと補助関数", 3, g_sample_initializes },
        //
        { "深度バッファ", 4, g_sample_depth },
        //
        { "3D空間のカメラ", 5, g_sample_camera },
        //
        { "効率的な描画のための頂点情報", 6, g_sample_struct },
        //
        { "実践的な3D描画", 7, g_sample_advance3d },
        //
        { "マルチパスレンダリングを使った演出", 8, g_sample_multipass },
        //
        { "バッファオブジェクトの活用", 9, g_sample_bufferobject },
        //
        { "覚えておくと便利な知識", 10, g_sample_flushfinish },
        //
        { "オフスクリーンレンダリング", 11, g_sample_framebuffer },
        //
        { "OpenGL ESのマルチスレッド", 14, g_sample_async },
// 終端
        { "", -1, NULL }, };

/**
 * アプリで解説するチャプター数を取得する
 */
int SampleList_getChapterNum() {
    int result = 0;

// サンプルアプリ情報が設定されているなら、次を参照する
    while (g_chapter_info[result].sample_info != NULL) {
        ++result;
    }
    return result;
}

/**
 * 任意の番号のチャプター番号を返す。
 */
ChapterInfo* SampleList_getChapter(int chapter_num) {
    return g_chapter_info + chapter_num;
}

/**
 * チャプターに所属するサンプル数を取得する。
 */
int SamplesList_getChapterSampleNum(int chapter_num) {
    SampleInfo *info = g_chapter_info[chapter_num].sample_info;
    if (!info) {
        return 0;
    }

    int result = 0;
    while (info[result].func_initialize) {
        ++result;
    }
    return result;
}

/**
 * チャプターサンプルの情報を取得する
 */
SampleInfo* SampleList_getSample(int chapter_num, int sample_num) {
    return g_chapter_info[chapter_num].sample_info + sample_num;
}
