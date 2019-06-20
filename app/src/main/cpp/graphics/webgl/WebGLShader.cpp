#include "defines.h"
#include "WebGLShader.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLShader"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        WebGLShader::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          WebGLShader::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          WebGLShader::s_class_ins;

bool WebGLShader::s_bInited = false;

WebGLShader* WebGLShader::create(WebGLGraphicsContext* context, DCenum type)
{
    return new WebGLShader(context, type);
}

WebGLShader::WebGLShader(WebGLGraphicsContext* context, DCenum type)
    : WebGLObject(context)
    , m_type(type)
    , m_source("")
{
    m_object = glCreateShader(type);
}

WebGLShader::~WebGLShader()
{
    deleteObject();
}

void WebGLShader::deleteObjectImpl(DCPlatformObject& obj)
{
    glDeleteShader(obj);
}

void WebGLShader::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }

    s_bInited = false;
}

bool WebGLShader::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLShader::wrap()
{
    HandleScope scope;

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
            //function
        s_class_func_templ->SetClassName(v8::String::New("WebGLShader"));
        s_class_proto = Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    Handle<Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    //class_obj->SetPrototype(v8::String::New("WebGLShader Object"));
    class_obj->Set(v8::String::New("WebGLShader Object"),v8::String::New("WebGLShader Object"));

    return scope.Close(class_obj);
}

}
