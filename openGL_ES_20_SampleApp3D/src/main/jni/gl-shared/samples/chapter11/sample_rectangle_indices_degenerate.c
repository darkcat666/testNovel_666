#include "support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // UV情報属性
    GLint attr_uv;

    // キューブに貼り付けるテクスチャ
    GLint unif_texture;

    // キューブに貼り付けるテクスチャ
    Texture *texture;
} Extension_RectangleIndicesDegenerate;

/**
 * アプリの初期化を行う
 */
void sample_RectangleIndicesDegenerate_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RectangleIndicesDegenerate*) malloc(sizeof(Extension_RectangleIndicesDegenerate));
    // サンプルアプリ用データを取り出す
    Extension_RectangleIndicesDegenerate *extension = (Extension_RectangleIndicesDegenerate*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        // 位置属性
                "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"
                        // varying
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_Position = attr_pos;"
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
    }
    {
        extension->texture = Texture_load(app, "simple-atlas.png", TEXTURE_RAW_RGBA8);
        assert(extension->texture);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_RectangleIndicesDegenerate_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RectangleIndicesDegenerate_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RectangleIndicesDegenerate *extension = (Extension_RectangleIndicesDegenerate*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attributeを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // 縮退化した図形を構築する
    {
        typedef struct RectVertex {
            vec2 pos;
            vec2 uv;
        } RectVertex;

        const RectVertex vertices[] = {
        // rect 0
        // v0(left top)
                { { -0.75f, 0.75f }, { 0.0f, 0.0f, } },
                // v1(left bottom)
                { { -0.75f, 0.25f }, { 0.0f, 1.0f } },
                // v2(right top)
                { { -0.25f, 0.75f }, { 0.5f, 0.0f, } },
                // v3(right bottom)
                { { -0.25f, 0.25f }, { 0.5f, 1.0f, } },
                // rect 1
                // v0(left top)
                { { 0.25f, -0.25f }, { 0.5f, 0.0f, } },
                // v1(left bottom)
                { { 0.25f, -0.75f }, { 0.5f, 1.0f, } },
                // v2(right top)
                { { 0.75f, -0.25f }, { 1.0f, 0.0f, } },
                // v3(right bottom)
                { { 0.75f, -0.75f }, { 1.0f, 1.0f } }, };

        const GLubyte indices[] = {
//
                0, 1, 2, 3, // rect 0
                3, 4, // 縮退
                4, 5, 6, 7, // rect 1
                };

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glUniform1i(extension->unif_texture, 0);

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (GLvoid*) vertices);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (GLvoid*) ((GLubyte*) vertices + sizeof(vec2)));
        glDrawElements(GL_TRIANGLE_STRIP, 4 + 2 + 4, GL_UNSIGNED_BYTE, indices);

        assert(glGetError() == GL_NO_ERROR);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RectangleIndicesDegenerate_destroy(GLApplication *app) {
// サンプルアプリ用データを取り出す
    Extension_RectangleIndicesDegenerate *extension = (Extension_RectangleIndicesDegenerate*) app->extension;

// シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

// シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    Texture_free(extension->texture);

// サンプルアプリ用のメモリを解放する
    free(app->extension);
}
