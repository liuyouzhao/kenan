//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SAL_H
#define KENAN_SAL_H

struct SI_ImageOp {
    unsigned char* (*SI_loadBase64ImageRGBA)(const char* base64, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadBase64ImageBGRA)(const char* base64, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadBase64ImageARGB)(const char* base64, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadBase64ImageABGR)(const char* base64, int &outWidth, int &outHeight);

    unsigned int* (*SI_loadBase64ImageRGBA_Int32)(const char* base64, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadBase64ImageBGRA_Int32)(const char* base64, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadBase64ImageARGB_Int32)(const char* base64, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadBase64ImageABGR_Int32)(const char* base64, int &outWidth, int &outHeight);

    unsigned char* (*SI_loadImageFileRGBA)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileBGRA)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileARGB)(const char *filePath, int &outWidth, int &outHeight);
    unsigned char* (*SI_loadImageFileABGR)(const char *filePath, int &outWidth, int &outHeight);

    unsigned int* (*SI_loadImageFileRGBA_Int32)(const char *filePath, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadImageFileBGRA_Int32)(const char *filePath, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadImageFileARGB_Int32)(const char *filePath, int &outWidth, int &outHeight);
    unsigned int* (*SI_loadImageFileABGR_Int32)(const char *filePath, int &outWidth, int &outHeight);

    void (*SI_unloadImageBuffer)(unsigned char **buf);
    void (*SI_unloadImageBuffer_Int32)(unsigned int **buf);

    int (*SI_saveBase64Image)(const char *filePath, const char* base64);
    int (*SI_saveBufferImage)(const char *filePath, unsigned char* buffer, int width, int height);
};

struct SI_FileOp {
    int (*SI_readFileFromRWLocation)(const char *filename, char *buf, int &len, int max);
    int (*SI_readFileFromROLocation)(const char *filename, char *buf, int &len, int max);
    int (*SI_writeFileToRWLocation_UpdateNew)(const char *filename, char *buf, int len);
    int (*SI_writeFileToRWLocation_AppendNew)(const char *filename, char *buf, int len);
};

struct SI_TaskOp {
    int (*SI_taskStart)(const char *taskId, const char *file, int rw);
    int (*SI_taskStart_Script)(const char *taskId, const char *script);
};

extern SI_ImageOp   si_imageOp;
extern SI_FileOp    si_fileOp;
extern SI_TaskOp    si_taskOp;
#endif //KENAN_SAL_H
