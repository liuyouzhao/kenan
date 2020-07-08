//
// Created by hujia on 19/07/19.
//
#include "defines.h"
#include "ResourceController.h"
#include "PlatformConfig.h"


namespace kenan_platform
{

unsigned int* ResourceController::getImageData(unsigned int id, unsigned int &length)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
    jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
    jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "GetImageData","(I)[I");

    jintArray intArr = (jintArray) jniEnv->CallStaticObjectMethod(JRMClass, jmethod, id);
    jsize      arrLen    = jniEnv->GetArrayLength(intArr);
    jint      *jInt     = jniEnv->GetIntArrayElements(intArr, JNI_FALSE);

    length = arrLen;

    unsigned int* buf = (unsigned int*) malloc(sizeof(unsigned int) * arrLen);
    memcpy(buf, jInt, sizeof(unsigned int) * arrLen);
    return buf;
}

unsigned int ResourceController::getImageWidth(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
    jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "GetImageWidth","(I)I");

	uintptr_t width = (uintptr_t) jniEnv->CallStaticObjectMethod(JRMClass, jmethod, id);

	return width;
}

unsigned int ResourceController::getImageHeight(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
    jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "GetImageHeight","(I)I");

	uintptr_t height = (uintptr_t) jniEnv->CallStaticObjectMethod(JRMClass, jmethod, id);

	return height;
}

int ResourceController::playSound(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
	jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "PlaySound","(I)I");

	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id);
	return result;
}

int ResourceController::stopSound(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

	jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "StopSound","(I)I");

	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id);
	return result;
}


int ResourceController::pauseSound(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

	jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "PauseSound","(I)I");

	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id);
	return result;
}


int ResourceController::resumeSound(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

	jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "ResumeSound","(I)I");

	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id);
	return result;
}

int ResourceController::setSoundVolume(unsigned int id, float left, float right)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

	jclass JRMClass = jniEnv->FindClass(RESOURCE_CONTROLLER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "SetSoundVolume","(I)IFF");

	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id, left, right);
	return result;
}

}