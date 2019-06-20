#ifndef WEBGLBUFFER_H
#define WEBGLBUFFER_H

#include "Types3D.h"
#include "v8.h"
#include "ObjectWrap.h"
#include "WebGLObject.h"
#include "WebGLGraphicsContext.h"

namespace DCanvas
{

class WebGLBuffer: public WebGLObject, ObjectWrap
{
public:
    ~WebGLBuffer();
    static WebGLBuffer* create(WebGLGraphicsContext*);
    virtual bool isBuffer() const {return true;}
    bool setTarget(DCenum target);
    DCenum getTarget();
    virtual void deleteObjectImpl(DCPlatformObject&);

    bool associateBufferData(DCsizeiptr size);
    bool associateBufferData(ArrayBuffer*);
    bool associateBufferData(ArrayBufferView*);
    bool associateBufferSubData(DCintptr offset, ArrayBuffer*);
    bool associateBufferSubData(DCintptr offset, ArrayBufferView*);

    DCsizeiptr byteLength() const { return m_byteLength; }

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);

    static bool HasInstance(v8::Handle<v8::Value> value);
    static void destroy();

private:
    WebGLBuffer(WebGLGraphicsContext*);
    DCenum      m_target;
    DCsizeiptr  m_byteLength;

    ArrayBuffer* m_elementArrayBuffer;

    bool associateBufferDataImpl(ArrayBuffer* array, DCintptr byteOffset, DCsizeiptr byteLength);
    bool associateBufferSubDataImpl(DCintptr offset, ArrayBuffer* array, DCintptr arrayByteOffset, DCsizeiptr byteLength);

    static bool s_bInited;
    static v8::Persistent<v8::FunctionTemplate>                 s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                   s_class_proto;
    static v8::Persistent<ObjectTemplate>                       s_class_ins;
};

} // namespace DCanvas

#endif // WEBGLBUFFER_H
