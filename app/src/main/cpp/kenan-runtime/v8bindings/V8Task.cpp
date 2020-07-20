//
// Created by hujia on 7/18/20.
//

#include "V8Task.h"
#include "SALTaskOperation.h"
#include "ObjectWrap.h"
#include "RuntimeApi.h"

using namespace kenan_sal;
using namespace kenan_runtime;
using namespace v8;

namespace kenan_v8bindings {

v8::Handle<v8::Value> V8Task::genSingleton(v8::Isolate *isolate) {
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();
    proto_t->Set(isolate, "start", v8::FunctionTemplate::New(isolate, V8Task::Task_Start));
    proto_t->Set(isolate, "stop", v8::FunctionTemplate::New(isolate, V8Task::Task_Stop));
    proto_t->Set(isolate, "has", v8::FunctionTemplate::New(isolate, V8Task::Task_Has));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    return instance;
}

void V8Task::Task_Start(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Task_Start : number of args exception! argn must:2 or 3");
        return;
    }

    char _id[TEXT_BUFFER_LENGTH] = {0};
    Local<String> id = args[0]->ToString();
    id->WriteUtf8(_id);
    std::string taskId = std::string(_id);

    bool rw = false;
    /// script run directly
    if(args.Length() >= 3 && args[2]->ToBoolean()->Value()) {
        rw = true;
        return;
    }

    char _file[PATH_MAX_LENGTH] = {0};
    Local<String> file = args[1]->ToString();
    file->WriteUtf8(_file);
    std::string sfile = std::string(_file);
    int ret = SAL_taskStart(taskId, sfile, rw);
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), ret));
    return;
}

void V8Task::Task_Stop(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Task_Start : number of args exception! argn must:1");
        return;
    }

    char _id[TEXT_BUFFER_LENGTH] = {0};
    Local<String> id = args[0]->ToString();
    id->WriteUtf8(_id);
    std::string taskId = std::string(_id);

    RuntimeApi::instance()->stopTask(taskId);
    return;
}

void V8Task::Task_Has(const v8::FunctionCallbackInfo<v8::Value>& args) {

}

}