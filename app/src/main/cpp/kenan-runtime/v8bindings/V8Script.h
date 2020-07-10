//
// Created by hujia on 7/10/20.
//

#ifndef KENAN_V8SCRIPT_H
#define KENAN_V8SCRIPT_H

#include <v8.h>

using namespace v8;

namespace kenan_v8bindings {

class V8Script {

public:
    static v8::Handle<v8::Value> genSingleton(v8::Isolate *isolate, Local<Context> context);

    static void Script_RunJavascript(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Script_IncludeJavascript(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
    static Local<Context> sContext;

};

}

#endif //KENAN_V8SCRIPT_H
