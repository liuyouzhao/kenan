//
// Created by hujia on 21/06/19.
//
#include "V8Main.h"
#include "V8Log.h"
#include "Log.h"

using namespace kenan_system;

namespace kenan_v8bindings {

V8Main* V8Main::m_instance = NULL;

v8::Local<v8::Context> V8Main::setupJavascriptEnvironment(Isolate *isolate, Local<Context> context)
{
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    context->Global()->Set(String::NewFromUtf8(isolate, "log"), V8Log::genSingleton(isolate));

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
    context = Context::New(isolate);

    //v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(context);

    V8Main::instance()->setupJavascriptEnvironment(isolate, context);

    Log::info("--------------- %s", javascript.c_str());
    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    /*if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        Log::error(__FUNCTION__, "Uncaught Exception:");
        Log::error(__FUNCTION__, "line %d ", message->GetLineNumber());
    }*/

    persistentContext.Reset(isolate, context);
}


void V8Main::runJavascript(std::string javascript)
{
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);
    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
}

void V8Main::onFrameUpdateCallback()
{
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> tmpContext = v8::Local<v8::Context>::New(isolate, persistentContext);

    //v8::TryCatch try_catch(isolate);

    v8::Context::Scope context_scope(tmpContext);

    v8::Local<v8::Value> functionValue = tmpContext->Global()->Get(v8::String::NewFromUtf8(isolate, "update"));
    v8::Local<v8::Function> function = Local<Function>::Cast(functionValue);
    if (function->IsFunction()) {
        function->Call(tmpContext->Global(), 0, NULL);
    }


   /* if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        Log::error(__FUNCTION__, "Uncaught Exception:");
        Log::error(__FUNCTION__, "line %d ", message->GetLineNumber());
    }*/
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