//
// Created by hujia on 7/18/20.
//

#include "V8Thread.h"

namespace kenan_v8bindings {

v8::Handle<v8::Value> V8Thread::genSingleton(v8::Isolate *isolate) {
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();
    proto_t->Set(isolate, "create", v8::FunctionTemplate::New(isolate, V8Thread::Thread_Create));
    proto_t->Set(isolate, "pause", v8::FunctionTemplate::New(isolate, V8Thread::Thread_Pause));
    proto_t->Set(isolate, "destroy", v8::FunctionTemplate::New(isolate, V8Thread::Thread_Destroy));
    proto_t->Set(isolate, "get_state", v8::FunctionTemplate::New(isolate, V8Thread::Thread_Get_State));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    return instance;
}

void V8Thread::Thread_Create(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

void V8Thread::Thread_Pause(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

void V8Thread::Thread_Destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

void V8Thread::Thread_Get_State(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

}