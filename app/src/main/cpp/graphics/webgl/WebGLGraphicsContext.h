#ifndef WEBGLGRAPHICSCONTEXT_H
#define WEBGLGRAPHICSCONTEXT_H

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

class WebGLBuffer;
class WebGLShader;
class WebGLProgram;
class WebGLTexture;
class WebGLObject;
class WebGLRenderBuffer;
class WebGLFrameBuffer;
class WebGLGetInfo;
class WebGLActiveInfo;
class WebGLUniformLocation;
class WebGLVertexArrayObjectOES;

enum AlphaOp
{
    AlphaDoNothing = 0,
    AlphaDoPremultiply = 1,
    AlphaDoUnmultiply = 2
};

enum SourceDataFormat
{
    SourceFormatRGBA8 = 0,
    SourceFormatRGBA16Little,
    SourceFormatRGBA16Big,
    SourceFormatRGBA32F,
    SourceFormatRGB8,
    SourceFormatRGB16Little,
    SourceFormatRGB16Big,
    SourceFormatRGB32F,
    SourceFormatBGR8,
    SourceFormatBGRA8,
    SourceFormatBGRA16Little,
    SourceFormatBGRA16Big,
    SourceFormatARGB8,
    SourceFormatARGB16Little,
    SourceFormatARGB16Big,
    SourceFormatABGR8,
    SourceFormatRGBA5551,
    SourceFormatRGBA4444,
    SourceFormatRGB565,
    SourceFormatR8,
    SourceFormatR16Little,
    SourceFormatR16Big,
    SourceFormatR32F,
    SourceFormatRA8,
    SourceFormatRA16Little,
    SourceFormatRA16Big,
    SourceFormatRA32F,
    SourceFormatAR8,
    SourceFormatAR16Little,
    SourceFormatAR16Big,
    SourceFormatA8,
    SourceFormatA16Little,
    SourceFormatA16Big,
    SourceFormatA32F,
    SourceFormatNumFormats
};

enum
{
    UNPACK_FLIP_Y_WEBGL = 0x9240,
    UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241,
    CONTEXT_LOST_WEBGL = 0x9242,
    UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243,
    BROWSER_DEFAULT_WEBGL = 0x9244,
    DEPTH_STENCIL_ATTACHMENT = 0x821A
};

class WebGLGraphicsContext : public RenderingContext
{

public:
    bool is3d() const { return true; }
    virtual void onFrame();
    static WebGLGraphicsContext* Create();
#if NATIVE_SURFACE
    static WebGLGraphicsContext* Create(int x, int y, int width, int height, NativeEnv*&);
#endif
    static WebGLGraphicsContext* Create(int x, int y, int width, int height);
    static WebGLGraphicsContext* Create(WebGLContextAttributes* attributes);
    bool Init();
    ~WebGLGraphicsContext();
    void destroy();
    void removeObject(WebGLObject* object);
    bool deleteObject(WebGLObject* object);

    void setAttributes(WebGLContextAttributes* webGLAttrs){ this->m_attributes = webGLAttrs; }

    DCint getMaxVertexAttribs() const { return m_maxVertexAttribs; }

    void viewport(const int x, const int y, const long width, const long height);
    void clear(const unsigned long mask);
    void clearColor(const float red, const float green, const float blue, const float alpha);
    void enable(const DCenum);
    bool bindBuffer(const DCenum, WebGLBuffer* buffer);
    WebGLBuffer* createBuffer();
    bool bufferData(DCenum target, DCsizei size, DCenum usage);
    bool bufferData(DCenum target, ArrayBufferView* data, DCenum usage);
    bool bufferData(DCenum target, ArrayBuffer* data, DCenum usage);
    bool bufferSubData(DCenum target, DCintptr offset, ArrayBufferView* data);
    bool bufferSubData(DCenum target, DCintptr offset, ArrayBuffer* data);
    WebGLShader* createShader(DCenum);
    void shaderSource(WebGLShader* shader, std::string);
    void compileShader(WebGLShader* shader);
    WebGLProgram* createProgram();
    void attachShader(WebGLProgram* program, WebGLShader* shader);
    void linkProgram(WebGLProgram* );
    void useProgram(WebGLProgram* );
    void depthFunc(DCenum func);
    void clearDepth(DCclampf depth);
    void flush();
    WebGLTexture* createTexture();
    bool bindTexture(const DCenum, WebGLTexture*);

