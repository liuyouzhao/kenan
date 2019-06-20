#include "defines.h"
#include "V8ArrayBufferView.h"
#include "V8ArrayType.h"
#include "Uint16Array.h"
#include "Uint32Array.h"
#include "Uint8Array.h"
#include "Int32Array.h"
#include "Int16Array.h"
#include "Int8Array.h"
#include "Float32Array.h"

#undef   LOG_TAG
#define  LOG_TAG    "ArrayBufferView"

namespace DCanvas
{

bool V8ArrayBufferView::s_parentot = false;
v8::Persistent<v8::FunctionTemplate> V8ArrayBufferView::s_parentFunctionTemplate_;

v8::Handle<v8::Value> V8ArrayBufferView::ArrayConstructor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return args.This();
}

void V8ArrayBufferView::TemplateDestroy()
{
    if (s_parentot)
        s_parentFunctionTemplate_.Dispose();

    V8ArrayType<Uint16Array>::TemplateDestroy();
    V8ArrayType<Uint32Array>::TemplateDestroy();
    V8ArrayType<Uint8Array>::TemplateDestroy();
    V8ArrayType<Int32Array>::TemplateDestroy();
    V8ArrayType<Int16Array>::TemplateDestroy();
    V8ArrayType<Int8Array>::TemplateDestroy();
    V8ArrayType<Float32Array>::TemplateDestroy();

    s_parentot = false;
}

Handle<FunctionTemplate> V8ArrayBufferView::CreateArray()
{
    if (!s_parentot)
    {
        s_parentFunctionTemplate_ = v8::Persistent<FunctionTemplate>::New(
                    v8::FunctionTemplate::New(ArrayConstructor));
        s_parentot = true;
    }

    return s_parentFunctionTemplate_;
}

bool V8ArrayBufferView::HasInstance(v8::Handle<v8::Value> value)
{
    if (s_parentot == false)
        return false;

    return s_parentFunctionTemplate_->HasInstance(value);
}

} // namespace DCanvas
