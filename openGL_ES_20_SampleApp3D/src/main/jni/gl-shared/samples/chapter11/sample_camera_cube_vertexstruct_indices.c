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
} Extension_CameraCubeVertexstructIndices;

/**
 * アプリの初期化を行う
 */
void sample_CameraCubeVertexstructIndices_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_CameraCubeVertexstructIndices*) malloc(sizeof(Extension_CameraCubeVertexstructIndices));
    // サンプルアプリ用データを取り出す
    Extension_CameraCubeVertexstructIndices *extension = (Extension_CameraCubeVertexstructIndices*) app->extension;

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
void sample_CameraCubeVertexstructIndices_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_CameraCubeVertexstructIndices_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_CameraCubeVertexstructIndices *extension = (Extension_CameraCubeVertexstructIndices*) app->extension;

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

        // 頂点構造体を定義
        typedef struct CubeVertex {
            vec3 position;
            vec2 uv;
        } CubeVertex;

        const CubeVertex cubeVertices[] = {
        //
                { { LEFT, TOP, FRONT }, { 0, 1 } }, // 左上手前
                { { LEFT, TOP, BACK }, { 0, 0 } }, // 左上奥
                { { RIGHT, TOP, FRONT }, { 1, 1 } }, // 右上手前
                { { RIGHT, TOP, BACK }, { 1, 0 } }, // 右上奥
                { { LEFT, BOTTOM, FRONT }, { 1, 1 } }, // 左下手前
                { { LEFT, BOTTOM, BACK }, { 1, 0 } }, // 左下奥
                { { RIGHT, BOTTOM, FRONT }, { 0, 1 } }, // 右下手前
                { { RIGHT, BOTTOM, BACK }, { 0, 0 } }, // 右下奥
                };

        const GLushort cubeIndices[] = {
        //
                0, 1, 2, //
                2, 1, 3, //

                2, 3, 6, //
                6, 3, 7, //

                6, 7, 4, //
                4, 7, 5, //

                4, 5, 0, //
                0, 5, 1, //

                1, 5, 3, //
                3, 5, 7, //

                0, 2, 4, //
                4, 2, 6, //
                };

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glUniform1i(extension->unif_texture, 0);

        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*) cubeVertices);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*) ((GLubyte*) cubeVertices + sizeof(vec3)));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);

        assert(glGetError() == GL_NO_ERROR);
    }

// バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_CameraCubeVertexstructIndices_destroy(GLApplication *app) {
// サンプルアプリ用データを取り出す
    Extension_CameraCubeVertexstructIndices *extension = (Extension_CameraCubeVertexstructIndices*) app->extension;

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
