//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SAL_H
#define KENAN_SAL_H

#endif //KENAN_SAL_H

struct SI_ImageOp {
    unsigned char* (*SI_loadBase64ImageRGBA)(const char* base64, int *outWidth, int *outHeight);
    unsigned char* (*SI_loadBase64ImageBGRA)(const char* base64, int *outWidth, int *outHeight);
    unsigned char* (*SI_loadBase64ImageARGB)(const char* base64, int *outWidth, int *outHeight);
    unsigned char* (*SI_loadBase64ImageABGR)(const char* base64, int *outWidth, int *outHeight);

    unsigned char* (*SI_loadImageFileRGBA)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileBGRA)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileARGB)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileABGR)(const char *filePath, int &outWidth, int &outHeight);

    int (*SI_saveBase64Image)(const char *filePath, const char* base64);
    int (*SI_saveBufferImage)(const char *filePath, unsigned char* buffer, int width, int height);
};
