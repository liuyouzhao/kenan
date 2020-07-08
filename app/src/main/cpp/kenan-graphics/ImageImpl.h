//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_IMAGEIMPL_H
#define KENAN_IMAGEIMPL_H

#include <string>

namespace kenan_graphics {

unsigned int *loadImageDataFromBase64String(const char* base64, int &outWidth, int &outHeight);
unsigned int *loadImageDataFromBase64String(std::string base64, int &outWidth, int &outHeight);

unsigned int *loadImageDataFromFile(const char *filePath, int &outWidth, int &outHeight);
unsigned int *loadImageDataFromFile(std::string filePath, int &outWidth, int &outHeight);

}


#endif //KENAN_IMAGEIMPL_H
