#include "support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

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

    // 透過量
    GLint unif_alpha;

    // サンプル用のPMDファイル
    PmdFile *pmd;

    // サンプルPMD用のテクスチャリスト
    PmdTextureList *textureList;

    // フィギュアの回転
    GLfloat rotate;

    // モデルの透過
    GLfloat alpha;
} Extension_RenderAlpha2Pass;

/**
 * アプリの初期化を行う
 */
void sample_RenderAlpha2Pass_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderAlpha2Pass*) malloc(sizeof(Extension_RenderAlpha2Pass));
    // サンプルアプリ用データを取り出す
    Extension_RenderAlpha2Pass *extension = (Extension_RenderAlpha2Pass*) app->extension;

    // シェーダーを用意する
    {
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
                        "uniform lowp float unif_alpha;"
                        // varyings
                        "varying mediump vec2 vary_uv;"
                        // main
                        "void main() {"
                        "   if(unif_color.a == 0.0) {"
                        "       gl_FragColor = texture2D(unif_tex_diffuse, vary_uv);"
                        "   } else {"
                        "       gl_FragColor = unif_color;"
                        "   }"
                        // アルファを書き込む
                        "   gl_FragColor.a *= unif_alpha;"
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
        extension->unif_wlp = glGetUniformLocation(extension->shader_program, "unif_wlp");
        assert(extension->unif_wlp >= 0);

        extension->unif_color = glGetUniformLocation(extension->shader_program, "unif_color");
        assert(extension->unif_color >= 0);

        extension->unif_alpha = glGetUniformLocation(extension->shader_program, "unif_alpha");
        assert(extension->unif_alpha >= 0);

        extension->unif_tex_diffuse = glGetUniformLocation(extension->shader_program, "unif_tex_diffuse");
        assert(extension->unif_tex_diffuse >= 0);
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
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);
    // アルファブレンドを有効にする
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    extension->alpha = 1.0f;
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderAlpha2Pass_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderAlpha2Pass_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderAlpha2Pass *extension = (Extension_RenderAlpha2Pass*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 属性を有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    mat4 wlpMatrix;

    // カメラを初期化する
    {
        vec3 pmdMax;
        vec3 pmdMin;

        PmdFile_calcAABB(extension->pmd, &pmdMin, &pmdMax);

        // カメラをセットアップする
        const vec3 camera_pos = vec3_create(0, pmdMax.y * 0.7f, pmdMin.z * 7.0f); // カメラ位置
        const vec3 camera_look = vec3_create(0, pmdMax.y * 0.3f, 0); // カメラ注視
        const vec3 camera_up = vec3_create(0, 1, 0); // カメラ上ベクトル

        const GLfloat prj_near = 1.0f;
        const GLfloat prj_far = (pmdMax.z - pmdMin.z) * 30.0f;
        const GLfloat prj_fovY = 45.0f;
        const GLfloat prj_aspect = (GLfloat) (app->surface_width) / (GLfloat) (app->surface_height);

        const mat4 lookMatrix = mat4_lookAt(camera_pos, camera_look, camera_up);
        const mat4 projectionMatrix = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);

        const mat4 world = mat4_rotate(vec3_create(0, 1, 0), extension->rotate);

        wlpMatrix = mat4_multiply(projectionMatrix, lookMatrix);
        wlpMatrix = mat4_multiply(wlpMatrix, world);

        extension->rotate += 1;
    }

    // PMDのレンダリングを行う
    PmdFile *pmd = extension->pmd;

    // 頂点をバインドする
    glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), &pmd->vertices[0].position);
    glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), &pmd->vertices[0].uv);

    // 描画行列アップロード
    glUniformMatrix4fv(extension->unif_wlp, 1, GL_FALSE, (GLfloat*) wlpMatrix.m);

    {
        // 透過量をアップロードする
        glUniform1f(extension->unif_alpha, extension->alpha);

        // 透過させる
        extension->alpha -= 0.0025f;
        if (extension->alpha < 0) {
            // 透過しきったら元に戻す
            extension->alpha = 1.0f;
        }
    }

    // 1パス目の描画
    {
        // 最初のパスではZバッファの書き込みを行う
        glDepthFunc(GL_LESS);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_TRUE);

        // 1パス目は単色で一括描画
        glUniform4f(extension->unif_color, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, pmd->indices_num, GL_UNSIGNED_SHORT, pmd->indices);
        assert(glGetError() == GL_NO_ERROR);
    }

    // 2パス目の描画
    {
        // 2回目のパスでは同じZ状態のみを描画する
        glDepthFunc(GL_EQUAL);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_FALSE);

        int i = 0;

        GLint beginIndicesIndex = 0;

        // マテリアル数だけ描画を行う
        for (i = 0; i < pmd->materials_num; ++i) {

            PmdMaterial *mat = &pmd->materials[i];

            // テクスチャを取り出す
            Texture *tex = PmdFile_getTexture(extension->textureList, mat->diffuse_texture_name);
            if (tex) {
                // テクスチャがロードできている
                glBindTexture(GL_TEXTURE_2D, tex->id);
                glUniform1i(extension->unif_tex_diffuse, 0);
                glUniform4f(extension->unif_color, 0, 0, 0, 0);
            } else {
                // カラー情報
                glUniform4f(extension->unif_color, mat->diffuse.x, mat->diffuse.y, mat->diffuse.z, mat->diffuse.w);
            }

            // インデックスバッファでレンダリング
            glDrawElements(GL_TRIANGLES, mat->indices_num, GL_UNSIGNED_SHORT, pmd->indices + beginIndicesIndex);
            assert(glGetError() == GL_NO_ERROR);
            beginIndicesIndex += mat->indices_num;
        }
    }

    // マスクを元に戻す
    // 戻さなければglClearの結果にも影響を与える
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderAlpha2Pass_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderAlpha2Pass *extension = (Extension_RenderAlpha2Pass*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // PMDファイルを解放する
    PmdFile_free(extension->pmd);
    PmdFile_freeTextureList(extension->textureList);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
