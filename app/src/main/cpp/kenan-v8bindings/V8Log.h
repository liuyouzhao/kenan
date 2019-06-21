//
// Created by hujia on 22/06/19.
//

#ifndef KENAN_V8LOG_H
#define KENAN_V8LOG_H

#include <v8.h>

using namespace v8;

class V8Log {
public:
    static void info(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void debug(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void error(const v8::FunctionCallbackInfo<v8::Value>& args);

    static Handle<Object> genSingleton(Isolate* isolate);

private:
    static Persistent<ObjectTemplate> logTemplate;
};



#endif //KENAN_V8LOG_H
