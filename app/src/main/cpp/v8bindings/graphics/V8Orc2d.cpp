/********************************
 *
 *implementation for graphics
 *used to render on the screen
 *
 *
 * ********************************/
#include "defines.h"
#include "V8Orc2d.h"
#include "gl2d_impl.h"
#include "fs.h"

#undef LOG_TAG
#define LOG_TAG    "V8Orc2d"
#define MAX_TEXT_LENGTH 512
namespace DCanvas
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
v8::Handle<v8::Value> V8Orc2d::Wrap(Gl2d_Impl* impl)
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>                    class_proto;
    v8::Handle<v8::Object>                            class_obj;
    v8::Handle<v8::FunctionTemplate>                  class_func_templ = FunctionTemplate::New();
    class_func_templ->SetClassName(v8::String::New("V8Orc2d"));
    class_proto = class_func_templ->PrototypeTemplate();

    //all class functions
    class_proto->Set(v8::String::New("System_Initiate"),FunctionTemplate::New(V8Orc2d::System_Initiate));
    class_proto->Set(v8::String::New("System_Shutdown"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("System_ChangeSize"),FunctionTemplate::New(V8Orc2d::System_ChangeSize));
    class_proto->Set(v8::String::New("System_GetErrorMessage"),FunctionTemplate::New(V8Orc2d::System_GetErrorMessage));

    class_proto->Set(v8::String::New("Effect_Load"),FunctionTemplate::New(V8Orc2d::Effect_Load));
    class_proto->Set(v8::String::New("Effect_Free"),FunctionTemplate::New(V8Orc2d::Effect_Free));
    class_proto->Set(v8::String::New("Effect_Free_All"),FunctionTemplate::New(V8Orc2d::Effect_Free_All));
    class_proto->Set(v8::String::New("Effect_Active"),FunctionTemplate::New(V8Orc2d::Effect_Active));
    class_proto->Set(v8::String::New("Effect_SetFloat"),FunctionTemplate::New(V8Orc2d::Effect_SetFloat));
    class_proto->Set(v8::String::New("Effect_SetFloatv2"),FunctionTemplate::New(V8Orc2d::Effect_SetFloatv2));

    class_proto->Set(v8::String::New("Gfx_SetClearColor"),FunctionTemplate::New(V8Orc2d::Gfx_SetClearColor));
    class_proto->Set(v8::String::New("Gfx_SetLineWidth"),FunctionTemplate::New(V8Orc2d::Gfx_SetLineWidth));
    class_proto->Set(v8::String::New("Gfx_Clear"),FunctionTemplate::New(V8Orc2d::Gfx_Clear));

    class_proto->Set(v8::String::New("Gfx_RenderLine"),FunctionTemplate::New(V8Orc2d::Gfx_RenderLine));
    //class_proto->Set(v8::String::New("Gfx_RenderTriple"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("Gfx_RenderTripleEx"),FunctionTemplate::New(V8Orc2d::Gfx_RenderTripleEx));
    class_proto->Set(v8::String::New("Gfx_RenderQuadEx"),FunctionTemplate::New(V8Orc2d::Gfx_RenderQuadEx));
    //class_proto->Set(v8::String::New("Gfx_RenderRectEx"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    //class_proto->Set(v8::String::New("Gfx_RenderRectExTexture"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("Gfx_RenderRoundEx"),FunctionTemplate::New(V8Orc2d::Gfx_RenderRoundEx));
    class_proto->Set(v8::String::New("Gfx_RenderTorusEx"),FunctionTemplate::New(V8Orc2d::Gfx_RenderTorusEx));

    //class_proto->Set(v8::String::New("Gfx_SetMatrix"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("Gfx_SwapBuffer"),FunctionTemplate::New(V8Orc2d::Gfx_SwapBuffer));

//    class_proto->Set(v8::String::New("Target_Create"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_Free"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_FreeAll"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_GetTexture"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_Active"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_ResetToScreen"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Target_ReadPixels"),FunctionTemplate::New(V8Orc2d::System_Shutdown));

    class_proto->Set(v8::String::New("Texture_Create"),FunctionTemplate::New(V8Orc2d::Texture_Create));
    class_proto->Set(v8::String::New("Texture_Load"),FunctionTemplate::New(V8Orc2d::Texture_Load));
    class_proto->Set(v8::String::New("Texture_Free"),FunctionTemplate::New(V8Orc2d::Texture_Free));
    class_proto->Set(v8::String::New("Texture_SaveToFile"),FunctionTemplate::New(V8Orc2d::Texture_SaveToFile));

    class_proto->Set(v8::String::New("Transform_rotate"),FunctionTemplate::New(V8Orc2d::Transform_rotate));
    //class_proto->Set(v8::String::New("Transform_rotate3d"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    //class_proto->Set(v8::String::New("Transform_rotateFromVector"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("Transform_translate"),FunctionTemplate::New(V8Orc2d::Transform_translate));
    //class_proto->Set(v8::String::New("Transform_translate3d"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    class_proto->Set(v8::String::New("Transform_scale"),FunctionTemplate::New(V8Orc2d::Transform_scale));
    class_proto->Set(v8::String::New("Transform_scaleNonUniform"),FunctionTemplate::New(V8Orc2d::Transform_scaleNonUniform));
    //class_proto->Set(v8::String::New("Transform_scale3d"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Transform_reset"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Transform_enable"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Transform_disable"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Transform_useMatrix2d"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
//    class_proto->Set(v8::String::New("Transform_getMatrix"),FunctionTemplate::New(V8Orc2d::System_Shutdown));
    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();

    class_ins-> SetInternalFieldCount(1);
    class_obj = class_ins->NewInstance();

    class_obj->SetInternalField(0, External::New(impl));
    LOGE("Orc2D wrap end");
    return handle_scope.Close(class_obj);
}


