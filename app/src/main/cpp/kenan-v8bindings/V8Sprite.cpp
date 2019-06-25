#include "V8Sprite.h"
#include "sprite2d.h"
#include "defines.h"
#include "ObjectWrap.h"

#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Sprite"

namespace kenan_v8bindings
{

V8Sprite::V8Sprite()
{
}

V8Sprite::~V8Sprite()
{
}

void V8Sprite::Destroy(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
	CHECK_INTERNAL(s, args)
	delete s;
	s = NULL;
}

void V8Sprite::Render(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() != 2) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "Render wrong arguments: 2");
		 return;
	}
	float x = args[0]->NumberValue();
	float y = args[1]->NumberValue();

	s->Render(x, y);
}

void V8Sprite::RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() != 3 && args.Length() != 4 && args.Length() != 5) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "Render wrong arguments: 3/4/5");
		 return;
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
}

void V8Sprite::SetColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() != 5) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "SetColor wrong arguments: 5");
		 return;
	}
	float r = args[0]->NumberValue();
	float g = args[1]->NumberValue();
	float b = args[2]->NumberValue();
	float a = args[3]->NumberValue();
	int i = (int) args[4]->NumberValue();

	s->SetColor(r, g, b, a, i);
}

void V8Sprite::SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() != 2) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "Render wrong arguments: 2");
		 return;
	}
	int x = (int) args[0]->NumberValue();
	int y = (int) args[1]->NumberValue();
	s->SetHotSpotInt(x, y);
}

//void        SetBlendMode(int blend) { quad.blend = blend; }
void V8Sprite::SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() < 1) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "Render wrong arguments: 1");
		 return;
	}
	int blend = (int) args[0]->NumberValue();
	s->SetBlendMode(blend);
}

//void Gl2dSprite::SetTexture(HTEXTURE tex)
void V8Sprite::SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() < 1) {
		 THROW_EXCEPTION(args.GetIsolate(), TError, "SetTexture wrong arguments: 1");
		 return;
	}
	HTEXTURE tex = (int) args[0]->NumberValue();
	s->SetTexture(tex);
}

void V8Sprite::SetUV(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Gl2dSprite *s = ObjectWrap::Unwrap<Gl2dSprite>(args.This());
    CHECK_INTERNAL(s, args)
	if(args.Length() != 4) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "SetUV wrong arguments: 4");
        return;
	}

	float tu1 = args[0]->NumberValue();
	float tv1 = args[1]->NumberValue();
	float tu2 = args[2]->NumberValue();
	float tv2 = 1.0f;

	s->SetUV(tu1, tv1, tu2, tv2);
}

static v8::Persistent<v8::FunctionTemplate> functionTemplate_t;
static v8::Persistent<v8::Template> proto_t;
static v8::Persistent<v8::ObjectTemplate> instance_t;

bool V8Sprite::templateReady = false;
Handle<Object> V8Sprite::Create(Isolate *isolate, const v8::FunctionCallbackInfo<v8::Value>& args)
{
    EscapableHandleScope handleScope(isolate);

    if(templateReady == false)
    {
        functionTemplate_t.Reset(isolate, v8::FunctionTemplate::New(isolate));
        proto_t.Reset(isolate, functionTemplate_t.Get(isolate)->PrototypeTemplate());
        proto_t.Get(isolate)->Set(isolate, "Render", v8::FunctionTemplate::New(isolate, V8Sprite::Render));
        proto_t.Get(isolate)->Set(isolate, "RenderEx", v8::FunctionTemplate::New(isolate, V8Sprite::RenderEx));
        proto_t.Get(isolate)->Set(isolate, "SetColor", v8::FunctionTemplate::New(isolate, V8Sprite::SetColor));
        proto_t.Get(isolate)->Set(isolate, "SetHotSpot", v8::FunctionTemplate::New(isolate, V8Sprite::SetHotSpot));
        proto_t.Get(isolate)->Set(isolate, "SetBlendMode", v8::FunctionTemplate::New(isolate, V8Sprite::SetBlendMode));
        proto_t.Get(isolate)->Set(isolate, "SetTexture", v8::FunctionTemplate::New(isolate, V8Sprite::SetTexture));
        proto_t.Get(isolate)->Set(isolate, "SetUV", v8::FunctionTemplate::New(isolate, V8Sprite::SetUV));
        proto_t.Get(isolate)->Set(isolate, "Destroy", v8::FunctionTemplate::New(isolate, V8Sprite::Destroy));

        instance_t.Reset(isolate, functionTemplate_t.Get(isolate)->InstanceTemplate());
        instance_t.Get(isolate)->SetInternalFieldCount(1);

        templateReady = true;
    }

    if (args.Length() != 8 && args.Length() != 7)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "SpriteConstructor Expected no arguments");
        return Object::New(isolate);
    }

    Gl2dSprite * s = NULL;

    HTEXTURE texture = (int) args[0]->NumberValue();
    float texx1 = args[1]->NumberValue();
    float texy1 = args[2]->NumberValue();
    float texx2 = args[3]->NumberValue();
    float texy2 = args[4]->NumberValue();

    float w = args[5]->NumberValue();
    float h = args[6]->NumberValue();

    int blend = 1;
    if(args.Length() == 8)
    {
    	blend = (int) args[7]->NumberValue();
    }

    s = new Gl2dSprite(texture, texx1, texy1, texx2, texy2, w, h, Gl2d_Impl::instance(), blend);

    v8::Local<v8::Object> instance = instance_t.Get(isolate)->NewInstance();
    instance->SetInternalField(0, External::New(isolate, s));
    return handleScope.Escape(instance);
}

} // namespace DCanvas
