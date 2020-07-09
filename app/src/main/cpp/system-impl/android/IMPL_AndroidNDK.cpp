//
// Created by hujia on 08/07/20.
//

#include "SAL.h"
#include <jni.h>
#include <string.h>
#include "defines.h"

extern JNIEnv *javaEnvironment;

#define LOG_TAG "IMPL_AndroidNDK"

static unsigned int *loadBase64ImageRGBA(const char* base64, int &outWidth, int &outHeight) {

    jstring jstrBase64 = javaEnvironment->NewStringUTF(base64);
    jclass clazz = javaEnvironment->FindClass("com/kenan/jni/AndroidSystemImpls");

    LOGD("clazz is %d", clazz);

    jmethodID method_Android_JavaIMPL_base64ToPixel = javaEnvironment->GetStaticMethodID(clazz, "Android_JavaIMPL_base64ToPixel", "(Ljava/lang/String;)[I");

    if(method_Android_JavaIMPL_base64ToPixel == 0) {
        LOGE("Method method_Android_JavaIMPL_base64ToPixel is not exist");
        return 0;
    }

    jobject result = javaEnvironment->CallStaticObjectMethod(clazz, method_Android_JavaIMPL_base64ToPixel, jstrBase64);

    if(result == NULL)
        return NULL;

    jint *int32Arr = javaEnvironment->GetIntArrayElements((jintArray)result, NULL);

    if(int32Arr == NULL)
        return NULL;

    int width = int32Arr[0];
    int height = int32Arr[1];

    unsigned int *pixels = new unsigned int[width * height];
    memcpy(pixels, int32Arr + 2, width * height * sizeof(unsigned int));

    outWidth = width;
    outHeight = height;

    return pixels;
}

static unsigned int *loadImageFile(const char* fileName, int &outWidth, int &outHeight) {
    jstring jstrFilename = javaEnvironment->NewStringUTF(fileName);
    jclass clazz = javaEnvironment->FindClass("com/kenan/jni/AndroidSystemImpls");
    jmethodID method_Android_JavaIMPL_base64ToPixel = javaEnvironment->GetStaticMethodID(clazz, "Android_JavaIMPL_loadImage", "(Ljava/lang/String;)[I");
    jobject result = javaEnvironment->CallStaticObjectMethod(clazz, method_Android_JavaIMPL_base64ToPixel, jstrFilename);

    if(result == NULL)
        return NULL;

    jint *int32Arr = javaEnvironment->GetIntArrayElements((jintArray)result, NULL);

    if(int32Arr == NULL)
        return NULL;

    int width = int32Arr[0];
    int height = int32Arr[1];

    unsigned int *pixels = new unsigned int[width * height];
    memcpy(pixels, int32Arr + 2, width * height * sizeof(unsigned int));

    outWidth = width;
    outHeight = height;

    return pixels;
}

static void unloadImageBuffer(unsigned int **pixels) {
    delete[] *pixels;
    *pixels = 0;
}

SI_ImageOp si_imageOp = {
    .SI_loadBase64ImageRGBA_Int32 = loadBase64ImageRGBA,
    .SI_loadImageFileRGBA_Int32 = loadImageFile,
    .SI_unloadImageBuffer_Int32 = unloadImageBuffer
};