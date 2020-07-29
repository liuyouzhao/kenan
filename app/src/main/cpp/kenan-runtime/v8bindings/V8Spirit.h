#ifndef V8Spirit_H
#define V8Spirit_H

#include <v8.h>
#include "ObjectWrap.h"
#include "V8Base.h"
#include "sprite2d.h"


using namespace v8;

namespace kenan_v8bindings
{

#define CHECK_INTERNAL(s, a) if(!s){\
                            THROW_EXCEPTION(a.GetIsolate(), TError, "Cannot use a destroyed object"); \
                            return;}

class V8Spirit: public V8Base
{

public:
	V8Spirit();
    virtual ~V8Spirit();

    static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Render(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SetUV(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Resize(const v8::FunctionCallbackInfo<v8::Value>& args);


private:
    static bool templateReady;
};

} // namepace DCanvas

#endif  // V8Spirit_H
