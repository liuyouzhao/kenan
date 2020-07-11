//
// Created by hujia on 22/06/19.
//

#include "V8Log.h"
#include "Log.h"
#include "defines.h"

using namespace kenan_system;

#undef LOG_TAG
#define  LOG_TAG    "V8Log"

namespace kenan_v8bindings {

Persistent<ObjectTemplate> V8Log::logTemplate;

void V8Log::info(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    std::string stringBuffer = "";
    for(int i = 0; i < args.Length(); i ++)
    {
        char name[TEXT_BUFFER_LENGTH] = {0};
        v8::Local<v8::String> jsString = args[i]->ToString();
        jsString->WriteUtf8(name);
        stringBuffer = stringBuffer + std::string(name);
    }
    Log::info("[javascript]", stringBuffer.c_str());
}

void V8Log::debug(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    std::string stringBuffer = "";
    for(int i = 0; i < args.Length(); i ++)
    {
        char name[TEXT_BUFFER_LENGTH] = {0};
        v8::Local<v8::String> jsString = args[i]->ToString();
        jsString->WriteUtf8(name);
        stringBuffer = stringBuffer + std::string(name);
    }
    Log::debug("[javascript]", stringBuffer.c_str());
}


void V8Log::error(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    std::string stringBuffer = "";
    for(int i = 0; i < args.Length(); i ++)
    {
        char name[TEXT_BUFFER_LENGTH] = {0};
        v8::Local<v8::String> jsString = args[i]->ToString();
        jsString->WriteUtf8(name);
        stringBuffer = stringBuffer + std::string(name);
    }
    Log::error("[javascript]", stringBuffer.c_str());
}


Handle<Object> V8Log::genSingleton(Isolate* isolate)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    t->Set(isolate, "func_property", v8::Number::New(isolate, 1));
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();
    proto_t->Set(isolate, "info", v8::FunctionTemplate::New(isolate, V8Log::info));
    proto_t->Set(isolate, "debug", v8::FunctionTemplate::New(isolate, V8Log::debug));
    proto_t->Set(isolate, "error", v8::FunctionTemplate::New(isolate, V8Log::error));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    return instance;
}
}