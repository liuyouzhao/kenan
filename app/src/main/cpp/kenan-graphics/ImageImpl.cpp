//
// Created by hujia on 08/07/20.
//

#include <vector>
#include "Assert.h"
#include "defines.h"
#include "Log.h"
#include "ImageImpl.h"
#include "SALImageOperation.h"

#undef LOG_TAG
#define  LOG_TAG    "ImageImpl"

using namespace kenan_sal;

namespace kenan_graphics {

/**
 * @Function: Decode base64 and save to file, then load from temp-file and delete the file
*/
unsigned int *loadImageDataFromBase64String(const char* base64, int &outWidth, int &outHeight) {
    return SAL_loadImageDataFromBase64(base64, outWidth, outHeight);
}

unsigned int *loadImageDataFromBase64String(std::string strBase64, int &outWidth, int &outHeight) {
    return loadImageDataFromBase64String(strBase64.c_str(), outWidth, outHeight);
}

/**
* @Function: load image from file
* filePath is relative file path inside assets,
*
* for example:
* /your-project/app/src/main/assets/images/character.png
* you must give images/a.png or /images/character.png
*
*/
unsigned int *loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight) {
    return SAL_loadImageDataFromFile(filePath, outWidth, outHeight);
}

unsigned int *loadImageDataFromFile(std::string filePath, int &outWidth, int &outHeight) {
    return loadImageDataFromFile(filePath.c_str(), outWidth, outHeight);
}

void unloadImageBuffer(unsigned char **buf) {
    return SAL_unloadImageBuffer(buf);
}

void unloadImageBuffer_Int32(unsigned int **buf) {
    return SAL_unloadImageBuffer_Int32(buf);
}

}