    WebGLRenderBuffer* createRenderbuffer();
    WebGLFrameBuffer* createFramebuffer();
    bool bindRenderbuffer(const DCenum, WebGLRenderBuffer*);

    void renderbufferStorage(DCenum target, DCenum info,
                             DCsizei width, DCsizei height);

    void framebufferTexture2D(DCenum target, DCenum attachment,
                              DCenum textarget, WebGLTexture* texture,
                              DCint level);

    void framebufferRenderbuffer(DCenum target, DCenum attachment,
                                 DCenum renderbuffertarget,
                                 WebGLRenderBuffer* renderBuffer);

    void bindFramebuffer(DCenum target, WebGLFrameBuffer* frameBuffer);

    DCuint getAttribLocation(WebGLProgram* , std::string name);
    DCuint getUniformLocation(WebGLProgram* , std::string name);
    void enableVertexAttribArray(DCuint index);

    void vertexAttribPointer(DCuint index, DCint size, DCenum type,
                             DCboolean normalized,
                             DCsizei stride, DCintptr offset);

    void drawArrays(DCenum mode, DCint first, DCsizei count);
    void drawElements(DCenum mode, DCsizei count, DCenum type, DCintptr indices);
    std::string getShaderInfoLog(WebGLShader*);
    void disable(DCenum cap);
    void activeTexture(DCenum);
    void generateMipmap(DCenum target);
    void pixelStorei(DCenum, DCint);

    void texImage2D(DCenum, DCint, DCenum, DCsizei, DCsizei,
                    DCint, DCenum, DCenum, ArrayBufferView*);

    void texImage2D(DCenum, DCint, DCenum, DCenum, DCenum, Image*);

    void texParameterf(DCenum target, DCenum pname, DCfloat param);
    void texParameteri(DCenum target, DCenum pname, DCint param);

    void uniform1f(DCint location, float x);
    void uniform1fv(DCint location, DCsizei count, float* v);
    void uniform1i(DCint location, int x);
    void uniform1iv(DCint location, DCsizei count, int* v);
    void uniform2f(DCint location, float x, float y);
    void uniform2fv(DCint location, DCsizei count, float* v);
    void uniform2i(DCint location, int x, int y);
    void uniform2iv(DCint location, DCsizei count, int* v);
    void uniform3f(DCint location, float x, float y, float z);
    void uniform3fv(DCint location, DCsizei count, float* v);
    void uniform3i(DCint location, int x, int y, int z);
    void uniform3iv(DCint location, DCsizei count, int* v);
    void uniform4f(DCint location, float x, float y, float z, float w);
    void uniform4fv(DCint location, DCsizei count, float* v);
    void uniform4i(DCint location, int x, int y, int z, int w);
    void uniform4iv(DCint location, DCsizei count, int* v);
    void uniformMatrix2fv(DCint location, int size, bool transpose,Float32Array* farry);
    void uniformMatrix3fv(DCint location, int size, bool transpose,Float32Array* farry);
    void uniformMatrix4fv(DCint location, int size, bool transpose,Float32Array* farry);

    void vertexAttrib1f(DCuint index, DCfloat x);
    void vertexAttrib1fv(DCuint index, Float32Array* values);
    void vertexAttrib1fv(DCuint index, DCfloat* values, DCsizei size);
    void vertexAttrib2f(DCuint index, DCfloat x, DCfloat y);
    void vertexAttrib2fv(DCuint index, Float32Array* values);
    void vertexAttrib2fv(DCuint index, DCfloat* values, DCsizei size);
    void vertexAttrib3f(DCuint index, DCfloat x, DCfloat y, DCfloat z);
    void vertexAttrib3fv(DCuint index, Float32Array* values);
    void vertexAttrib3fv(DCuint index, DCfloat* values, DCsizei size);
    void vertexAttrib4f(DCuint index, DCfloat x, DCfloat y, DCfloat z, DCfloat w);
    void vertexAttrib4fv(DCuint index, Float32Array* values);
    void vertexAttrib4fv(DCuint index, DCfloat* values, DCsizei size);

