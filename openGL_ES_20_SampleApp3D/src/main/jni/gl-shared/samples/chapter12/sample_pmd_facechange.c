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

    // サンプル用のPMDファイル
    PmdFile *pmd;

    // サンプルPMD用のテクスチャリスト
    PmdTextureList *textureList;

    // UVオフセット値
    GLint unif_uvoffset;

    // 現在の描画フレーム
    int frame;

    // 描画する目番号
    int eyeNumber;

    // 描画する口番号
    int mouthNumber;
} Extension_PmdFacechange;

/**
 * アプリの初期化を行う
 */
void sample_PmdFacechange_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_PmdFacechange*) malloc(sizeof(Extension_PmdFacechange));
    // サンプルアプリ用データを取り出す
    Extension_PmdFacechange *extension = (Extension_PmdFacechange*) app->extension;

    // 頂点シェーダーを用意する
    {
        // 表情用のデータを処理に加える
        const GLchar *vertex_shader_source =
        // attributes
                "attribute highp vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"

                        // uniforms
                        "uniform highp mat4 unif_wlp;"
                        "uniform mediump float unif_uvoffset;"
                        // varyings
                        "varying mediump vec2 vary_uv;"
                        // main
                        "void main() {"
                        "   gl_Position = unif_wlp * attr_pos;"
                        "   vary_uv = vec2(attr_uv.x, attr_uv.y + unif_uvoffset);"
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

        extension->unif_tex_diffuse = glGetUniformLocation(extension->shader_program, "unif_tex_diffuse");
        assert(extension->unif_tex_diffuse >= 0);

        extension->unif_uvoffset = glGetUniformLocation(extension->shader_program, "unif_uvoffset");
        assert(extension->unif_uvoffset >= 0);
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
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    srand((unsigned int) extension); // 適当な値で乱数リセット
    extension->frame = 0;
    extension->mouthNumber = 0;
    extension->eyeNumber = 0;
}

/**
 * レンダリングエリアが変更された
 */
void sample_PmdFacechange_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_PmdFacechange_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_PmdFacechange *extension = (Extension_PmdFacechange*) app->extension;

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

        const mat4 world = mat4_identity();

        wlpMatrix = mat4_multiply(projectionMatrix, lookMatrix);
        wlpMatrix = mat4_multiply(wlpMatrix, world);
    }

    // PMDのレンダリングを行う
    {
        PmdFile *pmd = extension->pmd;
        int i = 0;

        // 頂点をバインドする
        glVertexAttribPointer(extension->attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), &pmd->vertices[0].position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(PmdVertex), &pmd->vertices[0].uv);

        // 描画行列アップロード
        glUniformMatrix4fv(extension->unif_wlp, 1, GL_FALSE, (GLfloat*) wlpMatrix.m);

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

            // UV情報をずらす
            if (i == 2) {
                // マテリアル2番は目のUVを加工する
                const GLfloat offsetV = (1.0f / 4) * extension->eyeNumber;
                glUniform1f(extension->unif_uvoffset, offsetV);
            } else if (i == 3) {
                // マテリアル3番は口のUVを加工する
                const GLfloat offsetV = (1.0f / 4) * extension->mouthNumber;
                glUniform1f(extension->unif_uvoffset, offsetV);
            } else {
                // それ以外のマテリアルはUVの加工を行わない
                glUniform1f(extension->unif_uvoffset, 0);
            }

            // インデックスバッファでレンダリング
            glDrawElements(GL_TRIANGLES, mat->indices_num, GL_UNSIGNED_SHORT, pmd->indices + beginIndicesIndex);
            assert(glGetError() == GL_NO_ERROR);
            beginIndicesIndex += mat->indices_num;
        }
    }

    // 表情変更を行う
    {
        ++extension->frame;
        // 適当なペースで表情を変更する
        if (extension->frame % 60 == 0) {
            // 適当な乱数を発生させて表情を変更する
            extension->eyeNumber = (rand() / 100) % 4;
            extension->mouthNumber = (rand() / 100) % 4;
        }
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_PmdFacechange_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_PmdFacechange *extension = (Extension_PmdFacechange*) app->extension;

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
