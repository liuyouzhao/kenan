#ifndef V8PIXELARRAY_H
#define V8PIXELARRAY_H
#include <v8.h>
#include "ObjectWrap.h"

#include "ByteArray.h"

namespace DCanvas
{

class PixelArray;
class V8PixelArray : ObjectWrap
{
public:
    ~V8PixelArray();

    static void SetLength(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetLength(v8::Local<v8::String> property, const AccessorInfo& info);

    static void TemplateDestroy();
    static bool HasInstance(v8::Handle<v8::Value> value);

    static v8::Handle<v8::Value> Wrap(PixelArray* ptr);
private:
    V8PixelArray();

    static v8::Persistent<v8::FunctionTemplate>          s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>            s_class_proto;
    static v8::Persistent<ObjectTemplate>                s_class_inss;
    static bool s_bInited;
};

} // namespace DCanvas

#endif // V8PIXELARRAY_H
