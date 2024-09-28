/*
 * support_gl_shader.h
 *
 *  Created on: 2013/02/19
 */

#ifndef SUPPORT_GL_SHADER_H_
#define SUPPORT_GL_SHADER_H_

/**
 * 頂点・フラグメントシェーダーをリンクし、プログラムオブジェクトを作成する
 */
extern GLuint Shader_createProgramFromSource(const char* vertex_shader_source, const char* fragment_shader_source);


#endif /* SUPPORT_GL_SHADER_H_ */
