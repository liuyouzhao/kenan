//
// Created by hujia on 7/18/20.
//
#include "defines.h"
#include "V8MainEngine.h"
#include "V8Main.h"
#include "V8Log.h"
#include "V8Kenan2d.h"
#include "V8Sprite.h"
#include "V8EventService.h"
#include "V8Script.h"
#include "V8Graphics.h"
#include "Log.h"
#include <unistd.h>

#undef LOG_TAG
#define  LOG_TAG    "V8MainEngine"

namespace kenan_v8bindings {

V8MainEngine *V8MainEngine::sInstance = NULL;

void V8MainEngine::onFrameUpdateCallback()
{
    if(codeState != RUN_FIRST && codeState != RUNNING)
        return;

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
        sleep(1);
        exit(-1);
    }
    codeState = RUNNING;
}

}