    void blendColor(DCclampf red, DCclampf green, DCclampf blue, DCclampf alpha);
    void blendEquation(DCenum mode);
    void blendEquationSeparate(DCenum modeRGB, DCenum modeAlpha);
    void blendFunc(DCenum sfactor, DCenum dfactor);
    void blendFuncSeparate(DCenum srcRGB, DCenum dstRGB, DCenum srcAlpha, DCenum dstAlpha);
    void sampleCoverage(DCclampf value, DCboolean invert);
    void stencilFuncSeparate(DCenum face, DCenum func, DCint ref, DCuint mask);
    void stencilOp(DCenum fail, DCenum zfail, DCenum zpass);
    void stencilOpSeparate(DCenum face, DCenum fail, DCenum zfail, DCenum zpass);

    void clearStencil(DCint s);
    void colorMask(DCboolean red, DCboolean green, DCboolean blue, DCboolean alpha);
    void depthMask(DCboolean flag);
    void stencilMask(DCuint mask);
    void stencilMaskSeparate(DCenum face, DCuint mask);
    void depthRange(DCfloat zNear, DCfloat zFar);
    void scissor(DCint x, DCint y, DCint width, DCint height);
    bool deleteBuffer(WebGLBuffer* buffer);
    bool deleteProgram(WebGLProgram* program);
    bool deleteShader(WebGLShader* shader);
    bool deleteTexture(WebGLTexture* texture);
    bool deleteRenderbuffer(WebGLRenderBuffer* renderbuffer);
    bool deleteFramebuffer(WebGLFrameBuffer* framebuffer);
    void cullFace(DCenum mode);
    void frontFace(DCenum mode);
    void lineWidth(DCfloat width);
    void polygonOffset(DCfloat factor, DCfloat units);
    void bindAttribLocation(WebGLProgram* program, DCuint index, std::string name);
    bool detachShader(WebGLProgram* program, WebGLShader* shader);
    bool getAttachedShaders(WebGLProgram* program, std::vector<WebGLShader*>& shaderObjects);
    std::string getProgramInfoLog(WebGLProgram* program);
    std::string getShaderSource(WebGLShader* shader);
    bool isBuffer(WebGLBuffer* buffer);
    bool isProgram(WebGLProgram* program);
    bool isShader(WebGLShader* shader);
    bool isTexture(WebGLTexture* texture);
    bool isRenderbuffer(WebGLRenderBuffer* renderbuffer);
    bool isFramebuffer(WebGLFrameBuffer* framebuffer);
    void validateProgram(WebGLProgram* program);
    void finish();
    DCenum getError();
    void disableVertexAttribArray(DCuint index);
    bool isEnabled(DCenum cap);
    void hint(DCenum target, DCenum mode);

#if NATIVE_SURFACE
    virtual void setPosition(int x, int y);
    virtual void setSize(int w , int h);
    virtual void setAlpha(float alpha);
#endif

    void copyTexImage2D(DCenum target, DCint level,
                        DCenum internalformat,
                        DCint x, DCint y, DCsizei width,
                        DCsizei height, DCint border);

    void copyTexSubImage2D(DCenum target, DCint level,
                           DCint xoffset, DCint yoffset,
                           DCint x, DCint y,
                           DCsizei width, DCsizei height);

    void texSubImage2DBase(DCenum target, DCint level,
                           DCint xoffset, DCint yoffset,
                           DCsizei width, DCsizei height,
                           DCenum format, DCenum type, void* pixels);

    void texSubImage2D(DCenum target, DCint level,
                       DCint xoffset, DCint yoffset,
                       DCsizei width, DCsizei height,
                       DCenum format, DCenum type, ArrayBufferView* data);

