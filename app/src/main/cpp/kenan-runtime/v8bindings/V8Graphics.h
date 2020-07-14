//
// Created by hujia on 7/14/20.
//

#ifndef KENAN_V8GRAPHICS_H
#define KENAN_V8GRAPHICS_H


#include <v8.h>
#include "gl2d_impl.h"

namespace kenan_v8bindings
{

class V8Graphics {
    V8Graphics();
    virtual ~V8Graphics();
    static v8::Handle<v8::Value> genSingleton(v8::Isolate *isolate);

    static void Graphics_Image_Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_Destroy(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_SetSrc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_GetSrc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_SetImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Graphics_Image_GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif //KENAN_V8GRAPHICS_H
