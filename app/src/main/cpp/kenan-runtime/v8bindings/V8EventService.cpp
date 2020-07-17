//
// Created by hujia on 26/06/19.
//

#include "V8EventService.h"
#include "ObjectWrap.h"

#undef LOG_TAG
#define LOG_TAG "V8EventService"

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
    if(m_resourceListenerMap.find(ptr) != m_resourceListenerMap.end()) {
        std::string key = m_resourceListenerMap[ptr];
        LOGD("hookOnFinishResourceLoading");
        m_hookOnFinishResourceMarks.push_back(V8Event(std::string(V8E_GRAPHICS_IMAGE_ONLOAD),
                                                      std::string(V8E_SUCCESS), key));
    }
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

void V8EventService::callAllReadyHooks(Isolate* isolate, Local<Context> context)
{
    if(m_hookOnFinishResourceMarks.size() == 0)
        return;

    HandleScope handleScope(isolate);
    v8::Local<v8::Value> functionValue = context->Global()->Get(v8::String::NewFromUtf8(isolate, "onEvent"));
    if(functionValue->IsUndefined() || functionValue->IsNull()) {
        THROW_EXCEPTION(isolate, TError, "onEvent is Undefined");
        return;
    }

    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (!function->IsFunction()) {
        THROW_EXCEPTION(isolate, TError, "onEvent is not a function");
        return;
    }
    pthread_mutex_lock(&m_mutex);
    for(int i = 0; i < m_hookOnFinishResourceMarks.size(); i ++) {
        V8Event event = m_hookOnFinishResourceMarks[i];
        Local<Value> args[] = {
            String::NewFromUtf8(isolate, event.title.c_str()),
            String::NewFromUtf8(isolate, event.arg0.c_str()),
            String::NewFromUtf8(isolate, event.arg1.c_str()),
            String::NewFromUtf8(isolate, event.arg2.c_str()),
            String::NewFromUtf8(isolate, event.arg3.c_str())
        };
        function->Call(context->Global(), 5, args);
    }
    m_hookOnFinishResourceMarks.clear();
    pthread_mutex_unlock(&m_mutex);
}
}