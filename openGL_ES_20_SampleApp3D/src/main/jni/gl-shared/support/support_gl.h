//
//  support_gl.h
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/01/29.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#ifndef OpenGL_ES_20_support_gl_h
#define OpenGL_ES_20_support_gl_h

#ifdef __APPLE__
// iOS
#import     <OpenGLES/ES2/gl.h>
#import     <OpenGLES/ES2/glext.h>
#else
// ANDROID
// 実際にincludeされるファイルは "(AndroidNDKのフォルダ)/platforms/android-8/arch-arm/usr/include/" 配下にある。
#include    <GLES2/gl2.h>
#include    <GLES2/gl2ext.h>
#endif

/**
 * es20_XXX関数が成功した
 */
#define ES20_NO_ERROR    0

/**
 * es20_XXX関数が失敗した
 */
#define ES20_ERROR     1

/**
 * glGetError()を実行し、
 * GL_ERROR以外であればエラー内容を文字出力してES20_ERRORを返す。
 *
 */
extern int ES20_printGLError(char* file, int line);

/**
 * glCheckFramebufferStatus()を実行し、
 * GL_FRAMEBUFFER_COMPLETE以外であればエラー内容を文字出力してES20_ERRORを返す。
 */
extern int ES20_printFramebufferError(char* file, int line);

/**
 * 特定のExtensionに対応していればtrueを返す。
 * 対応していない場合、falseを返す
 */
extern bool ES20_hasExtension(const char* extension);

/**
 * 描画結果を画面へ反映する。
 * この関数を呼び出すまで、画面には反映されない。
 */
extern void ES20_postFrontBuffer(GLApplication *app);

/**
 * SJIS文字列をUTF8文字列に変換する。
 * 同じアドレスに書き込みを行うため、十分な長さを保持しておくこと。
 */
extern void ES20_sjis2utf8(GLchar *str, const int array_length);

#include    "support_gl_Texture.h"
#include    "support_gl_CompressedTexture.h"
#include    "support_gl_Vector.h"
#include    "support_gl_Sprite.h"
#include    "support_gl_Shader.h"
#include    "support_gl_Pmd.h"

#endif
