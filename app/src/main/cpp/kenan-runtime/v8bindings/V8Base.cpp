//
// Created by hujia on 7/29/20.
//

#include "V8Base.h"

using namespace v8;
using namespace kenan_system;

namespace kenan_v8bindings {

bool V8Base::checkArgumentLength(const v8::FunctionCallbackInfo<v8::Value>& args, int checkLength) {
    char funcName[128] = {0};
    char errorMessage[128] = {0};

    if(args.Length() < checkLength) {
        Local<String> functionName = args.This()->ToString();
        functionName->WriteUtf8(funcName);
        sprintf(errorMessage, "%s number of args exception!", funcName);
        THROW_EXCEPTION(args.GetIsolate(), TError, errorMessage);
        return false;
    }
    return true;
}

std::string V8Base::getMarkFromArgument(const v8::FunctionCallbackInfo<v8::Value>& args, int index) {
    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[index]->ToString();
    mark->WriteUtf8(name);
    std::string rt = std::string(name);
    return rt;
}

void V8Base::Destroy(const v8::FunctionCallbackInfo<v8::Value>& args, const char *modulePoolName) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)

    HandleScope handleScope(args.GetIsolate());

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);
    EntityManager::instance()->eliminate(name, modulePoolName);
}

}