#include "V8Image.h"
#include "Image.h"
#include "defines.h"

#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Image"

namespace DCanvas
{

v8::Persistent<v8::ObjectTemplate>              V8Image::s_classProto;
v8::Persistent<v8::ObjectTemplate>              V8Image::s_objectTemplate;
Persistent<FunctionTemplate>                    V8Image::s_functionTemplate;

V8Image::V8Image()
{
}

V8Image::~V8Image()
{
}

void V8Image::SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
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

v8::Handle<v8::Value> V8Image::GetSrc(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
    CASTTOCLASS(Image, info);

    return v8::String::NewFromUtf8(info.GetIsolate(), class_impl->getSrc().c_str());
}

void V8Image::SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
    CASTTOCLASS(Image, info);

    if (value->IsFunction())
    {
    }
    else
    {
        LOGE("onload function error!");
        return;
    }
    LOGD("onload setted");
    class_impl->m_onLoadSeted = true;
}

v8::Handle<v8::Value>  V8Image::GetOnLoad(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
    return v8::Undefined(GetIsolate());
}

void V8Image::SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info)
{
}

v8::Handle<v8::Value>  V8Image::GetWidth(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
    CASTTOCLASS(Image, info);

    return v8::Integer::New(GetIsolate(), class_impl->getWidth());
}

void V8Image::SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo<Value>& info)
{
}

v8::Handle<v8::Value>  V8Image::GetHeight(v8::Local<v8::String> property, const PropertyCallbackInfo<Value>& info)
{
    HandleScope scope(GetIsolate());
    CASTTOCLASS(Image, info);

    return v8::Integer::New(GetIsolate(), class_impl->getHeight());
}

void V8Image::ImageCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{

    // throw if called without `new'
    if (!args.IsConstructCall())
        return GetIsolate()->ThrowException(v8::String::NewFromUtf8(GetIsolate(), "Cannot call constructor as function"));

    // throw if we didn't get 0 args
    if (args.Length() != 0)
        return GetIsolate()->ThrowException(v8::String::NewFromUtf8(GetIsolate(), "Expected no arguments"));

    // (just an example of a handy utility function)
    // whether or not it was called as "new Point()" or just "Point()"
    printf("Is constructor call: %s\n", args.IsConstructCall()?"yes":"no");

    // create your c++ object that will follow the javascript object around
    // make sure not to make it on the stack or it won't be around later when you need it
    Image * t = new Image();

    // another handy helper function example
    // see how the internal field count is what it was set to earlier
    //   in the InstanceTemplate
    printf("Internal field count: %d\n", args.This()->InternalFieldCount()); // this prints the value '1'

    // put the new Point object into the internal field
    args.This()->SetInternalField(0, External::New(args.GetIsolate(), t));

    // return the new object back to the javascript caller
    args.GetReturnValue().Set(args.This());
}

/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void V8Image::ImageDestroy(Persistent<Object> self, Image* parameter)
{
    //  v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    LOGD("image destroy");
    SAGE_DELETE(parameter);
    self.ClearWeak();
    self.Dispose();
    self.Clear();
    LOGD("image destroy end!");
}

v8::Handle<v8::Value> V8Image::getImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	 HandleScope scope(args.GetIsolate());

	 Image *img = ObjectWrap::Unwrap<Image>(args.This());

	 unsigned int* intArr = img->getImageData();
	 int w = img->getWidth();
	 int h = img->getHeight();
	 v8::Local<v8::Array> pixels = v8::Array::New(w * h);
	 for (unsigned int i = 0; i < w * h; i++) {
		 pixels->Set(v8::Int32::New(i), v8::Uint32::New(intArr[i]));
	 }

	 return pixels;
}

v8::Persistent<v8::FunctionTemplate> V8Image::CreateImage(Isolate* isolate)
{
    isolate_ = isolate;
    s_functionTemplate = v8::Persistent<v8::FunctionTemplate>::New(FunctionTemplate::New(V8Image::ImageCreate));
    s_functionTemplate->SetClassName(v8::String::NewFromUtf8(GetIsolate(), "Image"));
    s_classProto = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->PrototypeTemplate());

    s_objectTemplate = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->InstanceTemplate());
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate ->SetAccessor(v8::String::NewFromUtf8(GetIsolate(), "src"),    V8Image::GetSrc,    V8Image::SetSrc);
    s_objectTemplate ->SetAccessor(v8::String::NewFromUtf8(GetIsolate(), "width"),  V8Image::GetWidth,  V8Image::SetWidth);
    s_objectTemplate ->SetAccessor(v8::String::NewFromUtf8(GetIsolate(), "height"), V8Image::GetHeight, V8Image::SetHeight);
    s_objectTemplate ->SetAccessor(v8::String::NewFromUtf8(GetIsolate(), "onload"), V8Image::GetOnLoad, V8Image::SetOnLoad);
    s_objectTemplate ->Set(v8::String::NewFromUtf8(GetIsolate(), "getImageData"),FunctionTemplate::New(V8Image::getImageData));
    return s_functionTemplate;
}

bool V8Image::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("Image::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}

} // namespace DCanvas
