/********************************
 *
 *implementation for graphics
 *used to render on the screen
 *
 *
 * ********************************/
#include "defines.h"
#include "V8Orc2d.h"
#include "V8Log.h"
#include "V8Sprite.h"
#include "gl2d_impl.h"
#include "ObjectWrap.h"
#include "fs.h"

#undef LOG_TAG
#define LOG_TAG    "V8Orc2d"
#define MAX_TEXT_LENGTH 512

using namespace v8;

namespace kenan_v8bindings
{

V8Orc2d::V8Orc2d()
{
}

V8Orc2d::~V8Orc2d()
{
}

/**
* for GraphicsContext can be used by js
**/
v8::Handle<v8::Value> V8Orc2d::genSingleton(Isolate *isolate)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    t->Set(isolate, "func_property", v8::Number::New(isolate, 1));
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();

    //all class functions
    proto_t->Set(isolate, "System_Initiate", FunctionTemplate::New(isolate, V8Orc2d::System_Initiate));
    proto_t->Set(isolate, "System_Shutdown", FunctionTemplate::New(isolate, V8Orc2d::System_Shutdown));
    proto_t->Set(isolate, "System_ChangeSize", FunctionTemplate::New(isolate, V8Orc2d::System_ChangeSize));
    proto_t->Set(isolate, "System_GetErrorMessage", FunctionTemplate::New(isolate, V8Orc2d::System_GetErrorMessage));

    proto_t->Set(isolate, "Effect_Load", FunctionTemplate::New(isolate, V8Orc2d::Effect_Load));
    proto_t->Set(isolate, "Effect_Free", FunctionTemplate::New(isolate, V8Orc2d::Effect_Free));
    proto_t->Set(isolate, "Effect_Free_All", FunctionTemplate::New(isolate, V8Orc2d::Effect_Free_All));
    proto_t->Set(isolate, "Effect_Active", FunctionTemplate::New(isolate, V8Orc2d::Effect_Active));
    proto_t->Set(isolate, "Effect_SetFloat", FunctionTemplate::New(isolate, V8Orc2d::Effect_SetFloat));
    proto_t->Set(isolate, "Effect_SetFloatv2", FunctionTemplate::New(isolate, V8Orc2d::Effect_SetFloatv2));

    proto_t->Set(isolate, "Gfx_SetClearColor", FunctionTemplate::New(isolate, V8Orc2d::Gfx_SetClearColor));
    proto_t->Set(isolate, "Gfx_SetLineWidth", FunctionTemplate::New(isolate, V8Orc2d::Gfx_SetLineWidth));
    proto_t->Set(isolate, "Gfx_Clear", FunctionTemplate::New(isolate, V8Orc2d::Gfx_Clear));

    proto_t->Set(isolate, "Gfx_RenderLine", FunctionTemplate::New(isolate, V8Orc2d::Gfx_RenderLine));
    proto_t->Set(isolate, "Gfx_RenderTripleEx", FunctionTemplate::New(isolate, V8Orc2d::Gfx_RenderTripleEx));
    proto_t->Set(isolate, "Gfx_RenderQuadEx", FunctionTemplate::New(isolate, V8Orc2d::Gfx_RenderQuadEx));
    proto_t->Set(isolate, "Gfx_RenderRoundEx", FunctionTemplate::New(isolate, V8Orc2d::Gfx_RenderRoundEx));
    proto_t->Set(isolate, "Gfx_RenderTorusEx", FunctionTemplate::New(isolate, V8Orc2d::Gfx_RenderTorusEx));

    proto_t->Set(isolate, "Gfx_SwapBuffer", FunctionTemplate::New(isolate, V8Orc2d::Gfx_SwapBuffer));

