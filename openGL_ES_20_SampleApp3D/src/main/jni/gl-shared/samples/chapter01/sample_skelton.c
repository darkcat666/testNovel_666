#include "support.h"

/**
 * アプリの初期化を行う
 */
void sample_Skelton_initialize(GLApplication *app) {
    __log("initialize");
}

/**
 * レンダリングエリアが変更された
 */
void sample_Skelton_resized(GLApplication *app) {
    __logf("resized(%d x %d)", app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_Skelton_rendering(GLApplication *app) {
//    __log("rendering");
}

/**
 * アプリのデータ削除を行う
 */
void sample_Skelton_destroy(GLApplication *app) {
    __log("destroy");
}
