#include    "support.h"

/**
 * ファイルのフルパス -> ファイル名に変換する
 */
char* util_getFileName(char* __file__) {
    return strrchr(__file__, '/') + 1;
}

