#include "V8Sprite.h"
#include "sprite2d.h"
#include "defines.h"

#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Sprite"

extern Gl2d_Impl* g_pImpl;
namespace DCanvas
{
v8::Persistent<v8::ObjectTemplate>              V8Sprite::s_classProto;
v8::Persistent<v8::ObjectTemplate>              V8Sprite::s_objectTemplate;
Persistent<FunctionTemplate>                    V8Sprite::s_functionTemplate;

V8Sprite::V8Sprite()
{
}

V8Sprite::~V8Sprite()
{
}

v8::Handle<v8::Value> V8Sprite::SpriteConstructor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return THROW_EXCEPTION(TError, "ArrayBuffer : Cannot call constructor as function");

    // throw if we didn't get 0 args
    if (args.Length() != 8 && args.Length() != 7)
        return THROW_EXCEPTION(TError, "SpriteConstructor Expected no arguments");

    Gl2dSprite * s = NULL;

    HTEXTURE texture = (int) args[0]->NumberValue();
    float texx1 = args[1]->NumberValue();
    float texy1 = args[2]->NumberValue();
    float texx2 = args[3]->NumberValue();
    float texy2 = args[4]->NumberValue();

    float w = args[5]->NumberValue();
    float h = args[6]->NumberValue();

    int blend = 1;
    if(args.Length() == 8) {
    	blend = (int) args[7]->NumberValue();
    }

    s = new Gl2dSprite(texture, texx1, texy1, texx2, texy2, w, h, g_pImpl, blend);

    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(s, WeakReferenceCallback(V8Sprite::SpriteDestructor));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(s));
    return self;
}

void V8Sprite::SpriteDestructor(Persistent<Object> self, Gl2dSprite* parameter)
{
     //v8 will call destructor so no need to delete more
    if(parameter)
        delete parameter;

    LOGD("this is destructor of V8Sprite");
    self.ClearWeak();
    self.Dispose();
    self.Clear();
}

v8::Handle<v8::Value> V8Sprite::Render(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() != 2) {
		 return THROW_EXCEPTION(TError, "Render wrong arguments: 2");
	}
	float x = args[0]->NumberValue();
	float y = args[1]->NumberValue();

	s->Render(x, y);
	return v8::Undefined();
}

v8::Handle<v8::Value> V8Sprite::RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() != 3 && args.Length() != 4 && args.Length() != 5) {
		 return THROW_EXCEPTION(TError, "Render wrong arguments: 3/4/5");
	}
	float x = args[0]->NumberValue();
	float y = args[1]->NumberValue();
	float rot = args[2]->NumberValue();
	float hscale = 1.0f;
	float vscale = 0.0f;

	if(args.Length() == 4) {
		hscale = args[3]->NumberValue();
	}
	else if(args.Length() == 5) {
		hscale = args[3]->NumberValue();
		vscale = args[4]->NumberValue();
	}

	s->RenderEx(x, y, rot, hscale, vscale);
	return v8::Undefined();
}

v8::Handle<v8::Value> V8Sprite::SetColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//void        SetColor(float r, float g, float b, float a, int i);
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() != 5) {
		 return THROW_EXCEPTION(TError, "SetColor wrong arguments: 5");
	}
	float r = args[0]->NumberValue();
	float g = args[1]->NumberValue();
	float b = args[2]->NumberValue();
	float a = args[3]->NumberValue();
	int i = (int) args[4]->NumberValue();

	s->SetColor(r, g, b, a, i);
	return v8::Undefined();
}

v8::Handle<v8::Value> V8Sprite::SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//void        SetHotSpot(int x, int y);
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() != 2) {
		 return THROW_EXCEPTION(TError, "Render wrong arguments: 2");
	}
	int x = (int) args[0]->NumberValue();
	int y = (int) args[1]->NumberValue();

	s->SetHotSpotInt(x, y);
	return v8::Undefined();
}

//void        SetBlendMode(int blend) { quad.blend = blend; }
v8::Handle<v8::Value> V8Sprite::SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//void        SetHotSpot(int x, int y);
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() < 1) {
		 return THROW_EXCEPTION(TError, "Render wrong arguments: 1");
	}
	int blend = (int) args[0]->NumberValue();

	s->SetBlendMode(blend);
	return v8::Undefined();
}

//void Gl2dSprite::SetTexture(HTEXTURE tex)
v8::Handle<v8::Value> V8Sprite::SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//void        SetHotSpot(int x, int y);
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() < 1) {
		 return THROW_EXCEPTION(TError, "SetTexture wrong arguments: 1");
	}
	HTEXTURE tex = (int) args[0]->NumberValue();

	s->SetTexture(tex);
	return v8::Undefined();
}

v8::Handle<v8::Value> V8Sprite::SetUV(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// SetUV(float tu1, float tv1, float tu2, float tv2);
	HandleScope scope;

	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());

	if(args.Length() != 4) {
		 return THROW_EXCEPTION(TError, "SetUV wrong arguments: 4");
	}

	float tu1 = args[0]->NumberValue();
	float tv1 = args[1]->NumberValue();
	float tu2 = args[2]->NumberValue();
	float tv2 = 1.0f;

	s->SetUV(tu1, tv1, tu2, tv2);
	return v8::Undefined();
}

v8::Persistent<v8::FunctionTemplate> V8Sprite::CreateV8SpriteTemplate()
{
	s_functionTemplate = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New(V8Sprite::SpriteConstructor));
	s_functionTemplate->SetClassName(v8::String::New("Orc2dSprite"));
	s_classProto = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->PrototypeTemplate());

	s_objectTemplate = v8::Persistent<ObjectTemplate>::New(s_functionTemplate->InstanceTemplate());
	s_objectTemplate->SetInternalFieldCount(1);

	s_objectTemplate ->Set(v8::String::New("Render"),FunctionTemplate::New(V8Sprite::Render));
	s_objectTemplate ->Set(v8::String::New("RenderEx"),FunctionTemplate::New(V8Sprite::RenderEx));
	s_objectTemplate ->Set(v8::String::New("SetColor"),FunctionTemplate::New(V8Sprite::SetColor));

	s_objectTemplate ->Set(v8::String::New("SetHotSpot"),FunctionTemplate::New(V8Sprite::SetHotSpot));
	s_objectTemplate ->Set(v8::String::New("SetBlendMode"),FunctionTemplate::New(V8Sprite::SetBlendMode));
	s_objectTemplate ->Set(v8::String::New("SetTexture"),FunctionTemplate::New(V8Sprite::SetTexture));
	s_objectTemplate ->Set(v8::String::New("SetUV"),FunctionTemplate::New(V8Sprite::SetUV));
    return s_functionTemplate;
}

bool V8Sprite::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("V8Sprite::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}

} // namespace DCanvas
