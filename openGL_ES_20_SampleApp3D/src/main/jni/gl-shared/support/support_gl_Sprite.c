/*
 * support_gl_shader.c
 *
 *  Created on: 2013/02/19
 */

#include    "support.h"

/**
 * スプライト座標への変換行列を作成する。
 * 描画元となるポリゴンは原点を中心に幅1.0/高さ1.0の大きさを持つ四角形である必要がある。
 */
mat4 Sprite_createPositionMatrix(const int surface_width, const int surface_height, const GLint x, const GLint y, const GLint width, const GLint height, const GLfloat rotate) {
    // アスペクト補正用行列
    const GLfloat surfaceAspect = (GLfloat) surface_width / (GLfloat) surface_height;
    const mat4 aspect = mat4_scale(1, surfaceAspect, 1);

    // 拡大率を作成
    // 四角形は長さ1.0のため、描画したい幅 / サーフェイス幅を求め、それを正規化ディスプレイ幅（2倍）をかければいい
    const GLfloat xScale = (GLfloat) width / (GLfloat) surface_width * 2.0f;
    const GLfloat yScale = (GLfloat) height / (GLfloat) surface_width * 2.0f;

    const mat4 scale = mat4_scale(xScale, yScale, 0);

    // 移動行列を作成
    // 左上座標は元座標とスケーリング値から計算可能。1.0f - xScaleしているのは目減りした量を計算するため。
    const GLfloat vertexLeft = 0.5f + (1.0f - xScale) * 0.5f;
    const GLfloat vertexTop = 0.5f + (1.0f - (yScale * surfaceAspect)) * 0.5f;
    const GLfloat moveX = (GLfloat) x / (GLfloat) surface_width * 2.0f;
    const GLfloat moveY = -((GLfloat) y / (GLfloat) surface_height * 2.0f);

    // 左上に移動し、さらに右下方へオフセットさせる
    const mat4 translate = mat4_translate(-vertexLeft + moveX, vertexTop + moveY, 0);

    // 回転を行う
    const mat4 rotateM = mat4_rotate(vec3_create(0, 0, 1), rotate);

    // 描画用の行列を生成する translate <- aspect <- rotate <- scale順
    mat4 matrix = mat4_multiply(translate, aspect);
    matrix = mat4_multiply(matrix, rotateM);
    matrix = mat4_multiply(matrix, scale);
    return matrix;
}

/**
 * UV座標の変換行列を作成する。
 * 描画元となるポリゴンのUVは0.0->1.0の座標を持つ四角形である必要がある。
 */
mat4 Sprite_createUvMatrix(const int texture_width, const int texture_height, const GLint x, const GLint y, const GLint width, const GLint height) {

    // スケーリングを行う

    const GLfloat xScale = (GLfloat) width / (GLfloat) texture_width;
    const GLfloat yScale = (GLfloat) height / (GLfloat) texture_height;

    const mat4 scale = mat4_scale(xScale, yScale, 0);

    // 移動を行う
    const GLfloat xMove = (GLfloat) x / (GLfloat) texture_width;
    const GLfloat yMove = (GLfloat) y / (GLfloat) texture_height;
    const mat4 translate = mat4_translate(xMove, yMove, 0);

    return mat4_multiply(translate, scale);
}

