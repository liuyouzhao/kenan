#ifndef V8RESOURCEMANAGER_H
#define V8RESOURCEMANAGER_H

#include <jni.h>
#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class V8ResourceManager : ObjectWrap
{
public:
    static v8::Handle<v8::Object> Wrap(JNIEnv* env);
    static bool HasInstance(v8::Handle<v8::Value>);

    /// load resource & release resource
    static v8::Handle<v8::Value> LoadImageResource(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> LoadSoundResource(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> ReleaseResource(const v8::FunctionCallbackInfo<v8::Value>&);

    /// images
    static v8::Handle<v8::Value> GetImageData(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> GetImageWidth(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> GetImageHeight(const v8::FunctionCallbackInfo<v8::Value>&);

    /// sounds
    static v8::Handle<v8::Value> PlaySound(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> StopSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> PauseSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> ResumeSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static v8::Handle<v8::Value> SetSoundVolume(const v8::FunctionCallbackInfo<v8::Value>&);

private:

    //function
    static v8::Handle<v8::ObjectTemplate>               s_classProto;
    //property
    static v8::Handle<v8::ObjectTemplate>               s_objectTemplate;
    static Handle<FunctionTemplate>                     s_functionTemplate;

    static JNIEnv*	s_JNIEnv;
};
}
#endif //V8RESOURCEMANAGER_H
