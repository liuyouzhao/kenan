#ifndef WEBGLPROGRAM_H
#define WEBGLPROGRAM_H

#include "Types3D.h"
#include "ObjectWrap.h"
#include "WebGLObject.h"
#include "WebGLGraphicsContext.h"
#include "WebGLShader.h"
#include "WebGLActiveInfo.h"

#include <vector>
namespace DCanvas
{

class WebGLGraphicsContext;

class WebGLProgram:public WebGLObject, ObjectWrap
{
public:
    ~WebGLProgram();
    static WebGLProgram* create(WebGLGraphicsContext*);

    virtual bool isProgram() const { return true;}

    WebGLShader* getAttachedShader(DCenum);
    bool attachShader(WebGLShader*);
    bool detachShader(WebGLShader*);
    unsigned getLinkCount() const { return m_linkCount; }

    bool cacheActiveAttribLocations();
    unsigned numActiveAttribLocations() const;
    DCint getActiveAttribLocation(DCuint index) const;
    bool isUsingVertexAttrib0() const;

    bool getLinkStatus() const { return m_linkStatus; }
    void setLinkStatus(bool status) { m_linkStatus = status; }

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);
    virtual void deleteObjectImpl(DCPlatformObject&);

    static bool HasInstance(v8::Handle<v8::Value> value);

    static void destroy();

private:
    WebGLProgram(WebGLGraphicsContext*);

    WebGLShader* m_vertexShader;
    WebGLShader* m_fragmentShader;

    DCuint m_linkCount;
    DCint m_linkStatus;

    std::vector<DCint> m_activeAttribLocations;

    static v8::Persistent<v8::FunctionTemplate>                  s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                    s_class_proto;
    static v8::Persistent<v8::ObjectTemplate>                    s_class_ins;
    static bool s_bInited;
};

} // namespace DCanvas

#endif // WEBGLPROGRAM_H
