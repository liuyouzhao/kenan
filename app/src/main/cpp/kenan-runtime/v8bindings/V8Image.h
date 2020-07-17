#ifndef V8IMAGE_H
#define V8IMAGE_H

#include <v8.h>
#include "ObjectWrap.h"


using namespace v8;

namespace kenan_v8bindings
{

class V8Image
{

public:
	V8Image();
    ~V8Image();

    static void GetSrc(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info);
    static void SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<void>& info);
    static void GetWidth(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info);
    static void GetHeight(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info);
    static void SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info);

    static void GetOnLoad(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info);
    static void SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<void>& info);

    static void GetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void getImageData(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<Object> Create(Isolate* isolate, const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Destroy(Isolate* isolate, const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Local<v8::Context> context;


private:
    //function
    static v8::Persistent<v8::ObjectTemplate>               s_classProto;
    //property
    static v8::Persistent<v8::ObjectTemplate>               s_objectTemplate;
    static Persistent<FunctionTemplate>                     s_functionTemplate;
    static Persistent<Template>                             s_proto;

    static bool s_templateReady;
};

} // namepace kenan_v8bindings

#endif  // V8IMAGE_H
