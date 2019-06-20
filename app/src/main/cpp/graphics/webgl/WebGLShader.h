#ifndef WEBGLSHADER_H
#define WEBGLSHADER_H

#include "Types3D.h"
#include "v8.h"
#include "ObjectWrap.h"
#include "WebGLObject.h"
#include "WebGLGraphicsContext.h"

#include "WebGLGraphicsContext.h"
#include <gl3d_impl.h>

namespace DCanvas
{

class WebGLShader: public WebGLObject, ObjectWrap
{
public:
    virtual ~WebGLShader();
    static WebGLShader* create(WebGLGraphicsContext*, DCenum);
    virtual bool isShader() const {return true;}

    virtual void deleteObjectImpl(DCPlatformObject& o);

    DCenum getType() const { return m_type; }
    const std::string& getSource() const { return m_source; }

    void setSource(const std::string& source) { m_source = source; }

    static bool HasInstance(v8::Handle<v8::Value> value);
    v8::Handle<v8::Value> wrap();
    static void destroy();

private:
    WebGLShader(WebGLGraphicsContext*, DCenum);

    DCenum m_type;
    std::string m_source;
    static v8::Persistent<v8::FunctionTemplate>                     s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                       s_class_proto;
    static v8::Persistent<ObjectTemplate>                           s_class_ins;

    static bool s_bInited;
};

} // namespace DCanvas

#endif // WEBGLSHADER_H
