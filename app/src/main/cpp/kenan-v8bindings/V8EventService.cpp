//
// Created by hujia on 26/06/19.
//

#include "V8EventService.h"
#include "ObjectWrap.h"

namespace kenan_v8bindings
{

V8EventService* V8EventService::s_self = NULL;

V8EventService::V8EventService()
{
    pthread_mutex_init(&m_mutex, NULL);
}

void V8EventService::hookOnFinishResourceLoading(void* ptr)
{
    pthread_mutex_lock(&m_mutex);
    std::string key = m_resourceListenerMap[ptr];
    m_hookOnFinishResourceMarks.push_back(key);
    pthread_mutex_unlock(&m_mutex);
}

void V8EventService::bind(void* ptr, std::string mark)
{
    pthread_mutex_lock(&m_mutex);
    m_resourceListenerMap[ptr] = mark;
    pthread_mutex_unlock(&m_mutex);
}

void V8EventService::unbind(void* ptr)
{
    pthread_mutex_lock(&m_mutex);
    m_resourceListenerMap.erase(ptr);
    pthread_mutex_unlock(&m_mutex);
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
    pthread_mutex_lock(&m_mutex);
    for(int i = 0; i < m_hookOnFinishResourceMarks.size(); i ++)
    {
        Local<Value> args[] = {String::NewFromUtf8(isolate, m_hookOnFinishResourceMarks[i].c_str())};
        function->Call(context->Global(), 1, args);
    }
    m_hookOnFinishResourceMarks.clear();
    pthread_mutex_unlock(&m_mutex);
}
}