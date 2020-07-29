//
// Created by hujia on 7/29/20.
//

#ifndef KENAN_V8BASE_H
#define KENAN_V8BASE_H

#include <v8.h>
#include <string>
#include "ObjectWrap.h"
#include "EntityManager.h"

using namespace kenan_system;

namespace kenan_v8bindings {

class V8Base : public ObjectWrap {
public:

    static bool checkArgumentLength(const v8::FunctionCallbackInfo<v8::Value>& args, int checkLength = 1);
    static std::string getMarkFromArgument(const v8::FunctionCallbackInfo<v8::Value>& args, int index = 0);

    static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& args, const char *modulePoolName);

    template<class T>
    static T* getPointerByName(const char* module, const char *name) {
        T *t = EntityManager::instance()->retrieve<T>(name, module);
        return t;
    }

    template<class T>
    static T* getPointerByArgs(const v8::FunctionCallbackInfo<v8::Value>& args, const char *module) {
        std::string name = getMarkFromArgument(args, 0);
        return getPointerByName<T>(module, name.c_str());
    }
};

}

#endif //KENAN_V8BASE_H
