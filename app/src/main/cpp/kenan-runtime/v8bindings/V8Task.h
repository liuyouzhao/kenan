//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_V8TASK_H
#define KENAN_V8TASK_H

#include <v8.h>

namespace kenan_v8bindings
{

class V8Task {
public:
    V8Task();
    virtual ~V8Task();
    static v8::Handle<v8::Value> genSingleton(v8::Isolate *isolate);

    static void Task_Start(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Task_Stop(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Task_Has(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif //KENAN_V8TASK_H
