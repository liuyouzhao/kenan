//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_V8THREAD_H
#define KENAN_V8THREAD_H

namespace kenan_v8bindings
{

class V8Thread {
public:
    V8Thread();
    virtual ~V8Thread();
    static v8::Handle<v8::Value> genSingleton(v8::Isolate *isolate);

    static void Thread_Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Thread_Pause(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Thread_Destroy(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Thread_Get_State(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif //KENAN_V8THREAD_H
