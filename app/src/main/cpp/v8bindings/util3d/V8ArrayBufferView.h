#ifndef V8ARRAYBUFFERVIEW_H
#define V8ARRAYBUFFERVIEW_H

#include "ArrayBuffer.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class V8ArrayBufferView
{
public:
    static bool HasInstance(v8::Handle<v8::Value> value);
    static void TemplateDestroy();

    static v8::Handle<v8::Value> ArrayConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static Handle<FunctionTemplate> CreateArray();

    static Persistent<FunctionTemplate> s_parentFunctionTemplate_;
    static bool s_parentot;
};

} //namespace DCanvas

#endif //V8ARRAYBUFFERVIEW_H
