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

    struct {
        // 描画対象のフレームバッファ
        GLuint framebuffer;

        // 描画対象の深度バッファ
        GLuint depthTexture;

        // フレームバッファ幅（ピクセル数）
        int width;

        // フレームバッファ高さ（ピクセル数）
        int height;
    } target;

    // iOS互換のため、デフォルトのフレームバッファを保存する
    GLuint defFrameBuffer;
} Extension_PmdFramebufferDepth;

/**
 * アプリの初期化を行う
 */
void sample_PmdFramebufferDepth_initialize(GLApplication *app) {
    // 深度テクスチャに対応していない場合、abortを行う
    if (!ES20_hasExtension("GL_OES_depth_texture")) {
        GLApplication_abortWithMessage(app, "非対応の拡張機能です。\nGL_OES_depth_texture");
        return;
    }

    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_PmdFramebufferDepth*) malloc(sizeof(Extension_PmdFramebufferDepth));
    // サンプルアプリ用データを取り出す
    Extension_PmdFramebufferDepth *extension = (Extension_PmdFramebufferDepth*) app->extension;

    // 頂点シェーダーを用意する
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
                "attribute mediump vec3 attr_pos;"
                        "attribute mediump vec3 attr_normal;"
                        // uniforms
                        "uniform mediump float unif_edgesize;"
                        "uniform mediump mat4 unif_wlp;"
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

    // レンダリングターゲットを用意する
    {
        // デフォルトのフレームバッファを取得する
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*) &extension->defFrameBuffer);
#ifdef  __APPLE__
            // iOSの場合はデフォルトのフレームバッファが無ければならない
            assert(extension->defFrameBuffer != 0);
#else
            // Androidの場合はデフォルトのフレームバッファは0になっている
            assert(extension->defFrameBuffer == 0);
#endif
        }

        // テクスチャの解像度を決める(Pixel)
        extension->target.width = 512;
        extension->target.height = 512;

        // 描画対象のテクスチャを生成する
        {
            glGenTextures(1, &extension->target.depthTexture);
            assert(glGetError() == GL_NO_ERROR);
            assert(extension->target.depthTexture != 0);

            // テクスチャのメモリを確保する
            glBindTexture(GL_TEXTURE_2D, extension->target.depthTexture);

            // ラップ設定とフィルタ設定
            // PowerVR SGX 540ではラッピングにGL_CLAMP_TO_EDGEを指定しなければ正常にテクスチャアクセスが行えない
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, extension->target.width, extension->target.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            assert(glGetError() == GL_NO_ERROR);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // テクスチャとバッファをフレームバッファへアタッチする
        {
            glGenFramebuffers(1, &extension->target.framebuffer);
            assert(glGetError() == GL_NO_ERROR);
            assert(extension->target.framebuffer != 0);

            // フレームバッファの設定
            glBindFramebuffer(GL_FRAMEBUFFER, extension->target.framebuffer);

            // テクスチャをカラーバッファにアタッチする
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, extension->target.depthTexture, 0);
            assert(glGetError() == GL_NO_ERROR);

            // フレームバッファとして有効な状態になっていることを確認する
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

            // フレームバッファをアンバインド
            glBindFramebuffer(GL_FRAMEBUFFER, extension->defFrameBuffer);
        }
    }

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    // 片面レンダリングを有効にする
    glEnable(GL_CULL_FACE);
}

/**
 * レンダリングエリアが変更された
 */
void sample_PmdFramebufferDepth_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * PMDファイルのレンダリングを行う
 */
void sample_PmdFramebufferDepth_renderingPMD(Extension_PmdFramebufferDepth *extension, const mat4 wlpMatrix) {
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
    }
}

/**
 * 立方体を描画する
 */
void sample_PmdFramebufferDepth_renderingCube(GLApplication *app, GLuint texture) {
    // サンプルアプリ用データを取り出す
    Extension_PmdFramebufferDepth *extension = (Extension_PmdFramebufferDepth*) app->extension;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCullFace(GL_BACK);

    // メインシェーダーで描画を行う
    glUseProgram(extension->main_shader.program);
    // 属性を有効にする
    glEnableVertexAttribArray(extension->main_shader.attr_pos);
    glEnableVertexAttribArray(extension->main_shader.attr_uv);

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
        glUniformMatrix4fv(extension->main_shader.unif_wlp, 1, GL_FALSE, (GLfloat*) wlp.m);
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
                0, 1, 2, 3, //
                3, 2, // 縮退
                2, 3, 6, 7, //
                7, 6, // 縮退
                6, 7, 4, 5, //
                5, 4, // 縮退
                4, 5, 0, 1, //
                1, 1, // 縮退
                1, 5, 3, 7, //
                7, 0, // 縮退
                0, 2, 4, 6, //
                };

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(extension->main_shader.unif_tex_diffuse, 0);
        glUniform4f(extension->main_shader.unif_color, 0, 0, 0, 0);

        glVertexAttribPointer(extension->main_shader.attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*) cubeVertices);
        glVertexAttribPointer(extension->main_shader.attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (GLvoid*) ((GLubyte*) cubeVertices + sizeof(vec3)));
        glDrawElements(GL_TRIANGLE_STRIP, 4 * 6 + 2 * 5, GL_UNSIGNED_SHORT, cubeIndices);

        assert(glGetError() == GL_NO_ERROR);
    }
}