    proto_t->Set(isolate, "Texture_Create", FunctionTemplate::New(isolate, V8Orc2d::Texture_Create));
    proto_t->Set(isolate, "Texture_Load", FunctionTemplate::New(isolate, V8Orc2d::Texture_Load));
    proto_t->Set(isolate, "Texture_Free", FunctionTemplate::New(isolate, V8Orc2d::Texture_Free));
    proto_t->Set(isolate, "Texture_SaveToFile", FunctionTemplate::New(isolate, V8Orc2d::Texture_SaveToFile));

    proto_t->Set(isolate, "Transform_rotate", FunctionTemplate::New(isolate, V8Orc2d::Transform_rotate));
    proto_t->Set(isolate, "Transform_translate", FunctionTemplate::New(isolate, V8Orc2d::Transform_translate));
    proto_t->Set(isolate, "Transform_scale", FunctionTemplate::New(isolate, V8Orc2d::Transform_scale));
    proto_t->Set(isolate, "Transform_scaleNonUniform", FunctionTemplate::New(isolate, V8Orc2d::Transform_scaleNonUniform));

    proto_t->Set(isolate, "Resource_CreateImage", FunctionTemplate::New(isolate, V8Orc2d::Resource_CreateImage));
    proto_t->Set(isolate, "Resource_DestroyImage", FunctionTemplate::New(isolate, V8Orc2d::Resource_DestroyImage));
    proto_t->Set(isolate, "Resource_CreateSound", FunctionTemplate::New(isolate, V8Orc2d::Resource_CreateSound));
    proto_t->Set(isolate, "Resource_DestroySound", FunctionTemplate::New(isolate, V8Orc2d::Resource_DestroySound));

    proto_t->Set(isolate, "Factory_CreateSpirit", FunctionTemplate::New(isolate, V8Orc2d::Factory_CreateSpirit));
    proto_t->Set(isolate, "Factory_DestroySpirit", FunctionTemplate::New(isolate, V8Orc2d::Factory_DestroySpirit));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();

    instance_t->SetInternalFieldCount(1);
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    instance->SetInternalField(0, External::New(isolate, Gl2d_Impl::instance()));
    LOGE("Orc2D wrap end");
    return instance;
}

/************************** javascript call functions *****************************/
/*------------------------------ for functions -----------------------------------*/

void V8Orc2d::System_Initiate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    if(args.Length() != 5)
    {
    	THROW_EXCEPTION(args.GetIsolate(), TError, "System_Initiate : number of args exception!");
    	return;
    }

    int x = 		(int) args[0]->NumberValue();
    int y = 		(int) args[1]->NumberValue();
    int width = 	(int) args[2]->NumberValue();
    int height = 	(int) args[3]->NumberValue();
    int layer = 	(int) args[4]->NumberValue();
	impl->System_Initiate(x, y, width, height, layer);

    return;
}

void V8Orc2d::System_Shutdown(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    impl->System_Shutdown();
}

void V8Orc2d::System_ChangeSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    //virtual void        CALL    System_ChangeSize(int x, int y, int width, int height);

    if(args.Length() != 4)
    {
    	THROW_EXCEPTION(args.GetIsolate(), TError, "System_ChangeSize : number of args exception!");
    	return;
    }
    int x = 		(int) args[0]->NumberValue();
    int y = 		(int) args[1]->NumberValue();
    int width = 	(int) args[2]->NumberValue();
    int height = 	(int) args[3]->NumberValue();
	impl->System_ChangeSize(x, y, width, height);
}

void V8Orc2d::System_GetErrorMessage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
	char* errMsg = impl->System_GetErrorMessage();
	args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), errMsg));
}


void V8Orc2d::Effect_Load(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    if(args.Length() != 1)
    {
    	THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_Load : number of args exception!");
    	return;
    }
    int eff = (int) args[0]->NumberValue();
    gl2EffectState effstate = (gl2EffectState) eff;
    HEFFECT hEff = impl->Effect_Load(effstate);

    args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(), hEff));
}

