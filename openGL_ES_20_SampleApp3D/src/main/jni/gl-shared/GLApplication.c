/*
 * GLApplication.c
 *
 *  Created on: 2013/04/08
 */

#include    "support.h"

/**
 * アプリ実行を停止している場合はapp_TRUE
 */
bool GLApplication_isAbort(GLApplication *app) {
    return (app->flags & GLAPP_FLAG_ABORT) ? true : false;
}