/**
 * 2D描画を行う
 */
void sample_PmdFramebufferDepth_renderingSprite(GLApplication *app, GLuint texture, int x, int y, int width, int height) {
    // サンプルアプリ用データを取り出す
    Extension_PmdFramebufferDepth *extension = (Extension_PmdFramebufferDepth*) app->extension;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCullFace(GL_BACK);

    // メインシェーダーで描画を行う
    glUseProgram(extension->main_shader.program);
    // 属性を有効にする
    glEnableVertexAttribArray(extension->main_shader.attr_pos);
    glEnableVertexAttribArray(extension->main_shader.attr_uv);

    const GLfloat position[] = {
    // v0(left top)
            -0.5f, 0.5f,
            // v1(left bottom)
            -0.5f, -0.5f,
            // v2(right top)
            0.5f, 0.5f,
            // v3(right bottom)
            0.5f, -0.5f, };

    //
    const GLfloat uv[] = {
    // v0(left top)
            0, 0,
            // v1(left bottom)
            0, 1,
            // v2(right top)
            1, 0,
            // v3(right bottom)
            1, 1, };

    // 頂点情報を関連付ける
    glVertexAttribPointer(extension->main_shader.attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
    glVertexAttribPointer(extension->main_shader.attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
    assert(glGetError() == GL_NO_ERROR);

    // アップロード
    {
        const mat4 matrix = Sprite_createPositionMatrix(app->surface_width, app->surface_height, x, y, width, height, 0);
        glUniformMatrix4fv(extension->main_shader.unif_wlp, 1, GL_FALSE, (GLfloat*) matrix.m);
        assert(glGetError() == GL_NO_ERROR);

        glUniform1i(extension->main_shader.unif_tex_diffuse, 0);
        glUniform4f(extension->main_shader.unif_color, 0, 0, 0, 0);
    }

    // 深度テストを無効化して描画
    glDisable(GL_DEPTH_TEST);
    {

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glEnable(GL_DEPTH_TEST);

    assert(glGetError() == GL_NO_ERROR);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_PmdFramebufferDepth_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_PmdFramebufferDepth *extension = (Extension_PmdFramebufferDepth*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // フレームバッファへ一度レンダリングする
    {
        glBindFramebuffer(GL_FRAMEBUFFER, extension->target.framebuffer);
        assert(glGetError() == GL_NO_ERROR);

        // Viewportを修正する
        glViewport(0, 0, extension->target.width, extension->target.height);

        // バッファをクリアする
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

            const GLfloat prj_aspect = (GLfloat) (extension->target.width) / (GLfloat) (extension->target.height);

            const mat4 lookMatrix = mat4_lookAt(camera_pos, camera_look, camera_up);
            const mat4 projectionMatrix = mat4_perspective(prj_near, prj_far, prj_fovY, prj_aspect);

            const mat4 world = mat4_rotate(vec3_create(0, 1, 0), extension->rotate);

            wlpMatrix = mat4_multiply(projectionMatrix, lookMatrix);
            wlpMatrix = mat4_multiply(wlpMatrix, world);

            extension->rotate += 1;
        }
        sample_PmdFramebufferDepth_renderingPMD(extension, wlpMatrix);

        // デフォルトのフレームバッファに戻す
        glBindFramebuffer(GL_FRAMEBUFFER, extension->defFrameBuffer);
    }

    // viewportを修正する
    glViewport(0, 0, app->surface_width, app->surface_height);

    // 立方体として描画する
    {

        sample_PmdFramebufferDepth_renderingCube(app, extension->target.depthTexture);
    }

    // 2Dとして描画する
    {
        sample_PmdFramebufferDepth_renderingSprite(app, extension->target.depthTexture, 10, 10, extension->target.width, extension->target.height);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_PmdFramebufferDepth_destroy(GLApplication *app) {
    if (GLApplication_isAbort(app)) {
        return;
    }

    // サンプルアプリ用データを取り出す
    Extension_PmdFramebufferDepth *extension = (Extension_PmdFramebufferDepth*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->main_shader.program);
    assert(glGetError() == GL_NO_ERROR);
    glDeleteProgram(extension->edge_shader.program);
    assert(glGetError() == GL_NO_ERROR);

    // バッファオブジェクトの解放
    glDeleteBuffers(1, &extension->vertices_buffer);
    glDeleteBuffers(1, &extension->indices_buffer);

    // バッファを解放する
    glDeleteTextures(1, &extension->target.depthTexture);
    glDeleteFramebuffers(1, &extension->target.framebuffer);

    // PMDファイルを解放する
    PmdFile_free(extension->pmd);
    PmdFile_freeTextureList(extension->textureList);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
