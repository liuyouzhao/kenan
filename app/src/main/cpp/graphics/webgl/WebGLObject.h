#ifndef WEBGLOBJECT_H
#define WEBGLOBJECT_H

#include "Types3D.h"
#include "WebGLGraphicsContext.h"

namespace DCanvas
{

class WebGLGraphicsContext;

class WebGLObject
{
public:
    WebGLGraphicsContext* context(){return m_context;}
    virtual ~WebGLObject()
    {
    }
    virtual bool isProgram() const {return false;}
    virtual bool isBuffer() const {return false;}
    virtual bool isFrameBuffer() const {return false;}
    virtual bool isRenderBuffer() const {return false;}
    virtual bool isShader() const {return false;}
    virtual bool isTexture() const {return false;}
    virtual void deleteObjectImpl(DCPlatformObject& o) = 0;

    void deleteObject()
    {
        m_deleted = true;
        deleteObjectImpl(m_object);
        m_object = NULL;
    }

    DCPlatformObject getObject() const {return m_object;}
    void  setObject(DCPlatformObject object){m_object = object;}

    bool isDeleted() { return m_deleted; }

    static void destroy() ;
    static bool HasInstance(v8::Handle<v8::Value> value);

protected:
    WebGLObject(WebGLGraphicsContext* context)
                : m_context(context)
                , m_object(0)
                , m_deleted(false)
    {
    }

    WebGLGraphicsContext*     m_context;
    DCPlatformObject          m_object;

    static bool s_parentot;
    static Persistent<FunctionTemplate> s_parentFunctionTemplate_;
    static v8::Handle<v8::Value> ObjectConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static Persistent<FunctionTemplate> createObject();

private:
    bool m_deleted;

};

} // namespace DCanvas

#endif // WEBGLOBJECT_H
