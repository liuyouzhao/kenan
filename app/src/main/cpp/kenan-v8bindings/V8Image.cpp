#include "V8Image.h"
#include "Image.h"
#include "defines.h"
#include "V8EventService.h"

#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Image"

using namespace kenan_graphics;
namespace kenan_v8bindings
{

v8::Persistent<v8::ObjectTemplate>              V8Image::s_classProto;
v8::Persistent<v8::ObjectTemplate>              V8Image::s_objectTemplate;
Persistent<FunctionTemplate>                    V8Image::s_functionTemplate;
Persistent<Template>                            V8Image::s_proto;

bool    V8Image::s_templateReady = false;

V8Image::V8Image()
{
}

V8Image::~V8Image()
{
}

void V8Image::SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<void>& info)
{
    HandleScope scope(info.GetIsolate());
    CASTTOCLASS(Image, info);
    //copy value to srcfile;
    v8::String::Utf8Value  utfValue(value);
    //char *srcfile = *(utfValue);
    std::string srcfile = *(utfValue);

    class_impl->setSrc(srcfile);
//    class_impl->m_src = new char[utfValue.length() +1];
//    strcpy(class_impl->m_src, srcfile);
//    class_impl->m_src[utfValue.length()] = 0;
//    class_impl->updata(utfValue.length());
}

void V8Image::GetSrc(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(info.GetIsolate());
    CASTTOCLASS(Image, info);

    info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), class_impl->getSrc().c_str()));
}

void onloadCallback(void *image)
{
    V8EventService::instance()->hookOnFinishResourceLoading(image);
}

void V8Image::SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<void>& info)
{
    HandleScope scope(info.GetIsolate());
    CASTTOCLASS(Image, info);

    if (value->IsFunction())
    {
    }
    else
    {
        LOGE("onload function error!");
        return;
    }
}

void  V8Image::GetOnLoad(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
}


void  V8Image::GetWidth(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(info.GetIsolate());
    CASTTOCLASS(Image, info);

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), class_impl->getWidth()));
}

void V8Image::SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info)
{
}

void  V8Image::GetHeight(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(info.GetIsolate());
    CASTTOCLASS(Image, info);

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), class_impl->getHeight()));
}

void V8Image::getImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope scope(args.GetIsolate());

	Image *img = ObjectWrap::Unwrap<Image>(args.GetIsolate(), args.This());


	unsigned int* intArr = img->getImageData();

	int w = img->getWidth();
	int h = img->getHeight();

	v8::Local<v8::Array> pixels = v8::Array::New(args.GetIsolate(), w * h);
	for (unsigned int i = 0; i < w * h; i++) {
	    pixels->Set(v8::Int32::New(args.GetIsolate(), i), v8::Uint32::New(args.GetIsolate(), intArr[i]));
	}
	args.GetReturnValue().Set(pixels);
}

v8::Handle<v8::Object> V8Image::Create(Isolate* isolate, const v8::FunctionCallbackInfo<v8::Value>& args)
{
    EscapableHandleScope handleScope(isolate);

    if(!s_templateReady)
    {
        s_functionTemplate.Reset(isolate, FunctionTemplate::New(isolate));
        s_classProto.Reset(isolate, s_functionTemplate.Get(isolate)->PrototypeTemplate());

        s_objectTemplate.Reset(isolate, s_functionTemplate.Get(isolate)->InstanceTemplate());
        s_objectTemplate.Get(isolate)->SetInternalFieldCount(1);
        s_objectTemplate.Get(isolate)->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),    V8Image::GetSrc,    V8Image::SetSrc);
        s_objectTemplate.Get(isolate)->SetAccessor(v8::String::NewFromUtf8(isolate, "width"),  V8Image::GetWidth);
        s_objectTemplate.Get(isolate)->SetAccessor(v8::String::NewFromUtf8(isolate, "height"), V8Image::GetHeight);
        //s_objectTemplate.Get(isolate)->SetAccessor(v8::String::NewFromUtf8(isolate, "onload"), V8Image::GetOnLoad, V8Image::SetOnLoad);

        s_proto.Reset(isolate, s_functionTemplate.Get(isolate)->PrototypeTemplate());
        s_proto.Get(isolate)->Set(isolate, "getImageData", FunctionTemplate::New(isolate, V8Image::getImageData));
    }


    Image *image = new Image();

    v8::Local<v8::Object> instance = s_objectTemplate.Get(isolate)->NewInstance();
    instance->SetInternalField(0, External::New(isolate, image));

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    V8EventService::instance()->bind(image, std::string(name));
    image->setOnLoadCallback(onloadCallback);
    image->setOnLoadFuncExist(true);
    return handleScope.Escape(instance);
}
} // namespace DCanvas
