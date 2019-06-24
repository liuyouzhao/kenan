//
// Created by hujia on 22/06/19.
//

#ifndef KENAN_V8KENAN_H
#define KENAN_V8KENAN_H



class V8Kenan {
public:
    static void version(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void onJavascriptFunctionCallback(Handle<Value> args);
};

#endif //KENAN_V8KENAN_H
