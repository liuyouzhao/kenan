#include "defines.h"
#include "V8PixelArray.h"
#include "PixelArray.h"
#include <v8.h>

#undef   LOG_TAG
#define  LOG_TAG    "V8PixelArray"

namespace DCanvas
{

bool V8PixelArray::s_bInited  = false;

v8::Persistent<v8::FunctionTemplate>        V8PixelArray::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          V8PixelArray::s_class_proto;
v8::Persistent<ObjectTemplate>              V8PixelArray::s_class_inss;


V8PixelArray::V8PixelArray()
{
}

V8PixelArray::~V8PixelArray()
{
}

void V8PixelArray::SetLength(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
/*
HAS_FIXED_POINT
v8::ThrowException(v8::Exception::Error(v8::String::New("can not access")));
return
*/
    HandleScope scope;
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    v8::ThrowException(v8::Exception::Error(v8::String::New("can not access")));
    LOGD("color setted! the color is ");
    LOGD("%s", *(v8::String::AsciiValue(value)));
}

v8::Handle<v8::Value> V8PixelArray::GetLength(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    PixelArray* pa = static_cast<PixelArray*>(ptr);

    return v8::Integer::New(pa->length());
}

void V8PixelArray::TemplateDestroy()
{
    if (s_bInited)
    {
        s_class_inss.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

////////////js wrap
bool V8PixelArray::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> V8PixelArray::Wrap(PixelArray* ptr)
{
    HandleScope scope;
    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

        //function
        s_class_func_templ->SetClassName(v8::String::New("PixelArray"));
        s_class_proto = Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());

        s_class_inss = Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_inss -> SetInternalFieldCount(1);
        //varables
        s_class_inss ->SetAccessor(v8::String::New("length"), V8PixelArray::GetLength, V8PixelArray::SetLength);
        s_bInited = true;
    }

    if (!ptr->m_isset)
    {
    	ptr->m_js_self = Persistent<v8::Object>::New(s_class_inss->NewInstance());
    	ptr->m_js_self->SetInternalField(0, External::New(ptr));
    	ptr->m_js_self->SetIndexedPropertiesToPixelData(ptr->data()->data(), ptr->length());
    	ptr->m_isset = true;
    }

    return scope.Close(ptr->m_js_self);
}

} // namespace DCanvas
