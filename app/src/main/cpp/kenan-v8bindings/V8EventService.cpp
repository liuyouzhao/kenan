//
// Created by hujia on 26/06/19.
//

#include "V8EventService.h"
#include "ObjectWrap.h"

namespace kenan_v8bindings
{

V8EventService* V8EventService::s_self = NULL;

void V8EventService::hookOnFinishResourceLoading(void* ptr)
{
    std::string key = m_resourceListenerMap[ptr];
    m_hookOnFinishResourceMarks.push_back(key);
}

void V8EventService::bind(void* ptr, std::string mark)
{
    m_resourceListenerMap[ptr] = mark;
}

#define LOG_TAG "V8EventService"
void V8EventService::callAllReadyHooks(Isolate* isolate, Local<Context> context)
{
    if(m_hookOnFinishResourceMarks.size() == 0)
        return;

    HandleScope handleScope(isolate);
    v8::Local<v8::Value> functionValue = context->Global()->Get(v8::String::NewFromUtf8(isolate, "onload"));
    if(functionValue->IsUndefined() || functionValue->IsNull())
    {
        THROW_EXCEPTION(isolate, TError, "onload is Undefined");
        return;
    }

    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (!function->IsFunction()) {
        THROW_EXCEPTION(isolate, TError, "onload is not a function");
        return;
    }

    for(int i = 0; i < m_hookOnFinishResourceMarks.size(); i ++)
    {
        Local<Value> args[] = {String::NewFromUtf8(isolate, m_hookOnFinishResourceMarks[i].c_str())};
        function->Call(context->Global(), 1, args);
    }
    m_hookOnFinishResourceMarks.clear();
}
}