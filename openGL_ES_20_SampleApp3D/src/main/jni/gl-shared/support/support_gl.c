#include "support.h"

int ES20_printGLError(char* file, int line) {
    const GLenum error = glGetError();
    if (error == GL_NO_ERROR) {
        return ES20_NO_ERROR;
    }
#define LOG_GL( error_enum )    case error_enum: printf(#error_enum " %s - L %d\n", util_getFileName(file), line ); return error != GL_NO_ERROR ? ES20_ERROR : ES20_NO_ERROR;
    switch (error) {
        LOG_GL(GL_INVALID_ENUM);
        LOG_GL(GL_INVALID_VALUE);
        LOG_GL(GL_INVALID_OPERATION);
            //        LOG_GL(GL_STACK_OVERFLOW);
            //        LOG_GL(GL_STACK_UNDERFLOW);
        LOG_GL(GL_OUT_OF_MEMORY);
            //        LOG_GL(GL_TABLE_TOO_LARGE);
            //        LOG_GL(GL_INVALID_ENUM);
    }
    return ES20_ERROR;
}

int ES20_printFramebufferError(char* file, int line) {
    const GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (error == GL_FRAMEBUFFER_COMPLETE) {
        return ES20_NO_ERROR;
    }
    /*
     #define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                0x8CD6
     #define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT        0x8CD7
     #define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS                0x8CD9
     #define GL_FRAMEBUFFER_UNSUPPORTED                          0x8CDD
     */
#define LOG_GL( error_enum )    case error_enum: printf(#error_enum " %s - L %d\n", util_getFileName(file), line ); return error != GL_NO_ERROR ? ES20_ERROR : ES20_NO_ERROR;
    switch (error) {
        LOG_GL(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
        LOG_GL(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
        LOG_GL(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
        LOG_GL(GL_FRAMEBUFFER_UNSUPPORTED);
    }
    return ES20_ERROR;
}

/**
 * 特定のExtensionに対応していればtrueを返す。
 * 対応していない場合、falseを返す
 */
bool ES20_hasExtension(const char* extension) {
    const GLubyte* EXTENSIONS = glGetString(GL_EXTENSIONS);

    // 文字列が見つかればtrue、見つからなければfalse
    return strstr((const char* const) EXTENSIONS, extension) != NULL;
}