/************************** javascript call functions *****************************/
/*------------------------------ for functions -----------------------------------*/

v8::Handle<v8::Value> V8Orc2d::System_Initiate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
/*
 * virtual bool        CALL    System_Initiate(int x = 0, int y = 0,
                                                int width = 800, int height = 600,
                                                int layer = 0x40000000);
 * */
    if(args.Length() != 5)
    	return THROW_EXCEPTION(TError, "System_Initiate : number of args exception!");

    int x = 		(int) args[0]->NumberValue();
    int y = 		(int) args[1]->NumberValue();
    int width = 	(int) args[2]->NumberValue();
    int height = 	(int) args[3]->NumberValue();
    int layer = 	(int) args[4]->NumberValue();
	impl->System_Initiate(x, y, width, height, layer);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::System_Shutdown(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    impl->System_Shutdown();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::System_ChangeSize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    //virtual void        CALL    System_ChangeSize(int x, int y, int width, int height);

    if(args.Length() != 4)
    	return THROW_EXCEPTION(TError, "System_ChangeSize : number of args exception!");

    int x = 		(int) args[0]->NumberValue();
    int y = 		(int) args[1]->NumberValue();
    int width = 	(int) args[2]->NumberValue();
    int height = 	(int) args[3]->NumberValue();
	impl->System_ChangeSize(x, y, width, height);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::System_GetErrorMessage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

	char* errMsg = impl->System_GetErrorMessage();

    return v8::String::New(errMsg);
}


v8::Handle<v8::Value> V8Orc2d::Effect_Load(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual HEFFECT     CALL    Effect_Load(gl2EffectState shader);
    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    if(args.Length() != 1)
    	return THROW_EXCEPTION(TError, "Effect_Load : number of args exception!");

    int eff = (int) args[0]->NumberValue();
    gl2EffectState effstate = (gl2EffectState) eff;
    HEFFECT hEff = impl->Effect_Load(effstate);

    return v8::Int32::New(hEff);
}

v8::Handle<v8::Value> V8Orc2d::Effect_Free(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Effect_Free(HEFFECT eff);
    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 1)
        	return THROW_EXCEPTION(TError, "Effect_Free : number of args exception!");

    int eff = (int) args[0]->NumberValue();
    HEFFECT hEff = (HEFFECT) eff;

    impl->Effect_Free(hEff);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Effect_Free_All(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Effect_Free_All();
    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    impl->Effect_Free_All();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Effect_Active(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void       CALL    Effect_Active(HEFFECT eff, bool hasTex = false);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 2)
           	return THROW_EXCEPTION(TError, "Effect_Active : number of args exception!");

    HEFFECT eff = (int) args[0]->NumberValue();
    bool hasTex = (bool) args[1]->BooleanValue();

    impl->Effect_Active(eff, hasTex);

    return v8::Undefined();
}
/*
 *
    virtual void        CALL    Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                                 float x3, float y3, float x4, float y4,
                                                 float tx[4], float ty[4],
                                                 int r[4], int g[4], int b[4], int a[4],
                                                 int blend, HTEXTURE htex);
    virtual void        CALL    Gfx_RenderRectEx(float left, float top, float right, float bottom,
                                                 int r, int g, int b, int a, int blend,
                                                 HTEXTURE htex = NO_TEXTURE );
 * */
