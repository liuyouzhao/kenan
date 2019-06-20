#ifndef V8ARRAYBUFFER_H
#define V8ARRAYBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class ArrayBuffer;
class V8ArrayBuffer : public ObjectWrap
{
public:
//    static ArrayBuffer* create(unsigned int numElements, unsigned int elementByteSize);
//    static ArrayBuffer* create(ArrayBuffer*);
//    static ArrayBuffer* create(const void * data, unsigned int byteLength);

//    virtual ~ArrayBuffer();
//    void* data() { return m_data;}
//    unsigned int byteLength() const { return m_byteLength; }

    static Handle<FunctionTemplate> CreateArray();
    static v8::Handle<v8::Value> ArrayBufferConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ArrayBufferDestructor(Persistent<Object> self, ArrayBuffer* parameter);
    static bool HasInstance(v8::Handle<v8::Value> value);

    static void TemplateDestroy();
    static v8::Handle<v8::Value> GetLength(v8::Local<v8::String> property,const AccessorInfo& info);
    static void DoNothing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

private:
//    ArrayBuffer(void* data, unsigned int lenInByte);
//    ArrayBuffer(unsigned int numElements, unsigned int elementByteSize);
//
//    void *          m_data;
//    unsigned int    m_byteLength;

    static v8::Persistent<v8::FunctionTemplate>                 s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                   s_class_proto;
    static v8::Persistent<v8::ObjectTemplate>                   s_class_ins;
    static bool s_bInited;
};

} // namespace DCanvas

#endif // V8ARRAYBUFFER_H
