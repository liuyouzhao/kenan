#ifndef V8DIRECTCANVAS_H
#define V8DIRECTCANVAS_H

#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class V8DirectCanvas : ObjectWrap
{
public:

    static void Wrap(v8::Handle<v8::Object>, DirectCanvas* pImpl);
    
    static v8::Handle<v8::Value> GetOnFrame(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetOnFrame(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
   
    static v8::Handle<v8::Value> GetOnTouchStart(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetOnTouchStart(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    
    static v8::Handle<v8::Value> GetOnTouchEnd(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetOnTouchEnd(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    static v8::Handle<v8::Value> GetOnTouchMove(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetOnTouchMove(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    
    static v8::Handle<v8::Value> GetIsReInited(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetIsReInited(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> SetUsed(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    static v8::Handle<v8::Value> OnFrame(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> OnDestroy(const v8::FunctionCallbackInfo<v8::Value>&);
//    static bool HasInstance(v8::Handle<v8::Value>);

private:

    static v8::Persistent<v8::ObjectTemplate>              class_proto;
    static v8::Handle<v8::Object>                          class_obj;
    static v8::Persistent<v8::FunctionTemplate>            class_func_templ;// = FunctionTemplate::New();


}; 
}
#endif
