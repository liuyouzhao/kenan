#include "defines.h"
#include "WebGLObject.h"
#include "WebGLBuffer.h"
#include "WebGLTexture.h"
#include "WebGLRenderBuffer.h"
#include "WebGLFrameBuffer.h"
#include "WebGLShader.h"
#include "WebGLProgram.h"

#undef   LOG_TAG
#define  LOG_TAG    "WebGLObject"

namespace DCanvas
{

bool WebGLObject::s_parentot = false;
Persistent<FunctionTemplate> WebGLObject::s_parentFunctionTemplate_;

v8::Handle<v8::Value> WebGLObject::ObjectConstructor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return args.This();
}

Persistent<FunctionTemplate> WebGLObject::createObject()
{

    if (!s_parentot)
    {
        s_parentFunctionTemplate_ = v8::Persistent<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(ObjectConstructor));
        s_parentot = true;
    }

    return s_parentFunctionTemplate_;
}

void WebGLObject::destroy()
{
    if (s_parentot)
    {
        s_parentFunctionTemplate_.Dispose();
    }
    WebGLFrameBuffer::destroy();
    WebGLBuffer::destroy();
    WebGLTexture::destroy();
    WebGLRenderBuffer::destroy();
    WebGLShader::destroy();
    WebGLProgram::destroy();

    s_parentot = false;

}

bool WebGLObject::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_parentot)
        return false;

    return s_parentFunctionTemplate_->HasInstance(value);
}

} // namespace DCanvas
