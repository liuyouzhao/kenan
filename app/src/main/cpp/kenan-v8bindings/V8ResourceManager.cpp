#include "defines.h"
#include "V8ResourceManager.h"
#include "ResourceLoader.h"
#include "ResourceController.h"
#include "V8Log.h"
#include "ObjectWrap.h"

using namespace kenan_platform;
namespace kenan_v8bindings
{

v8::Handle<v8::Value> V8ResourceManager::genSingleton(Isolate *isolate)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    t->Set(isolate, "func_property", v8::Number::New(isolate, 1));
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();

    //all class functions
    proto_t->Set(isolate, "LoadImageResource", FunctionTemplate::New(isolate, V8ResourceManager::LoadImageResource));
    proto_t->Set(isolate, "LoadSoundResource", FunctionTemplate::New(isolate, V8ResourceManager::LoadSoundResource));
    proto_t->Set(isolate, "ReleaseResource", FunctionTemplate::New(isolate, V8ResourceManager::ReleaseResource));
    proto_t->Set(isolate, "GetImageData", FunctionTemplate::New(isolate, V8ResourceManager::GetImageData));

#if 0
    s_objectTemplate->Set("GetImageWidth", FunctionTemplate::New(isolate, V8ResourceManager::GetImageWidth));
    s_objectTemplate->Set("GetImageHeight", FunctionTemplate::New(isolate, V8ResourceManager::GetImageHeight));

    s_objectTemplate->Set("PlaySound", FunctionTemplate::New(isolate, V8ResourceManager::PlaySound));
    s_objectTemplate->Set("StopSound", FunctionTemplate::New(isolate, V8ResourceManager::StopSound));
    s_objectTemplate->Set("PauseSound", FunctionTemplate::New(isolate, V8ResourceManager::PauseSound));
    s_objectTemplate->Set("ResumeSound", FunctionTemplate::New(isolate, V8ResourceManager::ResumeSound));
    s_objectTemplate->Set("SetSoundVolume", FunctionTemplate::New(isolate, V8ResourceManager::SetSoundVolume));
#endif
    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();
    return instance;
}

/// load resource & release resource
void V8ResourceManager::LoadImageResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "LoadImageResource : number of args exception! 1");

	char url[MAX_URL_LENGTH] = {0};
	v8::Local<v8::String> jstr_tmp = args[0]->ToString();
	int len = jstr_tmp->WriteUtf8(url);

    std::string uuid = ResourceLoader::loadImageResource(std::string(url));
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), uuid.c_str()));
}

void V8ResourceManager::LoadSoundResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "LoadSoundResource : number of args exception! 1");

    char url[MAX_URL_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[0]->ToString();
    int len = jstr_tmp->WriteUtf8(url);

    std::string uuid = ResourceLoader::loadImageResource(std::string(url));
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), uuid.c_str()));
}

void V8ResourceManager::ReleaseResource(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "ReleaseResource : number of args exception! 1");

    char url[MAX_URL_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[0]->ToString();
    int len = jstr_tmp->WriteUtf8(url);

	ResourceLoader::releaseResource(std::string(url));
}


/// images
void V8ResourceManager::GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "GetImageData : number of args exception! 1");

	int id = args[0]->NumberValue();
    unsigned int len = 0;
	unsigned int* intArr = ResourceController::getImageData(id, len);
	v8::Local<v8::Array> pixels;
	pixels = v8::Array::New(args.GetIsolate(), len);

	for (unsigned i = 0; i < len; i++)
		pixels->Set(v8::Int32::New(args.GetIsolate(), i), v8::Boolean::New(args.GetIsolate(), intArr[i]));

	args.GetReturnValue().Set(pixels);

	free(intArr);
}

void V8ResourceManager::GetImageWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if(args.Length() != 1)
		THROW_EXCEPTION(args.GetIsolate(), TError, "GetImageWidth : number of args exception! 1");

	int id = args[0]->NumberValue();
	uintptr_t width = ResourceController::getImageWidth(id);

	args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(),width));
}

void V8ResourceManager::GetImageHeight(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "GetImageHeight : number of args exception! 1");

    int id = args[0]->NumberValue();
    uintptr_t height = ResourceController::getImageHeight(id);

    args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(),height));
}

/// sounds
void V8ResourceManager::PlaySound(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "PlaySound : number of args exception! 1");

	int id = args[0]->NumberValue();

	int rt = ResourceController::playSound(id);

	if(rt != 0)
	    THROW_EXCEPTION(args.GetIsolate(), TError, "PlaySound : exception! ");

}

void V8ResourceManager::StopSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 1)
            THROW_EXCEPTION(args.GetIsolate(), TError, "StopSound : number of args exception! 1");

    int id = args[0]->NumberValue();

    int rt = ResourceController::stopSound(id);

    if(rt != 0)
        THROW_EXCEPTION(args.GetIsolate(), TError, "StopSound : exception! ");
}

void V8ResourceManager::PauseSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "PauseSound : number of args exception! 1");

    int id = args[0]->NumberValue();

    int rt = ResourceController::pauseSound(id);

    if(rt != 0)
        THROW_EXCEPTION(args.GetIsolate(), TError, "PauseSound : exception! ");
}

void V8ResourceManager::ResumeSound(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if(args.Length() != 1)
        THROW_EXCEPTION(args.GetIsolate(), TError, "ResumeSound : number of args exception! 1");

    int id = args[0]->NumberValue();

    int rt = ResourceController::resumeSound(id);

    if(rt != 0)
        THROW_EXCEPTION(args.GetIsolate(), TError, "ResumeSound : exception! ");
}

void V8ResourceManager::SetSoundVolume(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if(args.Length() != 3)
        THROW_EXCEPTION(args.GetIsolate(), TError, "SetSoundVolume : number of args exception! 1");

	int id = args[0]->NumberValue();
	float left = args[1]->NumberValue();
	float right = args[2]->NumberValue();

    int rt = ResourceController::setSoundVolume(id, left, right);

    if(rt != 0)
        THROW_EXCEPTION(args.GetIsolate(), TError, "SetSoundVolume : exception! ");
}

}
