#include "defines.h"
#include "WebGLFrameBuffer.h"
#include "Assert.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLFrameBuffer"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        WebGLFrameBuffer::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          WebGLFrameBuffer::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          WebGLFrameBuffer::s_class_ins;

bool WebGLFrameBuffer::s_bInited = false;

WebGLFrameBuffer::WebGLFrameBuffer(WebGLGraphicsContext* context)
    : WebGLObject(context)
{
    m_colorAttachment = NULL;
    m_depthAttachment = NULL;
    m_stencilAttachment = NULL;
    m_depthStencilAttachment = NULL;
    glGenFramebuffers(1, &m_object);
}

WebGLFrameBuffer* WebGLFrameBuffer::create(WebGLGraphicsContext* context)
{
    return new WebGLFrameBuffer(context);
}

WebGLFrameBuffer::~WebGLFrameBuffer()
{
    deleteObject();
}

void WebGLFrameBuffer::deleteObjectImpl(DCPlatformObject &o)
{
    glDeleteFramebuffers(1, &o);
}

void WebGLFrameBuffer::setAttachment(DCenum attachment, DCenum texTarget, WebGLTexture* texture, DCint level)
{
    if (!getObject())
        return;

    if (texture && !texture->getObject())
        texture = NULL;

    switch (attachment)
    {
        case GL_COLOR_ATTACHMENT0:
            m_colorAttachment = texture;
            if (texture)
            {
                m_texTarget = texTarget;
                m_texLevel = level;
            }
            break;
        case GL_DEPTH_ATTACHMENT:
            m_depthAttachment = texture;
            break;
        case GL_STENCIL_ATTACHMENT:
            m_stencilAttachment = texture;
            break;
        case GL_DEPTH_STENCIL_ATTACHMENT:
            m_depthStencilAttachment = texture;
            break;
        default:
            return;
    }
}

void WebGLFrameBuffer::setAttachment(DCenum attachment, WebGLRenderBuffer* renderbuffer)
{
    if (!getObject())
        return;

    if (renderbuffer && !renderbuffer->getObject())
        renderbuffer = NULL;

    switch (attachment)
    {
        case GL_COLOR_ATTACHMENT0:
            m_colorAttachment = renderbuffer;
            break;
        case GL_DEPTH_ATTACHMENT:
            m_depthAttachment = renderbuffer;
            break;
        case GL_STENCIL_ATTACHMENT:
            m_stencilAttachment = renderbuffer;
            break;
        case GL_DEPTH_STENCIL_ATTACHMENT:
            m_depthStencilAttachment = renderbuffer;
            break;
        default:
            return;
    }
}

WebGLObject* WebGLFrameBuffer::getAttachment(DCenum attachment) const
{
    if (!this->getObject())
        return 0;

    switch (attachment)
    {
        case GL_COLOR_ATTACHMENT0:
            return m_colorAttachment;
        case GL_DEPTH_ATTACHMENT:
            return m_depthAttachment;
        case GL_STENCIL_ATTACHMENT:
            return m_stencilAttachment;
        case GL_DEPTH_STENCIL_ATTACHMENT:
            return m_depthStencilAttachment;
        default:
            return 0;
    }
}


DCsizei WebGLFrameBuffer::getWidth() const
{
    //notImplemented
    return 0;
}

DCsizei WebGLFrameBuffer::getHeight() const
{
    //notImplemented
    return 0;
}

DCenum WebGLFrameBuffer::getColorBufferFormat() const
{
    //notImplemented
    return 0;
}

void WebGLFrameBuffer::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

bool WebGLFrameBuffer::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLFrameBuffer::wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("WebGLFrameBuffer"));
        s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    Handle<Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    class_obj->Set(v8::String::New("WebGLFrameBuffer Object"),v8::String::New("WebGLFrameBuffer Object"));

    s_class_func_templ->Inherit(WebGLObject::createObject());

    return scope.Close(class_obj);
}

} // namespace DCanvas