void V8Orc2d::Effect_Free(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_Free : number of args exception!");
        return;
    }
    int eff = (int) args[0]->NumberValue();
    HEFFECT hEff = (HEFFECT) eff;

    impl->Effect_Free(hEff);
}

void V8Orc2d::Effect_Free_All(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    impl->Effect_Free_All();
}

void V8Orc2d::Effect_Active(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_Active : number of args exception!");
        return;
    }
    HEFFECT eff = (int) args[0]->NumberValue();
    bool hasTex = (bool) args[1]->BooleanValue();

    impl->Effect_Active(eff, hasTex);
}

void V8Orc2d::Effect_SetFloat(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 3)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_SetFloat : number of args exception!");
        return;
    }
    HEFFECT eff = (int) args[0]->NumberValue();
    float value = args[1]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[2]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetFloat(eff, value, name);
}

void V8Orc2d::Effect_SetFloatv2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 4)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_SetFloatv2 : number of args exception!");
        return;
    }
    HEFFECT eff = (int) args[0]->NumberValue();
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[3]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetFloatv2(eff, x, y, name);
}

void V8Orc2d::Effect_SetInt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 3)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Effect_SetInt : number of args exception!");
        return;
    }
    HEFFECT eff = (int) args[0]->NumberValue();
    int value = (int) args[1]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[2]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetInt(eff, value, name);
}

void V8Orc2d::Gfx_SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 4)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_SetClearColor : number of args exception!");
        return;
    }
    float r, g, b, a;

    r = args[0]->NumberValue();
    g = args[1]->NumberValue();
    b = args[2]->NumberValue();
    a = args[3]->NumberValue();

    impl->Gfx_SetClearColor(r, g, b, a);
}

void V8Orc2d::Gfx_SetLineWidth(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_SetLineWidth : number of args exception!");
        return;
    }
    float width = args[0]->NumberValue();

    impl->Gfx_SetLineWidth(width);
}

void V8Orc2d::Gfx_Clear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    impl->Gfx_Clear();
}

void V8Orc2d::Gfx_RenderLine(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 9)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_RenderLine : number of args exception! argn must:9");
        return;
    }
    float x1, y1, x2, y2;
    float r, g, b, a, z=0.5;
    x1 = args[0]->NumberValue();y1 = args[1]->NumberValue();
    x2 = args[2]->NumberValue();y2 = args[3]->NumberValue();
    r = args[4]->NumberValue();
    g = args[5]->NumberValue();
    b = args[6]->NumberValue();
    a = args[7]->NumberValue();
    z = args[8]->NumberValue();

    impl->Gfx_RenderLine(x1, y1, x1, y2, r, g, b, a, z);
}

void V8Orc2d::Gfx_RenderTripleEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 12)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_RenderTripleEx : number of args exception! argn must:12");
        return;
    }
    float x1, y1, x2, y2, x3, y3;
    float r, g, b, a, z=0.5;
    int blend;
    HTEXTURE htex = NO_TEXTURE;//0x7FFFFFFF

    x1 = args[0]->NumberValue();y1 = args[1]->NumberValue();
    x2 = args[2]->NumberValue();y2 = args[3]->NumberValue();
    x3 = args[4]->NumberValue();y3 = args[5]->NumberValue();

    r = args[6]->NumberValue();
    g = args[7]->NumberValue();
    b = args[8]->NumberValue();
    a = args[9]->NumberValue();
    blend = (int) args[10]->NumberValue();
    htex = (int) args[11]->NumberValue();

    if(htex == 0) {
    	htex = NO_TEXTURE;
    }

    impl->Gfx_RenderTripleEx(x1, y1, x2, y2, x3, y3, r, g, b, a, blend, htex);
}

