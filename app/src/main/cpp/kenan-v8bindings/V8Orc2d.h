/*******************
*this class is for drawing graphics
*******************/

#ifndef V8ORC2D_H
#define V8ORC2D_H

#include <ObjectWrap.h>
#include <v8.h>
#include "gl2d_impl.h"

namespace DCanvas
{

class V8Orc2d : public ObjectWrap
{
public:
	typedef Gl2d_Impl ClassImpl;
	V8Orc2d();
    virtual ~V8Orc2d();

    static v8::Handle<v8::Value> Wrap(Gl2d_Impl* impl);

    //javascript functions
    static v8::Handle<v8::Value> System_Initiate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> System_Shutdown(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> System_ChangeSize(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> System_GetErrorMessage(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Effect_Load(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Effect_Free(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Effect_Free_All(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Effect_Active(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Effect_SetInt(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Effect_SetFloat(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Effect_SetFloatv2(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Gfx_SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Gfx_SetLineWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Gfx_Clear(const v8::FunctionCallbackInfo<v8::Value>& args);


    static v8::Handle<v8::Value> Gfx_RenderLine(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Gfx_RenderLineEx(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Gfx_RenderTripleEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Gfx_RenderQuadEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Gfx_RenderRoundEx(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Gfx_RenderTorusEx(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Handle<v8::Value> Gfx_SetMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Gfx_SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

//	static v8::Handle<v8::Value> Target_Create(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_Free(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_FreeAll(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_GetTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_Active(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_ResetToScreen(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Target_ReadPixels(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Handle<v8::Value> Texture_Create(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Texture_Load(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Texture_Free(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Texture_SaveToFile(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Handle<v8::Value> Transform_rotate(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_rotate3d(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_rotateFromVector(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Transform_translate(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_translate3d(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Transform_scale(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Transform_scaleNonUniform(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_scale3d(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_reset(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_enable(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_disable(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_useMatrix2d(const v8::FunctionCallbackInfo<v8::Value>& args);
//	static v8::Handle<v8::Value> Transform_getMatrix(const v8::FunctionCallbackInfo<v8::Value>& args);
private:
};

} // namespace DCanvas

#endif // V8ORC2D_H
