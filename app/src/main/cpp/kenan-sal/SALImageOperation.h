//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SALIMAGEOPERATION_H
#define KENAN_SALIMAGEOPERATION_H

namespace kenan_sal {

unsigned int *SAL_loadImageDataFromBase64(const char* base64, int &outWidth, int &outHeight);
unsigned int *SAL_loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight);

void SAL_unloadImageBuffer(unsigned char **buf);
void SAL_unloadImageBuffer_Int32(unsigned int **buf);

}

#endif //KENAN_SALIMAGEOPERATION_H