void V8Orc2d::Gfx_RenderQuadEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 14 && args.Length() != 16)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_RenderQuadEx : number of args exception! argn must:14/16");
        return;
    }
    float x1, y1, x2, y2, x3, y3, x4, y4;
    int blend;
    HTEXTURE htex = NO_TEXTURE;//0x7FFFFFFF
    x1 = args[0]->NumberValue();y1 = args[1]->NumberValue();
	x2 = args[2]->NumberValue();y2 = args[3]->NumberValue();
	x3 = args[4]->NumberValue();y3 = args[5]->NumberValue();
	x4 = args[6]->NumberValue();y4 = args[7]->NumberValue();
    if(args.Length() == 14) {
    	int r, g, b, a;
		r = (int)args[8]->NumberValue();
		g = (int)args[9]->NumberValue();
		b = (int)args[10]->NumberValue();
		a = (int)args[11]->NumberValue();
		blend = (int) args[12]->NumberValue();
		htex = (int) args[13]->NumberValue();
		if(htex == 0) {
			htex = NO_TEXTURE;
		}

		impl->Gfx_RenderQuadEx(x1, y1, x2, y2, x3, y3, x4, y4, r, g, b, a, blend, htex);
    }
    else if(args.Length() == 16) {
    	float tx[4], ty[4];
    	int r[4], g[4], b[4], a[4];

    	v8::Local<v8::Object> _arrTx = args[8]->ToObject();
    	v8::Local<v8::Object> _arrTy = args[9]->ToObject();
    	v8::Local<v8::Object> _arrR = args[10]->ToObject();
    	v8::Local<v8::Object> _arrG = args[11]->ToObject();
    	v8::Local<v8::Object> _arrB = args[12]->ToObject();
    	v8::Local<v8::Object> _arrA = args[13]->ToObject();
    	blend = (int) args[14]->NumberValue();
    	htex = (int) args[15]->NumberValue();

    	for(int i = 0; i < 4; i ++) {
    		tx[i] = _arrTx->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    		ty[i] = _arrTy->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    		r[i] = (int)_arrR->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    		g[i] = (int)_arrG->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    		b[i] = (int)_arrB->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    		a[i] = (int)_arrA->Get(v8::Int32::New(args.GetIsolate(), i))->NumberValue();
    	}
    	if(htex == 0) {
			htex = NO_TEXTURE;
		}
    	impl->Gfx_RenderQuadEx(x1, y1, x2, y2, x3, y3, x4, y4, tx, ty, r, g, b, a, blend, htex);
    }
}

void V8Orc2d::Gfx_RenderRoundEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    if(args.Length() != 8)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_RenderRoundEx : number of args exception! argn must:8");
        return;
    }
    float x0, y0, r0;
    int r, g, b, a;
    int blend;
    x0 = args[0]->NumberValue();
    y0 = args[1]->NumberValue();
    r0 = args[2]->NumberValue();

    r = (int)args[3]->NumberValue();
    g = (int)args[4]->NumberValue();
    b = (int)args[5]->NumberValue();
    a = (int)args[6]->NumberValue();
    blend = (int) args[7]->NumberValue();
    impl->Gfx_RenderRoundEx(x0, y0, r0, r, g, b, a, blend);
}

void V8Orc2d::Gfx_RenderTorusEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Gfx_RenderTorusEx : number of args exception! argn must:11");
        return;
    }
    float x0, y0, r0;
    float x1, y1, r1;
    int r[2], g[2], b[2], a[2];
    int blend;

    x0 = args[0]->NumberValue();
    y0 = args[1]->NumberValue();
    r0 = args[2]->NumberValue();
    x1 = args[3]->NumberValue();
    y1 = args[4]->NumberValue();
    r1 = args[5]->NumberValue();

    v8::Local<v8::Object> _arrR = args[6]->ToObject();
	v8::Local<v8::Object> _arrG = args[7]->ToObject();
	v8::Local<v8::Object> _arrB = args[8]->ToObject();
	v8::Local<v8::Object> _arrA = args[9]->ToObject();
	blend = (int) args[10]->NumberValue();

	for(int i = 0; i < 2; i ++) {
		r[i] = (int)_arrR->Get(v8::Integer::New(args.GetIsolate(), i))->NumberValue();
		g[i] = (int)_arrG->Get(v8::Integer::New(args.GetIsolate(), i))->NumberValue();
		b[i] = (int)_arrB->Get(v8::Integer::New(args.GetIsolate(), i))->NumberValue();
		a[i] = (int)_arrA->Get(v8::Integer::New(args.GetIsolate(), i))->NumberValue();
	}

    impl->Gfx_RenderRoundEx(x0, y0, r0, r[0], g[0], b[0], a[0], blend);

    return;
}

