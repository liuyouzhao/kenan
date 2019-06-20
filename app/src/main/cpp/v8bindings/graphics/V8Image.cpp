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

void V8Image::SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
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

v8::Handle<v8::Value> V8Image::GetSrc(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Image, info);

    return v8::String::New(class_impl->getSrc().c_str());
}

void V8Image::SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Image, info);

    if (value->IsFunction())
    {
        if (class_impl->m_onLoadSeted == true)
        {
            class_impl->m_js_OnLoad.Dispose();
            class_impl->m_js_OnLoad.Clear();
        }
        class_impl->m_js_OnLoad = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        LOGE("onload function error!");
        return;
    }
    LOGD("onload setted");
    class_impl->m_onLoadSeted = true;
}

v8::Handle<v8::Value>  V8Image::GetOnLoad(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8Image::SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
}

v8::Handle<v8::Value>  V8Image::GetWidth(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Image, info);

    return v8::Integer::New(class_impl->getWidth());
}

void V8Image::SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
}

v8::Handle<v8::Value>  V8Image::GetHeight(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Image, info);

    return v8::Integer::New(class_impl->getHeight());
}

// js T constructor function, called when `new T(...)' in js
v8::Handle<v8::Value> V8Image::ImageCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return ThrowException(v8::String::New("Cannot call constructor as function"));

    // throw if we didn't get 0 args
    if (args.Length() != 0)
        return ThrowException(v8::String::New("Expected no arguments"));

    // create the C++ Handle<T> to be wrapped
    Image * t = new Image();
    LOGE("begin new img");
    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    //Persistent<v8::Object> self = v8::Persistent<v8::Object>::New(s_objectTemplate->NewInstance());
    Persistent<v8::Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(V8Image::ImageDestroy));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(t));

    t->setSelf(self);

    LOGD("This is the Constructor of Image!");
    return self;
}
/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void V8Image::ImageDestroy(Persistent<Object> self, Image* parameter)
{
    //  v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    LOGD("image destroy");
    if (parameter)
    {
        //MemoryManager* mm = MemoryManager::getManager();
        //mm->DeleteImage(parameter);
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
    LOGD("image destroy end!");
}

v8::Handle<v8::Value> V8Image::getImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	 HandleScope scope;

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

v8::Persistent<v8::FunctionTemplate> V8Image::CreateImage()
{
    s_functionTemplate = v8::Persistent<v8::FunctionTemplate>::New(FunctionTemplate::New(V8Image::ImageCreate));
    s_functionTemplate->SetClassName(v8::String::New("Image"));
    s_classProto = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->PrototypeTemplate());

    s_objectTemplate = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->InstanceTemplate());
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate ->SetAccessor(v8::String::New("src"),    V8Image::GetSrc,    V8Image::SetSrc);
    s_objectTemplate ->SetAccessor(v8::String::New("width"),  V8Image::GetWidth,  V8Image::SetWidth);
    s_objectTemplate ->SetAccessor(v8::String::New("height"), V8Image::GetHeight, V8Image::SetHeight);
    s_objectTemplate ->SetAccessor(v8::String::New("onload"), V8Image::GetOnLoad, V8Image::SetOnLoad);
    s_objectTemplate ->Set(v8::String::New("getImageData"),FunctionTemplate::New(V8Image::getImageData));
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
