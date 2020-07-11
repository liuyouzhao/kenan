//
// Created by hujia on 7/10/20.
//
#include "defines.h"
#include "V8Script.h"
#include "SALFileOperation.h"

#undef LOG_TAG
#define LOG_TAG    "V8Script"

using namespace kenan_sal;

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

static void doRunScript(Isolate *isolate, Local<Context> context, const char *javascript) {

    HandleScope handleScope(isolate);

    v8::TryCatch try_catch(isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, javascript, v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    if (script.IsEmpty()) {
        LOGE("first run script something was wrong while compiling! %s", javascript);
        __LOGE(__FUNCTION__, "first run script something was wrong while compiling! %s", javascript);
    }
    else {
        script->Run(context);
        if (try_catch.HasCaught()) {
            v8::String::Utf8Value exception(try_catch.Exception());
            v8::Local<v8::Message> message = try_catch.Message();
            v8::String::Utf8Value smessage(message->Get());
            //v8::Local<v8::StackTrace> stackTrace(message->GetStackTrace());
            __LOGE(__FUNCTION__, "Uncaught Exception:");
            __LOGE(__FUNCTION__, "line %d ", message->GetLineNumber());
            __LOGE(__FUNCTION__, "Uncaught Exception:\n%s", (char*)*smessage);
        }
    }
}

/************************** javascript call functions *****************************/
/*------------------------------ for functions -----------------------------------*/
void V8Script::Script_IncludeJavascript(const v8::FunctionCallbackInfo<v8::Value>& args) {

    if(args.Length() == 0) {
        LOGW("Script_IncludeJavascript file path NOT given.");
        return;
    }

    v8::Local<v8::String> jsString = args[0]->ToString();
    char file[PATH_MAX_LENGTH] = {0};
    jsString->WriteUtf8(file);

    char *script = (char*) malloc(TEXT_SCRIPT_BUFFER_LENGTH);
    int len = 0;
    memset(script, 0, TEXT_SCRIPT_BUFFER_LENGTH);

    /**
    By default read from RO location
    */
    if(SAL_systemReadFile(file, script, len, TEXT_SCRIPT_BUFFER_LENGTH, SAL_FOLDER_TYPE::RO_LOCATION) < 0 &&
        SAL_systemReadFile(file, script, len, TEXT_SCRIPT_BUFFER_LENGTH, SAL_FOLDER_TYPE::RW_LOCATION) < 0) {
        LOGW("%s script file %s NOT exist", __FUNCTION__, file);
    }
    else {
        LOGW("code is: %s\n", script);
        doRunScript(args.GetIsolate(), sContext, script);
    }
    free(script);

    LOGD("%s call over", __FUNCTION__);
    return;
}

void V8Script::Script_RunJavascript(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = args.GetIsolate();

    v8::Local<v8::String> jsString = args[0]->ToString();
    char *javascript = new char[jsString->Length()];
    jsString->WriteUtf8(javascript);

    doRunScript(isolate, sContext, javascript);

    delete[] javascript;
    return;
}

}