#include "support.h"
#include <time.h>

typedef struct {

    // 通常レンダリング用シェーダ
    struct {
        // レンダリング用シェーダープログラム
        GLuint program;

        // 位置情報属性
        GLint attr_pos;

        // UV座標属性
        GLint attr_uv;

        // フラグメントシェーダの描画色
        GLint unif_color;

        // Diffuseテクスチャ
        GLint unif_tex_diffuse;

        // 描画行列
        GLint unif_wlp;
    } main_shader;

    // エッジ描画用シェーダー
    struct {
        // レンダリング用シェーダープログラム
        GLuint program;

        // 位置情報属性
        GLint attr_pos;

        // 法線
        GLint attr_normal;

        // エッジの描画サイズ
        GLint unif_edgesize;

        // 描画行列
        GLint unif_wlp;

        // フラグメントシェーダの描画色
        GLint unif_color;

    } edge_shader;

    // サンプル用のPMDファイル
    PmdFile *pmd;

    // 頂点バッファ
    GLuint vertices_buffer;

    // インデックスバッファ
    GLuint indices_buffer;

    // サンプルPMD用のテクスチャマッピング
    PmdTextureList *textureList;

    // フィギュアの回転
    GLfloat rotate;

    // サンプルの実行開始時間
    time_t startTime;
} Extension_PmdGlFlush;

/**
 * アプリの初期化を行う
 */
void sample_PmdGlFlush_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_PmdGlFlush*) malloc(sizeof(Extension_PmdGlFlush));
    // サンプルアプリ用データを取り出す
    Extension_PmdGlFlush *extension = (Extension_PmdGlFlush*) app->extension;

    // 頂点シェーダーを用意する
    {
        // カメラ用の行列を計算に加える
        const GLchar *vertex_shader_source =
        // attributes
                "attribute highp vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"

                        // uniforms
                        "uniform highp mat4 unif_wlp;"
                        // varyings
                        "varying mediump vec2 vary_uv;"
                        // main
                        "void main() {"
                        "   gl_Position = unif_wlp * attr_pos;"
                        "   vary_uv = attr_uv;"
                        "}";

        const GLchar *fragment_shader_source =

        // uniforms
                "uniform lowp vec4 unif_color;"
                        "uniform sampler2D unif_tex_diffuse;"
                        // varyings
                        "varying mediump vec2 vary_uv;"
                        // main
                        "void main() {"
                        "   if(unif_color.a == 0.0) {"
                        "       gl_FragColor = texture2D(unif_tex_diffuse, vary_uv);"
                        "   } else {"
                        "       gl_FragColor = unif_color;"
                        "   }"
                        "}";

        // コンパイルとリンクを行う
        extension->main_shader.program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);

        // attributeを取り出す
        {
            extension->main_shader.attr_pos = glGetAttribLocation(extension->main_shader.program, "attr_pos");
            assert(extension->main_shader.attr_pos >= 0);

            extension->main_shader.attr_uv = glGetAttribLocation(extension->main_shader.program, "attr_uv");
            assert(extension->main_shader.attr_uv >= 0);
        }

        // uniform変数のlocationを取得する
        {
            extension->main_shader.unif_wlp = glGetUniformLocation(extension->main_shader.program, "unif_wlp");
            assert(extension->main_shader.unif_wlp >= 0);

            extension->main_shader.unif_color = glGetUniformLocation(extension->main_shader.program, "unif_color");
            assert(extension->main_shader.unif_color >= 0);

            extension->main_shader.unif_tex_diffuse = glGetUniformLocation(extension->main_shader.program, "unif_tex_diffuse");
            assert(extension->main_shader.unif_tex_diffuse >= 0);
        }
    }

    // エッジシェーダーを用意する
    {
        // カメラ用の行列を計算に加える
        const GLchar *vertex_shader_source =
        // attributes
                "attribute highp vec3 attr_pos;"
                        "attribute mediump vec3 attr_normal;"
                        // uniforms
                        "uniform mediump float unif_edgesize;"
                        "uniform highp mat4 unif_wlp;"
                        // main
                        "void main() {"
                        "   gl_Position = unif_wlp * vec4( attr_pos + (attr_normal * unif_edgesize), 1.0 );"
                        "}";

        const GLchar *fragment_shader_source =

        // uniforms
                "uniform lowp vec4 unif_color;"
                // main
                        "void main() {"
                        "   gl_FragColor = unif_color;"
                        "}";
        // コンパイルとリンクを行う
        extension->edge_shader.program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);

        // attributeを取り出す
        {
            extension->edge_shader.attr_pos = glGetAttribLocation(extension->edge_shader.program, "attr_pos");
            assert(extension->edge_shader.attr_pos >= 0);

            extension->edge_shader.attr_normal = glGetAttribLocation(extension->edge_shader.program, "attr_normal");
            assert(extension->edge_shader.attr_normal >= 0);
        }

        // uniform変数のlocationを取得する
        {
            extension->edge_shader.unif_wlp = glGetUniformLocation(extension->edge_shader.program, "unif_wlp");
            assert(extension->edge_shader.unif_wlp >= 0);

            extension->edge_shader.unif_edgesize = glGetUniformLocation(extension->edge_shader.program, "unif_edgesize");
            assert(extension->edge_shader.unif_edgesize >= 0);

            extension->edge_shader.unif_color = glGetUniformLocation(extension->edge_shader.program, "unif_color");
            assert(extension->edge_shader.unif_color >= 0);
        }
    }

    {
        // PMDを読み込む
        extension->pmd = PmdFile_load(app, "pmd-sample.pmd");
        assert(extension->pmd);

        // テクスチャを読み込む
        extension->textureList = PmdFile_createTextureList(app, extension->pmd);

        // 読み込んだテクスチャファイル名を表示
        int i = 0;
        for (i = 0; i < extension->textureList->textures_num; ++i) {
            __logf("Mat[%d] name(%s)", i, extension->textureList->texture_names[i]);
        }

        extension->rotate = 0;

        // 頂点用バッファオブジェクトを生成＆転送する
        {
            // バッファ生成
            glGenBuffers(1, &extension->vertices_buffer);
            assert(glGetError() == GL_NO_ERROR);
            assert(extension->vertices_buffer != 0);

            // バインド
            glBindBuffer(GL_ARRAY_BUFFER, extension->vertices_buffer);
            assert(glGetError() == GL_NO_ERROR);

            // アップロード
            glBufferData(GL_ARRAY_BUFFER, sizeof(PmdVertex) * extension->pmd->vertices_num, extension->pmd->vertices, GL_STATIC_DRAW);
            assert(glGetError() == GL_NO_ERROR);

            // バインドを解除する
            // バインドを解除しない場合、VBOが優先される
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        // インデックス用バッファオブジェクトを生成する
        {
            // バッファ生成
            glGenBuffers(1, &extension->indices_buffer);
            assert(glGetError() == GL_NO_ERROR);
            assert(extension->indices_buffer != 0);

            // バインド
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, extension->indices_buffer);

            // アップロード
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * extension->pmd->indices_num, extension->pmd->indices, GL_STATIC_DRAW);
            assert(glGetError() == GL_NO_ERROR);

            // バインドを解除する
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    // 片面レンダリングを有効にする
    glEnable(GL_CULL_FACE);

    // 初期化終了時刻を打刻
    time(&extension->startTime);
}