v8::Handle<v8::Value> V8Orc2d::Effect_SetFloat(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Effect_SetFloat(HEFFECT eff, float value, const char* name);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 3)
           	return THROW_EXCEPTION(TError, "Effect_SetFloat : number of args exception!");

    HEFFECT eff = (int) args[0]->NumberValue();
    float value = args[1]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[2]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetFloat(eff, value, name);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Effect_SetFloatv2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Effect_SetFloatv2(HEFFECT eff, float x, float y, const char* name);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 4)
           	return THROW_EXCEPTION(TError, "Effect_SetFloatv2 : number of args exception!");

    HEFFECT eff = (int) args[0]->NumberValue();
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[3]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetFloatv2(eff, x, y, name);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Effect_SetInt(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Effect_SetInt(HEFFECT eff, int value, const char* name);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 3)
           	return THROW_EXCEPTION(TError, "Effect_SetInt : number of args exception!");

    HEFFECT eff = (int) args[0]->NumberValue();
    int value = (int) args[1]->NumberValue();

    char name[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[2]->ToString();
    jstr_tmp->WriteUtf8(name);

    impl->Effect_SetInt(eff, value, name);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Gfx_SetClearColor(float r, float g, float b, float a);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 4)
           	return THROW_EXCEPTION(TError, "Gfx_SetClearColor : number of args exception!");

    float r, g, b, a;

    r = args[0]->NumberValue();
    g = args[1]->NumberValue();
    b = args[2]->NumberValue();
    a = args[3]->NumberValue();

    impl->Gfx_SetClearColor(r, g, b, a);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_SetLineWidth(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Gfx_SetLineWidth(float width);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() < 1)
           	return THROW_EXCEPTION(TError, "Gfx_SetLineWidth : number of args exception!");

    float width = args[0]->NumberValue();

    impl->Gfx_SetLineWidth(width);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_Clear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Gfx_Clear();
    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    impl->Gfx_Clear();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_RenderLine(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// virtual void        CALL    Gfx_RenderLine(float x1, float y1, float x2, float y2,
    //    float r, float g, float b, float a, float z=0.5f);

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 9)
           	return THROW_EXCEPTION(TError, "Gfx_RenderLine : number of args exception! argn must:9");

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

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_RenderTripleEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	/*
	 * virtual void        CALL    Gfx_RenderTripleEx(float x1, float y1, float x2, float y2,
                                                  float x3, float y3, int r, int g, int b, int a,
                                                  int blend, HTEXTURE htex = NO_TEXTURE );
	 * */

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 12)
           	return THROW_EXCEPTION(TError, "Gfx_RenderTripleEx : number of args exception! argn must:12");

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

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_RenderQuadEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	/*
	 * virtual void        CALL    Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                               float x3, float y3, float x4, float y4,
                                               int r, int g, int b, int a, int blend,
                                               HTEXTURE htex = NO_TEXTURE);
      virtual void        CALL    Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                                 float x3, float y3, float x4, float y4,
                                                 float tx[4], float ty[4],
                                                 int r[4], int g[4], int b[4], int a[4],
                                                 int blend, HTEXTURE htex);
	 * */

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 14 && args.Length() != 16)
           	return THROW_EXCEPTION(TError, "Gfx_RenderQuadEx : number of args exception! argn must:14/16");

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
    		tx[i] = _arrTx->Get(v8::Integer::New(i))->NumberValue();
    		ty[i] = _arrTy->Get(v8::Integer::New(i))->NumberValue();
    		r[i] = (int)_arrR->Get(v8::Integer::New(i))->NumberValue();
    		g[i] = (int)_arrG->Get(v8::Integer::New(i))->NumberValue();
    		b[i] = (int)_arrB->Get(v8::Integer::New(i))->NumberValue();
    		a[i] = (int)_arrA->Get(v8::Integer::New(i))->NumberValue();
    	}
    	if(htex == 0) {
			htex = NO_TEXTURE;
		}
    	impl->Gfx_RenderQuadEx(x1, y1, x2, y2, x3, y3, x4, y4, tx, ty, r, g, b, a, blend, htex);
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_RenderRoundEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	/*
	 * virtual void        CALL    Gfx_RenderRoundEx(float x0, float y0, float r0,
                                                int r, int g, int b, int a, int blend);
	 * */
    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    if(args.Length() != 8)
           	return THROW_EXCEPTION(TError, "Gfx_RenderRoundEx : number of args exception! argn must:8");

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
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_RenderTorusEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	/*
	 *   virtual void        CALL    Gfx_RenderTorusEx(float x0, float y0, float r0,
                                                float x1, float y1, float r1,
                                                int r[2], int g[2], int b[2], int a[2], int blend);
	 * */

    HandleScope scope;
    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 1)
           	return THROW_EXCEPTION(TError, "Gfx_RenderTorusEx : number of args exception! argn must:11");

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
		r[i] = (int)_arrR->Get(v8::Integer::New(i))->NumberValue();
		g[i] = (int)_arrG->Get(v8::Integer::New(i))->NumberValue();
		b[i] = (int)_arrB->Get(v8::Integer::New(i))->NumberValue();
		a[i] = (int)_arrA->Get(v8::Integer::New(i))->NumberValue();
	}

    impl->Gfx_RenderRoundEx(x0, y0, r0, r[0], g[0], b[0], a[0], blend);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Gfx_SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());
    impl->Gfx_SwapBuffer();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Texture_Create(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual HTEXTURE    CALL    Texture_Create(int width, int height);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 2)
           	return THROW_EXCEPTION(TError, "Texture_Create : number of args exception! argn must:2");

    int width, height;
    width = (int) args[0]->NumberValue();
    height = (int) args[1]->NumberValue();

    int hTex = impl->Texture_Create(width, height);
    return v8::Int32::New(hTex);
}

