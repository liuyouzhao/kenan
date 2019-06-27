#ifndef V8Sprite_H
#define V8Sprite_H

#include <v8.h>
#include "ObjectWrap.h"
#include "sprite2d.h"

using namespace v8;

namespace kenan_v8bindings
{

#define CHECK_INTERNAL(s, a) if(!s){\
                            THROW_EXCEPTION(a.GetIsolate(), TError, "Cannot use a destroyed object"); \
                            return;}

class V8Sprite
{

public:
	V8Sprite();
    ~V8Sprite();
    static void Render(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetUV(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Resize(const v8::FunctionCallbackInfo<v8::Value>& args);

    static Handle<Object> Create(Isolate *isolate, const v8::FunctionCallbackInfo<v8::Value>& args);

    static bool HasInstance(v8::Handle<v8::Value>);

private:
    static bool templateReady;
};

} // namepace DCanvas

#endif  // V8Sprite_H
