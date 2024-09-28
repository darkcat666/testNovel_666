/*
 * support_gl_sprite.h
 *
 *  Created on: 2013/04/07
 */

#ifndef SUPPORT_GL_SPRITE_H_
#define SUPPORT_GL_SPRITE_H_

#include    "support_gl.h"

/**
 * スプライト座標への変換行列を作成する。
 * 描画元となるポリゴンは原点を中心に幅1.0/高さ1.0の大きさを持つ四角形である必要がある。
 */
extern mat4 Sprite_createPositionMatrix(const int surface_width, const int surface_height, const GLint x, const GLint y, const GLint width, const GLint height, const GLfloat rotate);

/**
 * UV座標の変換行列を作成する。
 * 描画元となるポリゴンのUVは0.0->1.0の座標を持つ四角形である必要がある。
 */
extern mat4 Sprite_createUvMatrix(const int texture_width, const int texture_height, const GLint x, const GLint y, const GLint width, const GLint height);

#endif /* SUPPORT_GL_SPRITE_H_ */
