//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SALIMAGEOPERATION_H
#define KENAN_SALIMAGEOPERATION_H

namespace kenan_sal {
unsigned int *SAL_loadImageDataFromBase64(const char* base64, int &outWidth, int &outHeight);
unsigned int *SAL_loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight);
}

#endif //KENAN_SALIMAGEOPERATION_H
