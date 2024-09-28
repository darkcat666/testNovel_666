#include "support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // フラグメントシェーダの描画色
    GLint unif_color;

} Extension_DepthBufferEnable;

/**
 * アプリの初期化を行う
 */
void sample_DepthBufferEnable_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_DepthBufferEnable*) malloc(sizeof(Extension_DepthBufferEnable));
    // サンプルアプリ用データを取り出す
    Extension_DepthBufferEnable *extension = (Extension_DepthBufferEnable*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source = "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "}";

        const GLchar *fragment_shader_source = "uniform lowp vec4 unif_color;"
                "void main() {"
                "   gl_FragColor = unif_color;"
                "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // uniform変数のlocationを取得する
    {
        extension->unif_color = glGetUniformLocation(extension->shader_program, "unif_color");
        assert(extension->unif_color >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    {
        // 深度テストを有効にする
        glEnable(GL_DEPTH_TEST);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_DepthBufferEnable_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_DepthBufferEnable_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_DepthBufferEnable *extension = (Extension_DepthBufferEnable*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

    // 深度バッファもクリアする。
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    // 三角形を描画する
    {
        // 描画色を指定する
        {
            // R, G, B, A
            glUniform4f(extension->unif_color, 1.0f, 1.0f, 1.0f, 1.0f);
        }

        const GLfloat positionTriangle[] = {
        // v0(center top z)
                0.0f, 0.5f, -0.5f,
                // v1(left bottom z)
                -0.5f, 0.0f, -0.5f,
                // v3(right bottom z)
                0.5f, 0.0f, -0.5f, };

        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) positionTriangle);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }

    // 四角形を描画する
    {
        // 描画色を指定する
        {
            // R, G, B, A
            glUniform4f(extension->unif_color, 1.0f, 0.0f, 1.0f, 1.0f);
        }

        const GLfloat positionQuad[] = {
                // v0(left top z)
                -0.25f, 0.25f, 0.5f,
                // v1(left bottom z)
                -0.25f, -0.25f, 0.5f,
                // v2(right top z)
                0.25f, 0.25f, 0.5f,
                // v3(right bottom z)
                0.25f, -0.25f, 0.5f, };

        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, 0, positionQuad);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_DepthBufferEnable_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_DepthBufferEnable *extension = (Extension_DepthBufferEnable*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