    void texSubImage2D(DCenum target, DCint level,
                       DCint xoffset, DCint yoffset,
                       DCenum format, DCenum type, Image* img);

    WebGLGetInfo getProgramParameter(WebGLProgram* program, DCenum pname);
    WebGLGetInfo getShaderParameter(WebGLShader* shader, DCenum pname);
    WebGLGetInfo getTexParameter(DCenum target, DCenum pname);
    WebGLGetInfo getRenderbufferParameter(DCenum target, DCenum pname);
    WebGLGetInfo getFramebufferAttachmentParameter(DCenum target, DCenum attachment, DCenum pname);
    WebGLGetInfo getParameter(DCenum pname);
    WebGLGetInfo getVertexAttrib(DCuint index, DCenum pname);
    DCsizeiptr getVertexAttribOffset(DCuint index, DCenum pname);
    WebGLActiveInfo* getActiveAttrib(WebGLProgram* program, DCuint index);
    WebGLActiveInfo* getActiveUniform(WebGLProgram* program, uint index);
    WebGLGetInfo getUniform(WebGLProgram* program, DCuint location);
    DCenum checkFramebufferStatus(DCenum target);

    void readPixels(DCint x, DCint y, DCsizei width, DCsizei height,
                    DCenum format, DCenum type, ArrayBufferView* pixels);
    void swapBuffer();
private:
    WebGLGraphicsContext();
#if NATIVE_SURFACE
    WebGLGraphicsContext(int x, int y, int width, int height, NativeEnv*&);
#endif
    WebGLGraphicsContext(int x, int y, int width, int height);
    WebGLGraphicsContext(WebGLContextAttributes* attributes);

    bool validateBlendEquation(DCenum mode);
    bool validateBlendFuncFactors(DCenum src, DCenum dst);
    bool validateStencilFunc(DCenum func);
    bool validateStencilFace(DCenum face);
    bool validateSize(DCint x, DCint y);
    bool validateTexFuncFormatAndType(DCenum format, DCenum type);
    bool validateTexFuncData(DCsizei width, DCsizei height,
                             DCenum format, DCenum type, ArrayBufferView* pixels);
    bool validateTexFuncLevel(DCenum target, DCint level);
    bool validateTexFuncParameters(DCenum target, DCint level, DCenum internalformat,
                                   DCsizei width, DCsizei height, DCint border,
                                   DCenum format, DCenum type);
    bool validateVertexAttribfv(DCuint index, DCfloat* v, DCsizei size, DCsizei expectedSize);
    bool validateFramebufferFuncParameters(DCenum target, DCenum attachment);
    bool validateDrawMode(DCenum mode);
    bool validateRenderingState(DCint numElementsRequired);

    bool isTexInternalFormatColorBufferCombinationValid(DCenum texInternalFormat,
                                                        DCenum colorBufferFormat);
    WebGLTexture* validateTextureBinding(DCenum target, bool useSixEnumsForCubeMap);
    WebGLBuffer* validateBufferDataParameters(DCenum target, DCenum usage);
    bool computeFormatAndTypeParameters(DCenum format, DCenum type,
                                        DCuint* componentsPerPixel, DCuint* bytesPerComponent);
    bool computeImageSizeInBytes(DCenum format, DCenum type,
                                 DCsizei width, DCsizei height,
                                 DCint alignment, DCuint* imageSizeInBytes,
                                 DCuint* paddingInBytes);
    DCenum getBoundFramebufferColorFormat();
    WebGLGetInfo getBooleanParameter(DCenum pname);
    WebGLGetInfo getBooleanArrayParameter(DCenum pname);
    WebGLGetInfo getFloatParameter(DCenum pname);
    WebGLGetInfo getIntParameter(DCenum pname);
    WebGLGetInfo getUnsignedIntParameter(DCenum pname);
    WebGLGetInfo getWebGLFloatArrayParameter(DCenum pname);
    WebGLGetInfo getWebGLIntArrayParameter(DCenum pname);
    bool simulateVertexAttrib0(DCsizei numVertex);
    void restoreStatesAfterVertexAttrib0Simulation();
    void initVertexAttrib0();
    void setVertexAttribValue(DCuint index, DCfloat v0, DCfloat v1, DCfloat v2, DCfloat v3);
    void setVertexAttribValues(DCuint index, DCsizei expectedSize, DCfloat* v);