void V8Orc2d::Gfx_SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    impl->Gfx_SwapBuffer();
}

void V8Orc2d::Texture_Create(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Texture_Create : number of args exception! argn must:2");
        return;
    }
    int width, height;
    width = (int) args[0]->NumberValue();
    height = (int) args[1]->NumberValue();

    int hTex = impl->Texture_Create(width, height);
    args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(), hTex));
}

void V8Orc2d::Texture_Load(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 3)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Texture_Load : number of args exception! argn must:3");
        return;
    }
    int width, height;
    v8::Local<v8::Object> _pixelArr = args[0]->ToObject();
    width = (int) args[1]->NumberValue();
    height = (int) args[2]->NumberValue();

    unsigned int* data = new unsigned int[width*height];

    for(int i = 0; i < width*height; i ++) {
    	data[i] = (unsigned int)_pixelArr->Get(v8::Integer::New(args.GetIsolate(), i))->Uint32Value();
    	//LOGE("HUJIAJIA 0x%x", data[i]);
    }
    int hTex = impl->Texture_Load(data, width, height);
    args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(), hTex));
}

void V8Orc2d::Texture_Free(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Texture_Free : number of args exception! argn must:1");
        return;
    }
    HTEXTURE hTex = (int) args[0]->NumberValue();
    impl->Texture_Free(hTex);
    args.GetReturnValue().Set(v8::Int32::New(args.GetIsolate(), hTex));
}

void V8Orc2d::Texture_SaveToFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());
    impl->Texture_SaveToFile();
}

void V8Orc2d::Transform_rotate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Transform_rotate : number of args exception! argn must:1");
        return;
    }
    int d = (int) args[0]->NumberValue();
    impl->Transform_rotate(d);
}

void V8Orc2d::Transform_translate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Transform_translate : number of args exception! argn must:1");
        return;
    }
    float tx = args[0]->NumberValue();
    float ty = args[1]->NumberValue();

    impl->Transform_translate(tx, ty);
}

void V8Orc2d::Transform_scale(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Transform_scale : number of args exception! argn must:1");
        return;
    }
    float s = args[0]->NumberValue();

    impl->Transform_scale(s);
}

void V8Orc2d::Transform_scaleNonUniform(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.GetIsolate(), args.This());

    if(args.Length() != 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Transform_scaleNonUniform : number of args exception! argn must:2");
    }
    float sx = args[0]->NumberValue();
    float sy = args[1]->NumberValue();

    impl->Transform_scaleNonUniform(sx, sy);
}

void V8Orc2d::Resource_CreateImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}

void V8Orc2d::Resource_DestroyImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}

void V8Orc2d::Resource_CreateSound(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}

void V8Orc2d::Resource_DestroySound(const v8::FunctionCallbackInfo<v8::Value>& args)
{
}

void V8Orc2d::Factory_CreateSpirit(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    Local<Object> instance = V8Sprite::Create(args.GetIsolate(), args);
    args.GetReturnValue().Set(instance);
}

void V8Orc2d::Factory_DestroySpirit(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() == 0)
    {
        return;
    }

    Local<Object> argObject = args[0]->ToObject(args.GetIsolate());

    Gl2dSprite *ptr = ObjectWrap::Unwrap<Gl2dSprite>(args.GetIsolate(), argObject);
    if(ptr)
    {
        delete ptr;
        ptr = NULL;
    }
}

}
