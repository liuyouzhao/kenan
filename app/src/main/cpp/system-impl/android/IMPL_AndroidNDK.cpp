//
// Created by hujia on 08/07/20.
//

#include "SAL.h"
#include <jni.h>
#include <string.h>
#include <pthread.h>
#include "defines.h"

//static JNIEnv *javaEnvironment = NULL;
extern JNIEnv *javaEnvironment;
extern JavaVM *jvm;

#define LOG_TAG "IMPL_AndroidNDK"


class __THREAD_ATTACHER {
public:
    __THREAD_ATTACHER() {
#if 0
        const jint stats = jvm->GetEnv((void**)&javaEnvironment, JNI_VERSION_1_6);
        if(stats == JNI_EDETACHED) {
            int ret = jvm->AttachCurrentThread(&javaEnvironment, NULL);
            if(ret != 0) {
                LOGE("[ERROR] __THREAD_ATTACHER AttachCurrentThread failed %d", ret);
                exit(-1);
            }
            else {
                LOGD("Thread-%d got attached", pthread_self());
            }
        }
#endif
    }

    ~__THREAD_ATTACHER() {
    }
};


static unsigned int *android_loadBase64ImageRGBA(const char* base64, int &outWidth, int &outHeight) {
    __THREAD_ATTACHER __ta;

    jstring jstrBase64 = javaEnvironment->NewStringUTF(base64);
    jclass clazz = javaEnvironment->FindClass("com/kenan/jni/AndroidSystemImpls");

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

static unsigned int *android_loadImageFile(const char* fileName, int &outWidth, int &outHeight) {
    __THREAD_ATTACHER __ta;

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

static void android_unloadImageBuffer(unsigned int **pixels) {
    delete[] *pixels;
    *pixels = 0;
}

static int readFileImpl(const char *filename, char *buf, int &len, int max, const char *methodName) {
    jstring jstrFilename = javaEnvironment->NewStringUTF(filename);

    jclass integerClazz = javaEnvironment->FindClass("java/lang/Integer");
    jobjectArray arrLen = javaEnvironment->NewObjectArray(1, integerClazz, NULL);

    jclass clazz = javaEnvironment->FindClass("com/kenan/jni/AndroidSystemImpls");
    jclass clazzDto = javaEnvironment->FindClass("com/kenan/jni/JavaToCppDto");
    jmethodID method_Android_JavaIMPL = javaEnvironment->GetStaticMethodID(clazz,
                    methodName,
                    "(Ljava/lang/String;)Lcom/kenan/jni/JavaToCppDto;");

    if(method_Android_JavaIMPL == 0) {
        LOGE("No Android_JavaIMPL_readFileStorage method in java");
        return -1;
    }

    jobject javaDto = javaEnvironment->CallStaticObjectMethod(clazz, method_Android_JavaIMPL, jstrFilename, arrLen);

    if(javaDto == NULL) {
        LOGE("android_readFileFromRWLocation return null from java");
        return -1;
    }

    jfieldID int0FieldID = javaEnvironment->GetFieldID(clazzDto, "int0", "I");
    jfieldID byteArray0FieldID = javaEnvironment->GetFieldID(clazzDto, "byteArray0", "[B");
    jint int32Len = javaEnvironment->GetIntField(javaDto, int0FieldID);
    jobject jobjByteArray = javaEnvironment->GetObjectField(javaDto, byteArray0FieldID);
    jbyte *content = javaEnvironment->GetByteArrayElements((jbyteArray)jobjByteArray, NULL);

    len = (jint)int32Len;

    LOGD("length is %d", len);

    if(len > max) {
        LOGW("The script is not completed, as it's length %d is larger than max length %d", len, max);
        len = max;
    }
    memcpy(buf, content, len);
    return len;
}

static int android_readFileFromROLocation(const char *filename, char *buf, int &len, int max) {
    __THREAD_ATTACHER __ta;
    return readFileImpl(filename, buf, len, max, "Android_JavaIMPL_readFileAssets");
}

static int android_readFileFromRWLocation(const char *filename, char *buf, int &len, int max) {
    __THREAD_ATTACHER __ta;
    return readFileImpl(filename, buf, len, max, "Android_JavaIMPL_readFileStorage");
}

static int android_writeFileToRWLocation_UpdateNew(const char *filename, char *buf, int len) {
    LOGI("%s Not implemented", __FUNCTION__);
    return 0;
}

static int android_writeFileToRWLocation_AppendNew(const char *filename, char *buf, int len) {
    LOGI("%s Not implemented", __FUNCTION__);
    return 0;
}

SI_ImageOp si_imageOp = {
    .SI_loadBase64ImageRGBA_Int32 = android_loadBase64ImageRGBA,
    .SI_loadImageFileRGBA_Int32 = android_loadImageFile,
    .SI_unloadImageBuffer_Int32 = android_unloadImageBuffer
};

SI_FileOp si_fileOp = {
    .SI_readFileFromRWLocation = android_readFileFromRWLocation,
    .SI_readFileFromROLocation = android_readFileFromROLocation,
    .SI_writeFileToRWLocation_UpdateNew = android_writeFileToRWLocation_UpdateNew,
    .SI_writeFileToRWLocation_AppendNew = android_writeFileToRWLocation_AppendNew
};