/*
 * support_gl_matrix.h
 *
 *  Created on: 2013/03/24
 */

#ifndef SUPPORT_GL_MATRIX_H_
#define SUPPORT_GL_MATRIX_H_

/**
 * 360度系からラジアン角度に修正する
 */
#define degree2radian(degree) ((degree * M_PI) / 180.0)

/**
 * 2次元ベクトルを保持する構造体
 */
typedef struct vec2 {
    /**
     * X値
     */
    GLfloat x;

    /**
     * Y値
     */
    GLfloat y;
} vec2;

/**
 * 3次元ベクトルを保持する構造体
 */
typedef struct vec3 {
    /**
     * X値
     */
    GLfloat x;

    /**
     * Y値
     */
    GLfloat y;

    /**
     * Z値
     */
    GLfloat z;
} vec3;

/**
 * 4次元ベクトルを保持する構造体
 */
typedef struct vec4 {
    /**
     * X値
     */
    GLfloat x;

    /**
     * Y値
     */
    GLfloat y;

    /**
     * Z値
     */
    GLfloat z;

    /**
     * W値
     */
    GLfloat w;
} vec4;

/**
 * 行列を保持する構造体
 */
typedef struct mat4 {
    GLfloat m[4][4];
} mat4;

/**
 * 2次元ベクトルを生成する
 */
extern vec2 vec2_create(const GLfloat x, const GLfloat y);

/**
 * 3次元ベクトルを生成する
 */
extern vec3 vec3_create(const GLfloat x, const GLfloat y, const GLfloat z);

/**
 * 正規化した3次元ベクトルを生成する
 */
extern vec3 vec3_createNormalized(const GLfloat x, const GLfloat y, const GLfloat z);

/**
 * 3次元ベクトルの長さを取得する
 */
extern GLfloat vec3_length(const vec3 v);

/**
 * 3次元ベクトルを正規化する
 */
extern vec3 vec3_normalize(const vec3 v);

/**
 * 3次元ベクトルの内積を計算する
 */
extern GLfloat vec3_dot(const vec3 v0, const vec3 v1);

/**
 * 3次元ベクトルの外積を計算する
 */
extern vec3 vec3_cross(const vec3 v0, const vec3 v1);

/**
 * 単位行列を生成する
 */
extern mat4 mat4_identity();

/**
 * 移動行列を作成する
 */
extern mat4 mat4_translate(const GLfloat x, const GLfloat y, const GLfloat z);

/**
 * 拡縮行列を作成する
 */
extern mat4 mat4_scale(const GLfloat x, const GLfloat y, const GLfloat z);

/**
 * 回転行列を生成する
 */
extern mat4 mat4_rotate(const vec3 axis, const GLfloat rotate);

/**
 * 行列の乗算を行う
 */
extern mat4 mat4_multiply(const mat4 a, const mat4 b);

/**
 * 視点変換行列を生成する
 */
extern mat4 mat4_lookAt(const vec3 eye, const vec3 look, const vec3 up);

/**
 * 射影行列を生成する
 */
extern mat4 mat4_perspective(const GLfloat near, const GLfloat far, const GLfloat fovY_degree, const GLfloat aspect);

#endif /* SUPPORT_GL_MATRIX_H_ */
