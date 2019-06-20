#include "defines.h"
#include "Script.h"
#include "V8ResourceManager.h"

#include <android/log.h>

#undef LOG_TAG
#define  LOG_TAG    "V8ResourceManager"

namespace DCanvas
{

v8::Handle<v8::ObjectTemplate>              V8ResourceManager::s_classProto;
v8::Handle<v8::ObjectTemplate>              V8ResourceManager::s_objectTemplate;
Handle<FunctionTemplate>                    V8ResourceManager::s_functionTemplate;
JNIEnv* V8ResourceManager::s_JNIEnv;
static const char* CLASS_NAME = "com/alibaba/cloudx/platform/ResourceManager";
static const int MAX_URL_LENGTH = 512;

v8::Handle<v8::Object> V8ResourceManager::Wrap(JNIEnv* env)
{
	s_JNIEnv = env;
    s_functionTemplate = FunctionTemplate::New();
    s_functionTemplate->SetClassName(v8::String::New("V8ResourceManager"));
    s_classProto = s_functionTemplate ->PrototypeTemplate();

    s_objectTemplate = s_functionTemplate->InstanceTemplate();
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate->Set("LoadImageResource", FunctionTemplate::New(V8ResourceManager::LoadImageResource));
    s_objectTemplate->Set("LoadSoundResource", FunctionTemplate::New(V8ResourceManager::LoadSoundResource));
    s_objectTemplate->Set("ReleaseResource", FunctionTemplate::New(V8ResourceManager::ReleaseResource));

    s_objectTemplate->Set("GetImageData", FunctionTemplate::New(V8ResourceManager::GetImageData));
    s_objectTemplate->Set("GetImageWidth", FunctionTemplate::New(V8ResourceManager::GetImageWidth));
    s_objectTemplate->Set("GetImageHeight", FunctionTemplate::New(V8ResourceManager::GetImageHeight));

    s_objectTemplate->Set("PlaySound", FunctionTemplate::New(V8ResourceManager::PlaySound));
    s_objectTemplate->Set("StopSound", FunctionTemplate::New(V8ResourceManager::StopSound));
    s_objectTemplate->Set("PauseSound", FunctionTemplate::New(V8ResourceManager::PauseSound));
    s_objectTemplate->Set("ResumeSound", FunctionTemplate::New(V8ResourceManager::ResumeSound));
    s_objectTemplate->Set("SetSoundVolume", FunctionTemplate::New(V8ResourceManager::SetSoundVolume));

    return s_objectTemplate->NewInstance();
}

/// load resource & release resource
v8::Handle<v8::Value> V8ResourceManager::LoadImageResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "LoadImageResource","(Ljava/lang/String;)I");

	if(args.Length() != 1)
	    	return THROW_EXCEPTION(TError, "LoadImageResource : number of args exception! 1");

	char url[MAX_URL_LENGTH] = {0};
	v8::Local<v8::String> jstr_tmp = args[0]->ToString();
	int len = jstr_tmp->WriteUtf8(url);

	jstring j_config_name = s_JNIEnv->NewString((const jchar*)url, len);
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, j_config_name);

	return v8::Int32::New((int)result);
}
v8::Handle<v8::Value> V8ResourceManager::LoadSoundResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "LoadSoundResource","(Ljava/lang/String;)I");

	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "LoadSoundResource : number of args exception! 1");

	char url[MAX_URL_LENGTH] = {0};
	v8::Local<v8::String> jstr_tmp = args[0]->ToString();
	int len = jstr_tmp->WriteAscii(url);


	jstring javaString = s_JNIEnv->NewStringUTF(url);

	jint result = s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, javaString);

	LOGE("%d is the id in C++ LoadSoundResource", result);
	return v8::Int32::New((int)result);
}
v8::Handle<v8::Value> V8ResourceManager::ReleaseResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "ReleaseResource","(I)I");

	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "ReleaseResource : number of args exception! 1");

	int id = args[0]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New((int)result);
}

/// images
v8::Handle<v8::Value> V8ResourceManager::GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "GetImageData","(I)[I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "GetImageData : number of args exception! 1");

	int id = args[0]->NumberValue();
	jintArray intArr = (jintArray) s_JNIEnv->CallStaticObjectMethod(JRMClass, jmethod, id);
	jsize      arrLen    = s_JNIEnv->GetArrayLength(intArr);
	jint      *jInt     = s_JNIEnv->GetIntArrayElements(intArr, JNI_FALSE);

	v8::Local<v8::Array> value;
	value = v8::Array::New(arrLen);
	for (unsigned i = 0; i < arrLen; i++)
		value->Set(v8::Int32::New(i), v8::Boolean::New(jInt[i]));
	return value;
}

v8::Handle<v8::Value> V8ResourceManager::GetImageWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "GetImageWidth","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "GetImageWidth : number of args exception! 1");

	int id = args[0]->NumberValue();
	uintptr_t width = (uintptr_t) s_JNIEnv->CallStaticObjectMethod(JRMClass, jmethod, id);

	return v8::Int32::New(width);
}

v8::Handle<v8::Value> V8ResourceManager::GetImageHeight(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "GetImageHeight","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "GetImageHeight : number of args exception! 1");

	int id = args[0]->NumberValue();
	uintptr_t height = (uintptr_t) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New(height);
}

  /// sounds
v8::Handle<v8::Value> V8ResourceManager::PlaySound(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "PlaySound","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "PlaySound : number of args exception! 1");

	int id = args[0]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New(result);
}

v8::Handle<v8::Value> V8ResourceManager::StopSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "StopSound","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "StopSound : number of args exception! 1");

	int id = args[0]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New(result);
}

v8::Handle<v8::Value> V8ResourceManager::PauseSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "PauseSound","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "PauseSound : number of args exception! 1");

	int id = args[0]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New(result);
}

v8::Handle<v8::Value> V8ResourceManager::ResumeSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "ResumeSound","(I)I");
	if(args.Length() != 1)
			return THROW_EXCEPTION(TError, "ResumeSound : number of args exception! 1");

	int id = args[0]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id);

	return v8::Int32::New(result);
}

v8::Handle<v8::Value> V8ResourceManager::SetSoundVolume(const v8::FunctionCallbackInfo<v8::Value>& args) {
	jclass JRMClass = s_JNIEnv->FindClass(CLASS_NAME);
	jmethodID jmethod = s_JNIEnv->GetStaticMethodID(JRMClass, "SetSoundVolume","(IFF)I");
	if(args.Length() != 3)
			return THROW_EXCEPTION(TError, "SetSoundVolume : number of args exception! 1");

	int id = args[0]->NumberValue();
	float left = args[1]->NumberValue();
	float right = args[2]->NumberValue();
	jint result = (jint) s_JNIEnv->CallStaticIntMethod(JRMClass, jmethod, id, left, right);

	return v8::Int32::New(result);
}

bool V8ResourceManager::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("V8ResourceManager::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}
}
