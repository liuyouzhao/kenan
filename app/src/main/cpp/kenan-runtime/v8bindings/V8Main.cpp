//
// Created by hujia on 21/06/19.
//
#include "defines.h"
#include "V8Main.h"
#include "V8Log.h"
#include "V8Kenan2d.h"
#include "V8Spirit.h"
#include "V8EventService.h"
#include "V8Script.h"
#include "V8Task.h"
#include "V8Graphics.h"
#include "Log.h"
#include <unistd.h>
#include <pthread.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Main"

using namespace kenan_system;

namespace kenan_v8bindings {
V8Main::V8Main() {
    pthread_mutex_init(&mutex, NULL);
}
V8Main::~V8Main() {
}

void Print(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}

void V8Main::setupJavascriptGlobalObjects(Isolate *isolate, Local<Context> context)
{
    context->Global()->Set(String::NewFromUtf8(isolate, "log"), V8Log::genSingleton(isolate));
    context->Global()->Set(String::NewFromUtf8(isolate, "orc2d"), V8Kenan2d::genSingleton(isolate));
    context->Global()->Set(String::NewFromUtf8(isolate, "kenan_api_task"), V8Task::genSingleton(isolate));
    context->Global()->Set(String::NewFromUtf8(isolate, "kenan_api_script"), V8Script::genSingleton(isolate, context));
    context->Global()->Set(String::NewFromUtf8(isolate, "kenan_api_graphics"), V8Graphics::genSingleton(isolate));
    codeState = NOT_READY;
}

v8::Local<v8::Context> V8Main::createJavascriptContext(Isolate *isolate)
{
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    return v8::Context::New(isolate, NULL, global);
}

void V8Main::initV8Environment()
{
    static bool globalInit = false;
    pthread_mutex_lock(&mutex);

    if(!globalInit) {
        v8::V8::InitializeICUDefaultLocation("./");
        v8::V8::InitializeExternalStartupData("./");
        platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
        globalInit = true;
    }

    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(create_params);
    pthread_mutex_unlock(&mutex);

    LOGD("initV8Environment OK");
}

int V8Main::firstRunJavascript(std::string javascript)
{
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    context = createJavascriptContext(isolate);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(context);

    setupJavascriptGlobalObjects(isolate, context);

    LOGD("setupJavascriptGlobalObjects [OK]");

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

    LOGD("create source code [OK]");
    v8::MaybeLocal<v8::Script> preScript = v8::Script::Compile(context, source);
    if(preScript.IsEmpty()) {
        LOGE("V8 compile the code file failed\n %s", javascript.c_str());
        exit(-1);
    }

    LOGD("Try to compile [OK]");
    v8::Local<v8::Script> script = preScript.ToLocalChecked();
    LOGD("firstRunJavascript compiled [OK]");

    if (script.IsEmpty())
    {
        __LOGE(__FUNCTION__, "first run script something was wrong while compiling! %s", javascript.c_str());
        return -1;
    }
    codeState = COMPILED;
    //v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    script->Run(context);
    LOGD("firstRunJavascript run [OK]");

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        //v8::Local<v8::StackTrace> stackTrace(message->GetStackTrace());
        __LOGE(__FUNCTION__, "Uncaught Exception:");
        __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
        __LOGE(__FUNCTION__, "Uncaught Exception:\n%s", (char*)*smessage);
        return -1;
    }
    persistentContext.Reset(isolate, context);
    codeState = RUN_FIRST;

    return 0;
}


void V8Main::runJavascript(std::string javascript)
{
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    if (script.IsEmpty())
    {
        __LOGE(__FUNCTION__, "single script something was wrong while compiling! %s", javascript.c_str());
        return;
    }
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        __LOGE(__FUNCTION__, "Uncaught Exception:\n%s", (char*)*smessage);
        __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
        return;
    }
}

int V8Main::onMessageCallback(unsigned long id, std::string title, std::string target, std::string data) {
    if(codeState != RUN_FIRST && codeState != RUNNING)
        return -1;

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::Value> functionValue = tmpContext->Global()->Get(v8::String::NewFromUtf8(isolate, "onMessage"));
    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (function->IsFunction()) {
        Local<Value> args[] = {
            Uint32::NewFromUnsigned(isolate, id),
            String::NewFromUtf8(isolate, title.c_str()),
            String::NewFromUtf8(isolate, target.c_str()),
            String::NewFromUtf8(isolate, data.c_str())
        };
        function->Call(tmpContext->Global(), 4, args);
    }

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        __LOGE(TAG_SYS, "Uncaught Exception:");
        __LOGE(TAG_SYS, "%s", (char*)*smessage);
        __LOGE(TAG_SYS, "line %d ", message->GetLineNumber());
        return -1;
    }
    codeState = RUNNING;
    return 0;
}

int V8Main::onFrameCallback()
{
    if(codeState != RUN_FIRST && codeState != RUNNING)
        return -1;

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::Value> functionValue = tmpContext->Global()->Get(v8::String::NewFromUtf8(isolate, "onFrame"));
    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (function->IsFunction()) {
        function->Call(tmpContext->Global(), 0, NULL);
    }

    V8EventService::instance()->callAllReadyHooks(isolate, tmpContext);

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        __LOGE(TAG_SYS, "Uncaught Exception:");
        __LOGE(TAG_SYS, "%s", (char*)*smessage);
        __LOGE(TAG_SYS, "line %d ", message->GetLineNumber());
        return -1;
    }
    codeState = RUNNING;
    return 0;
}

void V8Main::destroyV8Environment()
{
    persistentContext.Reset();
    isolate->Dispose();
}

}