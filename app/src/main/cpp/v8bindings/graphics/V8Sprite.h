#ifndef V8Sprite_H
#define V8Sprite_H

#include <v8.h>
#include "ObjectWrap.h"
#include "sprite2d.h"

using namespace v8;

namespace DCanvas
{

class V8Sprite : ObjectWrap
{

public:
	V8Sprite();
    ~V8Sprite();

    static v8::Handle<v8::Value> SpriteConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void SpriteDestructor(Persistent<Object> self, Gl2dSprite* parameter);

    static v8::Handle<v8::Value> Render(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetUV(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<FunctionTemplate> CreateV8SpriteTemplate();

    static bool HasInstance(v8::Handle<v8::Value>);

private:
    //function
    static v8::Persistent<v8::ObjectTemplate>               s_classProto;
    //property
    static v8::Persistent<v8::ObjectTemplate>               s_objectTemplate;
    static Persistent<FunctionTemplate>                     s_functionTemplate;
};

} // namepace DCanvas

#endif  // V8Sprite_H
