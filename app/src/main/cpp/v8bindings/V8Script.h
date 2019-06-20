#ifndef V8SCRIPT_H
#define V8SCRIPT_H

#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class V8Script : ObjectWrap
{
public:
    static v8::Handle<v8::Value> GetSrc(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetOnLoad(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    //    Handle<Value> Create(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ScriptCreate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> RunScript(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void ScriptDestroy(Persistent<Object> self, Script* parameter);
    static Handle<FunctionTemplate> createScript();
    static bool HasInstance(v8::Handle<v8::Value>);
private:

    //function
    static v8::Handle<v8::ObjectTemplate>               s_classProto;
    //property
    static v8::Handle<v8::ObjectTemplate>               s_objectTemplate;
    static Handle<FunctionTemplate>                     s_functionTemplate;
};
}
#endif
