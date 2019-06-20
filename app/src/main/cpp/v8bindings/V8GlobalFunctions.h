#ifndef V8GLOBAL_FUNCTIONS_H
#define V8GLOBAL_FUNCTIONS_H

#include <v8.h>

namespace DCanvas
{

v8::Handle<v8::Value> log(const v8::FunctionCallbackInfo<v8::Value>& args);
v8::Handle<v8::Value> writeToFile(const v8::FunctionCallbackInfo<v8::Value>& args);

} // namespace DCanvas

#endif // V8GLOBAL_FUNCTIONS_H
