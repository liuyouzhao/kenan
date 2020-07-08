//
// Created by hujia on 19/07/19.
//
#include "defines.h"
#include "ResourceLoader.h"
#include "PlatformConfig.h"

namespace kenan_platform
{

std::string ResourceLoader::loadImageResource(std::string filePath)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();
    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "LoadImageResource","(Ljava/lang/String;)java/lang/String;");

    jstring j_config_name = jniEnv->NewString((const jchar*)filePath.c_str(), filePath.size());
	jstring result = (jstring) jniEnv->CallStaticIntMethod(JRMClass, jmethod, j_config_name);

    const char *str = jniEnv->GetStringUTFChars(result, 0);
    return std::string(str);
}

std::string ResourceLoader::loadSoundResource(std::string filePath)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
	jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "LoadSoundResource","(Ljava/lang/String;)java/lang/String;");

    jstring j_config_name = jniEnv->NewString((const jchar*)filePath.c_str(), filePath.size());
	jstring result = (jstring) jniEnv->CallStaticIntMethod(JRMClass, jmethod, j_config_name);

    const char *str = jniEnv->GetStringUTFChars(result, 0);
	return std::string(str);
}

int ResourceLoader::releaseResource(std::string id)
{
    JNIEnv *jniEnv = PlatformConfig::instance()->env();

    jclass JRMClass = jniEnv->FindClass(RESOURCE_MANAGER_CLASS_NAME);
    jmethodID jmethod = jniEnv->GetStaticMethodID(JRMClass, "ReleaseResource","(java/lang/String;)I");
    jstring j_id = jniEnv->NewString((const jchar*)id.c_str(), id.size());
    jint rt = (jint) jniEnv->CallStaticIntMethod(JRMClass, jmethod, j_id);
    return rt;
}

}