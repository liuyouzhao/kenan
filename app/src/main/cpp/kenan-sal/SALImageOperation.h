//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SALIMAGEOPERATION_H
#define KENAN_SALIMAGEOPERATION_H

namespace kenan_sal {

external SI_ImageOp *si_imageOp;

unsigned int *SAL_loadImageDataFromBase64String(const char* base64, int &outWidth, int &outHeight);
unsigned int *SAL_loadImageDataFromBase64String(std::string base64, int &outWidth, int &outHeight);

unsigned int *SAL_loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight);
unsigned int *SAL_loadImageDataFromFile(std::string filePath, int &outWidth, int &outHeight);
}

#endif //KENAN_SALIMAGEOPERATION_H
