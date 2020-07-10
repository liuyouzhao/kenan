#ifndef V8RESOURCEMANAGER_H
#define V8RESOURCEMANAGER_H
#include <v8.h>
#include "ObjectWrap.h"

namespace kenan_v8bindings
{
class V8ResourceManager : ObjectWrap
{
public:
    static v8::Handle<v8::Value> genSingleton(Isolate *isolate);

    /// load resource & release resource
    static void LoadImageResource(const v8::FunctionCallbackInfo<v8::Value>&);
    static void LoadSoundResource(const v8::FunctionCallbackInfo<v8::Value>&);
    static void ReleaseResource(const v8::FunctionCallbackInfo<v8::Value>&);

    /// images
    static void GetImageData(const v8::FunctionCallbackInfo<v8::Value>&);
    static void GetImageWidth(const v8::FunctionCallbackInfo<v8::Value>&);
    static void GetImageHeight(const v8::FunctionCallbackInfo<v8::Value>&);

    /// sounds
    static void PlaySound(const v8::FunctionCallbackInfo<v8::Value>&);
    static void StopSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static void PauseSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static void ResumeSound(const v8::FunctionCallbackInfo<v8::Value>&);
    static void SetSoundVolume(const v8::FunctionCallbackInfo<v8::Value>&);

private:
};
}
#endif //V8RESOURCEMANAGER_H
