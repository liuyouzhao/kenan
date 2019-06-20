#ifndef V8WEBGLGRAPHICSCONTEXT_H
#define V8WEBGLGRAPHICSCONTEXT_H

#include "Types3D.h"
#include "ObjectWrap.h"
#include "RenderingContext.h"
#include "v8.h"
#include "gl3d_impl.h"
#include "WebGLContextAttributes.h"

#include "ArrayBuffer.h"
#include "ArrayBufferView.h"
#include "Float32Array.h"
#include "Int32Array.h"
#include "Image.h"

#include <string>
#include <set>
#include <vector>

#ifndef GL_DEPTH_STENCIL_ATTACHMENT
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#endif

namespace DCanvas
{

class V8WebGLGraphicsContext : public ObjectWrap
{

public:
    static v8::Handle<v8::Value> Wrap(WebGLGraphicsContext* ptr);
    static v8::Handle<v8::Value> ViewPort(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Clear(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ClearColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Enable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Disable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BindBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BufferData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BufferSubData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ShaderSource(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CompileShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> AttachShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> LinkProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> UseProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DepthFunc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ClearDepth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Flush(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BindTexture(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> CreateRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BindRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> RenderbufferStorage(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> FramebufferTexture2D(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> FramebufferRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BindFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetAttribLocation(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetUniformLocation(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> EnableVertexAttribArray(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttribPointer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DrawArrays(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DrawElements(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetShaderInfoLog(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> TexImage2D(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ActiveTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GenerateMipmap(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> PixelStorei(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> TexParameterf(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> TexParameteri(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Uniform1f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform1fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform1i(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform1iv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform2f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform2fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform2i(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform2iv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform3f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform3fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform3i(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform3iv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform4f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform4fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform4i(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Uniform4iv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> UniformMatrix2fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> UniformMatrix3fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> UniformMatrix4fv(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> UniformfvImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index);
    static v8::Handle<v8::Value> UniformivImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index);
    static v8::Handle<v8::Value> UniformMatrixfvImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index);

    static v8::Handle<v8::Value> VertexAttrib1f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib1fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib2f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib2fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib3f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib3fv(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib4f(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> VertexAttrib4fv(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> BlendColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BlendEquation(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BlendEquationSeparate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BlendFunc(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BlendFuncSeparate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SampleCoverage(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StencilFuncSeparate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StencilOp(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StencilOpSeparate(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> ClearStencil(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ColorMask(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DepthMask(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StencilMask(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StencilMaskSeparate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DepthRange(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Scissor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DeleteFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CullFace(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> FrontFace(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> LineWidth(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> PolygonOffset(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BindAttribLocation(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DetachShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetAttachedShaders(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetProgramInfoLog(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetShaderSource(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsShader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsTexture(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ValidateProgram(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Finish(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetError(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DisableVertexAttribArray(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsEnabled(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Hint(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> CopyTexImage2D(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CopyTexSubImage2D(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> TexSubImage2D(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetProgramParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetShaderParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetTexParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetRenderbufferParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetFramebufferAttachmentParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetParameter(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetVertexAttrib(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetVertexAttribOffset(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetActiveAttrib(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetActiveUniform(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetUniform(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CheckFramebufferStatus(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ReadPixels(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
};

} // namespace DCanvas

#endif // V8WEBGLGRAPHICSCONTEXT_H
