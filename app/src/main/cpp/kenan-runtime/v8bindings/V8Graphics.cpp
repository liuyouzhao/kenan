//
// Created by hujia on 7/14/20.
//
#include "defines.h"
#include "ObjectWrap.h"
#include "V8Graphics.h"
#include "V8EventService.h"
#include "V8Image.h"
#include "V8Spirit.h"
#include "RuntimeApi.h"
#undef LOG_TAG
#define  LOG_TAG    "V8Graphics"
using namespace v8;
using namespace kenan_runtime;

namespace kenan_v8bindings
{

v8::Handle<v8::Value> V8Graphics::genSingleton(v8::Isolate *isolate)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();

    proto_t->Set(isolate, "screen_get_width", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Screen_GetWidth));
    proto_t->Set(isolate, "screen_get_height", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Screen_GetHeight));

    proto_t->Set(isolate, "image_create", v8::FunctionTemplate::New(isolate, V8Image::Create));
    proto_t->Set(isolate, "image_destroy", v8::FunctionTemplate::New(isolate, V8Image::Destroy));
    proto_t->Set(isolate, "image_set_src", v8::FunctionTemplate::New(isolate, V8Image::SetSrc));
    proto_t->Set(isolate, "image_get_src", v8::FunctionTemplate::New(isolate, V8Image::GetSrc));
    proto_t->Set(isolate, "image_get_width", v8::FunctionTemplate::New(isolate, V8Image::GetWidth));
    proto_t->Set(isolate, "image_get_height", v8::FunctionTemplate::New(isolate, V8Image::GetHeight));
    proto_t->Set(isolate, "image_set_image_data", v8::FunctionTemplate::New(isolate, V8Image::SetImageData));
    proto_t->Set(isolate, "image_get_image_data", v8::FunctionTemplate::New(isolate, V8Image::GetImageData));
    proto_t->Set(isolate, "image_get_texture", v8::FunctionTemplate::New(isolate, V8Image::GetTexture));

    proto_t->Set(isolate, "spirit_create", v8::FunctionTemplate::New(isolate, V8Spirit::Create));
    proto_t->Set(isolate, "spirit_destroy", v8::FunctionTemplate::New(isolate, V8Spirit::Destroy));
    proto_t->Set(isolate, "spirit_render", v8::FunctionTemplate::New(isolate, V8Spirit::Render));
    proto_t->Set(isolate, "spirit_render_ex", v8::FunctionTemplate::New(isolate, V8Spirit::RenderEx));
    proto_t->Set(isolate, "spirit_set_color", v8::FunctionTemplate::New(isolate, V8Spirit::SetColor));
    proto_t->Set(isolate, "spirit_set_hot_spot", v8::FunctionTemplate::New(isolate, V8Spirit::SetHotSpot));
    proto_t->Set(isolate, "spirit_blend_mode", v8::FunctionTemplate::New(isolate, V8Spirit::SetBlendMode));
    proto_t->Set(isolate, "spirit_set_texture", v8::FunctionTemplate::New(isolate, V8Spirit::SetTexture));
    proto_t->Set(isolate, "spirit_set_uv", v8::FunctionTemplate::New(isolate, V8Spirit::SetUV));
    proto_t->Set(isolate, "spirit_resize", v8::FunctionTemplate::New(isolate, V8Spirit::Resize));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    LOGD("v8 graphics wrap end");
    return instance;
}

void V8Graphics::Graphics_Screen_GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), RuntimeApi::instance()->getOptions().getScreenWidth()));
}

void V8Graphics::Graphics_Screen_GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), RuntimeApi::instance()->getOptions().getScreenHeight()));
}

}