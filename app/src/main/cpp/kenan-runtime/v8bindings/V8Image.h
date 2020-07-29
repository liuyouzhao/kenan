//
// Created by hujia on 7/29/20.
//

#ifndef KENAN_V8IMAGE_H
#define KENAN_V8IMAGE_H

#include <v8.h>
#include "V8Base.h"

namespace kenan_v8bindings {

class V8Image: public V8Base {
public:
    static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetSrc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetSrc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif //KENAN_V8IMAGE_H
