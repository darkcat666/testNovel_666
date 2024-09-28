#include "support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // フラグメントシェーダの描画色
    GLint unif_color;

    // 視点変換
    GLint unif_lookat;

    // 射影
    GLint unif_projection;

    // カメラ位置
    vec3 cameraPos;

} Extension_CameraSetup;

/**
 * アプリの初期化を行う
 */
void sample_CameraSetup_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_CameraSetup*) malloc(sizeof(Extension_CameraSetup));
    // サンプルアプリ用データを取り出す
    Extension_CameraSetup *extension = (Extension_CameraSetup*) app->extension;

    // 頂点シェーダーを用意する
    {
        // カメラ用の行列を計算に加える
        const GLchar *vertex_shader_source =
        // 位置属性
                "attribute mediump vec4 attr_pos;"

        // look at
                        "uniform mediump mat4 unif_lookat;"
        // 射影
                        "uniform mediump mat4 unif_projection;"
                        "void main() {"
                        "   gl_Position = unif_projection * unif_lookat * attr_pos;"
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

        // カメラ行列を取得
        extension->unif_lookat = glGetUniformLocation(extension->shader_program, "unif_lookat");
        assert(extension->unif_lookat >= 0);

        extension->unif_projection = glGetUniformLocation(extension->shader_program, "unif_projection");
        assert(extension->unif_projection >= 0);

    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    {
        // 座標リセット
        extension->cameraPos = vec3_create(3, 1.5, -5);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_CameraSetup_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_CameraSetup_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_CameraSetup *extension = (Extension_CameraSetup*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    // カメラを初期化する
    {
        // カメラをセットアップする
        const vec3 camera_pos = extension->cameraPos; // カメラ位置
        const vec3 camera_look = vec3_create(0, 0, 0); // カメラ注視
        const vec3 camera_up = vec3_create(0, 1, 0); // カメラ上ベクトル
        const mat4 lookAt = mat4_lookAt(camera_pos, camera_look, camera_up);

        const GLfloat prj_near = 1.0f;
        const GLfloat prj_far = 30.0f;
        const GLfloat prj_fovY = 45.0f;
        const GLfloat prj_aspect = (GLfloat) (app->surface_width) / (GLfloat) (app->surface_height);
        const mat4 projection = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);

        // 行列を転送する
        glUniformMatrix4fv(extension->unif_lookat, 1, GL_FALSE, (GLfloat*) lookAt.m);
        glUniformMatrix4fv(extension->unif_projection, 1, GL_FALSE, (GLfloat*) projection.m);

        // 座標移動
        extension->cameraPos.x -= 0.01f;
        extension->cameraPos.z += 0.02f;
    }

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
void sample_CameraSetup_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_CameraSetup *extension = (Extension_CameraSetup*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
