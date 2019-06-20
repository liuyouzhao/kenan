#include "defines.h"
#include "WebGLProgram.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLProgram"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        WebGLProgram::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          WebGLProgram::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          WebGLProgram::s_class_ins;
bool WebGLProgram::s_bInited = false;

WebGLProgram* WebGLProgram::create(WebGLGraphicsContext* ctx)
{
    return new WebGLProgram(ctx);
}

void WebGLProgram::deleteObjectImpl(DCPlatformObject& o)
{
    glDeleteProgram(o);
}

WebGLProgram::WebGLProgram(WebGLGraphicsContext* ctx)
    : WebGLObject(ctx)
    , m_vertexShader(0)
    , m_fragmentShader(0)
    , m_linkCount(0)
{
    m_object = glCreateProgram();
}

WebGLProgram::~WebGLProgram()
{
    deleteObject();
}

bool WebGLProgram::attachShader(WebGLShader* shader)
{
    if (!shader || !shader->getObject())
        return false;

    switch (shader->getType())
    {
        case GL_VERTEX_SHADER:
            if (m_vertexShader)
                return false;

            m_vertexShader = shader;
            m_linkCount++;
            return true;
        case GL_FRAGMENT_SHADER:
            if (m_fragmentShader)
                return false;

            m_fragmentShader = shader;
            m_linkCount++;
            return true;
        default:
            return false;
    }
}

bool WebGLProgram::detachShader(WebGLShader* shader)
{
    if (!shader || !shader->getObject())
        return false;

    switch (shader->getType())
    {
        case GL_VERTEX_SHADER:
            if (m_vertexShader != shader)
                return false;

            m_vertexShader = 0;
            m_linkCount--;
            return true;
        case GL_FRAGMENT_SHADER:
            if (m_fragmentShader != shader)
                return false;

            m_fragmentShader = 0;
            m_linkCount--;
            return true;
        default:
            return false;
    }
}

WebGLShader* WebGLProgram::getAttachedShader(DCenum type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER:
            return m_vertexShader;
        case GL_FRAGMENT_SHADER:
            return m_fragmentShader;
        default:
            return 0;
    }
}

bool WebGLProgram::cacheActiveAttribLocations()
{
    m_activeAttribLocations.clear();
    if (!getObject())
        return false;

    WebGLGraphicsContext* context3d = context();

    // Assume link status has already been cached.
    if (!m_linkStatus)
        return false;

    DCint numAttribs = 0;
    ::glGetProgramiv(getObject(), GL_ACTIVE_ATTRIBUTES, &numAttribs);
    m_activeAttribLocations.resize(static_cast<size_t>(numAttribs));

    for (int i = 0; i < numAttribs; ++i)
    {
        WebGLActiveInfo info = *context3d->getActiveAttrib(this, i);
        m_activeAttribLocations[i] = context3d->getAttribLocation(this, info.name());
    }

    return true;
}

unsigned WebGLProgram::numActiveAttribLocations() const
{
    return m_activeAttribLocations.size();
}

DCint WebGLProgram::getActiveAttribLocation(DCuint index) const
{
    if (index >= numActiveAttribLocations())
        return -1;

    return m_activeAttribLocations[index];
}

bool WebGLProgram::isUsingVertexAttrib0() const
{
    for (unsigned ii = 0; ii < numActiveAttribLocations(); ++ii)
    {
        if (!getActiveAttribLocation(ii))
            return true;
    }
    return false;
}

void WebGLProgram::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

bool WebGLProgram::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLProgram::wrap(const v8::FunctionCallbackInfo<v8::Value>&args)
{
    HandleScope scope;


    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("WebGLProgram"));
          s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    Handle<Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    class_obj->Set(v8::String::New("WebGLProgram Object"),v8::String::New("WebGLProgram Object"));

    s_class_func_templ->Inherit(WebGLObject::createObject());

    return scope.Close(class_obj);
}

} // namespace DCanvas
