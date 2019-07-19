//
// Created by hujia on 19/07/19.
//
#include "defines.h"
#include "ResourceLoader.h"
#include "PlatformConfig.h"

namespace kenan_platform
{

unsigned int ResourceLoader::loadImageResource(std::string filePath)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "LoadImageResource","(Ljava/lang/String;)I");

    jstring j_config_name = jniEnv->NewString((const jchar*)filePath.c_str(), filePath.size());
	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, j_config_name);

	return result;
}

unsigned int ResourceLoader::loadSoundResource(std::string filePath)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "LoadSoundResource","(Ljava/lang/String;)I");

    jstring j_config_name = jniEnv->NewString((const jchar*)filePath.c_str(), filePath.size());
	jint result = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, j_config_name);

	return result;
}

unsigned int ResourceLoader::releaseResource(unsigned int id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
    jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "GetImageHeight","(I)I");
    uintptr_t height = (uintptr_t) jniEnv->CallStaticIntMethod(JRMClass, jmethod, id);

    return height;
}

}