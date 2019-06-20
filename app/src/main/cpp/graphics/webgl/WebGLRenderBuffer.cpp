#include "defines.h"
#include "WebGLRenderBuffer.h"

#undef	 LOG_TAG
#define  LOG_TAG    "WebGLRenderBuffer"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        WebGLRenderBuffer::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          WebGLRenderBuffer::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          WebGLRenderBuffer::s_class_ins;

bool WebGLRenderBuffer::s_bInited = false;

WebGLRenderBuffer::WebGLRenderBuffer(WebGLGraphicsContext* context)
    : WebGLObject(context)
{
    glGenRenderbuffers(1, &m_object);
}

WebGLRenderBuffer* WebGLRenderBuffer::create(WebGLGraphicsContext* context)
{
    return new WebGLRenderBuffer(context);
}

WebGLRenderBuffer::~WebGLRenderBuffer()
{
    deleteObject();
}

void WebGLRenderBuffer::deleteObjectImpl(DCPlatformObject &o)
{
    glDeleteRenderbuffers(1, &o);
}

void WebGLRenderBuffer::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

bool WebGLRenderBuffer::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLRenderBuffer::wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("WebGLRenderBuffer"));
        s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    Handle<Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    class_obj->Set(v8::String::New("WebGLRenderBuffer Object"),v8::String::New("WebGLRenderBuffer Object"));
    s_class_func_templ->Inherit(WebGLObject::createObject());

    return scope.Close(class_obj);
}

} // namespace DCanvas