    DCuint sizeInBytes(DCenum type);

    bool packPixels(const uint8_t* sourceData,
                    SourceDataFormat sourceDataFormat,
                    DCuint width, DCuint height,
                    DCuint sourceUnpackAlignment,
                    DCuint destinationFormat,
                    DCuint destinationType,
                    AlphaOp alphaOp, void* destinationData);

    void getIntegerv(DCenum pname, DCint* value);
    bool makeWebGLImageData(Image* img, DCenum format, DCenum type, DCubyte*& out);
    bool makeWebGLPixelsData(DCsizei width, DCsizei height,
                             DCenum format, DCenum type,
                             const void* pixels, DCubyte*& data);

    void flipVertically(void* imageData, DCsizei width, DCsizei height,
                        DCuint bytesPerPixel, DCuint unpackAlignment);

    void addObject(WebGLObject* object);
    void getExtensionStrings();

    class TextureUnitState
    {
        public:
            WebGLTexture* m_texture2DBinding;
            WebGLTexture* m_textureCubeMapBinding;
    };

    std::vector<TextureUnitState> m_textureUnits;
    unsigned long           m_activeTextureUnit;
    std::set<WebGLObject*>  m_canvasObjects;

    DCboolean m_unpackFlipY;
    DCboolean m_unpackPremultiplyAlpha;

    //HashSet<String> m_availableExtensions;
    std::vector<std::string> m_availableExtensions;
    DCboolean           m_oesTextureFloat;
    DCboolean           m_oesStandardDerivatives;
    DCboolean           m_isDepthStencilSupported;
    DCuint              m_packAlignment;
    DCuint              m_unpackAlignment;
    DCint               m_maxVertexAttribs;
    WebGLBuffer*        m_boundArrayBuffer;
    WebGLProgram*       m_currentProgram;
    WebGLFrameBuffer*   m_framebufferBinding;
    WebGLRenderBuffer*  m_renderbufferBinding;
    WebGLContextAttributes* m_attributes;

    bool isDepthStencilSupported() { return m_isDepthStencilSupported; }

    DCint m_maxTextureSize;
    DCint m_maxCubeMapTextureSize;
    DCint m_maxRenderbufferSize;
    DCint m_maxViewportDims[2];
    DCint m_maxTextureLevel;
    DCint m_maxCubeMapTextureLevel;

    WebGLVertexArrayObjectOES* m_defaultVertexArrayObject;
    WebGLVertexArrayObjectOES* m_boundVertexArrayObject;

    void setBoundVertexArrayObject(WebGLVertexArrayObjectOES* arrayObject)
    {
        if (arrayObject)
            m_boundVertexArrayObject = arrayObject;
        else
            m_boundVertexArrayObject = m_defaultVertexArrayObject;
    }

    class VertexAttribValue
    {
        public:
            VertexAttribValue()
            {
                initValue();
            }

            void initValue()
            {
                value[0] = 0.0f;
                value[1] = 0.0f;
                value[2] = 0.0f;
                value[3] = 1.0f;
            }

            DCfloat value[4];
    };

    std::vector<VertexAttribValue>  m_vertexAttribValue;
    WebGLBuffer*                    m_vertexAttrib0Buffer;
    long                            m_vertexAttrib0BufferSize;
    DCfloat                         m_vertexAttrib0BufferValue[4];
    bool                            m_forceAttrib0BufferRefill;
    bool                            m_vertexAttrib0UsedBefore;
    Gl3d_Impl*                      m_context;
    int                             m_x, m_y, m_width, m_height;
};

} // namespace DCanvas

#endif // WEBGLGRAPHICSCONTEXT_H
