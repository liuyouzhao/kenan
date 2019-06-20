#ifndef WEBGLRENDERBUFFER_H
#define WEBGLRENDERBUFFER_H

#include "WebGLObject.h"
#include "ObjectWrap.h"
#include "WebGLGraphicsContext.h"
#include "Types3D.h"

namespace DCanvas
{

class WebGLRenderBuffer: public WebGLObject, public ObjectWrap
{
public:
    ~WebGLRenderBuffer();
    static WebGLRenderBuffer* create(WebGLGraphicsContext*);
    virtual void deleteObjectImpl(DCPlatformObject& o);

    virtual bool isRenderBuffer() const {return true;}

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);

    static bool HasInstance(v8::Handle<v8::Value> value);
    static void destroy();

private:

    WebGLRenderBuffer(WebGLGraphicsContext*);

    static v8::Persistent<v8::FunctionTemplate>                  s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                    s_class_proto;
    static v8::Persistent<v8::ObjectTemplate>                    s_class_ins;
    static bool s_bInited;
};

} // namespace DCanvas

#endif // WEBGLRENDERBUFFER_H
