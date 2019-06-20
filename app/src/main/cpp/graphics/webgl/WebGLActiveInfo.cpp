#include "WebGLActiveInfo.h"
#include "defines.h"

namespace DCanvas
{

v8::Handle<v8::Value> WebGLActiveInfo::GetName(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebGLActiveInfo, info);

    std::string name = class_impl->name();
    return v8::String::New(name.c_str());
}

v8::Handle<v8::Value> WebGLActiveInfo::GetType(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebGLActiveInfo, info);

    DCenum type = class_impl->type();
    return v8::Uint32::New(type);
}

v8::Handle<v8::Value> WebGLActiveInfo::GetSize(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebGLActiveInfo, info);

    DCint size = class_impl->size();
    return v8::Int32::New(size);
}


void WebGLActiveInfo::DoNothing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{

}

v8::Handle<v8::Value> WebGLActiveInfo::wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    //v8 object
    v8::Local<v8::ObjectTemplate>                    class_proto;
    v8::Local<v8::Object>                            class_obj;
    v8::Local<v8::FunctionTemplate>                  class_func_templ;

    class_func_templ = FunctionTemplate::New();

    //function
    class_func_templ->SetClassName(v8::String::New("WebGLActiveInfo"));
    class_proto = class_func_templ->PrototypeTemplate();

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins -> SetInternalFieldCount(1);
    //varables
    class_ins->SetAccessor(v8::String::New("name"), WebGLActiveInfo::GetName, WebGLActiveInfo::DoNothing);
    class_ins->SetAccessor(v8::String::New("type"), WebGLActiveInfo::GetType, WebGLActiveInfo::DoNothing);
    class_ins->SetAccessor(v8::String::New("size"), WebGLActiveInfo::GetSize, WebGLActiveInfo::DoNothing);

    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
//class_obj.MakeWeak(this, WeakReferenceCallback(ArrayBuffer::ArrayBufferDestructor));
    class_obj->Set(v8::String::New("WebGLActiveInfo Object"),v8::String::New("WebGLActiveInfo Object"));

    return scope.Close(class_obj);
}

} // namespace DCanvas
