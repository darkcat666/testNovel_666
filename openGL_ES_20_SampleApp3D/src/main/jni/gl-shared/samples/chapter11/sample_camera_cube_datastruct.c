#include "support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // UV情報属性
    GLint attr_uv;

    /**
     * world look projection
     */
    GLint unif_wlp;

    // キューブに貼り付けるテクスチャ
    GLint unif_texture;

    // キューブに貼り付けるテクスチャ
    Texture *texture;

    // 回転角
    GLfloat rotate;
} Extension_CameraCubeDatastruct;

/**
 * アプリの初期化を行う
 */
void sample_CameraCubeDatastruct_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_CameraCubeDatastruct*) malloc(sizeof(Extension_CameraCubeDatastruct));
    // サンプルアプリ用データを取り出す
    Extension_CameraCubeDatastruct *extension = (Extension_CameraCubeDatastruct*) app->extension;

    // 頂点シェーダーを用意する
    {
        // カメラ用の行列を計算に加える
        const GLchar *vertex_shader_source =
        // 位置属性
                "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"
        // varying
                        "varying mediump vec2 vary_uv;"
        // uniform
                        "uniform mediump mat4 unif_wlp;"
                        "void main() {"
                        "   gl_Position = unif_wlp * attr_pos;"
                        "   vary_uv = attr_uv;"
                        "}";

        const GLchar *fragment_shader_source =
        //
                "varying mediump vec2 vary_uv;"
                        "uniform sampler2D unif_texture;"
                        "void main() {"
                        "   gl_FragColor = texture2D(unif_texture, vary_uv);"
                        "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);

        extension->attr_uv = glGetAttribLocation(extension->shader_program, "attr_uv");
        assert(extension->attr_uv >= 0);
    }

    // uniform変数のlocationを取得する
    {
        extension->unif_texture = glGetUniformLocation(extension->shader_program, "unif_texture");
        assert(extension->unif_texture >= 0);

        // 変換行列を取得
        extension->unif_wlp = glGetUniformLocation(extension->shader_program, "unif_wlp");
        assert(extension->unif_wlp >= 0);

    }

    {
        extension->texture = Texture_load(app, "texture_rgb_512x512.png", TEXTURE_RAW_RGBA8);
        assert(extension->texture);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    extension->rotate = 0;
}

/**
 * レンダリングエリアが変更された
 */
void sample_CameraCubeDatastruct_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_CameraCubeDatastruct_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_CameraCubeDatastruct *extension = (Extension_CameraCubeDatastruct*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // カメラを初期化する
    {
        // カメラをセットアップする
        const vec3 camera_pos = vec3_create(3, 3, -5); // カメラ位置
        const vec3 camera_look = vec3_create(0, 0, 0); // カメラ注視
        const vec3 camera_up = vec3_create(0, 1, 0); // カメラ上ベクトル

        const GLfloat prj_near = 1.0f;
        const GLfloat prj_far = 100.0f;
        const GLfloat prj_fovY = 45.0f;
        const GLfloat prj_aspect = (GLfloat) (app->surface_width) / (GLfloat) (app->surface_height);

        const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);
        const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
        const mat4 world = mat4_rotate(vec3_createNormalized(1, 1, 0), extension->rotate);

        // 行列を転送する
        mat4 wlp = mat4_multiply(projection, lookAt);
        wlp = mat4_multiply(wlp, world);
        glUniformMatrix4fv(extension->unif_wlp, 1, GL_FALSE, (GLfloat*) wlp.m);

        extension->rotate += 1;
    }

    // キューブを構築する
    {
        const GLfloat LEFT = -1.0f;
        const GLfloat RIGHT = 1.0f;
        const GLfloat FRONT = -1.0f;
        const GLfloat BACK = 1.0f;
        const GLfloat TOP = 1.0f;
        const GLfloat BOTTOM = -1.0f;

        const vec3 positionCube[] = {
        //
        // 上下面
                { LEFT, BOTTOM, FRONT }, { RIGHT, BOTTOM, FRONT }, { LEFT, BOTTOM, BACK }, //
                { LEFT, BOTTOM, BACK }, { RIGHT, BOTTOM, FRONT }, { RIGHT, BOTTOM, BACK }, //
                //
                { LEFT, TOP, FRONT }, { LEFT, TOP, BACK }, { RIGHT, TOP, FRONT }, //
                { LEFT, TOP, BACK }, { RIGHT, TOP, BACK }, { RIGHT, TOP, FRONT }, //

                //左右面
                { RIGHT, TOP, FRONT }, { RIGHT, TOP, BACK }, { RIGHT, BOTTOM, FRONT }, //
                { RIGHT, TOP, BACK }, { RIGHT, BOTTOM, BACK }, { RIGHT, BOTTOM, FRONT }, //
                //
                { LEFT, TOP, FRONT }, { LEFT, BOTTOM, FRONT }, { LEFT, TOP, BACK }, //
                { LEFT, TOP, BACK }, { LEFT, BOTTOM, FRONT }, { LEFT, BOTTOM, BACK }, //

                // 前後面
                { LEFT, TOP, BACK }, { LEFT, BOTTOM, BACK }, { RIGHT, TOP, BACK }, //
                { RIGHT, TOP, BACK }, { LEFT, BOTTOM, BACK }, { RIGHT, BOTTOM, BACK }, //
                //
                { LEFT, TOP, FRONT }, { RIGHT, TOP, FRONT }, { LEFT, BOTTOM, FRONT }, //
                { RIGHT, TOP, FRONT }, { RIGHT, BOTTOM, FRONT }, { LEFT, BOTTOM, FRONT }, //
                //
                };

        const vec2 uvCube[] = {
        //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, //
                };

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glUniform1i(extension->unif_texture, 0);

        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) positionCube);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uvCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

// バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_CameraCubeDatastruct_destroy(GLApplication *app) {
// サンプルアプリ用データを取り出す
    Extension_CameraCubeDatastruct *extension = (Extension_CameraCubeDatastruct*) app->extension;

// シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

// シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // テクスチャ解放
    Texture_free(extension->texture);

// サンプルアプリ用のメモリを解放する
    free(app->extension);
}
