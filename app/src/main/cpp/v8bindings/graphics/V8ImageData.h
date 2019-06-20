#ifndef V8IMAGEDATA_H
#define V8IMAGEDATA_H

#include <v8.h>
#include "ObjectWrap.h"
#include "PixelArray.h"
#include <gl2d_impl.h>

namespace DCanvas
{

class V8ImageData: ObjectWrap
{

public:
	V8ImageData();
    ~V8ImageData();

    static v8::Handle<v8::Value> Wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc);
    static v8::Handle<v8::Value> GetData(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetData(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetWidth(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetHeight(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static void ImageDataDestroy(Persistent<Object> self, ImageData* parameter);

    static void TemplateDestroy();
    static bool HasInstance(v8::Handle<v8::Value> value);

private:
    static bool s_bInited;
    static v8::Persistent<v8::FunctionTemplate>         s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>           s_class_proto;
    static v8::Persistent<ObjectTemplate>               s_class_ins;
};

} // namespace DCanvas

#endif // IMAGEDATA_H