/**
 * レンダリングエリアが変更された
 */
void sample_PmdGlFlush_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * PMDファイルのレンダリングを行う
 */
void sample_PmdGlFlush_renderingPMD(Extension_PmdGlFlush *extension, const mat4 wlpMatrix) {
    // バッファオブジェクトのバインドを行う
    glBindBuffer(GL_ARRAY_BUFFER, extension->vertices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, extension->indices_buffer);

    // PMDのレンダリングを行う
    {
        // シェーダーの利用を開始する
        glUseProgram(extension->main_shader.program);
        assert(glGetError() == GL_NO_ERROR);

        // 背面カリング
        glCullFace(GL_BACK);

        // 属性を有効にする
        glEnableVertexAttribArray(extension->main_shader.attr_pos);
        glEnableVertexAttribArray(extension->main_shader.attr_uv);

        // 行列アップロード
        glUniformMatrix4fv(extension->main_shader.unif_wlp, 1, GL_FALSE, (GLfloat*) wlpMatrix.m);

        PmdFile *pmd = extension->pmd;
        int i = 0;

        // 頂点をバインドする
        glVertexAttribPointer(extension->main_shader.attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), (GLvoid*) 0);
        glVertexAttribPointer(extension->main_shader.attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), (GLvoid*) sizeof(vec3));

        GLint beginIndicesIndex = 0;

        // マテリアル数だけ描画を行う
        for (i = 0; i < pmd->materials_num; ++i) {
            PmdMaterial *mat = &pmd->materials[i];

            // テクスチャを取り出す
            Texture *tex = PmdFile_getTexture(extension->textureList, mat->diffuse_texture_name);
            if (tex) {
                // テクスチャがロードできている
                glBindTexture(GL_TEXTURE_2D, tex->id);
                glUniform1i(extension->main_shader.unif_tex_diffuse, 0);
                glUniform4f(extension->main_shader.unif_color, 0, 0, 0, 0);
            } else {
                // カラー情報
                glUniform4f(extension->main_shader.unif_color, mat->diffuse.x, mat->diffuse.y, mat->diffuse.z, mat->diffuse.w);
            }

            // インデックスバッファでレンダリング
            glDrawElements(GL_TRIANGLES, mat->indices_num, GL_UNSIGNED_SHORT, (GLvoid*) (beginIndicesIndex * sizeof(GLushort)));
            assert(glGetError() == GL_NO_ERROR);

            // GPUでの処理を促す
            glFlush();
            beginIndicesIndex += mat->indices_num;
        }
    }

    // エッジのレンダリングを行う
    {
        // シェーダーの利用を開始する
        glUseProgram(extension->edge_shader.program);
        assert(glGetError() == GL_NO_ERROR);

        // 前面カリング
        glCullFace(GL_FRONT);

        // 属性を有効にする
        glEnableVertexAttribArray(extension->edge_shader.attr_pos);
        glEnableVertexAttribArray(extension->edge_shader.attr_normal);

        // 行列アップロード
        glUniformMatrix4fv(extension->edge_shader.unif_wlp, 1, GL_FALSE, (GLfloat*) wlpMatrix.m);
        assert(glGetError() == GL_NO_ERROR);

        PmdFile *pmd = extension->pmd;

        // 頂点をバインドする
        glVertexAttribPointer(extension->edge_shader.attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), (GLvoid*) 0);
        glVertexAttribPointer(extension->edge_shader.attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), (GLvoid*) (sizeof(vec3) + sizeof(vec2)));

        // エッジ色情報
        glUniform4f(extension->edge_shader.unif_color, 0.0f, 0.0f, 0.0f, 1.0f);
        // エッジの太さを指定
        glUniform1f(extension->edge_shader.unif_edgesize, 0.025f);
        assert(glGetError() == GL_NO_ERROR);

        // インデックスバッファでレンダリング
        glDrawElements(GL_TRIANGLES, pmd->indices_num, GL_UNSIGNED_SHORT, (GLvoid*) 0);
        assert(glGetError() == GL_NO_ERROR);

        // GPUでの処理を促す
        glFlush();
    }
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_PmdGlFlush_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_PmdGlFlush *extension = (Extension_PmdGlFlush*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 lookMatrix;
    mat4 projectionMatrix;

    // カメラを初期化する
    {
        vec3 pmdMax;
        vec3 pmdMin;

        PmdFile_calcAABB(extension->pmd, &pmdMin, &pmdMax);

        // カメラをセットアップする
        const vec3 camera_pos = vec3_create(pmdMin.z * 10.0f, pmdMax.y * 2, pmdMin.z * 10.0f); // カメラ位置
        const vec3 camera_look = vec3_create(0, pmdMax.y * 1.25f, 0); // カメラ注視
        const vec3 camera_up = vec3_create(0, 1, 0); // カメラ上ベクトル

        const GLfloat prj_near = 10.0f;
        const GLfloat prj_far = 10000.0f;
        const GLfloat prj_fovY = 55.0f;
        const GLfloat prj_aspect = (GLfloat) (app->surface_width) / (GLfloat) (app->surface_height);

        lookMatrix = mat4_lookAt(camera_pos, camera_look, camera_up);
        projectionMatrix = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);
    }

    // レンダリング負荷を掛けるために大量のモデルを描画する
    {
        const int xModels = 8; // 横並びのモデル数
        const int zModels = 8; // 奥へのモデル数
        const GLfloat offset = 3.0f; // モデル同士の隙間距離

        mat4 lp = mat4_multiply(projectionMatrix, lookMatrix);

        int x = 0;
        int z = 0;

        for (x = 0; x < xModels; ++x) {
            for (z = 0; z < zModels; ++z) {
                // ワールド座標生成
                mat4 pos = mat4_translate(x * offset, 0, z * offset);
                mat4 rotate = mat4_rotate(vec3_create(0, 1, 0), extension->rotate);

                mat4 world = mat4_multiply(pos, rotate);
                sample_PmdGlFlush_renderingPMD(extension, mat4_multiply(lp, world));
            }
        }

        // 回転を進める
        extension->rotate += 1;

    }

    // 360度回転できたところでチェック
    if (extension->rotate > 360) {
        time_t now;
        time(&now);

        char message[256] = "";
        sprintf(message, "[%d]秒で計測を完了しました", (int) (now - extension->startTime));
        GLApplication_abortWithMessage(app, message);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_PmdGlFlush_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_PmdGlFlush *extension = (Extension_PmdGlFlush*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->main_shader.program);
    assert(glGetError() == GL_NO_ERROR);
    glDeleteProgram(extension->edge_shader.program);
    assert(glGetError() == GL_NO_ERROR);

    // delete前はバッファが有効であり、バインド済みになっているはずである
    {
        // バッファでなければならない
        assert(glIsBuffer(extension->vertices_buffer) == GL_TRUE);

        GLuint buffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &buffer);
        assert(buffer == extension->vertices_buffer);
    }

    // バッファオブジェクトの解放
    glDeleteBuffers(1, &extension->vertices_buffer);
    glDeleteBuffers(1, &extension->indices_buffer);

    // delete後はバッファが無効であり、バインドが0に戻されているはずである
    {
        // バッファであってはならない
        assert(glIsBuffer(extension->vertices_buffer) == GL_FALSE);

        GLuint buffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &buffer);
        assert(buffer == 0);
    }

    // PMDファイルを解放する
    PmdFile_free(extension->pmd);
    PmdFile_freeTextureList(extension->textureList);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
