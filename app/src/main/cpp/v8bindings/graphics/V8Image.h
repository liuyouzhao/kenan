#ifndef V8IMAGE_H
#define V8IMAGE_H

#include <v8.h>
#include "ObjectWrap.h"

using namespace v8;

namespace DCanvas
{

class V8Image : ObjectWrap
{

public:
	V8Image();
    ~V8Image();

    static v8::Handle<v8::Value> GetSrc(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetWidth(v8::Local<v8::String> property,const AccessorInfo& info);
    static v8::Handle<v8::Value> GetHeight(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static void SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    static v8::Handle<v8::Value> GetOnLoad(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    static v8::Handle<v8::Value> getImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    //    Handle<Value> Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ImageCreate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ImageDestroy(Persistent<Object> self, Image* parameter);
    static v8::Persistent<FunctionTemplate> CreateImage();

    static bool HasInstance(v8::Handle<v8::Value>);

private:
    //function
    static v8::Persistent<v8::ObjectTemplate>               s_classProto;
    //property
    static v8::Persistent<v8::ObjectTemplate>               s_objectTemplate;
    static Persistent<FunctionTemplate>                     s_functionTemplate;
};

} // namepace DCanvas

#endif  // V8IMAGE_H
