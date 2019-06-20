#ifndef WEBGLFRAMEBUFFER_H
#define WEBGLFRAMEBUFFER_H

#include "WebGLObject.h"
#include "ObjectWrap.h"
#include "WebGLGraphicsContext.h"
#include "WebGLTexture.h"
#include "Types3D.h"
#include "WebGLRenderBuffer.h"

namespace DCanvas
{

class WebGLFrameBuffer: public WebGLObject, public ObjectWrap
{
public:
    ~WebGLFrameBuffer();
    static WebGLFrameBuffer* create(WebGLGraphicsContext*);
    virtual void deleteObjectImpl(DCPlatformObject& o);

    virtual bool isFrameBuffer() const {return true;}
    void setAttachment(DCenum attachment, DCenum texTarget, WebGLTexture* texture, DCint level);
    void setAttachment(DCenum attachment, WebGLRenderBuffer* renderbuffer);

    WebGLObject* getAttachment(DCenum attachment) const;

    DCenum getColorBufferFormat() const;
    DCsizei getWidth() const;
    DCsizei getHeight() const;

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);

    static bool HasInstance(v8::Handle<v8::Value> value);

    static void destroy();

private:

    WebGLFrameBuffer(WebGLGraphicsContext*);

    bool isColorAttached() const { return (m_colorAttachment && m_colorAttachment->getObject()); }
    bool isDepthAttached() const { return (m_depthAttachment && m_depthAttachment->getObject()); }
    bool isStencilAttached() const { return (m_stencilAttachment && m_stencilAttachment->getObject()); }
    bool isDepthStencilAttached() const { return (m_depthStencilAttachment && m_depthStencilAttachment->getObject()); }

    WebGLObject* m_colorAttachment;
    WebGLObject* m_depthAttachment;
    WebGLObject* m_stencilAttachment;
    WebGLObject* m_depthStencilAttachment;

    DCenum m_texTarget;
    DCint  m_texLevel;

    //static int m_nFuncTmpCount;
    static v8::Persistent<v8::FunctionTemplate>                 s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                   s_class_proto;
    static v8::Persistent<v8::ObjectTemplate>                   s_class_ins;
    static bool s_bInited;

};

} // namespace DCanvas

#endif // WEBGLFRAMEBUFFER_H