v8::Handle<v8::Value> V8Orc2d::Texture_Load(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual HTEXTURE    CALL    Texture_Load(const unsigned int *data, int width, int height);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 3)
           	return THROW_EXCEPTION(TError, "Texture_Load : number of args exception! argn must:3");

    int width, height;
    v8::Local<v8::Object> _pixelArr = args[0]->ToObject();
    width = (int) args[1]->NumberValue();
    height = (int) args[2]->NumberValue();

    unsigned int* data = new unsigned int[width*height];

    for(int i = 0; i < width*height; i ++) {
    	data[i] = (unsigned int)_pixelArr->Get(v8::Integer::New(i))->Uint32Value();
    	//LOGE("HUJIAJIA 0x%x", data[i]);
    }

    int hTex = impl->Texture_Load(data, width, height);
    return v8::Int32::New(hTex);
}

v8::Handle<v8::Value> V8Orc2d::Texture_Free(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Texture_Free(HTEXTURE tex);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() < 1)
           	return THROW_EXCEPTION(TError, "Texture_Free : number of args exception! argn must:1");

    HTEXTURE hTex = (int) args[0]->NumberValue();

    impl->Texture_Free(hTex);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Texture_SaveToFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Texture_SaveToFile();
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    impl->Texture_SaveToFile();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Transform_rotate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Transform_rotate(float d);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() < 1)
               	return THROW_EXCEPTION(TError, "Transform_rotate : number of args exception! argn must:1");

    int d = (int) args[0]->NumberValue();

    impl->Transform_rotate(d);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Transform_translate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Transform_translate(float tx, float ty);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 2)
               	return THROW_EXCEPTION(TError, "Transform_translate : number of args exception! argn must:1");

    float tx = args[0]->NumberValue();
    float ty = args[1]->NumberValue();

    impl->Transform_translate(tx, ty);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Transform_scale(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Transform_scale(float s)
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 1)
               	return THROW_EXCEPTION(TError, "Transform_scale : number of args exception! argn must:1");

    float s = args[0]->NumberValue();

    impl->Transform_scale(s);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Orc2d::Transform_scaleNonUniform(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	//virtual void        CALL    Transform_scaleNonUniform(float sx, float sy);
    HandleScope scope;

    Gl2d_Impl *impl = ObjectWrap::Unwrap<Gl2d_Impl>(args.This());

    if(args.Length() != 2)
               	return THROW_EXCEPTION(TError, "Transform_scaleNonUniform : number of args exception! argn must:2");

    float sx = args[0]->NumberValue();
    float sy = args[1]->NumberValue();

    impl->Transform_scaleNonUniform(sx, sy);

    return v8::Undefined();
}
}
//virtual void        CALL    Transform_scaleNonUniform(float sx, float sy);
