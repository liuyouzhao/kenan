#include "V8Spirit.h"
#include "sprite2d.h"
#include "defines.h"
#include "ObjectWrap.h"
#include "EntityManager.h"

#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "V8Spirit"

#ifdef MODULE_POOL_NAME
#undef MODULE_POOL_NAME
#endif
#define MODULE_POOL_NAME "graphics-spirit"

using namespace kenan_system;

namespace kenan_v8bindings {

V8Spirit::V8Spirit() {
}

V8Spirit::~V8Spirit() {
}

void V8Spirit::Render(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
	Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 3)
    CHECK_V8_ARGS3(args, String, Number, Number)

	float x = args[0]->NumberValue();
	float y = args[1]->NumberValue();

	s->Render(x, y);
}

void V8Spirit::RenderEx(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
	Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
	CHECK_V8_ARGNUM(args, 4)
    CHECK_V8_ARGS4(args, String, Number, Number, Number)

	float x = args[1]->NumberValue();
	float y = args[2]->NumberValue();
	float rot = args[3]->NumberValue();
	float hscale = 1.0f;
	float vscale = 0.0f;

	if(args.Length() == 5) {
	    CHECK_V8_ARGS5(args, String, Number, Number, Number, Number)
		hscale = args[4]->NumberValue();
	}
	else if(args.Length() == 6) {
	    CHECK_V8_ARGS6(args, String, Number, Number, Number, Number, Number)
		hscale = args[4]->NumberValue();
		vscale = args[5]->NumberValue();
	}

	s->RenderEx(x, y, rot, hscale, vscale);
}

void V8Spirit::SetColor(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
	Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 6)
    CHECK_V8_ARGS6(args, String, Number, Number, Number, Number, Number)

	float r = args[1]->NumberValue();
	float g = args[2]->NumberValue();
	float b = args[3]->NumberValue();
	float a = args[4]->NumberValue();
	int i = (int) args[5]->NumberValue();

	s->SetColor(r, g, b, a, i);
}

void V8Spirit::SetHotSpot(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

	std::string mark = getMarkFromArgument(args);
    Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 3)
    CHECK_V8_ARGS3(args, String, Number, Number)

	int x = (int) args[1]->NumberValue();
	int y = (int) args[2]->NumberValue();
	s->SetHotSpotInt(x, y);
}

//void        SetBlendMode(int blend) { quad.blend = blend; }
void V8Spirit::SetBlendMode(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
    Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
	CHECK_V8_ARGNUM(args, 2)
    CHECK_V8_ARGS2(args, String, Number)

	int blend = (int) args[1]->NumberValue();
	s->SetBlendMode(blend);
}

//void Gl2dSprite::SetTexture(HTEXTURE tex)
void V8Spirit::SetTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
    Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 2)
    CHECK_V8_ARGS2(args, String, Number)

	HTEXTURE tex = (int) args[1]->NumberValue();
	LOGD("s->SetTexture %d", tex);
	s->SetTexture(tex);
}

void V8Spirit::SetUV(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
    Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 4)
    CHECK_V8_ARGS4(args, String, Number, Number, Number)

	float tu1 = args[1]->NumberValue();
	float tv1 = args[2]->NumberValue();
	float tu2 = args[3]->NumberValue();
	float tv2 = 1.0f;

	s->SetUV(tu1, tv1, tu2, tv2);
}

void V8Spirit::Resize(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);
    Gl2dSprite *s = EntityManager::instance()->retrieve<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME);

    CHECK_INTERNAL(s, args)
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS2(args, String, Number)

    if(args.Length() == 2) {
        float w = args[1]->NumberValue();
        s->SetWidth(w);
        return;
    }
    CHECK_V8_ARGS3(args, String, Number, Number)
    float w = args[1]->NumberValue();
    float h = args[2]->NumberValue();

    s->SetWidth((int)w);
    s->SetHeight((int)h);
}

void V8Spirit::Create(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 8)
    CHECK_V8_ARGS8(args, String, Number, Number, Number, Number, Number, Number, Number)

    HandleScope handleScope(args.GetIsolate());

    std::string mark = getMarkFromArgument(args);

    Gl2dSprite * s = NULL;
    HTEXTURE texture = (int) args[1]->NumberValue();
    float texx1 = args[2]->NumberValue();
    float texy1 = args[3]->NumberValue();
    float texx2 = args[4]->NumberValue();
    float texy2 = args[5]->NumberValue();

    float w = args[6]->NumberValue();
    float h = args[7]->NumberValue();

    int blend = 1;
    if(args.Length() == 8) {
        blend = (int) args[7]->NumberValue();
    }

    s = new Gl2dSprite(texture, texx1, texy1, texx2, texy2, w, h, Gl2d_Impl::instance(), blend);
    EntityManager::instance()->store<Gl2dSprite>(mark.c_str(), MODULE_POOL_NAME, s);
    args.GetReturnValue().Set(args[0]->ToString());
}

void V8Spirit::Destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
    V8Base::Destroy(args, MODULE_POOL_NAME);
}

}
