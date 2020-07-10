//
// Created by hujia on 7/10/20.
//
#include "defines.h"
#include "V8Script.h"

#undef LOG_TAG
#define LOG_TAG    "V8Script"

namespace kenan_v8bindings
{

Local<Context> V8Script::sContext;

/**
* for GraphicsContext can be used by js
**/
v8::Handle<v8::Value> V8Script::genSingleton(Isolate *isolate, Local<Context> context)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    //t->Set(isolate, "func_property", v8::Number::New(isolate, 1));
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();

    //all class functions
    proto_t->Set(isolate, "run", FunctionTemplate::New(isolate, V8Script::Script_RunJavascript));
    proto_t->Set(isolate, "include", FunctionTemplate::New(isolate, V8Script::Script_IncludeJavascript));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();

    instance_t->SetInternalFieldCount(0);
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    sContext = context;

    return instance;
}

/************************** javascript call functions *****************************/
/*------------------------------ for functions -----------------------------------*/
void V8Script::Script_IncludeJavascript(const v8::FunctionCallbackInfo<v8::Value>& args) {


    ///TODO: implement this method
    LOGW("Script_IncludeJavascript NOT implemented");
    return;

/*
    Isolate *isolate = args.GetIsolate();

    HandleScope handleScope(args.GetIsolate());
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::String> jsString = args[0]->ToString();
    char filePath[PATH_MAX_LENGTH] = {0};
    jsString->WriteUtf8(filePath);


    char javascript[TEXT_BUFFER_LENGTH] = {0};

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript, v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(sContext, source).ToLocalChecked();
    if (script.IsEmpty()) {
        LOGE("first run script something was wrong while compiling! %s", javascript);
        __LOGE(__FUNCTION__, "first run script something was wrong while compiling! %s", javascript);
        return;
    }
    v8::Local<v8::Value> result = script->Run(sContext).ToLocalChecked();

    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        LOGE("Uncaught Exception:");
        LOGE("line %d ", message->GetLineNumber());
        __LOGE(__FUNCTION__, "Uncaught Exception:");
        __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
    }
    return;
    */
}

void V8Script::Script_RunJavascript(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = args.GetIsolate();

    HandleScope handleScope(args.GetIsolate());
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::String> jsString = args[0]->ToString();
    char *javascript = new char[jsString->Length()];
    jsString->WriteUtf8(javascript);
    delete[] javascript;

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript, v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(sContext, source).ToLocalChecked();
    if (script.IsEmpty()) {
        LOGE("first run script something was wrong while compiling! %s", javascript);
        __LOGE(__FUNCTION__, "first run script something was wrong while compiling! %s", javascript);
        return;
    }
    v8::Local<v8::Value> result = script->Run(sContext).ToLocalChecked();

    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        LOGE("Uncaught Exception:");
        LOGE("line %d ", message->GetLineNumber());
        __LOGE(__FUNCTION__, "Uncaught Exception:");
        __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
    }
    return;
}

}