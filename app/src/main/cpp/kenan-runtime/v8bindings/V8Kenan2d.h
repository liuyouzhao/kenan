/*******************
*this class is for drawing graphics
*******************/

#ifndef V8ORC2D_H
#define V8ORC2D_H

#include <v8.h>
#include "gl2d_impl.h"

namespace kenan_v8bindings
{

class V8Kenan2d
{
public:
	V8Kenan2d();
    virtual ~V8Kenan2d();

    static v8::Handle<v8::Value> genSingleton(v8::Isolate *isolate);

    //javascript functions
    static void System_Initiate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void System_Shutdown(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void System_ChangeSize(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void System_GetErrorMessage(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Effect_Load(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Effect_Free(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Effect_Free_All(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Effect_Active(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Effect_SetInt(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Effect_SetFloat(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Effect_SetFloatv2(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Gfx_SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Gfx_SetLineWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Gfx_Clear(const v8::FunctionCallbackInfo<v8::Value>& args);


    static void Gfx_RenderLine(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Gfx_RenderLineEx(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Gfx_RenderTripleEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Gfx_RenderQuadEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Gfx_RenderRoundEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Gfx_RenderTorusEx(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Gfx_SetMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Gfx_SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Gfx_GetBatchCount(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Texture_Create(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Texture_Load(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Texture_Free(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Texture_SaveToFile(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Transform_rotate(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Transform_translate(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Transform_scale(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Transform_scaleNonUniform(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Factory_CreateSpirit(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Factory_DestroySpirit(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
};

} // namespace kenan_v8bindings

#endif // V8ORC2D_H
