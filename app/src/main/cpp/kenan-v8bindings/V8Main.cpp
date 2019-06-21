//
// Created by hujia on 21/06/19.
//
#include "V8Main.h"
#include "libplatform/libplatform.h"
#include "V8Log.h"
#include "Log.h"

using namespace kenan_system;

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
    v8::Context::Scope context_scope(context);

    V8Main::instance()->setupJavascriptEnvironment(isolate, context);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

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

void V8Main::destroyV8Environment()
{
    persistentContext.Reset();
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
}