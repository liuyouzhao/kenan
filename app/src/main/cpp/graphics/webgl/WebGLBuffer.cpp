#include "defines.h"
#include "WebGLBuffer.h"
#include "gl3d_impl.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLBuffer"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>    WebGLBuffer::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>      WebGLBuffer::s_class_proto;
v8::Persistent<v8::ObjectTemplate>      WebGLBuffer::s_class_ins;

bool WebGLBuffer::s_bInited = false;

WebGLBuffer::WebGLBuffer(WebGLGraphicsContext* context)
    : WebGLObject(context)
    , m_target(0)
    , m_byteLength(0)
    , m_elementArrayBuffer(NULL)
{
    glGenBuffers(1, &m_object);
}

WebGLBuffer* WebGLBuffer::create(WebGLGraphicsContext*context)
{
    LOGE("constructors of webglbuffer");
    return new WebGLBuffer(context);
}

WebGLBuffer::~WebGLBuffer()
{
    ///should delete m_object
    deleteObject();
}

bool WebGLBuffer::setTarget(DCenum target)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bind buffer with type error");
        return false;
    }

    m_target = target;
    return true;
}

DCenum WebGLBuffer::getTarget()
{
    return m_target;
}

void WebGLBuffer::deleteObjectImpl(DCPlatformObject& object)
{
    glDeleteBuffers(1, &object);
}

bool WebGLBuffer::associateBufferDataImpl(ArrayBuffer* array, DCintptr byteOffset, DCsizeiptr byteLength)
{
    if (byteLength < 0 || byteOffset < 0)
        return false;

    if (array && byteLength)
    {
        if (byteOffset + byteLength > static_cast<int32_t>(array->byteLength()))
        {
            return false;
        }
    }

    switch (m_target)
    {
        case GL_ELEMENT_ARRAY_BUFFER:
            m_byteLength = byteLength;

            if (byteLength)
            {
                m_elementArrayBuffer = ArrayBuffer::create(byteLength, 1);

                if (!m_elementArrayBuffer)
                {
                    m_byteLength = 0;
                    return false;
                }

                if (array)
                {
                    // We must always clone the incoming data because client-side
                    // modifications without calling bufferData or bufferSubData
                    // must never be able to change the validation results.
                    memcpy(static_cast<unsigned char*>(m_elementArrayBuffer->data()),
                           static_cast<unsigned char*>(array->data()) + byteOffset,
                           byteLength);
                }
            }
            else
            {
                m_elementArrayBuffer = 0;
            }
            return true;
        case GL_ARRAY_BUFFER:
            m_byteLength = byteLength;
            return true;
        default:
            return false;
    }
}

bool WebGLBuffer::associateBufferData(DCsizeiptr size)
{
    if (size < 0)
        return false;

    return associateBufferDataImpl(0, 0, size);
}

bool WebGLBuffer::associateBufferData(ArrayBuffer* array)
{
    if (!array)
        return false;

    return associateBufferDataImpl(array, 0, array->byteLength());
}

bool WebGLBuffer::associateBufferData(ArrayBufferView* array)
{
    if (!array)
        return false;

    return associateBufferDataImpl(array->buffer(), array->byteOffset(), array->byteLength());
}

bool WebGLBuffer::associateBufferSubDataImpl(DCintptr offset, ArrayBuffer* array, DCintptr arrayByteOffset, DCsizeiptr byteLength)
{
    if (!array || offset < 0 || arrayByteOffset < 0 || byteLength < 0)
        return false;

    if (byteLength)
    {
        if (arrayByteOffset + byteLength > static_cast<int32_t>(array->byteLength())
            || offset + byteLength > m_byteLength)
        {
            return false;
        }
    }

    switch (m_target)
    {
        case GL_ELEMENT_ARRAY_BUFFER:
            if (byteLength)
            {
                if (!m_elementArrayBuffer)
                    return false;

                memcpy(static_cast<unsigned char*>(m_elementArrayBuffer->data()) + offset,
                       static_cast<unsigned char*>(array->data()) + arrayByteOffset,
                       byteLength);
            }
            return true;
        case GL_ARRAY_BUFFER:
            return true;
        default:
            return false;
    }
}

bool WebGLBuffer::associateBufferSubData(DCintptr offset, ArrayBuffer* array)
{
    if (!array)
        return false;
    return associateBufferSubDataImpl(offset, array, 0, array->byteLength());
}

bool WebGLBuffer::associateBufferSubData(DCintptr offset, ArrayBufferView* array)
{
    if (!array)
        return false;
    return associateBufferSubDataImpl(offset, array->buffer(), array->byteOffset(), array->byteLength());
}

void WebGLBuffer::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

bool WebGLBuffer::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLBuffer::wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (s_bInited == false)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("WebGLBuffer"));
        s_class_proto = Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    v8::Local<v8::Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    class_obj->Set(v8::String::New("WebGLBuffer Object"),v8::String::New("WebGLBuffer Object"));

    return scope.Close(class_obj);
}

} // namespace DCanvas
