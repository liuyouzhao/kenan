#include "V8ArrayBuffer.h"
#include "ArrayBuffer.h"
#include "defines.h"

#include <android/log.h>
#define  LOG_TAG    "ArrayBuffer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        V8ArrayBuffer::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          V8ArrayBuffer::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          V8ArrayBuffer::s_class_ins;
bool V8ArrayBuffer::s_bInited = false;

void V8ArrayBuffer::TemplateDestroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }

    s_bInited = false;
}

bool V8ArrayBuffer::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

// js T constructor function, called when `new T(...)' in js
v8::Handle<v8::Value> V8ArrayBuffer::ArrayBufferConstructor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return THROW_EXCEPTION(TError, "ArrayBuffer : Cannot call constructor as function");

    // throw if we didn't get 0 args
    if (args.Length() != 1)
        return THROW_EXCEPTION(TError, "Expected no arguments");

    ArrayBuffer * t = NULL;
    if (!args[0]->IsInt32())
        return THROW_EXCEPTION(TError, "wrong type arguments");

    if (args.Length() == 1)
    {
        if(args[0]->IsInt32())
        {
            int len = args[0]->Int32Value();
            t = ArrayBuffer::create(len, 1);
        }
    }
    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(V8ArrayBuffer::ArrayBufferDestructor));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(t));
    self->SetIndexedPropertiesToExternalArrayData(t->data(),
                                               v8::kExternalByteArray,
                                               t->byteLength());
    return self;
}

Handle<FunctionTemplate> V8ArrayBuffer::CreateArray()
{

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New(V8ArrayBuffer::ArrayBufferConstructor));
        s_class_func_templ->SetClassName(v8::String::New("ArrayBuffer"));
        s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());

        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins->SetInternalFieldCount(1);
        s_class_ins->SetAccessor(v8::String::New("length"), V8ArrayBuffer::GetLength, V8ArrayBuffer::DoNothing);

        s_bInited = true;
    }

    return s_class_func_templ;
}

/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void V8ArrayBuffer::ArrayBufferDestructor(Persistent<Object> self, ArrayBuffer* parameter)
{
     //v8 will call destructor so no need to delete more
    if(parameter)
        delete parameter;

    LOGD("this is destructor of ArrayBuffer");
    self.ClearWeak();
    self.Dispose();
    self.Clear();
}

v8::Handle<v8::Value> V8ArrayBuffer::GetLength(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(ArrayBuffer, info);

    unsigned int length = class_impl->byteLength();
    return v8::Uint32::New(length);
}

void V8ArrayBuffer::DoNothing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{

}

} // namespace DCanvas
