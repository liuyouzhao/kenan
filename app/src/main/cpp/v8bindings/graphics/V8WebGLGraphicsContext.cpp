#include "defines.h"
#include "WebGLGraphicsContext.h"
#include "V8WebGLGraphicsContext.h"
#include "WebGLShader.h"
#include "WebGLBuffer.h"
#include "WebGLProgram.h"
#include "WebGLTexture.h"
#include "WebGLRenderBuffer.h"
#include "WebGLFrameBuffer.h"
#include "Float32Array.h"
#include "Uint16Array.h"
#include "ArrayBuffer.h"
#include "WebGLGetInfo.h"
#include "WebGLActiveInfo.h"
#include "Assert.h"
#include "V8ArrayType.h"
#include "V8Image.h"

#undef LOG_TAG
#define  LOG_TAG    "V8WebGLGraphicsContext"

#ifndef GL_STENCIL_INDEX
#define GL_STENCIL_INDEX                  0x1901
#endif

namespace DCanvas
{

v8::Handle<v8::Value> V8WebGLGraphicsContext::Wrap(WebGLGraphicsContext* ptr)
{
    HandleScope scope;

    v8::Handle<v8::ObjectTemplate>                  class_proto;
    v8::Handle<v8::Object>                          class_obj;
    v8::Handle<v8::FunctionTemplate>                class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("WebGLGraphicsContext"));
    class_proto = class_func_templ->PrototypeTemplate();
    class_proto->Set(v8::String::New("viewport"),FunctionTemplate::New(V8WebGLGraphicsContext::ViewPort));
    class_proto->Set(v8::String::New("clear"),FunctionTemplate::New(V8WebGLGraphicsContext::Clear));
    class_proto->Set(v8::String::New("clearColor"),FunctionTemplate::New(V8WebGLGraphicsContext::ClearColor));
    class_proto->Set(v8::String::New("createBuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateBuffer));
    class_proto->Set(v8::String::New("createTexture"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateTexture));
    class_proto->Set(v8::String::New("createShader"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateShader));
    class_proto->Set(v8::String::New("createRenderbuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateRenderbuffer));
    class_proto->Set(v8::String::New("createFramebuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateFramebuffer));
    class_proto->Set(v8::String::New("createProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::CreateProgram));
    class_proto->Set(v8::String::New("bindBuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::BindBuffer));
    class_proto->Set(v8::String::New("bindTexture"),FunctionTemplate::New(V8WebGLGraphicsContext::BindTexture));
    class_proto->Set(v8::String::New("bufferData"),FunctionTemplate::New(V8WebGLGraphicsContext::BufferData));
    class_proto->Set(v8::String::New("bufferSubData"),FunctionTemplate::New(V8WebGLGraphicsContext::BufferSubData));
    class_proto->Set(v8::String::New("shaderSource"),FunctionTemplate::New(V8WebGLGraphicsContext::ShaderSource));
    class_proto->Set(v8::String::New("compileShader"),FunctionTemplate::New(V8WebGLGraphicsContext::CompileShader));
    class_proto->Set(v8::String::New("getAttribLocation"),FunctionTemplate::New(V8WebGLGraphicsContext::GetAttribLocation));
    class_proto->Set(v8::String::New("getUniformLocation"),FunctionTemplate::New(V8WebGLGraphicsContext::GetUniformLocation));
    class_proto->Set(v8::String::New("enableVertexAttribArray"),FunctionTemplate::New(V8WebGLGraphicsContext::EnableVertexAttribArray));
    class_proto->Set(v8::String::New("vertexAttribPointer"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttribPointer));
    class_proto->Set(v8::String::New("drawArrays"),FunctionTemplate::New(V8WebGLGraphicsContext::DrawArrays));
    class_proto->Set(v8::String::New("attachShader"),FunctionTemplate::New(V8WebGLGraphicsContext::AttachShader));
    class_proto->Set(v8::String::New("linkProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::LinkProgram));
    class_proto->Set(v8::String::New("useProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::UseProgram));
    class_proto->Set(v8::String::New("enable"),FunctionTemplate::New(V8WebGLGraphicsContext::Enable));
    class_proto->Set(v8::String::New("disable"),FunctionTemplate::New(V8WebGLGraphicsContext::Disable));
    class_proto->Set(v8::String::New("depthFunc"),FunctionTemplate::New(V8WebGLGraphicsContext::DepthFunc));
    class_proto->Set(v8::String::New("clearDepth"),FunctionTemplate::New(V8WebGLGraphicsContext::ClearDepth));
    class_proto->Set(v8::String::New("drawElements"),FunctionTemplate::New(V8WebGLGraphicsContext::DrawElements));
    class_proto->Set(v8::String::New("flush"),FunctionTemplate::New(V8WebGLGraphicsContext::Flush));
    class_proto->Set(v8::String::New("getShaderInfoLog"),FunctionTemplate::New(V8WebGLGraphicsContext::GetShaderInfoLog));
    class_proto->Set(v8::String::New("texImage2D"),FunctionTemplate::New(V8WebGLGraphicsContext::TexImage2D));
    class_proto->Set(v8::String::New("activeTexture"),FunctionTemplate::New(V8WebGLGraphicsContext::ActiveTexture));
    class_proto->Set(v8::String::New("generateMipmap"),FunctionTemplate::New(V8WebGLGraphicsContext::GenerateMipmap));
    class_proto->Set(v8::String::New("pixelStorei"),FunctionTemplate::New(V8WebGLGraphicsContext::PixelStorei));
    class_proto->Set(v8::String::New("bindFramebuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::BindFramebuffer));
    class_proto->Set(v8::String::New("bindRenderbuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::BindRenderbuffer));
    class_proto->Set(v8::String::New("renderbufferStorage"),FunctionTemplate::New(V8WebGLGraphicsContext::RenderbufferStorage));
    class_proto->Set(v8::String::New("framebufferTexture2D"),FunctionTemplate::New(V8WebGLGraphicsContext::FramebufferTexture2D));
    class_proto->Set(v8::String::New("framebufferRenderbuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::FramebufferRenderbuffer));

    class_proto->Set(v8::String::New("swapBuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::SwapBuffer));

    class_proto->Set(v8::String::New("texParameterf"),FunctionTemplate::New(V8WebGLGraphicsContext::TexParameterf));
    class_proto->Set(v8::String::New("texParameteri"),FunctionTemplate::New(V8WebGLGraphicsContext::TexParameteri));

    class_proto->Set(v8::String::New("uniform1f"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform1f));
    class_proto->Set(v8::String::New("uniform1fv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform1fv));
    class_proto->Set(v8::String::New("uniform1i"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform1i));
    class_proto->Set(v8::String::New("uniform1iv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform1iv));
    class_proto->Set(v8::String::New("uniform2f"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform2f));
    class_proto->Set(v8::String::New("uniform2fv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform2fv));
    class_proto->Set(v8::String::New("uniform2i"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform2i));
    class_proto->Set(v8::String::New("uniform2iv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform2iv));
    class_proto->Set(v8::String::New("uniform3f"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform3f));
    class_proto->Set(v8::String::New("uniform3fv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform3fv));
    class_proto->Set(v8::String::New("uniform3i"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform3i));
    class_proto->Set(v8::String::New("uniform3iv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform3iv));
    class_proto->Set(v8::String::New("uniform4f"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform4f));
    class_proto->Set(v8::String::New("uniform4fv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform4fv));
    class_proto->Set(v8::String::New("uniform4i"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform4i));
    class_proto->Set(v8::String::New("uniform4iv"),FunctionTemplate::New(V8WebGLGraphicsContext::Uniform4iv));
    class_proto->Set(v8::String::New("uniformMatrix2fv"),FunctionTemplate::New(V8WebGLGraphicsContext::UniformMatrix2fv));
    class_proto->Set(v8::String::New("uniformMatrix3fv"),FunctionTemplate::New(V8WebGLGraphicsContext::UniformMatrix3fv));
    class_proto->Set(v8::String::New("uniformMatrix4fv"),FunctionTemplate::New(V8WebGLGraphicsContext::UniformMatrix4fv));

    class_proto->Set(v8::String::New("vertexAttrib1f"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib1f));
    class_proto->Set(v8::String::New("vertexAttrib1fv"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib1fv));
    class_proto->Set(v8::String::New("vertexAttrib2f"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib2f));
    class_proto->Set(v8::String::New("vertexAttrib2fv"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib2fv));
    class_proto->Set(v8::String::New("vertexAttrib3f"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib3f));
    class_proto->Set(v8::String::New("vertexAttrib3fv"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib3fv));
    class_proto->Set(v8::String::New("vertexAttrib4f"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib4f));
    class_proto->Set(v8::String::New("vertexAttrib4fv"),FunctionTemplate::New(V8WebGLGraphicsContext::VertexAttrib4fv));

    class_proto->Set(v8::String::New("blendColor"),FunctionTemplate::New(V8WebGLGraphicsContext::BlendColor));
    class_proto->Set(v8::String::New("blendEquation"),FunctionTemplate::New(V8WebGLGraphicsContext::BlendEquation));
    class_proto->Set(v8::String::New("blendEquationSeparate"),FunctionTemplate::New(V8WebGLGraphicsContext::BlendEquationSeparate));
    class_proto->Set(v8::String::New("blendFunc"),FunctionTemplate::New(V8WebGLGraphicsContext::BlendFunc));
    class_proto->Set(v8::String::New("blendFuncSeparate"),FunctionTemplate::New(V8WebGLGraphicsContext::BlendFuncSeparate));
    class_proto->Set(v8::String::New("sampleCoverage"),FunctionTemplate::New(V8WebGLGraphicsContext::SampleCoverage));
    class_proto->Set(v8::String::New("stencilFuncSeparate"),FunctionTemplate::New(V8WebGLGraphicsContext::StencilFuncSeparate));
    class_proto->Set(v8::String::New("stencilOp"),FunctionTemplate::New(V8WebGLGraphicsContext::StencilOp));
    class_proto->Set(v8::String::New("stencilOpSeparate"),FunctionTemplate::New(V8WebGLGraphicsContext::StencilOpSeparate));

    class_proto->Set(v8::String::New("clearStencil"),FunctionTemplate::New(V8WebGLGraphicsContext::ClearStencil));
    class_proto->Set(v8::String::New("colorMask"),FunctionTemplate::New(V8WebGLGraphicsContext::ColorMask));
    class_proto->Set(v8::String::New("depthMask"),FunctionTemplate::New(V8WebGLGraphicsContext::DepthMask));
    class_proto->Set(v8::String::New("stencilMask"),FunctionTemplate::New(V8WebGLGraphicsContext::StencilMask));
    class_proto->Set(v8::String::New("stencilMaskSeparate"),FunctionTemplate::New(V8WebGLGraphicsContext::StencilMaskSeparate));
    class_proto->Set(v8::String::New("depthRange"),FunctionTemplate::New(V8WebGLGraphicsContext::DepthRange));
    class_proto->Set(v8::String::New("scissor"),FunctionTemplate::New(V8WebGLGraphicsContext::Scissor));
    class_proto->Set(v8::String::New("deleteBuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteBuffer));
    class_proto->Set(v8::String::New("deleteProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteProgram));
    class_proto->Set(v8::String::New("deleteShader"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteShader));
    class_proto->Set(v8::String::New("deleteTexture"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteTexture));
    class_proto->Set(v8::String::New("deleteRenderbuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteRenderbuffer));
    class_proto->Set(v8::String::New("deleteFramebuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteFramebuffer));
    class_proto->Set(v8::String::New("deleteFramebuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::DeleteFramebuffer));
    class_proto->Set(v8::String::New("cullFace"),FunctionTemplate::New(V8WebGLGraphicsContext::CullFace));
    class_proto->Set(v8::String::New("frontFace"),FunctionTemplate::New(V8WebGLGraphicsContext::FrontFace));
    class_proto->Set(v8::String::New("lineWidth"),FunctionTemplate::New(V8WebGLGraphicsContext::LineWidth));
    class_proto->Set(v8::String::New("polygonOffset"),FunctionTemplate::New(V8WebGLGraphicsContext::PolygonOffset));
    class_proto->Set(v8::String::New("bindAttribLocation"),FunctionTemplate::New(V8WebGLGraphicsContext::BindAttribLocation));
    class_proto->Set(v8::String::New("detachShader"),FunctionTemplate::New(V8WebGLGraphicsContext::DetachShader));
    class_proto->Set(v8::String::New("getAttachedShaders"),FunctionTemplate::New(V8WebGLGraphicsContext::GetAttachedShaders));
    class_proto->Set(v8::String::New("getProgramInfoLog"),FunctionTemplate::New(V8WebGLGraphicsContext::GetProgramInfoLog));
    class_proto->Set(v8::String::New("getShaderSource"),FunctionTemplate::New(V8WebGLGraphicsContext::GetShaderSource));
    class_proto->Set(v8::String::New("isBuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::IsBuffer));
    class_proto->Set(v8::String::New("isProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::IsProgram));
    class_proto->Set(v8::String::New("isShader"),FunctionTemplate::New(V8WebGLGraphicsContext::IsShader));
    class_proto->Set(v8::String::New("isTexture"),FunctionTemplate::New(V8WebGLGraphicsContext::IsTexture));
    class_proto->Set(v8::String::New("isRenderbuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::IsRenderbuffer));
    class_proto->Set(v8::String::New("isFramebuffer"),FunctionTemplate::New(V8WebGLGraphicsContext::IsFramebuffer));
    class_proto->Set(v8::String::New("validateProgram"),FunctionTemplate::New(V8WebGLGraphicsContext::ValidateProgram));
    class_proto->Set(v8::String::New("finish"),FunctionTemplate::New(V8WebGLGraphicsContext::Finish));
    class_proto->Set(v8::String::New("GetError"),FunctionTemplate::New(V8WebGLGraphicsContext::GetError));
    class_proto->Set(v8::String::New("disableVertexAttribArray"),FunctionTemplate::New(V8WebGLGraphicsContext::DisableVertexAttribArray));
    class_proto->Set(v8::String::New("isEnabled"),FunctionTemplate::New(V8WebGLGraphicsContext::IsEnabled));
    class_proto->Set(v8::String::New("copyTexImage2D"),FunctionTemplate::New(V8WebGLGraphicsContext::CopyTexImage2D));
    class_proto->Set(v8::String::New("texSubImage2D"),FunctionTemplate::New(V8WebGLGraphicsContext::TexSubImage2D));
    class_proto->Set(v8::String::New("hint"),FunctionTemplate::New(V8WebGLGraphicsContext::Hint));

    class_proto->Set(v8::String::New("getShaderParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetShaderParameter));
    class_proto->Set(v8::String::New("getProgramParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetProgramParameter));
    class_proto->Set(v8::String::New("getTexParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetTexParameter));
    class_proto->Set(v8::String::New("getRenderbufferParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetRenderbufferParameter));
    class_proto->Set(v8::String::New("getFramebufferAttachmentParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetFramebufferAttachmentParameter));
    class_proto->Set(v8::String::New("getParameter"),FunctionTemplate::New(V8WebGLGraphicsContext::GetParameter));
    class_proto->Set(v8::String::New("getVertexAttribOffset"),FunctionTemplate::New(V8WebGLGraphicsContext::GetVertexAttribOffset));
    class_proto->Set(v8::String::New("getVertexAttrib"),FunctionTemplate::New(V8WebGLGraphicsContext::GetVertexAttrib));
    class_proto->Set(v8::String::New("getActiveAttrib"),FunctionTemplate::New(V8WebGLGraphicsContext::GetActiveAttrib));
    class_proto->Set(v8::String::New("getActiveUniform"),FunctionTemplate::New(V8WebGLGraphicsContext::GetActiveUniform));
    class_proto->Set(v8::String::New("getUniform"),FunctionTemplate::New(V8WebGLGraphicsContext::GetUniform));
    class_proto->Set(v8::String::New("checkFramebufferStatus"),FunctionTemplate::New(V8WebGLGraphicsContext::CheckFramebufferStatus));
    class_proto->Set(v8::String::New("readPixels"),FunctionTemplate::New(V8WebGLGraphicsContext::ReadPixels));
    class_proto->Set(v8::String::New("copyTexSubImage2D"),FunctionTemplate::New(V8WebGLGraphicsContext::CopyTexSubImage2D));

    class_proto->Set(v8::String::New("DEPTH_BUFFER_BIT"),Uint32::New(GL_DEPTH_BUFFER_BIT));
    class_proto->Set(v8::String::New("STENCIL_BUFFER_BIT"),Uint32::New(GL_STENCIL_BUFFER_BIT));
    class_proto->Set(v8::String::New("COLOR_BUFFER_BIT"),Uint32::New(GL_COLOR_BUFFER_BIT));
    class_proto->Set(v8::String::New("POINTS"),Uint32::New(GL_POINTS));
    class_proto->Set(v8::String::New("LINES"),Uint32::New(GL_LINES));
    class_proto->Set(v8::String::New("LINE_LOOP"),Uint32::New(GL_LINE_LOOP));
    class_proto->Set(v8::String::New("LINE_STRIP"),Uint32::New(GL_LINE_STRIP));
    class_proto->Set(v8::String::New("TRIANGLES"),Uint32::New(GL_TRIANGLES));
    class_proto->Set(v8::String::New("TRIANGLE_STRIP"),Uint32::New(GL_TRIANGLE_STRIP));
    class_proto->Set(v8::String::New("TRIANGLE_FAN"),Uint32::New(GL_TRIANGLE_FAN));
    class_proto->Set(v8::String::New("ZERO"),Uint32::New(GL_ZERO));
    class_proto->Set(v8::String::New("ONE"),Uint32::New(GL_ONE));
    class_proto->Set(v8::String::New("SRC_COLOR"),Uint32::New(GL_SRC_COLOR));
    class_proto->Set(v8::String::New("ONE_MINUS_SRC_COLOR"),Uint32::New(GL_ONE_MINUS_SRC_COLOR));
    class_proto->Set(v8::String::New("SRC_ALPHA"),Uint32::New(GL_SRC_ALPHA));
    class_proto->Set(v8::String::New("ONE_MINUS_SRC_ALPHA"),Uint32::New(GL_ONE_MINUS_SRC_ALPHA));
    class_proto->Set(v8::String::New("DST_ALPHA"),Uint32::New(GL_DST_ALPHA));
    class_proto->Set(v8::String::New("ONE_MINUS_DST_ALPHA"),Uint32::New(GL_ONE_MINUS_DST_ALPHA));
    class_proto->Set(v8::String::New("DST_COLOR"),Uint32::New(GL_DST_COLOR));
    class_proto->Set(v8::String::New("ONE_MINUS_DST_COLOR"),Uint32::New(GL_ONE_MINUS_DST_COLOR));
    class_proto->Set(v8::String::New("SRC_ALPHA_SATURATE"),Uint32::New(GL_SRC_ALPHA_SATURATE));
    class_proto->Set(v8::String::New("FUNC_ADD"),Uint32::New(GL_FUNC_ADD));
    class_proto->Set(v8::String::New("BLEND_EQUATION"),Uint32::New(GL_BLEND_EQUATION));
    class_proto->Set(v8::String::New("BLEND_EQUATION_RGB"),Uint32::New(GL_BLEND_EQUATION_RGB));
    class_proto->Set(v8::String::New("BLEND_EQUATION_ALPHA"),Uint32::New(GL_BLEND_EQUATION_ALPHA));
    class_proto->Set(v8::String::New("FUNC_SUBTRACT"),Uint32::New(GL_FUNC_SUBTRACT));
    class_proto->Set(v8::String::New("FUNC_REVERSE_SUBTRACT"),Uint32::New(GL_FUNC_REVERSE_SUBTRACT));
    class_proto->Set(v8::String::New("BLEND_DST_RGB"),Uint32::New(GL_BLEND_DST_RGB));
    class_proto->Set(v8::String::New("BLEND_SRC_RGB"),Uint32::New(GL_BLEND_SRC_RGB));
    class_proto->Set(v8::String::New("BLEND_DST_ALPHA"),Uint32::New(GL_BLEND_DST_ALPHA));
    class_proto->Set(v8::String::New("BLEND_SRC_ALPHA"),Uint32::New(GL_BLEND_SRC_ALPHA));
    class_proto->Set(v8::String::New("CONSTANT_COLOR"),Uint32::New(GL_CONSTANT_COLOR));
    class_proto->Set(v8::String::New("ONE_MINUS_CONSTANT_COLOR"),Uint32::New(GL_ONE_MINUS_CONSTANT_COLOR));
    class_proto->Set(v8::String::New("CONSTANT_ALPHA"),Uint32::New(GL_CONSTANT_ALPHA));
    class_proto->Set(v8::String::New("ONE_MINUS_CONSTANT_ALPHA"),Uint32::New(GL_ONE_MINUS_CONSTANT_ALPHA));
    class_proto->Set(v8::String::New("BLEND_COLOR"),Uint32::New(GL_BLEND_COLOR));
    class_proto->Set(v8::String::New("ARRAY_BUFFER"),Uint32::New(GL_ARRAY_BUFFER));
    class_proto->Set(v8::String::New("ELEMENT_ARRAY_BUFFER"),Uint32::New(GL_ELEMENT_ARRAY_BUFFER));
    class_proto->Set(v8::String::New("ARRAY_BUFFER_BINDING"),Uint32::New(GL_ARRAY_BUFFER_BINDING));
    class_proto->Set(v8::String::New("ELEMENT_ARRAY_BUFFER_BINDING"),Uint32::New(GL_ELEMENT_ARRAY_BUFFER_BINDING));
    class_proto->Set(v8::String::New("STREAM_DRAW"),Uint32::New(GL_STREAM_DRAW));
    class_proto->Set(v8::String::New("STATIC_DRAW"),Uint32::New(GL_STATIC_DRAW));
    class_proto->Set(v8::String::New("DYNAMIC_DRAW"),Uint32::New(GL_DYNAMIC_DRAW));
    class_proto->Set(v8::String::New("BUFFER_SIZE"),Uint32::New(GL_BUFFER_SIZE));
    class_proto->Set(v8::String::New("BUFFER_USAGE"),Uint32::New(GL_BUFFER_USAGE));
    class_proto->Set(v8::String::New("CURRENT_VERTEX_ATTRIB"),Uint32::New(GL_CURRENT_VERTEX_ATTRIB));
    class_proto->Set(v8::String::New("FRONT"),Uint32::New(GL_FRONT));
    class_proto->Set(v8::String::New("BACK"),Uint32::New(GL_BACK));
    class_proto->Set(v8::String::New("FRONT_AND_BACK"),Uint32::New(GL_FRONT_AND_BACK));
    class_proto->Set(v8::String::New("TEXTURE_2D"),Uint32::New(GL_TEXTURE_2D));
    class_proto->Set(v8::String::New("CULL_FACE"),Uint32::New(GL_CULL_FACE));
    class_proto->Set(v8::String::New("BLEND"),Uint32::New(GL_BLEND));
    class_proto->Set(v8::String::New("DITHER"),Uint32::New(GL_DITHER));
    class_proto->Set(v8::String::New("STENCIL_TEST"),Uint32::New(GL_STENCIL_TEST));
    class_proto->Set(v8::String::New("DEPTH_TEST"),Uint32::New(GL_DEPTH_TEST));
    class_proto->Set(v8::String::New("SCISSOR_TEST"),Uint32::New(GL_SCISSOR_TEST));
    class_proto->Set(v8::String::New("POLYGON_OFFSET_FILL"),Uint32::New(GL_POLYGON_OFFSET_FILL));
    class_proto->Set(v8::String::New("SAMPLE_ALPHA_TO_COVERAGE"),Uint32::New(GL_SAMPLE_ALPHA_TO_COVERAGE));
    class_proto->Set(v8::String::New("SAMPLE_COVERAGE"),Uint32::New(GL_SAMPLE_COVERAGE));
    class_proto->Set(v8::String::New("NO_ERROR"),Uint32::New(GL_NO_ERROR));
    class_proto->Set(v8::String::New("INVALID_ENUM"),Uint32::New(GL_INVALID_ENUM));
    class_proto->Set(v8::String::New("INVALID_VALUE"),Uint32::New(GL_INVALID_VALUE));
    class_proto->Set(v8::String::New("INVALID_OPERATION"),Uint32::New(GL_INVALID_OPERATION));
    class_proto->Set(v8::String::New("OUT_OF_MEMORY"),Uint32::New(GL_OUT_OF_MEMORY));
    class_proto->Set(v8::String::New("CW"),Uint32::New(GL_CW));
    class_proto->Set(v8::String::New("CCW"),Uint32::New(GL_CCW));
    class_proto->Set(v8::String::New("LINE_WIDTH"),Uint32::New(GL_LINE_WIDTH));
    class_proto->Set(v8::String::New("ALIASED_POINT_SIZE_RANGE"),Uint32::New(GL_ALIASED_POINT_SIZE_RANGE));
    class_proto->Set(v8::String::New("ALIASED_LINE_WIDTH_RANGE"),Uint32::New(GL_ALIASED_LINE_WIDTH_RANGE));
    class_proto->Set(v8::String::New("CULL_FACE_MODE"),Uint32::New(GL_CULL_FACE_MODE));
    class_proto->Set(v8::String::New("FRONT_FACE"),Uint32::New(GL_FRONT_FACE));
    class_proto->Set(v8::String::New("DEPTH_RANGE"),Uint32::New(GL_DEPTH_RANGE));
    class_proto->Set(v8::String::New("DEPTH_WRITEMASK"),Uint32::New(GL_DEPTH_WRITEMASK));
    class_proto->Set(v8::String::New("DEPTH_CLEAR_VALUE"),Uint32::New(GL_DEPTH_CLEAR_VALUE));
    class_proto->Set(v8::String::New("DEPTH_FUNC"),Uint32::New(GL_DEPTH_FUNC));
    class_proto->Set(v8::String::New("STENCIL_CLEAR_VALUE"),Uint32::New(GL_STENCIL_CLEAR_VALUE));
    class_proto->Set(v8::String::New("STENCIL_FUNC"),Uint32::New(GL_STENCIL_FUNC));
    class_proto->Set(v8::String::New("STENCIL_FAIL"),Uint32::New(GL_STENCIL_FAIL));
    class_proto->Set(v8::String::New("STENCIL_PASS_DEPTH_FAIL"),Uint32::New(GL_STENCIL_PASS_DEPTH_FAIL));
    class_proto->Set(v8::String::New("STENCIL_PASS_DEPTH_PASS"),Uint32::New(GL_STENCIL_PASS_DEPTH_PASS));
    class_proto->Set(v8::String::New("STENCIL_REF"),Uint32::New(GL_STENCIL_REF));
    class_proto->Set(v8::String::New("STENCIL_VALUE_MASK"),Uint32::New(GL_STENCIL_VALUE_MASK));
    class_proto->Set(v8::String::New("STENCIL_WRITEMASK"),Uint32::New(GL_STENCIL_WRITEMASK));
    class_proto->Set(v8::String::New("STENCIL_BACK_FUNC"),Uint32::New(GL_STENCIL_BACK_FUNC));
    class_proto->Set(v8::String::New("STENCIL_BACK_FAIL"),Uint32::New(GL_STENCIL_BACK_FAIL));
    class_proto->Set(v8::String::New("STENCIL_BACK_PASS_DEPTH_FAIL"),Uint32::New(GL_STENCIL_BACK_PASS_DEPTH_FAIL));
    class_proto->Set(v8::String::New("STENCIL_BACK_PASS_DEPTH_PASS"),Uint32::New(GL_STENCIL_BACK_PASS_DEPTH_PASS));
    class_proto->Set(v8::String::New("STENCIL_BACK_REF"),Uint32::New(GL_STENCIL_BACK_REF));
    class_proto->Set(v8::String::New("STENCIL_BACK_VALUE_MASK"),Uint32::New(GL_STENCIL_BACK_VALUE_MASK));
    class_proto->Set(v8::String::New("STENCIL_BACK_WRITEMASK"),Uint32::New(GL_STENCIL_BACK_WRITEMASK));
    class_proto->Set(v8::String::New("VIEWPORT"),Uint32::New(GL_VIEWPORT));
    class_proto->Set(v8::String::New("SCISSOR_BOX"),Uint32::New(GL_SCISSOR_BOX));
    class_proto->Set(v8::String::New("COLOR_CLEAR_VALUE"),Uint32::New(GL_COLOR_CLEAR_VALUE));
    class_proto->Set(v8::String::New("COLOR_WRITEMASK"),Uint32::New(GL_COLOR_WRITEMASK));
    class_proto->Set(v8::String::New("UNPACK_ALIGNMENT"),Uint32::New(GL_UNPACK_ALIGNMENT));
    class_proto->Set(v8::String::New("PACK_ALIGNMENT"),Uint32::New(GL_PACK_ALIGNMENT));
    class_proto->Set(v8::String::New("MAX_TEXTURE_SIZE"),Uint32::New(GL_MAX_TEXTURE_SIZE));
    class_proto->Set(v8::String::New("MAX_VIEWPORT_DIMS"),Uint32::New(GL_MAX_VIEWPORT_DIMS));
    class_proto->Set(v8::String::New("SUBPIXEL_BITS"),Uint32::New(GL_SUBPIXEL_BITS));
    class_proto->Set(v8::String::New("RED_BITS"),Uint32::New(GL_RED_BITS));
    class_proto->Set(v8::String::New("GREEN_BITS"),Uint32::New(GL_GREEN_BITS));
    class_proto->Set(v8::String::New("BLUE_BITS"),Uint32::New(GL_BLUE_BITS));
    class_proto->Set(v8::String::New("ALPHA_BITS"),Uint32::New(GL_ALPHA_BITS));
    class_proto->Set(v8::String::New("DEPTH_BITS"),Uint32::New(GL_DEPTH_BITS));
    class_proto->Set(v8::String::New("STENCIL_BITS"),Uint32::New(GL_STENCIL_BITS));
    class_proto->Set(v8::String::New("POLYGON_OFFSET_UNITS"),Uint32::New(GL_POLYGON_OFFSET_UNITS));
    class_proto->Set(v8::String::New("POLYGON_OFFSET_FACTOR"),Uint32::New(GL_POLYGON_OFFSET_FACTOR));
    class_proto->Set(v8::String::New("TEXTURE_BINDING_2D"),Uint32::New(GL_TEXTURE_BINDING_2D));
    class_proto->Set(v8::String::New("SAMPLE_BUFFERS"),Uint32::New(GL_SAMPLE_BUFFERS));
    class_proto->Set(v8::String::New("SAMPLES"),Uint32::New(GL_SAMPLES));
    class_proto->Set(v8::String::New("SAMPLE_COVERAGE_VALUE"),Uint32::New(GL_SAMPLE_COVERAGE_VALUE));
    class_proto->Set(v8::String::New("SAMPLE_COVERAGE_INVERT"),Uint32::New(GL_SAMPLE_COVERAGE_INVERT));
    class_proto->Set(v8::String::New("NUM_COMPRESSED_TEXTURE_FORMATS"),Uint32::New(GL_NUM_COMPRESSED_TEXTURE_FORMATS));
    class_proto->Set(v8::String::New("COMPRESSED_TEXTURE_FORMATS"),Uint32::New(GL_COMPRESSED_TEXTURE_FORMATS));
    class_proto->Set(v8::String::New("DONT_CARE"),Uint32::New(GL_DONT_CARE));
    class_proto->Set(v8::String::New("FASTEST"),Uint32::New(GL_FASTEST));
    class_proto->Set(v8::String::New("NICEST"),Uint32::New(GL_NICEST));
    class_proto->Set(v8::String::New("GENERATE_MIPMAP_HINT"),Uint32::New(GL_GENERATE_MIPMAP_HINT));
    class_proto->Set(v8::String::New("BYTE"),Uint32::New(GL_BYTE));
    class_proto->Set(v8::String::New("UNSIGNED_BYTE"),Uint32::New(GL_UNSIGNED_BYTE));
    class_proto->Set(v8::String::New("SHORT"),Uint32::New(GL_SHORT));
    class_proto->Set(v8::String::New("UNSIGNED_SHORT"),Uint32::New(GL_UNSIGNED_SHORT));
    class_proto->Set(v8::String::New("INT"),Uint32::New(GL_INT));
    class_proto->Set(v8::String::New("UNSIGNED_INT"),Uint32::New(GL_UNSIGNED_INT));
    class_proto->Set(v8::String::New("FLOAT"),Uint32::New(GL_FLOAT));
    class_proto->Set(v8::String::New("FIXED"),Uint32::New(GL_FIXED));
    class_proto->Set(v8::String::New("DEPTH_COMPONENT"),Uint32::New(GL_DEPTH_COMPONENT));
    class_proto->Set(v8::String::New("ALPHA"),Uint32::New(GL_ALPHA));
    class_proto->Set(v8::String::New("RGB"),Uint32::New(GL_RGB));
    class_proto->Set(v8::String::New("RGBA"),Uint32::New(GL_RGBA));
    class_proto->Set(v8::String::New("LUMINANCE"),Uint32::New(GL_LUMINANCE));
    class_proto->Set(v8::String::New("LUMINANCE_ALPHA"),Uint32::New(GL_LUMINANCE_ALPHA));
    class_proto->Set(v8::String::New("UNSIGNED_SHORT_4_4_4_4"),Uint32::New(GL_UNSIGNED_SHORT_4_4_4_4));
    class_proto->Set(v8::String::New("UNSIGNED_SHORT_5_5_5_1"),Uint32::New(GL_UNSIGNED_SHORT_5_5_5_1));
    class_proto->Set(v8::String::New("UNSIGNED_SHORT_5_6_5"),Uint32::New(GL_UNSIGNED_SHORT_5_6_5));
    class_proto->Set(v8::String::New("FRAGMENT_SHADER"),Uint32::New(GL_FRAGMENT_SHADER));
    class_proto->Set(v8::String::New("VERTEX_SHADER"),Uint32::New(GL_VERTEX_SHADER));
    class_proto->Set(v8::String::New("MAX_VERTEX_ATTRIBS"),Uint32::New(GL_MAX_VERTEX_ATTRIBS));
    class_proto->Set(v8::String::New("MAX_VERTEX_UNIFORM_VECTORS"),Uint32::New(GL_MAX_VERTEX_UNIFORM_VECTORS));
    class_proto->Set(v8::String::New("MAX_VARYING_VECTORS"),Uint32::New(GL_MAX_VARYING_VECTORS));
    class_proto->Set(v8::String::New("MAX_COMBINED_TEXTURE_IMAGE_UNITS"),Uint32::New(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS));
    class_proto->Set(v8::String::New("MAX_VERTEX_TEXTURE_IMAGE_UNITS"),Uint32::New(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS));
    class_proto->Set(v8::String::New("MAX_TEXTURE_IMAGE_UNITS"),Uint32::New(GL_MAX_TEXTURE_IMAGE_UNITS));
    class_proto->Set(v8::String::New("MAX_FRAGMENT_UNIFORM_VECTORS"),Uint32::New(GL_MAX_FRAGMENT_UNIFORM_VECTORS));
    class_proto->Set(v8::String::New("SHADER_TYPE"),Uint32::New(GL_SHADER_TYPE));
    class_proto->Set(v8::String::New("DELETE_STATUS"),Uint32::New(GL_DELETE_STATUS));
    class_proto->Set(v8::String::New("LINK_STATUS"),Uint32::New(GL_LINK_STATUS));
    class_proto->Set(v8::String::New("VALIDATE_STATUS"),Uint32::New(GL_VALIDATE_STATUS));
    class_proto->Set(v8::String::New("ATTACHED_SHADERS"),Uint32::New(GL_ATTACHED_SHADERS));
    class_proto->Set(v8::String::New("ACTIVE_UNIFORMS"),Uint32::New(GL_ACTIVE_UNIFORMS));
    class_proto->Set(v8::String::New("ACTIVE_UNIFORM_MAX_LENGTH"),Uint32::New(GL_ACTIVE_UNIFORM_MAX_LENGTH));
    class_proto->Set(v8::String::New("ACTIVE_ATTRIBUTES"),Uint32::New(GL_ACTIVE_ATTRIBUTES));
    class_proto->Set(v8::String::New("ACTIVE_ATTRIBUTE_MAX_LENGTH"),Uint32::New(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
    class_proto->Set(v8::String::New("SHADING_LANGUAGE_VERSION"),Uint32::New(GL_SHADING_LANGUAGE_VERSION));
    class_proto->Set(v8::String::New("CURRENT_PROGRAM"),Uint32::New(GL_CURRENT_PROGRAM));
    class_proto->Set(v8::String::New("NEVER"),Uint32::New(GL_NEVER));
    class_proto->Set(v8::String::New("LESS"),Uint32::New(GL_LESS));
    class_proto->Set(v8::String::New("EQUAL"),Uint32::New(GL_EQUAL));
    class_proto->Set(v8::String::New("LEQUAL"),Uint32::New(GL_LEQUAL));
    class_proto->Set(v8::String::New("GREATER"),Uint32::New(GL_GREATER));
    class_proto->Set(v8::String::New("NOTEQUAL"),Uint32::New(GL_NOTEQUAL));
    class_proto->Set(v8::String::New("GEQUAL"),Uint32::New(GL_GEQUAL));
    class_proto->Set(v8::String::New("ALWAYS"),Uint32::New(GL_ALWAYS));
    class_proto->Set(v8::String::New("KEEP"),Uint32::New(GL_KEEP));
    class_proto->Set(v8::String::New("REPLACE"),Uint32::New(GL_REPLACE));
    class_proto->Set(v8::String::New("INCR"),Uint32::New(GL_INCR));
    class_proto->Set(v8::String::New("DECR"),Uint32::New(GL_DECR));
    class_proto->Set(v8::String::New("INVERT"),Uint32::New(GL_INVERT));
    class_proto->Set(v8::String::New("INCR_WRAP"),Uint32::New(GL_INCR_WRAP));
    class_proto->Set(v8::String::New("DECR_WRAP"),Uint32::New(GL_DECR_WRAP));
    class_proto->Set(v8::String::New("VENDOR"),Uint32::New(GL_VENDOR));
    class_proto->Set(v8::String::New("RENDERER"),Uint32::New(GL_RENDERER));
    class_proto->Set(v8::String::New("VERSION"),Uint32::New(GL_VERSION));
    class_proto->Set(v8::String::New("EXTENSIONS"),Uint32::New(GL_EXTENSIONS));
    class_proto->Set(v8::String::New("NEAREST"),Uint32::New(GL_NEAREST));
    class_proto->Set(v8::String::New("LINEAR"),Uint32::New(GL_LINEAR));
    class_proto->Set(v8::String::New("NEAREST_MIPMAP_NEAREST"),Uint32::New(GL_NEAREST_MIPMAP_NEAREST));
    class_proto->Set(v8::String::New("LINEAR_MIPMAP_NEAREST"),Uint32::New(GL_LINEAR_MIPMAP_NEAREST));
    class_proto->Set(v8::String::New("NEAREST_MIPMAP_LINEAR"),Uint32::New(GL_NEAREST_MIPMAP_LINEAR));
    class_proto->Set(v8::String::New("LINEAR_MIPMAP_LINEAR"),Uint32::New(GL_LINEAR_MIPMAP_LINEAR));
    class_proto->Set(v8::String::New("TEXTURE_MAG_FILTER"),Uint32::New(GL_TEXTURE_MAG_FILTER));
    class_proto->Set(v8::String::New("TEXTURE_MIN_FILTER"),Uint32::New(GL_TEXTURE_MIN_FILTER));
    class_proto->Set(v8::String::New("TEXTURE_WRAP_S"),Uint32::New(GL_TEXTURE_WRAP_S));
    class_proto->Set(v8::String::New("TEXTURE_WRAP_T"),Uint32::New(GL_TEXTURE_WRAP_T));
    class_proto->Set(v8::String::New("TEXTURE"),Uint32::New(GL_TEXTURE));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP"),Uint32::New(GL_TEXTURE_CUBE_MAP));
    class_proto->Set(v8::String::New("TEXTURE_BINDING_CUBE_MAP"),Uint32::New(GL_TEXTURE_BINDING_CUBE_MAP));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_POSITIVE_X"),Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_X));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_NEGATIVE_X"),Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_X));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_POSITIVE_Y"),Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_Y));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_NEGATIVE_Y"),Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_POSITIVE_Z"),Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_Z));
    class_proto->Set(v8::String::New("TEXTURE_CUBE_MAP_NEGATIVE_Z"),Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z));
    class_proto->Set(v8::String::New("MAX_CUBE_MAP_TEXTURE_SIZE"),Uint32::New(GL_MAX_CUBE_MAP_TEXTURE_SIZE));
    class_proto->Set(v8::String::New("TEXTURE0"),Uint32::New(GL_TEXTURE0));
    class_proto->Set(v8::String::New("TEXTURE1"),Uint32::New(GL_TEXTURE1));
    class_proto->Set(v8::String::New("TEXTURE2"),Uint32::New(GL_TEXTURE2));
    class_proto->Set(v8::String::New("TEXTURE3"),Uint32::New(GL_TEXTURE3));
    class_proto->Set(v8::String::New("TEXTURE4"),Uint32::New(GL_TEXTURE4));
    class_proto->Set(v8::String::New("TEXTURE5"),Uint32::New(GL_TEXTURE5));
    class_proto->Set(v8::String::New("TEXTURE6"),Uint32::New(GL_TEXTURE6));
    class_proto->Set(v8::String::New("TEXTURE7"),Uint32::New(GL_TEXTURE7));
    class_proto->Set(v8::String::New("TEXTURE8"),Uint32::New(GL_TEXTURE8));
    class_proto->Set(v8::String::New("TEXTURE9"),Uint32::New(GL_TEXTURE9));
    class_proto->Set(v8::String::New("TEXTURE10"),Uint32::New(GL_TEXTURE10));
    class_proto->Set(v8::String::New("TEXTURE11"),Uint32::New(GL_TEXTURE11));
    class_proto->Set(v8::String::New("TEXTURE12"),Uint32::New(GL_TEXTURE12));
    class_proto->Set(v8::String::New("TEXTURE13"),Uint32::New(GL_TEXTURE13));
    class_proto->Set(v8::String::New("TEXTURE14"),Uint32::New(GL_TEXTURE14));
    class_proto->Set(v8::String::New("TEXTURE15"),Uint32::New(GL_TEXTURE15));
    class_proto->Set(v8::String::New("TEXTURE16"),Uint32::New(GL_TEXTURE16));
    class_proto->Set(v8::String::New("TEXTURE17"),Uint32::New(GL_TEXTURE17));
    class_proto->Set(v8::String::New("TEXTURE18"),Uint32::New(GL_TEXTURE18));
    class_proto->Set(v8::String::New("TEXTURE19"),Uint32::New(GL_TEXTURE19));
    class_proto->Set(v8::String::New("TEXTURE20"),Uint32::New(GL_TEXTURE20));
    class_proto->Set(v8::String::New("TEXTURE21"),Uint32::New(GL_TEXTURE21));
    class_proto->Set(v8::String::New("TEXTURE22"),Uint32::New(GL_TEXTURE22));
    class_proto->Set(v8::String::New("TEXTURE23"),Uint32::New(GL_TEXTURE23));
    class_proto->Set(v8::String::New("TEXTURE24"),Uint32::New(GL_TEXTURE24));
    class_proto->Set(v8::String::New("TEXTURE25"),Uint32::New(GL_TEXTURE25));
    class_proto->Set(v8::String::New("TEXTURE26"),Uint32::New(GL_TEXTURE26));
    class_proto->Set(v8::String::New("TEXTURE27"),Uint32::New(GL_TEXTURE27));
    class_proto->Set(v8::String::New("TEXTURE28"),Uint32::New(GL_TEXTURE28));
    class_proto->Set(v8::String::New("TEXTURE29"),Uint32::New(GL_TEXTURE29));
    class_proto->Set(v8::String::New("TEXTURE30"),Uint32::New(GL_TEXTURE30));
    class_proto->Set(v8::String::New("TEXTURE31"),Uint32::New(GL_TEXTURE31));
    class_proto->Set(v8::String::New("ACTIVE_TEXTURE"),Uint32::New(GL_ACTIVE_TEXTURE));
    class_proto->Set(v8::String::New("REPEAT"),Uint32::New(GL_REPEAT));
    class_proto->Set(v8::String::New("CLAMP_TO_EDGE"),Uint32::New(GL_CLAMP_TO_EDGE));
    class_proto->Set(v8::String::New("MIRRORED_REPEAT"),Uint32::New(GL_MIRRORED_REPEAT));
    class_proto->Set(v8::String::New("FLOAT_VEC2"),Uint32::New(GL_FLOAT_VEC2));
    class_proto->Set(v8::String::New("FLOAT_VEC3"),Uint32::New(GL_FLOAT_VEC3));
    class_proto->Set(v8::String::New("FLOAT_VEC4"),Uint32::New(GL_FLOAT_VEC4));
    class_proto->Set(v8::String::New("INT_VEC2"),Uint32::New(GL_INT_VEC2));
    class_proto->Set(v8::String::New("INT_VEC3"),Uint32::New(GL_INT_VEC3));
    class_proto->Set(v8::String::New("INT_VEC4"),Uint32::New(GL_INT_VEC4));
    class_proto->Set(v8::String::New("BOOL"),Uint32::New(GL_BOOL));
    class_proto->Set(v8::String::New("BOOL_VEC2"),Uint32::New(GL_BOOL_VEC2));
    class_proto->Set(v8::String::New("BOOL_VEC3"),Uint32::New(GL_BOOL_VEC3));
    class_proto->Set(v8::String::New("BOOL_VEC4"),Uint32::New(GL_BOOL_VEC4));
    class_proto->Set(v8::String::New("FLOAT_MAT2"),Uint32::New(GL_FLOAT_MAT2));
    class_proto->Set(v8::String::New("FLOAT_MAT3"),Uint32::New(GL_FLOAT_MAT3));
    class_proto->Set(v8::String::New("FLOAT_MAT4"),Uint32::New(GL_FLOAT_MAT4));
    class_proto->Set(v8::String::New("SAMPLER_2D"),Uint32::New(GL_SAMPLER_2D));
    class_proto->Set(v8::String::New("SAMPLER_CUBE"),Uint32::New(GL_SAMPLER_CUBE));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_ENABLED"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_ENABLED));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_SIZE"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_SIZE));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_STRIDE"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_STRIDE));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_TYPE"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_TYPE));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_NORMALIZED"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_POINTER"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_POINTER));
    class_proto->Set(v8::String::New("VERTEX_ATTRIB_ARRAY_BUFFER_BINDING"),Uint32::New(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING));
    class_proto->Set(v8::String::New("COMPILE_STATUS"),Uint32::New(GL_COMPILE_STATUS));
    class_proto->Set(v8::String::New("INFO_LOG_LENGTH"),Uint32::New(GL_INFO_LOG_LENGTH));
    class_proto->Set(v8::String::New("SHADER_SOURCE_LENGTH"),Uint32::New(GL_SHADER_SOURCE_LENGTH));
    class_proto->Set(v8::String::New("SHADER_COMPILER"),Uint32::New(GL_SHADER_COMPILER));
    class_proto->Set(v8::String::New("SHADER_BINARY_FORMATS"),Uint32::New(GL_SHADER_BINARY_FORMATS));
    class_proto->Set(v8::String::New("NUM_SHADER_BINARY_FORMATS"),Uint32::New(GL_NUM_SHADER_BINARY_FORMATS));
    class_proto->Set(v8::String::New("LOW_FLOAT"),Uint32::New(GL_LOW_FLOAT));
    class_proto->Set(v8::String::New("MEDIUM_FLOAT"),Uint32::New(GL_MEDIUM_FLOAT));
    class_proto->Set(v8::String::New("HIGH_FLOAT"),Uint32::New(GL_HIGH_FLOAT));
    class_proto->Set(v8::String::New("LOW_INT"),Uint32::New(GL_LOW_INT));
    class_proto->Set(v8::String::New("MEDIUM_INT"),Uint32::New(GL_MEDIUM_INT));
    class_proto->Set(v8::String::New("HIGH_INT"),Uint32::New(GL_HIGH_INT));
    class_proto->Set(v8::String::New("FRAMEBUFFER"),Uint32::New(GL_FRAMEBUFFER));
    class_proto->Set(v8::String::New("RENDERBUFFER"),Uint32::New(GL_RENDERBUFFER));
    class_proto->Set(v8::String::New("RGBA4"),Uint32::New(GL_RGBA4));
    class_proto->Set(v8::String::New("RGB5_A1"),Uint32::New(GL_RGB5_A1));
    class_proto->Set(v8::String::New("RGB565"),Uint32::New(GL_RGB565));
    class_proto->Set(v8::String::New("DEPTH_COMPONENT16"),Uint32::New(GL_DEPTH_COMPONENT16));
    class_proto->Set(v8::String::New("STENCIL_INDEX"),Uint32::New(GL_STENCIL_INDEX));
    class_proto->Set(v8::String::New("STENCIL_INDEX8"),Uint32::New(GL_STENCIL_INDEX8));
    class_proto->Set(v8::String::New("RENDERBUFFER_WIDTH"),Uint32::New(GL_RENDERBUFFER_WIDTH));
    class_proto->Set(v8::String::New("RENDERBUFFER_HEIGHT"),Uint32::New(GL_RENDERBUFFER_HEIGHT));
    class_proto->Set(v8::String::New("RENDERBUFFER_INTERNAL_FORMAT"),Uint32::New(GL_RENDERBUFFER_INTERNAL_FORMAT));
    class_proto->Set(v8::String::New("RENDERBUFFER_RED_SIZE"),Uint32::New(GL_RENDERBUFFER_RED_SIZE));
    class_proto->Set(v8::String::New("RENDERBUFFER_GREEN_SIZE"),Uint32::New(GL_RENDERBUFFER_GREEN_SIZE));
    class_proto->Set(v8::String::New("RENDERBUFFER_BLUE_SIZE"),Uint32::New(GL_RENDERBUFFER_BLUE_SIZE));
    class_proto->Set(v8::String::New("RENDERBUFFER_ALPHA_SIZE"),Uint32::New(GL_RENDERBUFFER_ALPHA_SIZE));
    class_proto->Set(v8::String::New("RENDERBUFFER_DEPTH_SIZE"),Uint32::New(GL_RENDERBUFFER_DEPTH_SIZE));
    class_proto->Set(v8::String::New("RENDERBUFFER_STENCIL_SIZE"),Uint32::New(GL_RENDERBUFFER_STENCIL_SIZE));
    class_proto->Set(v8::String::New("FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE"),Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE));
    class_proto->Set(v8::String::New("FRAMEBUFFER_ATTACHMENT_OBJECT_NAME"),Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME));
    class_proto->Set(v8::String::New("FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL"),Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL));
    class_proto->Set(v8::String::New("FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE"),Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE));
    class_proto->Set(v8::String::New("COLOR_ATTACHMENT0"),Uint32::New(GL_COLOR_ATTACHMENT0));
    class_proto->Set(v8::String::New("DEPTH_ATTACHMENT"),Uint32::New(GL_DEPTH_ATTACHMENT));
    class_proto->Set(v8::String::New("STENCIL_ATTACHMENT"),Uint32::New(GL_STENCIL_ATTACHMENT));
    class_proto->Set(v8::String::New("NONE"),Uint32::New(GL_NONE));
    class_proto->Set(v8::String::New("FRAMEBUFFER_COMPLETE"),Uint32::New(GL_FRAMEBUFFER_COMPLETE));
    class_proto->Set(v8::String::New("FRAMEBUFFER_INCOMPLETE_ATTACHMENT"),Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT));
    class_proto->Set(v8::String::New("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"),Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT));
    class_proto->Set(v8::String::New("FRAMEBUFFER_INCOMPLETE_DIMENSIONS"),Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS));
    class_proto->Set(v8::String::New("FRAMEBUFFER_UNSUPPORTED"),Uint32::New(GL_FRAMEBUFFER_UNSUPPORTED));
    class_proto->Set(v8::String::New("FRAMEBUFFER_BINDING"),Uint32::New(GL_FRAMEBUFFER_BINDING));
    class_proto->Set(v8::String::New("RENDERBUFFER_BINDING"),Uint32::New(GL_RENDERBUFFER_BINDING));
    class_proto->Set(v8::String::New("MAX_RENDERBUFFER_SIZE"),Uint32::New(GL_MAX_RENDERBUFFER_SIZE));
    class_proto->Set(v8::String::New("INVALID_FRAMEBUFFER_OPERATION"),Uint32::New(GL_INVALID_FRAMEBUFFER_OPERATION));

    class_proto->Set(v8::String::New("DEPTH_STENCIL"),Uint32::New(GL_DEPTH_STENCIL_OES));
    class_proto->Set(v8::String::New("DEPTH_STENCIL_ATTACHMENT"),Uint32::New(0x821A));

    class_proto->Set(v8::String::New("UNPACK_FLIP_Y_WEBGL"),Uint32::New(UNPACK_FLIP_Y_WEBGL));
    class_proto->Set(v8::String::New("UNPACK_PREMULTIPLY_ALPHA_WEBGL"),Uint32::New(UNPACK_PREMULTIPLY_ALPHA_WEBGL));
    class_proto->Set(v8::String::New("CONTEXT_LOST_WEBGL"),Uint32::New(CONTEXT_LOST_WEBGL));
    class_proto->Set(v8::String::New("UNPACK_COLORSPACE_CONVERSION_WEBGL"),Uint32::New(UNPACK_COLORSPACE_CONVERSION_WEBGL));
    class_proto->Set(v8::String::New("BROWSER_DEFAULT_WEBGL"),Uint32::New(BROWSER_DEFAULT_WEBGL));

    class_proto->Set(v8::String::New("FALSE"),Boolean::New(GL_FALSE));
    class_proto->Set(v8::String::New("TRUE"),Boolean::New(GL_TRUE));

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins-> SetInternalFieldCount(1);
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(ptr));
    LOGD("WebGLGraphicsContext wrap end");

    return v8::Persistent<v8::Object>::New(scope.Close(class_obj));
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ViewPort(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "ViewPort : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "ViewPort : arguments wrong type");
    }

    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();
    int w = args[2]->Int32Value();
    int h = args[3]->Int32Value();

    context->viewport(x, y, w, h);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Clear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "Clear : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Clear : arguments wrong type");
    }

    unsigned int mask = args[0]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->clear(mask);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ClearColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "ClearColor : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "ClearColor : arguments wrong type");
    }

    float r = args[0]->NumberValue();
    float g = args[1]->NumberValue();
    float b = args[2]->NumberValue();
    float a = args[3]->NumberValue();

    context->clearColor(r, g, b, a);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Enable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "Enable : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Enable : argument 1 wrong type");
    }

    DCenum cap = args[0]->Uint32Value();

    context->enable(cap);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "CreateBuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLBuffer* buffer = context->createBuffer();

    return buffer->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BindBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BindBuffer : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BindBuffer : argument 1 wrong type");
    }

    WebGLBuffer* buffer = NULL;

    if (WebGLBuffer::HasInstance(args[1]))
    {
        buffer = ObjectWrap::Unwrap<WebGLBuffer>(args[1]->ToObject());
    }
    else if (!args[1]->IsNull())
    {
        return THROW_EXCEPTION(TError, "BindBuffer : argument 2 is not match");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();

    if (!context->bindBuffer(target, buffer))
    {
        return THROW_EXCEPTION(TError, "BindBuffer : bindBuffer failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BufferData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "BufferData : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BufferData :arguments wrong type Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();
    DCenum usage = args[2]->Uint32Value();

    if (V8ArrayBuffer::HasInstance(args[1]))
    {
        ArrayBuffer* buffer = ObjectWrap::Unwrap<ArrayBuffer>(args[1]->ToObject());
        if (!context->bufferData(target, buffer, usage))
        {
            return THROW_EXCEPTION(TError, "BufferData : bufferData using ArrayBuffer failed");
        }
    }
    else if (args[1]->IsInt32())
    {
        DCsizei size = args[1]->Int32Value();
        if (!context->bufferData(target, size, usage))
        {
            return THROW_EXCEPTION(TError, "BufferData : bufferData using size failed");
        }
    }
    else if (V8ArrayBufferView::HasInstance(args[1]))
    {
        ArrayBufferView* bufferView = ObjectWrap::Unwrap<ArrayBufferView>(args[1]->ToObject());
        if (!context->bufferData(target, bufferView, usage))
        {
            return THROW_EXCEPTION(TError, "BufferData : bufferData using ArrayBufferView failed");
        }
    }
    else if (args[1]->IsNull())
    {
        if (!context->bufferData(target, (ArrayBufferView*)NULL, usage))
        {
            return THROW_EXCEPTION(TTypeError, "BufferData : bufferData using null failed");
        }
    }
    else
    {
        return THROW_EXCEPTION(TError, "BufferData : argument 2 is not match");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BufferSubData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "BufferSubData : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BufferSubData : arguments wrong type Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();
    DCintptr offset = args[1]->Int32Value();

    if (V8ArrayBuffer::HasInstance(args[2]))
    {
        ArrayBuffer* buffer = ObjectWrap::Unwrap<ArrayBuffer>(args[2]->ToObject());
        if (!context->bufferSubData(target, offset, buffer))
        {
            return THROW_EXCEPTION(TError, "BufferSubData : bufferSubData using ArrayBuffer failed");
        }
    }
    else if (V8ArrayBufferView::HasInstance(args[2]))
    {
        ArrayBufferView* bufferView = ObjectWrap::Unwrap<ArrayBufferView>(args[2]->ToObject());
        if (!context->bufferSubData(target, offset, bufferView))
        {
            return THROW_EXCEPTION(TError, "BufferSubData : bufferSubData using ArrayBufferView failed");
        }
    }
    else
    {
        return THROW_EXCEPTION(TError, "BufferSubData : argument 3 is not match");
    }


    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteBuffer : number of arguments Exception");
    }
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLBuffer* buffer = NULL;
    if (WebGLBuffer::HasInstance(args[0]))
    {
        buffer = ObjectWrap::Unwrap<WebGLBuffer>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteBuffer : argument 1 is not WebGLBuffer");
    }

    if (!context->deleteBuffer(buffer))
    {
        return THROW_EXCEPTION(TError, "DeleteBuffer : deleteBuffer failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteProgram : arguments Exception");
    }
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteProgram : argument 1 is not WebGLProgram");
    }

    if (!context->deleteProgram(program))
    {
        return THROW_EXCEPTION(TError, "DeleteProgram : deleteProgram failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteShader : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[0]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteShader : argument 1 is not WebGLShader");
    }

    if (!context->deleteShader(shader))
    {
        return THROW_EXCEPTION(TError, "DeleteShader : deleteShader failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteTexture : number of arguments Exception");
    }
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLTexture* texture = NULL;
    if (WebGLTexture::HasInstance(args[0]))
    {
        texture = ObjectWrap::Unwrap<WebGLTexture>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteTexture : argument 1 is not WebGLTexture");
    }

    if (!context->deleteTexture(texture))
    {
        return THROW_EXCEPTION(TError, "DeleteTexture : deleteTexture failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteRenderbuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLRenderBuffer* rbuffer = NULL;
    if (WebGLRenderBuffer::HasInstance(args[0]))
    {
        rbuffer = ObjectWrap::Unwrap<WebGLRenderBuffer>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteRenderbuffer : argument 1 is not WebGLRenderBuffer");
    }

    if (!context->deleteRenderbuffer(rbuffer))
    {
        return THROW_EXCEPTION(TError, "DeleteRenderbuffer : arguments Exception!!");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DeleteFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DeleteFramebuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLFrameBuffer* fbuffer = NULL;
    if (WebGLFrameBuffer::HasInstance(args[0]))
    {
        fbuffer = ObjectWrap::Unwrap<WebGLFrameBuffer>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DeleteFramebuffer : argument 1 is not WebGLFrameBuffer");
    }

    if (!context->deleteFramebuffer(fbuffer))
    {
        return THROW_EXCEPTION(TError, "DeleteFramebuffer : deleteFramebuffer failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "CreateTexture : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLTexture* texture = context->createTexture();

    return texture->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BindTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BindTexture : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BindTexture : arguments wrong type");
    }

    WebGLTexture* texture = NULL;

    if (WebGLTexture::HasInstance(args[1]))
    {
        texture = ObjectWrap::Unwrap<WebGLTexture>(args[1]->ToObject());
    }
    else if (!args[1]->IsNull())
    {
        return THROW_EXCEPTION(TError, "BindTexture : argument 2 is not match");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();

    if (!context->bindTexture(target, texture))
    {
        return THROW_EXCEPTION(TError, "BindTexture : bindTexture failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "CreateShader : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "CreateShader : argument 1 wrong type");
    }

    DCenum type = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLShader* shader = context->createShader(type);

    return shader->wrap();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ShaderSource(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "ShaderSource : number of arguments Exception");
    }

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[0]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "ShaderSource : argument 1 is not WebGLShader");
    }

    if (!args[1]->IsString())
    {
        return THROW_EXCEPTION(TError, "ShaderSource : argument 2 is not string");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    v8::String::AsciiValue value(args[1]);
    std::string source(*value);

    //shader->setSource(source);
    context->shaderSource(shader, source);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CompileShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "CompileShader : number of arguments Exception");
    }

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[0]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "ShaderSource : argument 1 is not WebGLShader");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->compileShader(shader);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "CreateProgram : arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLProgram* program = context->createProgram();

    return program->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::AttachShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "AttachShader :arguments Exception!!");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "ShaderSource : argument 1 is not WebGLProgram");
    }

    WebGLShader* shader = NULL;

    if (WebGLShader::HasInstance(args[1]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[1]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "ShaderSource : argument 2 is not WebGLShader");
    }

    context->attachShader(program, shader);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::LinkProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "LinkProgram : number of arguments Exception");
    }

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "LinkProgram : argument 1 is not WebGLProgram");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->linkProgram(program);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UseProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "UseProgram : number of arguments Exception");
    }

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "LinkProgram : argument 1 is not WebGLProgram");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->useProgram(program);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformfvImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "UniformfvImpl : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "UniformfvImpl : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCint location = args[0]->Uint32Value();

    Float32Array* farry = NULL;
    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        farry = Float32Array::create(len);
        for (int i = 0; i < len; ++i)
        {
            farry->set(i, tmp->Get(v8::Int32::New(i))->NumberValue());
        }
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[1]))
    {
        farry = ObjectWrap::Unwrap<Float32Array>(args[1]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "UniformfvImpl : argument 2 is not match");
    }

    switch (index)
    {
        case 1:
            context->uniform1fv(location, farry->length(), farry->data());
            break;
        case 2:
            context->uniform2fv(location, farry->length() / 2, farry->data());
            break;
        case 3:
            context->uniform3fv(location, farry->length() / 3, farry->data());
            break;
        case 4:
            context->uniform4fv(location, farry->length() / 4, farry->data());
            break;
        default:
            ASSERT_NOT_REACHED();
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformivImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "UniformivImpl : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "UniformivImpl : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCint location = args[0]->Uint32Value();

    Int32Array* iarry = NULL;
    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        iarry = Int32Array::create(len);
        for (int i = 0; i < len; ++i)
        {
            iarry->set(i, tmp->Get(v8::Int32::New(i))->Int32Value());
        }
    }
    else if (V8ArrayType<Int32Array>::HasInstance(args[1]))
    {
        iarry = ObjectWrap::Unwrap<Int32Array>(args[1]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "UniformivImpl : argument 2 is not match");
    }

    switch (index)
    {
        case 1:
            context->uniform1iv(location, iarry->length(), iarry->data());
            break;
        case 2:
            context->uniform2iv(location, iarry->length() / 2, iarry->data());
            break;
        case 3:
            context->uniform3iv(location, iarry->length() / 3, iarry->data());
            break;
        case 4:
            context->uniform4iv(location, iarry->length() / 4, iarry->data());
            break;
        default:
            ASSERT_NOT_REACHED();
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform1f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "Uniform1f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "Uniform1f : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCint location = args[0]->Uint32Value();
    float value = args[1]->NumberValue();
    context->uniform1f(location, value);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform1fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformfvImpl(args, 1);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform1i(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "Uniform1i : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Uniform1i : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCint location = args[0]->Uint32Value();
    int value = args[1]->Int32Value();
    context->uniform1i(location, value);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform1iv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformivImpl(args, 1);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform2f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "Uniform2f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber() || !args[2]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "Uniform2f : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();
    context->uniform2f(location, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform2fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformfvImpl(args, 2);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform2i(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "Uniform2i : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Uniform2i : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    int x = args[1]->Int32Value();
    int y = args[2]->Int32Value();

    context->uniform2i(location, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform2iv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformivImpl(args, 2);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform3f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "Uniform3f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "Uniform3f : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();
    float z = args[3]->NumberValue();

    context->uniform3f(location, x, y, z);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform3fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformfvImpl(args, 3);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform3i(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "Uniform3i : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Uniform3i : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    int x = args[1]->Int32Value();
    int y = args[2]->Int32Value();
    int z = args[3]->Int32Value();

    context->uniform3i(location, x, y, z);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform3iv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformivImpl(args, 3);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform4f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 5)
    {
        return THROW_EXCEPTION(TError, "Uniform4f : numebr of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber() || !args[2]->IsNumber()
     || !args[3]->IsNumber() || !args[4]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "Uniform4f : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();
    float z = args[3]->NumberValue();
    float w = args[4]->NumberValue();

    context->uniform4f(location, x, y, z, w);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform4fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformfvImpl(args, 4);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform4i(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 5)
    {
        return THROW_EXCEPTION(TError, "Uniform4i : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
     || !args[3]->IsInt32() || !args[4]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Uniform4i : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint location = args[0]->Uint32Value();
    int x = args[1]->Int32Value();
    int y = args[2]->Int32Value();
    int z = args[3]->Int32Value();
    int w = args[4]->Int32Value();

    context->uniform4i(location, x, y, z, w);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Uniform4iv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformivImpl(args, 4);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformMatrixfvImpl(const v8::FunctionCallbackInfo<v8::Value>& args, DCint index)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "UniformMatrixfvImpl : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsBoolean())
    {
        return THROW_EXCEPTION(TError, "UniformMatrixfvImpl : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCint location = args[0]->Uint32Value();
    bool transpose = args[1]->BooleanValue();
    Float32Array* farry = NULL;

    if (args[2]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[2]);
        int len = array->Length();
        v8::Local<v8::Object> tmp = args[2]->ToObject();
        farry = Float32Array::create(len);
        for (int i = 0; i < len; ++i)
        {
            farry->set(i, tmp->Get(v8::Int32::New(i))->NumberValue());
        }
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[2]))
    {
        farry = ObjectWrap::Unwrap<Float32Array>(args[2]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "UniformMatrixfvImpl : argument 3 is not match");
    }

    switch (index)
    {
        case 2:
            context->uniformMatrix2fv(location, 1, transpose, farry);
            break;
        case 3:
            context->uniformMatrix3fv(location, 1, transpose, farry);
            break;
        case 4:
            context->uniformMatrix4fv(location, 1, transpose, farry);
            break;
        default:
            ASSERT_NOT_REACHED();
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformMatrix2fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformMatrixfvImpl(args, 2);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformMatrix3fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformMatrixfvImpl(args, 3);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::UniformMatrix4fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return UniformMatrixfvImpl(args, 4);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetAttribLocation(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetAttribLocation : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetAttribLocation : argument 1 is not WebGLProgram");
    }

    if (!args[1]->IsString())
        return THROW_EXCEPTION(TError, "GetAttribLocation : argument 2 is not string");

    v8::String::AsciiValue value(args[1]);
    std::string name(*value);

    DCuint location = context->getAttribLocation(program, name);

    return v8::Uint32::New(location);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetUniformLocation(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetUniformLocation : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetUniformLocation : argument 1 is not WebGLProgram");
    }

    if (!args[1]->IsString())
        return THROW_EXCEPTION(TError, "GetUniformLocation : argument 2 is not string");

    v8::String::AsciiValue value(args[1]);
    std::string name(*value);

    DCuint location = context->getUniformLocation(program, name);

    return v8::Uint32::New(location);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::EnableVertexAttribArray(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "EnableVertexAttribArray : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "EnableVertexAttribArray : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCuint attrib = args[0]->Uint32Value();

    context->enableVertexAttribArray(attrib);

    return v8::Undefined();
}


v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttribPointer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 6)
    {
        return THROW_EXCEPTION(TError, "VertexAttribPointer : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
     || !args[3]->IsBoolean() || !args[4]->IsInt32() || !args[5]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "VertexAttribPointer : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCuint index = args[0]->Uint32Value();
    DCint size = args[1]->Int32Value();
    DCenum type = args[2]->Uint32Value();
    DCboolean normalized = args[3]->BooleanValue();
    DCsizei stride = args[4]->Int32Value();
    DCintptr offset = args[5]->Int32Value();

    context->vertexAttribPointer(index, size, type, normalized, stride, offset);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DrawArrays(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "DrawArrays : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "DrawArrays : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum mode = args[0]->Uint32Value();
    DCint first = args[1]->Int32Value();
    DCsizei count = args[2]->Int32Value();

    if (first < 0 || count < 0)
    {
        return THROW_EXCEPTION(TError,
            "DrawArrays : argument first or count is invalid Exception!!");
    }

    if (!count)
        return v8::Undefined();

    context->drawArrays(mode, first, count);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DepthFunc(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DepthFunc : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "DepthFunc : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum func = args[0]->Uint32Value();

    context->depthFunc(func);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ClearDepth(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "ClearDepth : number of arguments Exception");
    }

    if (!args[0]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "ClearDepth : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCclampf depth = args[0]->NumberValue();

    context->clearDepth(depth);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Flush(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "Flush : arguments Exception!!");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->flush();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DrawElements(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "DrawElements : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "DrawElements : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum mode = args[0]->Uint32Value();
    DCsizei count = args[1]->Int32Value();
    DCenum type = args[2]->Uint32Value();
    DCintptr indices = args[3]->Int32Value();

    context->drawElements(mode, count, type, indices);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetShaderInfoLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GetShaderInfoLog : number of arguments Exception");
    }

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[0]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetShaderInfoLog : argument 1 is not WebGLShader");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    std::string infoLog = context->getShaderInfoLog(shader);

    return v8::String::New(infoLog.c_str());
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Disable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "Disable : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Disable : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum cap = args[0]->Uint32Value();

    context->disable(cap);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::TexImage2D(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCuint len = args.Length();

    DCenum target, internalformat, format, type;
    Image* img;
    ArrayBufferView* abv;
    DCint level;
    DCsizei width, height;
    DCint border;

    switch (len)
    {
        case 6:
            if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
             || !args[3]->IsInt32() || !args[4]->IsInt32())
            {
                return THROW_EXCEPTION(TError, "TexImage2D : arguments wrong type");
            }

            target = args[0]->Uint32Value();
            level = args[1]->Int32Value();
            internalformat = args[2]->Uint32Value();
            format = args[3]->Uint32Value();
            type = args[4]->Uint32Value();

            if (args[5]->IsNull())
            {
                img = NULL;
            }
            else if (V8Image::HasInstance(args[5]))
            {
                img = ObjectWrap::Unwrap<Image>(args[5]->ToObject());
            }
            else
            {
                return THROW_EXCEPTION(TError, "TexImage2D : argument 6 is not Image");
            }

            context->texImage2D(target, level, internalformat, format, type, img);
            break;

        case 9:
            if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
             || !args[3]->IsInt32() || !args[4]->IsInt32() || !args[5]->IsInt32()
             || !args[6]->IsInt32() || !args[7]->IsInt32())
            {
                return THROW_EXCEPTION(TError, "TexImage2D : arguments wrong type");
            }

            target = args[0]->Uint32Value();
            level = args[1]->Int32Value();
            internalformat = args[2]->Uint32Value();
            width = args[3]->Int32Value();
            height = args[4]->Int32Value();
            border = args[5]->Int32Value();
            format = args[6]->Uint32Value();
            type = args[7]->Uint32Value();

            if (args[8]->IsNull())
            {
                abv = NULL;
            }
            else if (V8ArrayBufferView::HasInstance(args[8]))
            {
                abv = ObjectWrap::Unwrap<ArrayBufferView>(args[8]->ToObject());
            }
            else
            {
                return THROW_EXCEPTION(TError, "TexImage2D : arguments 9 is not arraybufferView");
            }

            context->texImage2D(target, level, internalformat, width, height, border, format, type, abv);
            break;

        default:
        {
            return THROW_EXCEPTION(TError, "TexImage2D : number of arguments Exception");
        }
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::TexParameterf(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "TexParameterf : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "TexParameterf : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();
    DCfloat param = args[2]->NumberValue();

    context->texParameterf(target, pname, param);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::TexParameteri(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "TexParameteri : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "TexParameteri : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();
    DCint param = args[2]->Int32Value();

    context->texParameteri(target, pname, param);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ActiveTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "ActiveTexture : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "ActiveTexture : arguments wrong type!!");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();

    context->activeTexture(target);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GenerateMipmap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope Scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GenerateMipmap : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GenerateMipmap : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum type = args[0]->Uint32Value();

    context->generateMipmap(type);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::PixelStorei(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope Scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "PixelStorei : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || (!args[1]->IsInt32() && !args[1]->IsBoolean()))
    {
        return THROW_EXCEPTION(TError, "PixelStorei : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum pname = args[0]->Uint32Value();
    DCint param = args[1]->Int32Value();

    context->pixelStorei(pname, param);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "CreateRenderbuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLRenderBuffer* render_buffer = context->createRenderbuffer();

    return render_buffer->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CreateFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "CreateFramebuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLFrameBuffer* frame_buffer = context->createFramebuffer();

    return frame_buffer->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BindRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BindRenderbuffer : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BindRenderbuffer : arguments wrong type");
    }

    WebGLRenderBuffer* buffer = NULL;

    if (WebGLRenderBuffer::HasInstance(args[1]))
    {
        buffer = ObjectWrap::Unwrap<WebGLRenderBuffer>(args[1]->ToObject());
    }
    else if(!args[1]->IsNull())
    {
        return THROW_EXCEPTION(TError, "BindRenderbuffer : argument 2 is not WebGLRenderBuffer");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();

    if (!context->bindRenderbuffer(target, buffer))
    {
        return THROW_EXCEPTION(TError, "BindRenderbuffer : bindRenderbuffer failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::RenderbufferStorage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "RenderbufferStorage : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "RenderbufferStorage : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();
    DCenum info = args[1]->Uint32Value();
    DCsizei width = args[2]->Int32Value();
    DCsizei height = args[3]->Int32Value();

    context->renderbufferStorage(target, info, width, height);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::FramebufferTexture2D(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 5)
    {
        return THROW_EXCEPTION(TError, "FramebufferTexture2D : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[4]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "FramebufferTexture2D : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();
    DCenum attachment = args[1]->Uint32Value();
    DCenum textarget = args[2]->Uint32Value();
    DCint  level = args[4]->Int32Value();

    WebGLTexture* texture = NULL;ObjectWrap::Unwrap<WebGLTexture>(args[3]->ToObject());
    if (WebGLTexture::HasInstance(args[3]))
    {
        texture = ObjectWrap::Unwrap<WebGLTexture>(args[3]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "FramebufferTexture2D : argument 4 is not WebGLTexture");
    }

    if (!texture || !texture->getObject())
        return THROW_EXCEPTION(TError, "FramebufferTexture2D : WebGLTexture Null Exception");

    context->framebufferTexture2D(target, attachment, textarget, texture, level);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::FramebufferRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "FramebufferRenderbuffer : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "FramebufferRenderbuffer : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum target = args[0]->Uint32Value();
    DCenum attachment = args[1]->Uint32Value();
    DCenum renderbuffertarget = args[2]->Uint32Value();

    WebGLRenderBuffer* renderbuffer = NULL;
    if (WebGLRenderBuffer::HasInstance(args[3]))
    {
        renderbuffer = ObjectWrap::Unwrap<WebGLRenderBuffer>(args[3]->ToObject());
    }
    else if (!args[3]->IsNull())
    {
        return THROW_EXCEPTION(TError, "FramebufferRenderbuffer : argument 4 is not WebGLRenderBuffer");
    }

    context->framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BindFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BindFramebuffer : numbwer of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BindFramebuffer : arguments wrong type");
    }

    WebGLFrameBuffer* buffer = NULL;

    if (WebGLFrameBuffer::HasInstance(args[1]))
    {
        buffer = ObjectWrap::Unwrap<WebGLFrameBuffer>(args[1]->ToObject());
    }
    else if (!args[1]->IsNull())
    {
        return THROW_EXCEPTION(TError, "BindFramebuffer : argument 2 is not WebGLFrameBuffer");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();

    context->bindFramebuffer(target, buffer);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BlendColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "BlendColor : number of arguments Exception");
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "BlendColor : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCfloat red = args[0]->NumberValue();
    DCfloat green = args[1]->NumberValue();
    DCfloat blue = args[2]->NumberValue();
    DCfloat alpha = args[3]->NumberValue();

    context->blendColor(red, green, blue, alpha);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BlendEquation(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "BlendEquation : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BlendEquation : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum mode = args[0]->Uint32Value();

    context->blendEquation(mode);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BlendEquationSeparate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BlendEquationSeparate : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BlendEquationSeparate : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum modeRGB = args[0]->Uint32Value();
    DCenum modeAlpha = args[1]->Uint32Value();

    context->blendEquationSeparate(modeRGB, modeAlpha);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BlendFunc(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "BlendFunc : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BlendFunc : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum sfactor = args[0]->Uint32Value();
    DCenum dfactor = args[1]->Uint32Value();

    context->blendFunc(sfactor, dfactor);

    return v8::Undefined();
}


v8::Handle<v8::Value> V8WebGLGraphicsContext::BlendFuncSeparate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "BlendFuncSeparate : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "BlendFuncSeparate : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum srcRGB = args[0]->Uint32Value();
    DCenum dstRGB = args[1]->Uint32Value();
    DCenum srcAlpha = args[2]->Uint32Value();
    DCenum dstAlpha = args[3]->Uint32Value();

    context->blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::SampleCoverage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "SampleCoverage : number of arguments Exception");
    }

    if (!args[0]->IsNumber() || !args[1]->IsBoolean())
    {
        return THROW_EXCEPTION(TError, "SampleCoverage : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCfloat value = args[0]->NumberValue();
    DCboolean invert = args[1]->BooleanValue();

    context->sampleCoverage(value, invert);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::StencilFuncSeparate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "StencilFuncSeparate : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "StencilFuncSeparate : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum face = args[0]->Uint32Value();
    DCenum func = args[1]->Uint32Value();
    DCint ref = args[2]->Int32Value();
    DCuint mask = args[3]->Uint32Value();

    context->stencilFuncSeparate(face, func, ref, mask);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::StencilOp(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "StencilOp : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "StencilOp : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum fail = args[0]->Uint32Value();
    DCenum zfail = args[1]->Uint32Value();
    DCenum zpass = args[2]->Uint32Value();

    context->stencilOp(fail, zfail, zpass);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::StencilOpSeparate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "StencilOpSeparate : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32()
    || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "StencilOpSeparate : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum face = args[0]->Uint32Value();
    DCenum fail = args[1]->Uint32Value();
    DCenum zfail = args[2]->Uint32Value();
    DCenum zpass = args[3]->Uint32Value();

    context->stencilOpSeparate(face, fail, zfail, zpass);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ClearStencil(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "ClearStencil : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "ClearStencil : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint s = args[0]->Int32Value();

    context->clearStencil(s);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ColorMask(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "ColorMask : number of arguments Exception");
    }

    if (!args[0]->IsBoolean() || !args[1]->IsBoolean()
    || !args[2]->IsBoolean() || !args[3]->IsBoolean())
    {
        return THROW_EXCEPTION(TError, "ColorMask : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCboolean red = args[0]->BooleanValue();
    DCboolean green = args[1]->BooleanValue();
    DCboolean blue = args[2]->BooleanValue();
    DCboolean alpha = args[3]->BooleanValue();

    context->colorMask(red, green, blue, alpha);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DepthMask(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "DepthMask : number of arguments Exception");
    }

    if (!args[0]->IsBoolean())
    {
        return THROW_EXCEPTION(TError, "DepthMask : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCboolean flag = args[0]->BooleanValue();

    context->depthMask(flag);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::StencilMask(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "StencilMask : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "StencilMask : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCuint mask = args[0]->Uint32Value();

    context->stencilMask(mask);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::StencilMaskSeparate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "StencilMaskSeparate : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "StencilMaskSeparate : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum face = args[0]->Uint32Value();
    DCuint mask = args[1]->Uint32Value();

    context->stencilMaskSeparate(face, mask);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DepthRange(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "DepthRange : number of arguments Exception");
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "DepthRange : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCfloat zNear = args[0]->NumberValue();
    DCfloat zFar = args[1]->NumberValue();

    context->depthRange(zNear, zFar);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Scissor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "Scissor : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Scissor : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCint x = args[0]->Int32Value();
    DCint y = args[1]->Int32Value();
    DCint width = args[2]->Int32Value();
    DCint height = args[3]->Int32Value();

    context->scissor(x, y, width, height);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CullFace(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "CullFace : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "CullFace : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum mode = args[0]->Uint32Value();

    context->cullFace(mode);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::FrontFace(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "FrontFace : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "FrontFace : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCenum mode = args[0]->Uint32Value();

    context->frontFace(mode);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::LineWidth(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "LineWidth : number of arguments Exception");
    }

    if (!args[0]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "LineWidth : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCfloat width = args[0]->NumberValue();

    context->lineWidth(width);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::PolygonOffset(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "PolygonOffset : number of arguments Exception");
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "PolygonOffset : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCfloat factor = args[0]->NumberValue();
    DCfloat units = args[1]->NumberValue();

    context->polygonOffset(factor, units);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::BindAttribLocation(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "BindAttribLocation : number of arguments Exception");
    }

    if ( !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "BindAttribLocation : argument 2 is not number");
    }

    WebGLProgram* program = NULL;

    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "BindAttribLocation : argument 1 is not WebGLProgram");
    }

    if (!args[2]->IsString())
        return THROW_EXCEPTION(TError, "BindAttribLocation : argument 3 is not string");

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    DCuint index = args[1]->NumberValue();
    v8::String::AsciiValue value(args[2]);
    std::string name(*value);

    context->bindAttribLocation(program, index, name);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DetachShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "DetachShader : number of arguments Exception");
    }

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DetachShader : argument 1 is not WebGLProgram");
    }

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[1]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[1]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "DetachShader : argument 2 is not WebGLShader");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!context->detachShader(program, shader))
    {
        return THROW_EXCEPTION(TError, "DetachShader : detachShader failed");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetAttachedShaders(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GetAttachedShaders : number of arguments Exception");
    }

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetAttachedShaders : argument 1 is not WebGLProgram");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    std::vector<WebGLShader*> shaderObjects;

    if (!context->getAttachedShaders(program, shaderObjects))
    {
        return THROW_EXCEPTION(TError, "GetAttachedShaders : getAttachedShaders failed");
    }

    DCint size = shaderObjects.size();
    LOGD("GetAttachedShaders   ---------------  size :%d", size);
    v8::Local<v8::Array> arr = v8::Array::New(size);

    for (int i = 0; i < size; i++)
    {
        //v8::Handle<v8::Object> obj = v8::Object::New();
        //obj->SetPointerInInternalField(0,shaderObjects[i]);
        arr->Set(v8::Int32::New(i), shaderObjects[i]->wrap());
    }
    return scope.Close(v8::Local<v8::Value>::New(arr));
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetProgramInfoLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GetProgramInfoLog : number of arguments Exception");
    }

    WebGLProgram* program = NULL;
    if (WebGLProgram::HasInstance(args[0]))
    {
        program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetProgramInfoLog : argument 1 is not WebGLProgram");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    std::string infoLog = context->getProgramInfoLog(program);

    return v8::String::New(infoLog.c_str());
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetShaderSource(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GetShaderSource : number of arguments Exception");
    }

    WebGLShader* shader = NULL;
    if (WebGLShader::HasInstance(args[0]))
    {
        shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());
    }
    else
    {
        return THROW_EXCEPTION(TError, "GetShaderSource : argument 1 is not WebGLShader");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    std::string src = context->getShaderSource(shader);

    return v8::String::New(src.c_str());
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsBuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLBuffer::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsBuffer : argument is invalid");
    }

    WebGLBuffer* buffer = ObjectWrap::Unwrap<WebGLBuffer>(args[0]->ToObject());

    DCboolean out = context->isBuffer(buffer);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsProgram : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLProgram::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsProgram : argument is invalid");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());

    DCboolean out = context->isProgram(program);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsShader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsShader : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLShader::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsShader : argument is invalid");
    }

    WebGLShader* shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());

    DCboolean out = context->isShader(shader);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsTexture(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsTexture : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLTexture::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsTexture : argument is invalid");
    }

    WebGLTexture* texture = ObjectWrap::Unwrap<WebGLTexture>(args[0]->ToObject());

    DCboolean out = context->isTexture(texture);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsRenderbuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsRenderbuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLRenderBuffer::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsRenderbuffer : argument is invalid");
    }

    WebGLRenderBuffer* rbuffer = ObjectWrap::Unwrap<WebGLRenderBuffer>(args[0]->ToObject());

    DCboolean out = context->isRenderbuffer(rbuffer);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsFramebuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsFramebuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLFrameBuffer::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "IsFramebuffer : argument is invalid");
    }

    WebGLFrameBuffer* fbuffer = ObjectWrap::Unwrap<WebGLFrameBuffer>(args[0]->ToObject());

    DCboolean out = context->isFramebuffer(fbuffer);
    return v8::Boolean::New(out);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ValidateProgram(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "ValidateProgram : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (!WebGLProgram::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "ValidateProgram : argument is invalid");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());

    context->validateProgram(program);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Finish(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "Finish arguments Exception!!");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->finish();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetError(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError, "GetError arguments Exception!!");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    return v8::Uint32::New(context->getError());
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::DisableVertexAttribArray(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError,
                "DisableVertexAttribArray : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "DisableVertexAttribArray : argument 1 wrong type");
    }

    DCuint index = args[0]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->disableVertexAttribArray(index);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::IsEnabled(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "IsEnabled : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "IsEnabled : argument 1 wrong type");
    }

    DCuint cap = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    return v8::Boolean::New(context->isEnabled(cap));
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CopyTexImage2D(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 8)
    {
        return THROW_EXCEPTION(TError, "CopyTexImage2D : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
     || !args[3]->IsInt32() || !args[4]->IsInt32() || !args[5]->IsInt32()
     || !args[6]->IsInt32() || !args[7]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "CopyTexImage2D : arguments wrong type");
    }

    DCenum target = args[0]->Uint32Value();
    DCint level = args[1]->Int32Value();
    DCenum internalformat = args[2]->Uint32Value();
    DCint x = args[3]->Int32Value();
    DCint y = args[4]->Int32Value();
    DCsizei width = args[5]->Int32Value();
    DCsizei height = args[6]->Int32Value();
    DCint border = args[7]->Int32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->copyTexImage2D(target, level, internalformat, x, y, width, height, border);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CopyTexSubImage2D(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 8)
    {
        return THROW_EXCEPTION(TError, "CopyTexSubImage2D : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
     || !args[3]->IsInt32() || !args[4]->IsInt32() || !args[5]->IsInt32()
     || !args[6]->IsInt32() || !args[7]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "CopyTexSubImage2D : arguments wrong type");
    }

    DCenum target = args[0]->Uint32Value();
    DCint level = args[1]->Int32Value();
    DCint xoffset = args[2]->Int32Value();
    DCint yoffset = args[3]->Int32Value();
    DCint x = args[4]->Int32Value();
    DCint y = args[5]->Int32Value();
    DCsizei width = args[6]->Int32Value();
    DCsizei height = args[7]->Int32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::TexSubImage2D(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCuint len = args.Length();

    DCenum target, internalformat, format, type;
    Image* img;
    ArrayBufferView* abv;
    DCint level, xoffset, yoffset;
    DCsizei width, height;
    DCint border;

    switch (len)
    {
        case 7:
            if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
             || !args[3]->IsInt32() || !args[4]->IsInt32() || !args[5]->IsInt32())
            {
                return THROW_EXCEPTION(TError, "TexSubImage2D : arguments wrong type");
            }

            target = args[0]->Uint32Value();
            level = args[1]->Int32Value();
            xoffset = args[2]->Int32Value();
            yoffset = args[3]->Int32Value();
            format = args[4]->Uint32Value();
            type = args[5]->Uint32Value();

            if (args[6]->IsNull())
            {
                img = NULL;
            }
            else if (V8Image::HasInstance(args[6]))
            {
                img = ObjectWrap::Unwrap<Image>(args[6]->ToObject());
            }
            else
            {
                return THROW_EXCEPTION(TError, "TexSubImage2D : argument 7 is not match");
            }

            context->texSubImage2D(target, level, xoffset, yoffset, format, type, img);
            break;

        case 9:
            if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32()
             || !args[3]->IsInt32() || !args[4]->IsInt32() || !args[5]->IsInt32()
             || !args[6]->IsInt32() || !args[7]->IsInt32())
            {
                return THROW_EXCEPTION(TError, "TexSubImage2D : arguments wrong type");
            }
            target = args[0]->Uint32Value();
            level = args[1]->Int32Value();
            xoffset = args[2]->Int32Value();
            yoffset = args[3]->Int32Value();
            width = args[4]->Int32Value();
            height = args[5]->Int32Value();
            format = args[6]->Uint32Value();
            type = args[7]->Uint32Value();

            if (args[8]->IsNull())
            {
                abv = NULL;
            }
            else if (V8ArrayBufferView::HasInstance(args[8]))
            {
                abv = ObjectWrap::Unwrap<ArrayBufferView>(args[8]->ToObject());
            }
            else
            {
                return THROW_EXCEPTION(TError, "TexSubImage2D : argument 9 is not match");
            }

            context->texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, abv);
            break;

        default:
            return THROW_EXCEPTION(TError, "TexSubImage2D : number of arguments Exception");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::Hint(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "Hint : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "Hint : arguments wrong type");
    }

    DCenum target = args[0]->Uint32Value();
    DCenum mode = args[1]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    context->hint(target, mode);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib1f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1f : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    DCfloat x = args[1]->NumberValue();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->vertexAttrib1f(index, x);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib2f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib2f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber() || !args[2]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib2f : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    DCfloat x = args[1]->NumberValue();
    DCfloat y = args[2]->NumberValue();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->vertexAttrib2f(index, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib3f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib3f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber()
    || !args[2]->IsNumber() || !args[3]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib3f : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    DCfloat x = args[1]->NumberValue();
    DCfloat y = args[2]->NumberValue();
    DCfloat z = args[3]->NumberValue();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->vertexAttrib3f(index, x, y, z);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib4f(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 5)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib4f : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsNumber() || !args[2]->IsNumber()
    || !args[3]->IsNumber() || !args[4]->IsNumber())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib4f : arguments wrong type");
    }
    DCenum index = args[0]->Uint32Value();
    DCfloat x = args[1]->NumberValue();
    DCfloat y = args[2]->NumberValue();
    DCfloat z = args[3]->NumberValue();
    DCfloat w = args[4]->NumberValue();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->vertexAttrib4f(index, x, y, z, w);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib1fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1fv : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1fv : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        DCfloat values[len];
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        for (int i = 0; i < len; ++i)
        {
            values[i] = tmp->Get(v8::Int32::New(i))->Int32Value();
        }
        context->vertexAttrib1fv(index, values, len);
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[1]))
    {
        Float32Array* values = ObjectWrap::Unwrap<Float32Array>(args[1]->ToObject());
        context->vertexAttrib1fv(index, values);
    }
    else
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1fv argument 2 is not match");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib2fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib2fv : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib2fv : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        DCfloat values[len];
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        for (int i = 0; i < len; ++i)
        {
            values[i] = tmp->Get(v8::Int32::New(i))->Int32Value();
        }
        context->vertexAttrib2fv(index, values, len);
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[1]))
    {
        Float32Array* values = ObjectWrap::Unwrap<Float32Array>(args[1]->ToObject());
        context->vertexAttrib2fv(index, values);
    }
    else
    {
        return THROW_EXCEPTION(TError, "VertexAttrib2fv : argument 2 is not match");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib3fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib3fv : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib3fv : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        DCfloat values[len];
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        for (int i = 0; i < len; ++i)
        {
            values[i] = tmp->Get(v8::Int32::New(i))->Int32Value();
        }
        context->vertexAttrib3fv(index, values, len);
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[1]))
    {
        Float32Array* values = ObjectWrap::Unwrap<Float32Array>(args[1]->ToObject());
        context->vertexAttrib3fv(index, values);
    }
    else
    {
        return THROW_EXCEPTION(TError, "VertexAttrib3fv : argument 2 is not match");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::VertexAttrib4fv(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "VertexAttrib4fv : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "VertexAttrib4fv : arguments wrong type");
    }

    DCenum index = args[0]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());

    if (args[1]->IsArray())
    {
        v8::Array* array = v8::Array::Cast(*args[1]);
        int len = array->Length();
        DCfloat values[len];
        v8::Local<v8::Object> tmp = args[1]->ToObject();
        for (int i = 0; i < len; ++i)
        {
            values[i] = tmp->Get(v8::Int32::New(i))->Int32Value();
        }
        context->vertexAttrib4fv(index, values, len);
    }
    else if (V8ArrayType<Float32Array>::HasInstance(args[1]))
    {
        Float32Array* values = ObjectWrap::Unwrap<Float32Array>(args[1]->ToObject());
        context->vertexAttrib4fv(index, values);
    }
    else
    {
        return THROW_EXCEPTION(TError, "VertexAttrib1fv : argument 2 is not match");
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetProgramParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetProgramParameter : number of arguments Exception");
    }

    if (!WebGLProgram::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "GetProgramParameter : arguments Exception");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum pname = args[1]->Uint32Value();
    WebGLGetInfo info = context->getProgramParameter(program, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeBool:
            return v8::Boolean::New(info.getBool());
            break;
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
            break;
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetShaderParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetShaderParameter : number of arguments Exception");
    }

    if (!WebGLShader::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TError, "GetShaderParameter : arguments Exception");
    }

    WebGLShader* shader = ObjectWrap::Unwrap<WebGLShader>(args[0]->ToObject());

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum pname = args[1]->Uint32Value();
    WebGLGetInfo info = context->getShaderParameter(shader, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeBool:
            return v8::Boolean::New(info.getBool());
            break;
        case WebGLGetInfo::kTypeUnsignedInt:
            return v8::Uint32::New(info.getUnsignedInt());
            break;
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetTexParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetTexParameter : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetTexParameter : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();
    WebGLGetInfo info = context->getTexParameter(target, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeUnsignedInt:
            return v8::Uint32::New(info.getUnsignedInt());
            break;
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetRenderbufferParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetRenderbufferParameter : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetRenderbufferParameter : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();
    WebGLGetInfo info = context->getRenderbufferParameter(target, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
            break;
        case WebGLGetInfo::kTypeUnsignedInt:
            return v8::Uint32::New(info.getUnsignedInt());
            break;
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetFramebufferAttachmentParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 3)
    {
        return THROW_EXCEPTION(TError,
            "GetFramebufferAttachmentParameter : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32())
    {
        return THROW_EXCEPTION(TError,
            "GetFramebufferAttachmentParameter : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum target = args[0]->Uint32Value();
    DCenum attachment = args[1]->Uint32Value();
    DCenum pname = args[2]->Uint32Value();
    WebGLGetInfo info = context->getFramebufferAttachmentParameter(target, attachment, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
            break;
        case WebGLGetInfo::kTypeUnsignedInt:
            return v8::Uint32::New(info.getUnsignedInt());
            break;
        case WebGLGetInfo::kTypeWebGLTexture:
            return info.getWebGLTexture()->wrap(args);
            break;
        case WebGLGetInfo::kTypeWebGLRenderBuffer:
            return info.getWebGLRenderBuffer()->wrap(args);
            break;
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetParameter(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "GetParameter : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetParameter : arguments wrong type");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum pname = args[0]->Uint32Value();
    WebGLGetInfo info = context->getParameter(pname);

    std::vector<bool> arrayb;
    v8::Local<v8::Array> value;
    switch (info.getType())
    {
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
        case WebGLGetInfo::kTypeUnsignedInt:
            return v8::Uint32::New(info.getUnsignedInt());
        case WebGLGetInfo::kTypeBool:
            return v8::Boolean::New(info.getBool());
        case WebGLGetInfo::kTypeBoolArray:
            arrayb = info.getBoolArray();
            value = v8::Array::New(arrayb.size());
            for (unsigned i = 0; i < arrayb.size(); i++)
                value->Set(v8::Int32::New(i), v8::Boolean::New(arrayb[i]));
            return value;
        case WebGLGetInfo::kTypeFloat:
            return v8::Number::New(info.getFloat());
        case WebGLGetInfo::kTypeNull:
            return v8::Null();
        case WebGLGetInfo::kTypeString:
            return v8::String::New(info.getString().c_str());
        case WebGLGetInfo::kTypeWebGLBuffer:
            return info.getWebGLBuffer()->wrap(args);
        case WebGLGetInfo::kTypeWebGLFloatArray:
        	return V8ArrayType<Float32Array>::Wrap(info.getWebGLFloatArray());
//            return info.getWebGLFloatArray()->wrap(args);
        case WebGLGetInfo::kTypeWebGLFrameBuffer:
            return info.getWebGLFrameBuffer()->wrap(args);
        case WebGLGetInfo::kTypeWebGLIntArray:
        	return V8ArrayType<Int32Array>::Wrap(info.getWebGLIntArray());
//            return info.getWebGLIntArray()->wrap(args);
        case WebGLGetInfo::kTypeWebGLProgram:
            return info.getWebGLProgram()->wrap(args);
        case WebGLGetInfo::kTypeWebGLRenderBuffer:
            return info.getWebGLRenderBuffer()->wrap(args);
        case WebGLGetInfo::kTypeWebGLTexture:
            return info.getWebGLTexture()->wrap(args);
        case WebGLGetInfo::kTypeWebGLUnsignedByteArray:
        	return V8ArrayType<Uint8Array>::Wrap(info.getWebGLUnsignedByteArray());
//            return info.getWebGLUnsignedByteArray()->wrap(args);
        case WebGLGetInfo::kTypeWebGLObjectArray:
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetVertexAttrib(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetVertexAttrib : number of arguments Exception");
    }

    if (!args[0]->IsInt32() || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetVertexAttrib : arguments wrong type");
    }

    DCuint index = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();
    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLGetInfo info = context->getVertexAttrib(index, pname);

    switch (info.getType())
    {
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
        case WebGLGetInfo::kTypeWebGLFloatArray:
        	return V8ArrayType<Float32Array>::Wrap(info.getWebGLFloatArray());
 //           return info.getWebGLFloatArray()->wrap(args);
        case WebGLGetInfo::kTypeBool:
            return v8::Boolean::New(info.getBool());
        case WebGLGetInfo::kTypeString:
            return v8::String::New(info.getString().c_str());
        case WebGLGetInfo::kTypeWebGLBuffer:
            return info.getWebGLBuffer()->wrap(args);
        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetVertexAttribOffset(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetVertexAttribOffset : number of arguments Exception");
    }

    DCuint index = args[0]->Uint32Value();
    DCenum pname = args[1]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    return v8::Int32::New(context->getVertexAttribOffset(index, pname));
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetActiveAttrib(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetActiveAttrib : number of arguments Exception");
    }

    if (!WebGLProgram::HasInstance(args[0]) || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetActiveAttrib : arguments wrong type");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    DCuint index = args[1]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLActiveInfo* info = context->getActiveAttrib(program, index);
    return info->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetActiveUniform(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetActiveUniform : number of arguments Exception");
    }

    if (!WebGLProgram::HasInstance(args[0]) || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetActiveUniform : arguments wrong type");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    DCuint index = args[1]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLActiveInfo* info = context->getActiveUniform(program, index);
    return info->wrap(args);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::GetUniform(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TError, "GetUniform : number of arguments Exception");
    }

    if (!WebGLProgram::HasInstance(args[0]) || !args[1]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "GetUniform : arguments wrong type");
    }

    WebGLProgram* program = ObjectWrap::Unwrap<WebGLProgram>(args[0]->ToObject());
    DCuint location = args[1]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    WebGLGetInfo info = context->getUniform(program, location);

    std::vector<bool> arrayb;
    v8::Local<v8::Array> value;
    switch (info.getType())
    {
        case WebGLGetInfo::kTypeInt:
            return v8::Int32::New(info.getInt());
        case WebGLGetInfo::kTypeBool:
            return v8::Boolean::New(info.getBool());
        case WebGLGetInfo::kTypeBoolArray:
            arrayb = info.getBoolArray();
            value = v8::Array::New(arrayb.size());
            for (unsigned i = 0; i < arrayb.size(); i++)
                value->Set(v8::Int32::New(i), v8::Boolean::New(arrayb[i]));
            return value;
        case WebGLGetInfo::kTypeFloat:
            return v8::Number::New(info.getFloat());

        case WebGLGetInfo::kTypeWebGLFloatArray:
        	return V8ArrayType<Float32Array>::Wrap(info.getWebGLFloatArray());
           // return info.getWebGLFloatArray()->wrap(args);

        case WebGLGetInfo::kTypeWebGLIntArray:
        	return V8ArrayType<Int32Array>::Wrap(info.getWebGLIntArray());
           // return info.getWebGLIntArray()->wrap(args);

        default:
            return v8::Null();
    }
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::CheckFramebufferStatus(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TError, "CheckFramebufferStatus : number of arguments Exception");
    }

    if (!args[0]->IsInt32())
    {
        return THROW_EXCEPTION(TError, "CheckFramebufferStatus : arguments wrong type");
    }

    DCenum target = args[0]->Uint32Value();

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    DCenum result = context->checkFramebufferStatus(target);

    return v8::Uint32::New(result);
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::ReadPixels(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 7)
    {
        return THROW_EXCEPTION(TError, "ReadPixels : number of arguments Exception");
    }

    if (!V8ArrayBufferView::HasInstance(args[6]))
    {
        return THROW_EXCEPTION(TError, "ReadPixels : argument 7 is not ArrayBufferView");
    }

    DCint x = args[0]->Int32Value();
    DCint y = args[1]->Int32Value();
    DCsizei width = args[2]->Int32Value();
    DCsizei height = args[3]->Int32Value();
    DCenum format = args[4]->Uint32Value();
    DCenum type = args[5]->Uint32Value();

    ArrayBufferView* abv = ObjectWrap::Unwrap<ArrayBufferView>(args[6]->ToObject());

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->readPixels(x, y, width, height, format, type, abv);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebGLGraphicsContext::SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length()!=0)
    {
        return THROW_EXCEPTION(TError, "SwapBuffer : number of arguments Exception");
    }

    WebGLGraphicsContext* context = ObjectWrap::Unwrap<WebGLGraphicsContext>(args.This());
    context->swapBuffer();
    return v8::Undefined();
}

} // namespace DCanvas
