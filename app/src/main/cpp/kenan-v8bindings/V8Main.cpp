//
// Created by hujia on 21/06/19.
//
#include "defines.h"
#include "V8Main.h"
#include "V8Log.h"
#include "V8Orc2d.h"
#include "V8Sprite.h"
#include "Log.h"

using namespace kenan_system;

namespace kenan_v8bindings {

V8Main* V8Main::m_instance = NULL;

void Print(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}



void V8Main::setupJavascriptGlobalObjects(Isolate *isolate, Local<Context> context)
{
    context->Global()->Set(String::NewFromUtf8(isolate, "log"), V8Log::genSingleton(isolate));
    context->Global()->Set(String::NewFromUtf8(isolate, "orc2d"), V8Orc2d::genSingleton(isolate));
    codeState = NOT_READY;
}

v8::Local<v8::Context> V8Main::createJavascriptContext(Isolate *isolate)
{
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    return v8::Context::New(isolate, NULL, global);
}

void V8Main::initV8Environment()
{
    v8::V8::InitializeICUDefaultLocation("./");
    v8::V8::InitializeExternalStartupData("./");
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(create_params);
}

void V8Main::firstRunJavascript(std::string javascript)
{
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    context = createJavascriptContext(isolate);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(context);

    setupJavascriptGlobalObjects(isolate, context);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    if (script.IsEmpty())
    {
        __LOGE(__FUNCTION__, "first run script something was wrong while compiling! %s", javascript.c_str());
        return;
    }
    codeState = COMPILED;
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        __LOGE(__FUNCTION__, "Uncaught Exception:");
        __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
        codeState = COMPILED;
        return;
    }
    persistentContext.Reset(isolate, context);
    codeState = RUN_FIRST;
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

void V8Main::onFrameUpdateCallback()
{
    if(codeState != RUN_FIRST && codeState != RUNNING)
        return;

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);

    v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::Value> functionValue = tmpContext->Global()->Get(v8::String::NewFromUtf8(isolate, "update"));
    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (function->IsFunction()) {
        function->Call(tmpContext->Global(), 0, NULL);
    }

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        __LOGE(TAG_SYS, "Uncaught Exception:");
        __LOGE(TAG_SYS, "%s", (char*)*smessage);
        __LOGE(TAG_SYS, "line %d ", message->GetLineNumber());
        return;
    }
    codeState = RUNNING;
}

void V8Main::destroyV8Environment()
{
    persistentContext.Reset();
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
}

}