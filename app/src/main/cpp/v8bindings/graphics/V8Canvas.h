#ifndef V8CANVAS_H
#define V8CANVAS_H

#include <v8.h>
#include "ObjectWrap.h"
#include "Canvas.h"

using namespace v8;

namespace DCanvas
{

class V8Canvas: ObjectWrap
{
public:
	typedef Canvas ClassImpl;
	V8Canvas();
    ~V8Canvas();

    static v8::Handle<v8::Value>   GetContext(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value>   SetLayer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value>   Wrap(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value>   Wrap(void* ptrIn);

    static v8::Handle<v8::Integer>   s_Width;
    static v8::Handle<v8::Integer>   s_Height;

private:
};

}// namespace V8Canvas

#endif // V8CANVAS_H
