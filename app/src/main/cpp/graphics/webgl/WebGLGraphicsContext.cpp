#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "defines.h"
#include "WebGLGraphicsContext.h"

#include "WebGLShader.h"
#include "WebGLBuffer.h"
#include "WebGLProgram.h"
#include "WebGLTexture.h"
#include "WebGLRenderBuffer.h"
#include "WebGLFrameBuffer.h"

#include "ImageDataTypeConvert.h"
#include "StripComments.h"
#include "Assert.h"
#include "WebGLGetInfo.h"
#include "WebGLActiveInfo.h"
#include "WebGLVertexArrayObjectOES.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLGraphicsContext"

namespace DCanvas
{

WebGLGraphicsContext* WebGLGraphicsContext::Create()
{
    return new WebGLGraphicsContext();
}

#if NATIVE_SURFACE
WebGLGraphicsContext* WebGLGraphicsContext::Create(int x, int y, int width, int height, NativeEnv*& nativeEnv)
{
    return new WebGLGraphicsContext(x, y, width, height, nativeEnv);
}
#endif
WebGLGraphicsContext* WebGLGraphicsContext::Create(int x, int y, int width, int height)
{
    return new WebGLGraphicsContext(x, y, width, height);
}

WebGLGraphicsContext* WebGLGraphicsContext::Create(WebGLContextAttributes* attributes)
{
    return new WebGLGraphicsContext(attributes);
}

#if NATIVE_SURFACE
WebGLGraphicsContext::WebGLGraphicsContext(int x, int y, int width, int height, NativeEnv*& nativeEnv)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
{
    m_context = Gl3d_Impl::create();

    if (!nativeEnv)
    {
        m_context->System_Initiate(m_x, m_y, m_width, m_height, 0x40000000);
        nativeEnv = m_context->getNativeEnv();
    }
    else
    {
        m_context->setNativeEnv(nativeEnv);
    }

    m_context->_Init(width, height);
    Init();
}
#endif

WebGLGraphicsContext::WebGLGraphicsContext(int x, int y, int width, int height)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
{
    //Init();
}

WebGLGraphicsContext::WebGLGraphicsContext()
{
    m_x = m_y = 0;
    m_width  = CLOUDX_SCREEN_WIDTH;
    m_height = CLOUDX_SCREEN_HEIGHT;
    Init();
}

WebGLGraphicsContext::WebGLGraphicsContext(WebGLContextAttributes* attributes)
{
    m_x = m_y = 0;
    m_width  = CLOUDX_SCREEN_WIDTH;
    m_height = CLOUDX_SCREEN_HEIGHT;
    Init();
    m_attributes = WebGLContextAttributes::create(attributes->attributes());
}

WebGLGraphicsContext::~WebGLGraphicsContext()
{
    LOGD("begin! WebGLGraphicsContext destructor");

    for (std::set<WebGLObject*>::iterator it = m_canvasObjects.begin(); it != m_canvasObjects.end(); ++it)
    {
        delete *it;
    }
    m_canvasObjects.clear();

    if (m_context)
    {
        delete m_context;
        m_context = NULL;
    }

    LOGD("end! WebGLGraphicsContext destructor");
}

void WebGLGraphicsContext::removeObject(WebGLObject* object)
{
}

bool WebGLGraphicsContext::Init()
{
    m_boundArrayBuffer = NULL;
    m_currentProgram = NULL;
    m_framebufferBinding = NULL;
    m_renderbufferBinding = NULL;
    m_attributes = NULL;

    m_defaultVertexArrayObject = NULL;
    m_boundVertexArrayObject = NULL;

    DCint numCombinedTextureImageUnits = 0;
    getIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numCombinedTextureImageUnits);
    m_textureUnits.clear();
    m_textureUnits.resize(numCombinedTextureImageUnits);

    m_activeTextureUnit = 0;
    m_canvasObjects.clear();

    m_unpackFlipY = false;
    m_unpackPremultiplyAlpha = false;

    getExtensionStrings();
    m_oesTextureFloat = false;

    std::vector<std::string>::iterator it = std::find(m_availableExtensions.begin()
            , m_availableExtensions.end(),"GL_OES_texture_float");

    if (it != m_availableExtensions.end())
    {
        m_oesTextureFloat = true;
    }

    m_oesStandardDerivatives = false;
    m_isDepthStencilSupported = false;
    m_packAlignment = 4;
    m_unpackAlignment = 4;
    m_boundArrayBuffer = 0;
    m_currentProgram = 0;
    m_renderbufferBinding = 0;
    m_framebufferBinding = 0;

    DCint numVertexAttribs = 0;
    getIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);
    m_maxVertexAttribs = numVertexAttribs;

    m_maxTextureSize = 0;
    getIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
    m_maxTextureLevel = WebGLTexture::computeLevelCount(m_maxTextureSize, m_maxTextureSize);
    m_maxCubeMapTextureSize = 0;
    getIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &m_maxCubeMapTextureSize);
    m_maxCubeMapTextureLevel = WebGLTexture::computeLevelCount(m_maxCubeMapTextureSize, m_maxCubeMapTextureSize);
    m_maxRenderbufferSize = 0;
    getIntegerv(GL_MAX_RENDERBUFFER_SIZE, &m_maxRenderbufferSize);
    m_maxViewportDims[0] = m_maxViewportDims[1] = 0;
    getIntegerv(GL_MAX_VIEWPORT_DIMS, m_maxViewportDims);

    m_defaultVertexArrayObject = WebGLVertexArrayObjectOES::create(this, WebGLVertexArrayObjectOES::VaoTypeDefault);
    addObject(m_defaultVertexArrayObject);
    m_boundVertexArrayObject = m_defaultVertexArrayObject;

    m_vertexAttribValue.resize(m_maxVertexAttribs);
    initVertexAttrib0();
    return true;
}


WebGLProgram* WebGLGraphicsContext::createProgram()
{
    WebGLProgram* o = WebGLProgram::create(this);
    m_canvasObjects.insert((WebGLObject*)o);
    return o;
}

WebGLShader* WebGLGraphicsContext::createShader(DCenum type)
{
    WebGLShader* o = WebGLShader::create(this, type);
    m_canvasObjects.insert(o);
    return o;
}

WebGLTexture* WebGLGraphicsContext::createTexture()
{
    WebGLTexture* o = WebGLTexture::create(this);
    m_canvasObjects.insert(o);
    return o;
}

WebGLBuffer* WebGLGraphicsContext::createBuffer()
{
    WebGLBuffer* o = WebGLBuffer::create(this);
    m_canvasObjects.insert(o);
    return o;
}

WebGLRenderBuffer* WebGLGraphicsContext::createRenderbuffer()
{
    WebGLRenderBuffer *o = WebGLRenderBuffer::create(this);
    m_canvasObjects.insert(o);
    return o;
}

WebGLFrameBuffer* WebGLGraphicsContext::createFramebuffer()
{
    WebGLFrameBuffer *o = WebGLFrameBuffer::create(this);
    m_canvasObjects.insert(o);
    return o;
}

void WebGLGraphicsContext::addObject(WebGLObject* object)
{
    m_canvasObjects.insert(object);
}

void WebGLGraphicsContext::destroy()
{
    //notImplement
}

void WebGLGraphicsContext::onFrame()
{
    //FIXME with two or more canvas in one surface how to clear
    //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WebGLGraphicsContext::viewport(const int x, const int y, const long width, const long height)
{
    glViewport(x, y, width, height);
}

void WebGLGraphicsContext::clear(const unsigned long mask)
{
    glClear(mask);
}

void WebGLGraphicsContext::clearColor(const float red, const float green, const float blue, const float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void WebGLGraphicsContext::enable(const DCenum cap)
{
    glEnable(cap);
}

bool WebGLGraphicsContext::bindBuffer(const DCenum ta, WebGLBuffer* buffer)
{
    if (ta == GL_ARRAY_BUFFER)
        m_boundArrayBuffer = buffer;
    else if (ta == GL_ELEMENT_ARRAY_BUFFER)
        m_boundVertexArrayObject->setElementArrayBuffer(buffer);
    else
        return false;

    m_boundArrayBuffer = buffer;

    if (buffer == NULL)
    {
        glBindBuffer(ta, NULL);
        return true;
    }

    if (!buffer->setTarget(ta))
    {
        LOGE("bindBuffer : buffer did not set target");
        return false;
    }

    if (buffer->getObject())
    {
        glBindBuffer(ta, buffer->getObject());
    }
    else
    {
        LOGE("bindBuffer : buffer is empty");
        return false;
    }

    return true;
}

bool WebGLGraphicsContext::bufferData(DCenum target, DCsizei size, DCenum usage)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bufferData : target is not match");
        return false;
    }

    if (GL_STATIC_DRAW != usage && GL_STREAM_DRAW != usage && GL_DYNAMIC_DRAW != usage)
    {
        LOGE("bufferData : usage is not match");
        return false;
    }

    WebGLBuffer* buffer = validateBufferDataParameters(target, usage);

    if (buffer == NULL)
    {
        LOGE("bufferData : binding buffer is null");
        return false;
    }

    if (!buffer->associateBufferData(size))
    {
        LOGE("bufferData : associateBufferData return false");
        return false;
    }

    glBufferData(target, size, 0, usage);
    return true;
}

bool WebGLGraphicsContext::bufferData(DCenum target, ArrayBufferView* data, DCenum usage)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bufferData : target is not match");
        return false;
    }

    if (GL_STATIC_DRAW != usage && GL_STREAM_DRAW != usage && GL_DYNAMIC_DRAW != usage)
    {
        LOGE("bufferData : usage is not match");
        return false;
    }

    WebGLBuffer* buffer = validateBufferDataParameters(target, usage);

    if (!buffer)
    {
        LOGE("bufferData : binding buffer is null");
        return false;
    }

    if (!buffer->associateBufferData(data))
    {
        LOGE("bufferData : associateBufferData return false");
        return false;
    }

    if (!data)
    {
        glBufferData(target, 0, NULL, usage);
    }
    else
    {
        glBufferData(target, data->byteLength(), data->baseAddress(), usage);
    }

    return true;
}

bool WebGLGraphicsContext::bufferData(DCenum target, ArrayBuffer* data, DCenum usage)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bufferData : target is not match");
        return false;
    }

    if (GL_STATIC_DRAW != usage && GL_STREAM_DRAW != usage && GL_DYNAMIC_DRAW != usage)
    {
        LOGE("bufferData : usage is not match");
        return false;
    }

    WebGLBuffer* buffer = validateBufferDataParameters(target, usage);

    if (!buffer)
    {
        LOGE("bufferData : binding buffer is null");
        return false;
    }

    if (!buffer->associateBufferData(data))
    {
        LOGE("bufferData : associateBufferData return false");
        return false;
    }

    glBufferData(target, data->byteLength(), data->data(), usage);
    return true;
}

bool WebGLGraphicsContext::bufferSubData(DCenum target, DCintptr offset, ArrayBufferView* data)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bufferSubData : target is not match");
        return false;
    }

    if (!data)
    {
        LOGE("bufferSubData : ArrayBufferView is null");
        return false;
    }

    DCint length = data->byteLength();

    if (offset < 0 ||  length - offset < 0)
    {
        LOGE("bufferSubData : offset is invalid");
        return false;
    }

    WebGLBuffer* buffer = validateBufferDataParameters(target, GL_STATIC_DRAW);
    if (!buffer)
    {
        LOGE("bufferSubData : binding buffer is null");
        return false;
    }

    if (!buffer->associateBufferSubData(offset, data))
    {
        LOGE("bufferSubData : associateBufferData return false");
        return false;
    }

    glBufferSubData(target, offset, data->byteLength(), data->baseAddress());
    return true;
}

bool WebGLGraphicsContext::bufferSubData(DCenum target, DCintptr offset, ArrayBuffer* data)
{
    if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    {
        LOGE("bufferSubData : target is not match");
        return false;
    }

    if (!data)
    {
        LOGE("bufferSubData : ArrayBufferView is null");
        return false;
    }

    DCint length = data->byteLength();

    if (offset < 0 ||  length - offset < 0)
    {
        LOGE("bufferSubData : offset is invalid");
        return false;
    }

    WebGLBuffer* buffer = validateBufferDataParameters(target, GL_STATIC_DRAW);
    if (!buffer)
    {
        LOGE("bufferSubData : binding buffer is null");
        return false;
    }

    if (!buffer->associateBufferSubData(offset, data))
    {
        LOGE("bufferSubData : associateBufferData return false");
        return false;
    }

    glBufferSubData(target, offset, data->byteLength(), data->data());
    return true;
}

bool WebGLGraphicsContext::deleteObject(WebGLObject* object)
{
    if (object->getObject())
    {
        LOGE("deleteObject : delete object");
        object->deleteObject();
        return true;
    }
    else
    {
        LOGE("deleteObject : object is empty");
        return false;
    }
}

bool WebGLGraphicsContext::deleteBuffer(WebGLBuffer* buffer)
{
    if (buffer->isBuffer() && deleteObject(buffer))
    {
        LOGE("WebGLBuffer : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::deleteProgram(WebGLProgram* program)
{
    if (program->isProgram() && deleteObject(program))
    {
        LOGE("WebGLProgram : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::deleteShader(WebGLShader* shader)
{
    if (shader->isShader() && deleteObject(shader))
    {
        LOGE("WebGLShader : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::deleteTexture(WebGLTexture* texture)
{
    if (texture->isTexture() && deleteObject(texture))
    {
        LOGE("WebGLTexture : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::deleteRenderbuffer(WebGLRenderBuffer* renderbuffer)
{
    if (renderbuffer->isRenderBuffer() && deleteObject(renderbuffer))
    {
        LOGE("WebGLRenderBuffer : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::deleteFramebuffer(WebGLFrameBuffer* framebuffer)
{
    if (framebuffer->isFrameBuffer() && deleteObject(framebuffer))
    {
        LOGE("WebGLFrameBuffer : delete");
        return true;
    }
    else
    {
        return false;
    }
}

bool WebGLGraphicsContext::bindTexture(const DCenum target, WebGLTexture* texture)
{
    if (texture == NULL)
    {
        glBindTexture(target, NULL);
        return true;
    }

    if (texture && texture->getTarget() && texture->getTarget() != target)
    {
        LOGE("bindTexture : texture did not set target ,or set other target");
        return false;
    }

    DCint maxLevel = 0;

    if (target == GL_TEXTURE_2D)
    {
        m_textureUnits[m_activeTextureUnit].m_texture2DBinding = texture;
        maxLevel = m_maxTextureLevel;
    }
    else if (target == GL_TEXTURE_CUBE_MAP)
    {
        m_textureUnits[m_activeTextureUnit].m_textureCubeMapBinding = texture;
        maxLevel = m_maxCubeMapTextureLevel;
    }
    else
    {
        LOGE("bindTexture : target is not match");
        return false;
    }

    if (texture->getObject())
    {
        glBindTexture(target, texture->getObject());
    }
    else
    {
        LOGE("bindTexture : texture is empty");
        return false;
    }

    if (texture)
    {
        texture->setTarget(target, maxLevel);
    }

    return true;
}

void WebGLGraphicsContext::shaderSource(WebGLShader* shader, std::string source)
{
    shader->setSource(source);

    if (!shader || !shader->getObject() || shader->getSource().empty())
    {
        LOGE("shaderSource : shader is null, or shader source is empty");
        return;
    }

    StripComments tmp = StripComments(shader->getSource());

    const char* s = tmp.result();

    glShaderSource(shader->getObject(), 1, (const char **)(&s), /*&translatedShaderLength*/NULL);
}

void WebGLGraphicsContext::compileShader(WebGLShader* shader)
{
    if (!shader || !shader->getObject())
    {
        LOGE("compileShader : shader is null or empty");
        return;
    }

    glCompileShader(shader->getObject());
}

void WebGLGraphicsContext::attachShader(WebGLProgram* program, WebGLShader* shader)
{
    if (!program || !program->getObject() || !shader || !shader->getObject())
    {
        LOGE("attachShader : program and shader is null or empty");
        return ;
    }

    program->attachShader(shader);
    glAttachShader(program->getObject(), shader->getObject());
}

void WebGLGraphicsContext::linkProgram(WebGLProgram* program)
{
    if (!program || !program->getObject())
    {
        LOGE("linkProgram : program is null or empty");
        return ;
    }

    if (!program->getAttachedShader(GL_VERTEX_SHADER)
            || !program->getAttachedShader(GL_FRAGMENT_SHADER))
    {
        LOGE("linkProgram : not enough attached shader");
        program->setLinkStatus(false);
        return;
    }

    glLinkProgram(program->getObject());

    DCint value = 0;
    glGetProgramiv(program->getObject(), GL_LINK_STATUS, &value);
    program->setLinkStatus(static_cast<bool>(value));
    program->cacheActiveAttribLocations();
}

void WebGLGraphicsContext::useProgram(WebGLProgram* program)
{
    if (!program || !program->getObject())
    {
        LOGE("useProgram : program is NULL or empty");
        return ;
    }

    if (m_currentProgram != program)
        m_currentProgram = program;

    glUseProgram(program->getObject());
}

void WebGLGraphicsContext::uniform1f(DCint location, float x)
{
    glUniform1f(location, x);
}

void WebGLGraphicsContext::uniform1fv(DCint location, DCsizei count, float* v)
{
    glUniform1fv(location, count, v);
}

void WebGLGraphicsContext::uniform1i(DCint location, int value)
{
    glUniform1i(location, value);
}

void WebGLGraphicsContext::uniform1iv(DCint location, DCsizei count, int* v)
{
    glUniform1iv(location, count, v);
}

void WebGLGraphicsContext::uniform2f(DCint location, float x, float y)
{
    glUniform2f(location, x, y);
}

void WebGLGraphicsContext::uniform2fv(DCint location, DCsizei count, float* v)
{
    glUniform2fv(location, count, v);
}

void WebGLGraphicsContext::uniform2i(DCint location, int x, int y)
{
    glUniform2i(location, x, y);
}

void WebGLGraphicsContext::uniform2iv(DCint location, DCsizei count, int* v)
{
    glUniform2iv(location, count, v);
}

void WebGLGraphicsContext::uniform3f(DCint location, float x, float y, float z)
{
    glUniform3f(location, x, y, z);
}

void WebGLGraphicsContext::uniform3fv(DCint location, DCsizei count, float* v)
{
    glUniform3fv(location, count, v);
}

void WebGLGraphicsContext::uniform3i(DCint location, int x, int y, int z)
{
    glUniform3i(location, x, y, z);
}

void WebGLGraphicsContext::uniform3iv(DCint location, DCsizei count, int* v)
{
    glUniform3iv(location, count, v);
}

void WebGLGraphicsContext::uniform4f(DCint location, float x, float y, float z, float w)
{
    glUniform4f(location, x, y, z, w);
}

void WebGLGraphicsContext::uniform4fv(DCint location, DCsizei count, float* v)
{
    glUniform4fv(location, count, v);
}

void WebGLGraphicsContext::uniform4i(DCint location, int x, int y, int z, int w)
{
    glUniform4i(location, x, y, z, w);
}

void WebGLGraphicsContext::uniform4iv(DCint location, DCsizei count, int* v)
{
    glUniform4iv(location, count, v);
}

void WebGLGraphicsContext::uniformMatrix2fv(DCint location, int size, bool transpose, Float32Array* farry)
{
    glUniformMatrix2fv(location, size, transpose, farry->data());
}

void WebGLGraphicsContext::uniformMatrix3fv(DCint location, int size, bool transpose, Float32Array* farry)
{
    glUniformMatrix3fv(location, size, transpose, farry->data());
}

void WebGLGraphicsContext::uniformMatrix4fv(DCint location, int size, bool transpose, Float32Array* farry)
{
    glUniformMatrix4fv(location, size, transpose, farry->data());
}

DCuint WebGLGraphicsContext::getAttribLocation(WebGLProgram* program, std::string name)
{
    if (!program)
    {
        LOGE("getAttribLocation : program is NULL");
        return -1;
    }

    return glGetAttribLocation(program->getObject(), (const char*)name.c_str());
}

DCuint WebGLGraphicsContext::getUniformLocation(WebGLProgram* program, std::string name)
{
    if (!program)
    {
        LOGE("getUniformLocation : program is NULL");
        return -1;
    }

    return glGetUniformLocation(program->getObject(), (const char*)name.c_str());
}

void WebGLGraphicsContext::enableVertexAttribArray(DCuint index)
{
    if (index >= (DCuint)m_maxVertexAttribs)
    {
        LOGE("enableVertexAttribArray : index is invalid");
        return;
    }

    WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(index);
    state.enabled = true;

    glEnableVertexAttribArray(index);
}

void WebGLGraphicsContext::vertexAttribPointer(DCuint index, DCint size, DCenum type
    , DCboolean normalized, DCsizei stride, DCintptr offset)
{
    if (index >= (DCuint)m_maxVertexAttribs)
    {
        LOGE("VertexAttribPointer : index is invalid");
        return;
    }

    switch (type)
    {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_FLOAT:
            break;
        default:
            LOGE("VertexAttribPointer : type is not match");

        return ;
    }

    if (size < 1 || size > 4 || stride < 0 || stride > 255 || offset < 0)
    {
        LOGE("VertexAttribPointer : size or stride is invalid");
        return ;
    }

    if (!m_boundArrayBuffer)
    {
        LOGE("VertexAttribPointer : boundArrayBuffer is null");
        return;
    }

    DCuint typeSize = sizeInBytes(type);
    if (!typeSize)
    {
        LOGE("VertexAttribPointer : sizeInBytes not match");
        return;
    }
    if ((stride % typeSize) || (offset % typeSize))
    {
        LOGE("VertexAttribPointer : stride or offset is invalid");
        return;
    }

    DCsizei bytesPerElement = size * typeSize;

    DCsizei validatedStride = stride ? stride : bytesPerElement;

    WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(index);
    state.bufferBinding = m_boundArrayBuffer;
    state.bytesPerElement = bytesPerElement;
    state.size = size;
    state.type = type;
    state.normalized = normalized;
    state.stride = validatedStride;
    state.originalStride = stride;
    state.offset = offset;

    glVertexAttribPointer(index, size, type, normalized, stride, (const void*)offset);
}

void WebGLGraphicsContext::drawArrays(DCenum mode, DCint first, DCsizei count)
{
    if (!validateDrawMode(mode))
    {
        LOGE("drawArrays : mode is invalid");
        return;
    }

    if (first < 0 || count < 0)
    {
        LOGE("drawArrays : first or count is invalid");
        return;
    }

    if (!count)
        return;

    if (m_framebufferBinding && !m_framebufferBinding->getObject())
    {
        LOGE("drawArrays : binding frameBuffer is invalid");
        return;
    }

    if (!validateRenderingState(first + count))
    {
        LOGE("drawArrays : validateRenderingState(sum) is invalid");
        return;
    }

    //bool vertexAttrib0Simulated = false;
    //vertexAttrib0Simulated = simulateVertexAttrib0(first + count - 1);

    glDrawArrays(mode, first, count);

    //if (!isGLES2Compliant() && vertexAttrib0Simulated)
    //if (vertexAttrib0Simulated)
    //restoreStatesAfterVertexAttrib0Simulation();
    //if (!isGLES2NPOTStrict())
    //    handleNPOTTextures(false);

}

void WebGLGraphicsContext::depthFunc(DCenum func)
{
    switch (func)
    {
        case GL_NEVER:
            break;
        case GL_LESS:
            break;
        case GL_LEQUAL:
            break;
        case GL_EQUAL:
            break;
        case GL_GREATER:
            break;
        case GL_NOTEQUAL:
            break;
        case GL_GEQUAL:
            break;
        case GL_ALWAYS:
            break;
        default:
            LOGE("depthfunc : type is not match");
            return;
    }

    glDepthFunc(func);
}

void WebGLGraphicsContext::clearDepth(DCclampf depth)
{
    glClearDepthf(depth);
}

void WebGLGraphicsContext::flush()
{
    glFlush();
}

void WebGLGraphicsContext::drawElements(DCenum mode, DCsizei count, DCenum type, DCintptr offset)
{
    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
            break;
        default:
            LOGE("drawElements : type is not match");
            return;
    }

    if (count < 0 || offset < 0)
    {
        LOGE("drawElements : count or offset invalid");
        return;
    }

    if (!count)
        return;

    glDrawElements(mode, count, type, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
}

std::string WebGLGraphicsContext::getShaderInfoLog(WebGLShader* shader)
{
    int res = 0;

    if (!shader || !shader->getObject())
    {
        LOGE("getShaderInfoLog : shader is null or empty");
        return std::string("");
    }

    glGetShaderiv(shader->getObject(), GL_INFO_LOG_LENGTH, &res);

    if (!res)
    {
        return std::string("");
    }

    char info[res];
    GLsizei size;

    glGetShaderInfoLog(shader->getObject(), res, &size, info);

    std::string str(info);
    return str;
}

void WebGLGraphicsContext::disable(DCenum cap)
{
    glDisable(cap);
}

void WebGLGraphicsContext::pixelStorei(DCenum pname, DCint param)
{
    switch (pname)
    {
        case UNPACK_FLIP_Y_WEBGL:
            m_unpackFlipY = (DCboolean)param;
            break;

        case UNPACK_PREMULTIPLY_ALPHA_WEBGL:
            m_unpackPremultiplyAlpha = (DCboolean)param;
            break;

        case GL_PACK_ALIGNMENT://without breadk
        case GL_UNPACK_ALIGNMENT:
            if (param == 1 || param == 2 || param == 4 || param == 8)
            {
                if (pname == GL_PACK_ALIGNMENT)
                {
                    m_packAlignment = param;
                }
                else if (GL_UNPACK_ALIGNMENT)
                {
                    m_unpackAlignment = param;
                }

                glPixelStorei(pname, param);
            }
            else
            {
                LOGE("pixelStorei : argument 2 is not match");
                return;
            }
            break;

        default:
            LOGE("pixelStorei : argument 1 is not match");
            return;
    }
}

void WebGLGraphicsContext::texImage2D(DCenum target, DCint level
                                    , DCenum internalformat, DCenum format
                                    , DCenum type, Image* img)
{
    DCubyte* data = NULL;
    int width = 0;
    int height = 0;

    WebGLTexture* tex = validateTextureBinding(target, true);

    if (!tex)
    {
        LOGE("texImage2D : binding texture is null");
        return;
    }

    if (img != NULL)
    {
        width = img->getWidth();
        height = img->getHeight();

        if (!makeWebGLImageData(img, format, type, data))
        {
            LOGE("texImage2D : makeWebGLImageData failed");
            return;
        }
    }

    glTexImage2D(target, level, internalformat, width, height, 0, format, type, (const void*)data);

    if (data)
    {
        delete[] data;
        data = NULL;
    }

    tex->setLevelInfo(target, level, internalformat, width, height, type);
}

void WebGLGraphicsContext::texImage2D(DCenum target, DCint level
                                    , DCenum internalformat, DCsizei width
                                    , DCsizei height, DCint border, DCenum format
                                    , DCenum type, ArrayBufferView* arraydata)
{
    WebGLTexture* tex = validateTextureBinding(target, true);

    if (!tex)
    {
        LOGE("texImage2D : binding texture is null");
        return;
    }

    DCubyte* data = NULL;

    if (arraydata != NULL)
    {
        if (!validateTexFuncData(width, height, format, type, arraydata))
        {
            LOGE("texImage2D : validateTexFuncData check failed");
            return;
        }

        if (!makeWebGLPixelsData(width, height, format, type, (const void*)arraydata->baseAddress(), data))
        {
            LOGE("texImage2D : makeWebGLPixelsData failed");
            return;
        }
    }

    glTexImage2D(target, level, internalformat, width, height, border, format, type, (const void*)data);

    if (data)
    {
        delete[] data;
        data = NULL;
    }

    tex->setLevelInfo(target, level, internalformat, width, height, type);
}

bool WebGLGraphicsContext::makeWebGLImageData(Image* img, DCenum format, DCenum type, DCubyte*& out)
{
    int width = img->getWidth();
    int height = img->getHeight();
    DCuint* data = new DCuint[width * height];
    memcpy(data, img->getImageData(), 4 * width * height);

    AlphaOp neededAlphaOp = AlphaDoNothing;
    if (m_unpackPremultiplyAlpha)
        neededAlphaOp = AlphaDoPremultiply;
    else if (!m_unpackPremultiplyAlpha)
        neededAlphaOp = AlphaDoUnmultiply;

    DCuint componentsPerPixel, bytesPerComponent;
    if (!computeFormatAndTypeParameters(format, type,
                                        &componentsPerPixel,
                                        &bytesPerComponent))
    {
        LOGE("makeWebGLImageData : computeFormatAndTypeParameters check failed");
        if (data)
        {
            delete[] data;
            data = NULL;
        }
        return false;
    }

    DCuint bytesPerPixel = componentsPerPixel * bytesPerComponent;
    out = new DCubyte[width * height * bytesPerPixel];

    if (!packPixels((uint8_t*)data, SourceFormatRGBA8, img->getWidth(), img->getHeight(),
                   m_unpackAlignment, format, type, neededAlphaOp, out))
    {
        LOGE("makeWebGLImageData : packPixels failed");
        if (data)
        {
            delete[] data;
            data = NULL;
        }
        return false;
    }

    if (m_unpackFlipY)
        flipVertically(out, width, height, bytesPerPixel, 1);

    if (data)
    {
        delete[] data;
        data = NULL;
    }
    return true;
}

bool WebGLGraphicsContext::makeWebGLPixelsData(DCsizei width, DCsizei height, DCenum format,
                                                DCenum type, const void* pixels, DCubyte*& data)
{
    SourceDataFormat sourceDataFormat = SourceFormatRGBA8;
    switch (type)
    {
        case GL_UNSIGNED_BYTE:
            switch (format)
            {
                case GL_RGBA:
                    sourceDataFormat = SourceFormatRGBA8;
                    break;
                case GL_RGB:
                    sourceDataFormat = SourceFormatRGB8;
                    break;
                case GL_ALPHA:
                    sourceDataFormat = SourceFormatA8;
                    break;
                case GL_LUMINANCE:
                    sourceDataFormat = SourceFormatR8;
                    break;
                case GL_LUMINANCE_ALPHA:
                    sourceDataFormat = SourceFormatRA8;
                    break;
                default:
                    LOGE("makeWebGLPixelsData : type or format is invalid");
                    return false;
            } // switch (format)
            break;
        case GL_FLOAT: // OES_texture_float
            switch (format)
            {
                case GL_RGBA:
                    sourceDataFormat = SourceFormatRGBA32F;
                    break;
                case GL_RGB:
                    sourceDataFormat = SourceFormatRGB32F;
                    break;
                case GL_ALPHA:
                    sourceDataFormat = SourceFormatA32F;
                    break;
                case GL_LUMINANCE:
                    sourceDataFormat = SourceFormatR32F;
                    break;
                case GL_LUMINANCE_ALPHA:
                    sourceDataFormat = SourceFormatRA32F;
                    break;
                default:
                    LOGE("makeWebGLPixelsData : type or format is invalid");
                    return false;
            } // switch (format)
            break;
        case GL_UNSIGNED_SHORT_5_5_5_1:
            sourceDataFormat = SourceFormatRGBA5551;
            break;
        case GL_UNSIGNED_SHORT_4_4_4_4:
            sourceDataFormat = SourceFormatRGBA4444;
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
            sourceDataFormat = SourceFormatRGB565;
            break;
        default:
            LOGE("makeWebGLPixelsData : type is invalid");
            return false;
    } // switch (type)

    // Resize the output buffer.
    DCuint componentsPerPixel, bytesPerComponent;
    if (!computeFormatAndTypeParameters(format, type,
                                        &componentsPerPixel,
                                        &bytesPerComponent))
    {
        LOGE("makeWebGLPixelsData : computeFormatAndTypeParameters check failed");
        return false;
    }

    DCuint bytesPerPixel = componentsPerPixel * bytesPerComponent;
    data = new DCubyte[width * height * bytesPerPixel];

    if (!packPixels(static_cast<const uint8_t*>(pixels),
                    sourceDataFormat,
                    width, height, m_unpackAlignment,
                    format, type,
                    (m_unpackPremultiplyAlpha ? AlphaDoPremultiply : AlphaDoNothing),
                    data))
    {
        LOGE("makeWebGLPixelsData : packPixels failed");
        return false;
    }

    // The pixel data is now tightly packed.
    if (m_unpackFlipY)
        flipVertically(data, width, height, bytesPerPixel, 1);

    return true;
}

void WebGLGraphicsContext::flipVertically(void* imageData, DCsizei width, DCsizei height, DCuint bytesPerPixel, DCuint unpackAlignment)
{
    if (!width || !height)
    {
        LOGE("flipVertically : width or height is zero");
        return;
    }

    DCuint validRowBytes = width * bytesPerPixel;
    DCuint totalRowBytes = validRowBytes;
    DCuint remainder = validRowBytes % unpackAlignment;

    if (remainder)
        totalRowBytes += (unpackAlignment - remainder);

    uint8_t* tempRow = new uint8_t[validRowBytes];
    uint8_t* data = static_cast<uint8_t*>(imageData);

    for (int i = 0; i < height / 2; i++)
    {
        uint8_t* lowRow = data + (totalRowBytes * i);
        uint8_t* highRow = data + (totalRowBytes * (height - i - 1));
        memcpy(tempRow, lowRow, validRowBytes);
        memcpy(lowRow, highRow, validRowBytes);
        memcpy(highRow, tempRow, validRowBytes);
    }

    if (tempRow)
    {
        delete[] tempRow;
        tempRow = NULL;
    }
}

void WebGLGraphicsContext::texParameterf(DCenum target, DCenum pname, DCfloat param)
{
    glTexParameterf(target, pname, param);
}

void WebGLGraphicsContext::texParameteri(DCenum target, DCenum pname, DCint param)
{
    glTexParameteri(target, pname, param);
}

void WebGLGraphicsContext::activeTexture(DCenum texture)
{
    if (texture - GL_TEXTURE0 >= m_textureUnits.size())
    {
        LOGE("activeTexture : argument 1 is invalid");
        return;
    }

     m_activeTextureUnit = texture - GL_TEXTURE0;
    glActiveTexture(texture);
}

void WebGLGraphicsContext::generateMipmap(DCenum target)
{
    WebGLTexture* tex = validateTextureBinding(target, false);

    if (!tex)
    {
        LOGE("generateMipmap : binding texture is null");
        return;
    }

    if (!tex->canGenerateMipmaps())
    {
        LOGE("generateMipmap : can not generate mipmaps");
        return;
    }

    //switch (target) {
    //case GL_TEXTURE_2D:
    //    tex = m_textureUnits[m_activeTextureUnit].m_texture2DBinding;
    //    break;
    //default:
    //    LOGE("not implement now!!");
    //}

    glGenerateMipmap(target);

    tex->generateMipmapLevelInfo();
}

bool WebGLGraphicsContext::bindRenderbuffer(const DCenum target, WebGLRenderBuffer* render_buffer)
{
    m_renderbufferBinding = render_buffer;

    if (render_buffer == NULL)
    {
        glBindRenderbuffer(target, NULL);
        return true;
    }

    if (target != GL_RENDERBUFFER)
    {
        LOGE("bindRenderbuffer : target is not RENDERBUFFER");
        return false;
    }

    if (render_buffer->getObject())
    {
        glBindRenderbuffer(target, render_buffer->getObject());
    }
    else
    {
        LOGE("bindRenderbuffer : buffer is empty");
        return false;
    }

    return true;
}

void WebGLGraphicsContext::renderbufferStorage(DCenum target, DCenum info, DCsizei width, DCsizei height)
{
    if (target != GL_RENDERBUFFER)
    {
        LOGE("renderbufferStorage : target is not RENDERBUFFER");
        return ;
    }

    if (!m_renderbufferBinding || !m_renderbufferBinding->getObject())
    {
        LOGE("renderbufferStorage : binding renderbuffer is null or empty");
        return;
    }

    if (!validateSize(width, height))
    {
        LOGE("renderbufferStorage : size width or height is invalid");
        return;
    }

    DCenum format;
    switch (info)
    {
        case GL_DEPTH_COMPONENT16:
            format = GL_DEPTH_COMPONENT16;
            glRenderbufferStorage(target, format, width, height);
            break;
        case GL_RGBA4:
        case GL_RGB5_A1:
            format = GL_RGBA;
            glRenderbufferStorage(target, format, width, height);
            break;
        case GL_RGB565:
            format = GL_RGB;
            glRenderbufferStorage(target, format, width, height);
            break;
        case GL_STENCIL_INDEX8:
            glRenderbufferStorage(target, info, width, height);

            break;
        case GL_DEPTH_STENCIL_OES:
            if (isDepthStencilSupported())
                glRenderbufferStorage(target, GL_DEPTH24_STENCIL8_OES, width, height);
            break;
        default:
            LOGE("invalid info ");
    }

}

void WebGLGraphicsContext::framebufferTexture2D(DCenum target, DCenum attachment,
                                                DCenum textarget, WebGLTexture* texture,
                                                DCint level)
{
    if (!m_framebufferBinding)
    {
        LOGE("framebufferTexture2D : binding framebuffer is null");
        return;
    }

    if (!texture || !texture->getObject())
    {
        LOGE("framebufferTexture2D : texture is null or empty");
        return ;
    }

    m_framebufferBinding->setAttachment(attachment, textarget, texture, level);

    glFramebufferTexture2D(target, attachment, textarget, texture->getObject(), level);
}

void WebGLGraphicsContext::framebufferRenderbuffer(DCenum target, DCenum attachment,
                                                   DCenum renderbuffertarget,
                                                   WebGLRenderBuffer* renderbuffer)
{
    if (renderbuffer == NULL)
    {
        glFramebufferRenderbuffer(target, attachment, renderbuffertarget, NULL);
        return;
    }

    if (!renderbuffer || !renderbuffer->getObject())
    {
        LOGE("framebufferRenderbuffer : renderbuffer is null or empty");
        return ;
    }

    DCPlatformObject bufferObject = renderbuffer->getObject();

    if (!m_framebufferBinding)
    {
        LOGE("framebufferRenderbuffer : binding framebuffer is null ");
        return;
    }

    bool reattachDepth = false;
    bool reattachStencil = false;
    bool reattachDepthStencilDepth = false;
    bool reattachDepthStencilStencil = false;

    switch (attachment)
    {
        case GL_DEPTH_STENCIL_ATTACHMENT:
            glFramebufferRenderbuffer(target, GL_DEPTH_ATTACHMENT, renderbuffertarget, bufferObject);
            glFramebufferRenderbuffer(target, GL_STENCIL_ATTACHMENT, renderbuffertarget, bufferObject);
            if (!bufferObject)
            {
                reattachDepth = true;
                reattachStencil = true;
            }
            break;
        case GL_DEPTH_ATTACHMENT:
            glFramebufferRenderbuffer(target, attachment, renderbuffertarget, bufferObject);
            if (!bufferObject)
                reattachDepthStencilDepth = true;
            break;
        case GL_STENCIL_ATTACHMENT:
            glFramebufferRenderbuffer(target, attachment, renderbuffertarget, bufferObject);
            if (!bufferObject)
                reattachDepthStencilStencil = true;
            break;
        default:
            glFramebufferRenderbuffer(target, attachment, renderbuffertarget, bufferObject);
    }

    m_framebufferBinding->setAttachment(attachment, renderbuffer);

    if (reattachDepth)
    {
        WebGLObject* object =  m_framebufferBinding->getAttachment(GL_DEPTH_ATTACHMENT);
        if (object)
        {
            glFramebufferRenderbuffer(target, GL_DEPTH_ATTACHMENT,
                    renderbuffertarget, object->getObject());
        }
    }

    if (reattachStencil)
    {
        WebGLObject* object =  m_framebufferBinding->getAttachment(GL_STENCIL_ATTACHMENT);
        if (object)
        {
            glFramebufferRenderbuffer(target, GL_STENCIL_ATTACHMENT,
                    renderbuffertarget, object->getObject());
        }
    }

    if (reattachDepthStencilDepth)
    {
        WebGLObject* object =  m_framebufferBinding->getAttachment(DEPTH_STENCIL_ATTACHMENT);
        if (object)
        {
            glFramebufferRenderbuffer(target, GL_DEPTH_ATTACHMENT,
                    renderbuffertarget, object->getObject());
        }
    }

    if (reattachDepthStencilStencil)
    {
        WebGLObject* object =  m_framebufferBinding->getAttachment(DEPTH_STENCIL_ATTACHMENT);
        if (object)
        {
            glFramebufferRenderbuffer(target, GL_STENCIL_ATTACHMENT,
                    renderbuffertarget, object->getObject());
        }
    }

}

void WebGLGraphicsContext::bindFramebuffer(DCenum target, WebGLFrameBuffer* framebuffer)
{
    m_framebufferBinding  = framebuffer;

    if (target != GL_FRAMEBUFFER)
    {
        LOGE("bindFramebuffer : target is not FRAMEBUFFER");
        return ;
    }

    if (framebuffer == NULL)
    {
        glBindFramebuffer(target, NULL);
        return;
    }

    if (!framebuffer || !framebuffer->getObject())
    {
        LOGE("bindFramebuffer : framebuffer is null or empty");
        return ;
    }

    glBindFramebuffer(target, framebuffer->getObject());
}

void WebGLGraphicsContext::blendColor(DCclampf red, DCclampf green, DCclampf blue, DCclampf alpha)
{
    glBlendColor(red, green, blue, alpha);
}

bool WebGLGraphicsContext::validateBlendEquation(DCenum mode)
{
    switch (mode)
    {
        case GL_FUNC_ADD:
        case GL_FUNC_SUBTRACT:
        case GL_FUNC_REVERSE_SUBTRACT:
            return true;
        default:
            return false;
    }
}

void WebGLGraphicsContext::blendEquation(DCenum mode)
{
    if (!validateBlendEquation(mode))
    {
        LOGE("blendEquation : mode is invalid");
        return;
    }

    glBlendEquation(mode);
}

void WebGLGraphicsContext::blendEquationSeparate(DCenum modeRGB, DCenum modeAlpha)
{
    if (!validateBlendEquation(modeRGB) || !validateBlendEquation(modeAlpha))
    {
        LOGE("blendEquationSeparate : mode is invalid");
        return;
    }

    glBlendEquationSeparate(modeRGB, modeAlpha);
}

bool WebGLGraphicsContext::validateBlendFuncFactors(DCenum src, DCenum dst)
{
    if (((src == GL_CONSTANT_COLOR || src == GL_ONE_MINUS_CONSTANT_COLOR)
         && (dst == GL_CONSTANT_ALPHA || dst == GL_ONE_MINUS_CONSTANT_ALPHA))
         || ((dst == GL_CONSTANT_COLOR || dst == GL_ONE_MINUS_CONSTANT_COLOR)
         && (src == GL_CONSTANT_ALPHA || src == GL_ONE_MINUS_CONSTANT_ALPHA)))
    {
        return false;
    }

    return true;
}

void WebGLGraphicsContext::blendFunc(DCenum sfactor, DCenum dfactor)
{
    if (!validateBlendFuncFactors(sfactor, dfactor))
    {
        LOGE("blendFunc : factor is invalid!");
        return;
    }

    glBlendFunc(sfactor, dfactor);
}

void WebGLGraphicsContext::blendFuncSeparate(DCenum srcRGB, DCenum dstRGB, DCenum srcAlpha, DCenum dstAlpha)
{
    if (!validateBlendFuncFactors(srcRGB, dstRGB))
    {
        LOGE("blendFuncSeparate : factors is invalid!");
        return;
    }

    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void WebGLGraphicsContext::sampleCoverage(DCclampf value, DCboolean invert)
{
    glSampleCoverage(value, invert);
}

bool WebGLGraphicsContext::validateStencilFunc(DCenum func)
{
    switch (func)
    {
        case GL_NEVER:
        case GL_LESS:
        case GL_LEQUAL:
        case GL_GREATER:
        case GL_GEQUAL:
        case GL_EQUAL:
        case GL_NOTEQUAL:
        case GL_ALWAYS:
            return true;
        default:
            return false;
    }
}

bool WebGLGraphicsContext::validateStencilFace(DCenum face)
{
    switch (face)
    {
        case GL_FRONT_AND_BACK:
        case GL_FRONT:
        case GL_BACK:
            return true;
        default:
            return false;
    }
}

void WebGLGraphicsContext::stencilFuncSeparate(DCenum face, DCenum func, DCint ref, DCuint mask)
{
    if (!validateStencilFace(face))
    {
        LOGE("stencilFuncSeparate : face is invalid!");
        return;
    }

    if (!validateStencilFunc(func))
    {
        LOGE("stencilFuncSeparate : func is invalid!");
        return;
    }

    glStencilFuncSeparate(face, func, ref, mask);
}

void WebGLGraphicsContext::stencilOp(DCenum fail, DCenum zfail, DCenum zpass)
{
    glStencilOp(fail, zfail, zpass);
}

void WebGLGraphicsContext::stencilOpSeparate(DCenum face, DCenum fail, DCenum zfail, DCenum zpass)
{
    glStencilOpSeparate(face, fail, zfail, zpass);
}

void WebGLGraphicsContext::clearStencil(DCint s)
{
    glClearStencil(s);
}

void WebGLGraphicsContext::colorMask(DCboolean red, DCboolean green, DCboolean blue, DCboolean alpha)
{
    glColorMask(red, green, blue, alpha);
}

void WebGLGraphicsContext::depthMask(DCboolean flag)
{
    glDepthMask(flag);
}

void WebGLGraphicsContext::stencilMask(DCuint mask)
{
    glStencilMask(mask);
}

void WebGLGraphicsContext::stencilMaskSeparate(DCenum face, DCuint mask)
{
    if (!validateStencilFace(face))
    {
        LOGE("stencilMaskSeparate : face is invalid!!");
        return;
    }

    glStencilMaskSeparate(face, mask);
}

void WebGLGraphicsContext::depthRange(DCfloat zNear, DCfloat zFar)
{
    glDepthRangef(zNear, zFar);
}

void WebGLGraphicsContext::scissor(DCint x, DCint y, DCint width, DCint height)
{
    if (width < 0 || height < 0)
    {
        LOGE("scissor : width or height is invalid");
        return;
    }

    glScissor(x, y, width, height);
}

void WebGLGraphicsContext::cullFace(DCenum mode)
{
    glCullFace(mode);
}

void WebGLGraphicsContext::frontFace(DCenum mode)
{
    glFrontFace(mode);
}

void WebGLGraphicsContext::lineWidth(DCfloat width)
{
    glLineWidth(width);
}

void WebGLGraphicsContext::polygonOffset(DCfloat factor, DCfloat units)
{
    glPolygonOffset(factor, units);
}

void WebGLGraphicsContext::bindAttribLocation(WebGLProgram* program, DCuint index, std::string name)
{
    if (!program)
    {
        LOGE("bindAttribLocation : program is null");
        return;
    }

    glBindAttribLocation(program->getObject(), index, (const char*)name.c_str());
}

bool WebGLGraphicsContext::detachShader(WebGLProgram* program, WebGLShader* shader)
{
    if (!program || !shader || !program->getObject() || !shader->getObject())
    {
        LOGE("detachShader : program or shader is null or empty");
        return false;
    }

    if (!program->detachShader(shader))
    {
        LOGE("detachShader : detachShader failed, Maybe it's a code bug");
        return false;
    }

    glDetachShader(program->getObject(), shader->getObject());
    return true;
}

bool WebGLGraphicsContext::getAttachedShaders(WebGLProgram* program, std::vector<WebGLShader*>& shaderObjects)
{
    if (!program || !program->getObject())
    {
        LOGE("getAttachedShaders : program is null or empty");
        return false;
    }

    shaderObjects.clear();

    const DCenum shaderType[] =
    {
        GL_VERTEX_SHADER,
           GL_FRAGMENT_SHADER
    };

    for (unsigned i = 0; i < sizeof(shaderType) / sizeof(DCenum); ++i)
    {
        WebGLShader* shader = program->getAttachedShader(shaderType[i]);

        if (shader)
            shaderObjects.push_back(shader);
    }

    return true;
}

std::string WebGLGraphicsContext::getProgramInfoLog(WebGLProgram* program)
{
    if (!program || !program->getObject())
    {
        LOGE("getProgramInfoLog : program is null or empty");
        return std::string("");
    }

    DCint length = 0;
    glGetProgramiv(program->getObject(), GL_INFO_LOG_LENGTH, &length);

    if (!length)
        return std::string("(empty string)");

    DCsizei size;
    char info[length];

    glGetProgramInfoLog(program->getObject(), length, &size, info);

    std::string str = std::string(info);

    return str;
}

std::string WebGLGraphicsContext::getShaderSource(WebGLShader* shader)
{
    if (!shader || !shader->getObject())
    {
        LOGE("getShaderSource : shader is null or empty");
        return std::string("");
    }

    DCint length = 0;
    glGetShaderiv(shader->getObject(), GL_SHADER_SOURCE_LENGTH, &length);

    if (!length)
    {
        return std::string("(empty string)");
    }

    DCsizei size;
    char src[length];

    glGetShaderSource(shader->getObject(), length, &size, src);
    std::string str(src);

    return str;
}

bool WebGLGraphicsContext::isBuffer(WebGLBuffer* buffer)
{
    if (!buffer || !buffer->getObject())
    {
        LOGE("isBuffer : buffer is null or empty");
        return false;
    }

    return glIsBuffer(buffer->getObject());
}

bool WebGLGraphicsContext::isProgram(WebGLProgram* program)
{
    if (!program || !program->getObject())
    {
        LOGE("isProgram : program is null or empty");
        return false;
    }

    return glIsProgram(program->getObject());
}

bool WebGLGraphicsContext::isShader(WebGLShader* shader)
{
    if (!shader || !shader->getObject())
    {
        LOGE("isShader : shader is null or empty");
        return false;
    }

    return glIsShader(shader->getObject());
}

bool WebGLGraphicsContext::isTexture(WebGLTexture* texture)
{
    if (!texture || !texture->getObject())
    {
        LOGE("isTexture : texture is null or empty");
        return false;
    }

    return glIsTexture(texture->getObject());
}

bool WebGLGraphicsContext::isRenderbuffer(WebGLRenderBuffer* renderbuffer)
{
    if (!renderbuffer || !renderbuffer->getObject())
    {
        LOGE("isRenderbuffer : renderbuffer is null or empty");
        return false;
    }

    return glIsRenderbuffer(renderbuffer->getObject());
}

bool WebGLGraphicsContext::isFramebuffer(WebGLFrameBuffer* framebuffer)
{
    if (!framebuffer || !framebuffer->getObject())
    {
        LOGE("isFramebuffer : framebuffer is null or empty");
        return false;
    }

    return glIsFramebuffer(framebuffer->getObject());
}

void WebGLGraphicsContext::validateProgram(WebGLProgram* program)
{
    if (!program || !program->getObject())
    {
        LOGE("validateProgram : program is null or empty");
        return;
    }

    glValidateProgram(program->getObject());
}

bool WebGLGraphicsContext::validateTexFuncFormatAndType(DCenum format, DCenum type)
{
    switch (format)
    {
        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA:
        case GL_RGB:
        case GL_RGBA:
            break;
        default:
            LOGE("validateTexFuncFormatAndType : format is not match");
            return false;
    }

    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            break;
        case GL_FLOAT:
            if (m_oesTextureFloat)
                break;
            LOGE("validateTexFuncFormatAndType : not open extention - OES_texture_float");
            return false;
        default:
            LOGE("validateTexFuncFormatAndType : type is not match");
            return false;
    }

    // Verify that the combination of format and type is supported.
    switch (format)
    {
        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA:
            if (type != GL_UNSIGNED_BYTE
                && type != GL_FLOAT)
            {
                LOGE("validateTexFuncFormatAndType : type and format is not match");
                return false;
            }
            break;
        case GL_RGB:
            if (type != GL_UNSIGNED_BYTE
                && type != GL_UNSIGNED_SHORT_5_6_5
                && type != GL_FLOAT)
            {
                LOGE("validateTexFuncFormatAndType : type and format is not match");
                return false;
            }
            break;
        case GL_RGBA:
            if (type != GL_UNSIGNED_BYTE
                && type != GL_UNSIGNED_SHORT_4_4_4_4
                && type != GL_UNSIGNED_SHORT_5_5_5_1
                && type != GL_FLOAT)
            {
                LOGE("validateTexFuncFormatAndType : type and format is not match");
                return false;
            }
            break;
        default:
            break;
    }

    return true;
}

bool WebGLGraphicsContext::validateTexFuncData(DCsizei width, DCsizei height,
                                                DCenum format, DCenum type,
                                                ArrayBufferView* pixels)
{
    if (!pixels)
        return true;

    if (!validateTexFuncFormatAndType(format, type))
    {
        LOGE("validateTexFuncData : validateTexFuncFormatAndType check failed");
        return false;
    }

    switch (type)
    {
        case GL_UNSIGNED_BYTE:
            if (!pixels->isUnsignedByteArray())
            {
                LOGE("validateTexFuncData : ArrayBufferView is not Uint8Array");
                return false;
            }
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            if (!pixels->isUnsignedShortArray())
            {
                LOGE("validateTexFuncData : ArrayBufferView is not Uint16Array");
                return false;
            }
            break;
        case GL_FLOAT: // OES_texture_float
            if (!pixels->isFloatArray())
            {
                LOGE("validateTexFuncData : ArrayBufferView is not Float32Array");
                return false;
            }
            break;
        default:
            break;
    }

    DCuint totalBytesRequired;
    if (!computeImageSizeInBytes(format, type, width, height, m_unpackAlignment, &totalBytesRequired, 0))
    {
        LOGE("validateTexFuncData : computeImageSizeInBytes check failed");
        return false;
    }

    LOGE("validateTexFuncData:  pixels->byteLength = %d", pixels->byteLength());
    LOGE("validateTexFuncData:  totalBytesRequired = %d", totalBytesRequired);

    if (pixels->byteLength() < totalBytesRequired)
    {
        LOGE("validateTexFuncData : pixels data size is invalid");
        return false;
    }

    return true;
}

bool WebGLGraphicsContext::computeFormatAndTypeParameters(DCenum format,
                                                       DCenum type,
                                                       DCuint* componentsPerPixel,
                                                       DCuint* bytesPerComponent)
{
    switch (format)
    {
        case GL_ALPHA:
            *componentsPerPixel = 1;
            break;
        case GL_LUMINANCE:
            *componentsPerPixel = 1;
            break;
        case GL_LUMINANCE_ALPHA:
            *componentsPerPixel = 2;
            break;
        case GL_RGB:
            *componentsPerPixel = 3;
            break;
        case GL_RGBA:
        case GL_EXT_texture_format_BGRA8888: // GL_EXT_texture_format_BGRA8888
            *componentsPerPixel = 4;
            break;
        default:
            LOGE("computeFormatAndTypeParameters : format is not match");
            return false;
    }

    switch (type)
    {
        case GL_UNSIGNED_BYTE:
            *bytesPerComponent = sizeof(DCubyte);
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            *componentsPerPixel = 1;
            *bytesPerComponent = sizeof(DCushort);
            break;
        case GL_FLOAT: // OES_texture_float
            *bytesPerComponent = sizeof(DCfloat);
            break;
        default:
            LOGE("computeFormatAndTypeParameters : type is not match");
            return false;
    }

    return true;
}

bool WebGLGraphicsContext::computeImageSizeInBytes(DCenum format, DCenum type,
                                                   DCsizei width, DCsizei height,
                                                   DCint alignment,
                                                   DCuint* imageSizeInBytes,
                                                   DCuint* paddingInBytes)
{
    if (!imageSizeInBytes)
    {
        LOGE("computeImageSizeInBytes : imageSizeInBytes is NULL, Maybe it's a code bug");
        return false;
    }

    if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
        LOGE("computeImageSizeInBytes : alignment is not match");
        return false;
    }

    if (width < 0 || height < 0)
    {
        LOGE("computeImageSizeInBytes : width or height is invalid");
        return false;
    }

    DCuint bytesPerComponent, componentsPerPixel;
    if (!computeFormatAndTypeParameters(format, type,
               &bytesPerComponent, &componentsPerPixel))
    {
        LOGE("computeImageSizeInBytes : computeFormatAndTypeParameters check failed");
        return false;
    }

    if (!width || !height)
    {
        *imageSizeInBytes = 0;

        if (paddingInBytes)
            *paddingInBytes = 0;

        return true;
    }

    DCuint checkedValue = bytesPerComponent * componentsPerPixel;
    checkedValue *=  width;

    DCuint validRowSize = checkedValue;
    DCuint padding = 0;
    DCuint residual = validRowSize % alignment;

    if (residual)
    {
        padding = alignment - residual;
        checkedValue += padding;
    }
    // Last row needs no padding.
    checkedValue *= (height - 1);
    checkedValue += validRowSize;
    //if (!checkedValue.valid())
    //    return GL_INVALID_VALUE;
    *imageSizeInBytes = checkedValue;//.value();

    if (paddingInBytes)
        *paddingInBytes = padding;

    return true;
}

void WebGLGraphicsContext::finish()
{
    glFinish();
}

DCenum WebGLGraphicsContext::getError()
{
    return glGetError();
}

void WebGLGraphicsContext::disableVertexAttribArray(DCuint index)
{
    if (index >= (DCuint)m_maxVertexAttribs)
    {
        LOGE("disableVertexAttribArray : index is invalid");
        return;
    }

    WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(index);
    state.enabled = false;
    glDisableVertexAttribArray(index);
}

bool WebGLGraphicsContext::isEnabled(DCenum cap)
{
    return glIsEnabled(cap);
}

DCenum WebGLGraphicsContext::getBoundFramebufferColorFormat()
{
    if (m_framebufferBinding && m_framebufferBinding->getObject())
        return m_framebufferBinding->getColorBufferFormat();

    if (m_attributes->alpha())
        return GL_RGBA;

    return GL_RGB;
}

bool WebGLGraphicsContext::validateTexFuncLevel(DCenum target, DCint level)
{
    if (level < 0)
    {
        LOGE("validateTexFuncLevel : level is invalid");
        return false;
    }

    switch (target)
    {
        case GL_TEXTURE_2D:
            if (level > m_maxTextureLevel)
            {
                LOGE("validateTexFuncLevel : level is too large");
                return false;
            }
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            if (level > m_maxCubeMapTextureLevel)
            {
                LOGE("validateTexFuncLevel : level is too large");
                return false;
            }
            break;
    }
    // This function only checks if level is legal.
    return true;
}

bool WebGLGraphicsContext::validateTexFuncParameters(DCenum target, DCint level,
                                                      DCenum internalformat,
                                                      DCsizei width, DCsizei height, DCint border,
                                                      DCenum format, DCenum type)
{
    // We absolutely have to validate the format and type combination.
    // The texImage2D entry points taking HTMLImage, etc. will produce
    // temporary data based on this combination, so it must be legal.
    if (!validateTexFuncFormatAndType(format, type))
    {
        LOGE("validateTexFuncParameters : validateTexFuncFormatAndType check failed");
        return false;
    }

    if (!validateTexFuncLevel(target, level))
    {
        LOGE("validateTexFuncParameters : validateTexFuncLevel check failed");
        return false;
    }

    if (width < 0 || height < 0) {
        LOGE("validateTexFuncParameters : width or height is invalid");
        return false;
    }

    switch (target)
    {
        case GL_TEXTURE_2D:
            if (width > m_maxTextureSize || height > m_maxTextureSize)
            {
                LOGE("validateTexFuncParameters : width or height is too large");
                return false;
            }
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            if (width != height || width > m_maxCubeMapTextureSize)
            {
                LOGE("validateTexFuncParameters : width or height is too large");
                return false;
            }
            break;
        default:
            LOGE("validateTexFuncParameters : target is not match");
            return false;
    }

    if (format != internalformat)
    {
        LOGE("validateTexFuncParameters : format is not equal to internal format");
        return false;
    }

    if (border)
    {
        LOGE("validateTexFuncParameters : border is not zero");
        return false;
    }

    return true;
}

bool WebGLGraphicsContext::validateSize(DCint x, DCint y)
{
    if (x < 0 || y < 0)
    {
        LOGE("validateSize : x, y is not invalid");
        return false;
    }

    return true;
}

WebGLTexture* WebGLGraphicsContext::validateTextureBinding(DCenum target, bool useSixEnumsForCubeMap)
{
    WebGLTexture* tex = NULL;

    switch (target)
    {
        case GL_TEXTURE_2D:
            tex = m_textureUnits[m_activeTextureUnit].m_texture2DBinding;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            if (!useSixEnumsForCubeMap)
            {
                LOGE("validateTextureBinding : not use SixEnumsForCubeMap");
                return NULL;
            }
            tex = m_textureUnits[m_activeTextureUnit].m_textureCubeMapBinding;
            break;
        case GL_TEXTURE_CUBE_MAP:
            if (useSixEnumsForCubeMap)
            {
                LOGE("validateTextureBinding : use SixEnumsForCubeMap");
                return NULL;
            }
            tex = m_textureUnits[m_activeTextureUnit].m_textureCubeMapBinding;
            break;
        default:
            LOGE("validateTextureBinding : target is not match");
            return NULL;
    }

    if (!tex)
    {
        LOGE("validateTextureBinding : can't get correct texture, return null");
    }

    return tex;
}

void WebGLGraphicsContext::getIntegerv(DCenum pname, DCint* value)
{
    glGetIntegerv(pname, value);
}

bool WebGLGraphicsContext::isTexInternalFormatColorBufferCombinationValid(DCenum texInternalFormat,
                                                                           DCenum colorBufferFormat)
{
    switch (colorBufferFormat)
    {
        case GL_ALPHA:
            if (texInternalFormat == GL_ALPHA)
            {
                return true;
            }
            break;
        case GL_RGB:
            if (texInternalFormat == GL_LUMINANCE
                || texInternalFormat == GL_RGB)
            {
                return true;
            }
            break;
        case GL_RGBA:
            return true;
    }

    return false;
}

void WebGLGraphicsContext::copyTexImage2D(DCenum target, DCint level,
                                          DCenum internalformat,
                                          DCint x, DCint y,
                                          DCsizei width, DCsizei height,
                                          DCint border)
{
    if (!validateTexFuncParameters(target, level, internalformat,
            width, height, border, internalformat, GL_UNSIGNED_BYTE))
    {
        LOGE("copyTexImage2D : validateTexFuncParameters check failed");
        return;
    }

    WebGLTexture* tex = validateTextureBinding(target, true);
    if (!tex)
    {
        LOGE("copyTexImage2D : binding texture is null");
        return;
    }

    if (!isTexInternalFormatColorBufferCombinationValid(internalformat, getBoundFramebufferColorFormat()))
    {
        LOGE("copyTexImage2D : internalformat is invalid");
        return;
    }

    if (level && WebGLTexture::isNPOT(width, height))
    {
        LOGE("copyTexImage2D : width or height is not power of 2");
        return;
    }

    if (m_framebufferBinding && !m_framebufferBinding->getObject())
    {
        LOGE("copyTexImage2D : binding framebuffer is null or empty");
        return;
    }

    glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);

    tex->setLevelInfo(target, level, internalformat, width, height, GL_UNSIGNED_BYTE);
}

void WebGLGraphicsContext::copyTexSubImage2D(DCenum target, DCint level,
                                             DCint xoffset, DCint yoffset,
                                             DCint x, DCint y,
                                             DCsizei width, DCsizei height)
{
    if (!validateTexFuncLevel(target, level))
    {
        LOGE("copyTexSubImage2D : validateTexFuncLevel check failed");
        return;
    }

    WebGLTexture* tex = validateTextureBinding(target, true);
    if (!tex)
    {
        LOGE("copyTexSubImage2D : binding texture is null");
        return;
    }

    if (!validateSize(xoffset, yoffset) || !validateSize(width, height))
    {
        LOGE("copyTexSubImage2D : offset or x, y is invalid");
        return;
    }

    if (xoffset + width > tex->getWidth(target, level)
            || yoffset + height > tex->getHeight(target, level))
    {
        LOGE("copyTexSubImage2D : sub texture out of the src texture");
        return;
    }

    if (!isTexInternalFormatColorBufferCombinationValid(
            tex->getInternalFormat(target, level), getBoundFramebufferColorFormat()))
    {
        LOGE("copyTexSubImage2D : isTexInternalFormatColorBufferCombinationValid check failed");
        return;
    }

    if (m_framebufferBinding && !m_framebufferBinding->getObject())
    {
        LOGE("copyTexSubImage2D : binding framebuffer is null or empty");
        return;
    }

    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void WebGLGraphicsContext::texSubImage2DBase(DCenum target, DCint level, DCint xoffset, DCint yoffset,
                                              DCsizei width, DCsizei height,
                                              DCenum format, DCenum type, void* pixels)
{
    if (!validateTexFuncParameters(target, level, format, width, height, 0, format, type))
    {
        LOGE("texSubImage2DBase : validateTexFuncParameters check failed");
        return;
    }

    if (!validateSize(xoffset, yoffset))
    {
        LOGE("texSubImage2DBase : validateSize check failed");
        return;
    }

    WebGLTexture* tex = validateTextureBinding(target, true);
    if (!tex)
    {
        LOGE("texSubImage2DBase : binding texture is null");
        return;
    }

    if (xoffset + width > tex->getWidth(target, level)
            || yoffset + height > tex->getHeight(target, level))
    {
        LOGE("texSubImage2DBase : sub texture out of the src texture");
        return;
    }
    if (tex->getInternalFormat(target, level) != format
            || tex->getType(target, level) != type)
    {
        LOGE("texSubImage2DBase : format or type is not equal to src texture's");
        return;
    }

    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}


void WebGLGraphicsContext::texSubImage2D(DCenum target, DCint level,
                                         DCint xoffset, DCint yoffset,
                                         DCsizei width, DCsizei height,
                                         DCenum format, DCenum type,
                                         ArrayBufferView* pixels)
{
    if (!validateTexFuncData(width, height, format, type, pixels))
    {
        LOGE("texSubImage2D : validateTexFuncData check failed");
        return;
    }

    DCubyte* outData = NULL;
    if (pixels)
    {
        if (!makeWebGLPixelsData(width, height, format, type, pixels->baseAddress(), outData))
        {
            LOGE("texSubImage2D : makeWebGLPixelsData falied");
            return;
        }
    }

    texSubImage2DBase(target, level, xoffset, yoffset, width, height, format, type, outData);

    if (outData)
    {
        delete[] outData;
        outData = NULL;
    }
}

void WebGLGraphicsContext::texSubImage2D(DCenum target, DCint level,
                                         DCint xoffset, DCint yoffset,
                                         DCenum format, DCenum type, Image* img)
{
    DCubyte* outData = NULL;

    if (img)
    {
        DCsizei width = img->getWidth();
        DCsizei height = img->getHeight();
        DCsizei totalBytes = width * height * 4;

        if (!makeWebGLImageData(img, format, type, outData))
        {
            LOGE("texSubImage2D : makeWebGLImageData failed");
            return;
        }
    }

    texSubImage2DBase(target, level, xoffset, yoffset, img->getWidth(), img->getHeight(), format, type, outData);

    if (outData)
    {
        delete[] outData;
        outData = NULL;
    }
}

void WebGLGraphicsContext::hint(DCenum target, DCenum mode)
{
    bool isValid = false;

    switch (target)
    {
        case GL_GENERATE_MIPMAP_HINT:
            isValid = true;
            break;
        case GL_OES_standard_derivatives: // OES_standard_derivatives
            if (m_oesStandardDerivatives)
                isValid = true;
            break;
    }

    if (!isValid)
    {
        LOGE("hint : target is not match");
        return;
    }

    glHint(target, mode);
}

bool WebGLGraphicsContext::packPixels(const uint8_t* sourceData,
                                   SourceDataFormat sourceDataFormat,
                                   DCuint width,
                                   DCuint height,
                                   DCuint sourceUnpackAlignment,
                                   DCuint destinationFormat,
                                   DCuint destinationType,
                                   AlphaOp alphaOp,
                                   void* destinationData)
{
    switch (destinationType)
    {
        case GL_UNSIGNED_BYTE:
        {
            uint8_t* destination = static_cast<uint8_t*>(destinationData);
            if (sourceDataFormat == SourceFormatRGBA8 &&
                destinationFormat == GL_RGBA &&
                sourceUnpackAlignment <= 4 &&
                alphaOp == AlphaDoNothing)
            {
                // No conversion necessary.
                memcpy(destinationData, sourceData, width * height * 4);
                break;
            }

            switch (destinationFormat)
            {
                case GL_RGB:
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRGB8, 3);
                            break;
                        case AlphaDoPremultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRGB8Premultiply, 3);
                            break;
                        case AlphaDoUnmultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRGB8Unmultiply, 3);
                            break;
                    }
                    break;
                case GL_RGBA:
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            ASSERT(sourceDataFormat != SourceFormatRGBA8
                                || sourceUnpackAlignment > 4); // Handled above with fast case.

                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, 0, 4);
                            break;
                        case AlphaDoPremultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRGBA8Premultiply, 4);
                            break;
                        case AlphaDoUnmultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRGBA8Unmultiply, 4);
                            break;
                        default:
                            ASSERT_NOT_REACHED();
                            break;
                    }
                    break;
                case GL_ALPHA:
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the alpha channel is chosen
                    // from the RGBA data.
                    doPacking<uint8_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToA8, 1);
                    break;
                case GL_LUMINANCE:
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the red channel is chosen
                    // from the RGBA data.
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToR8, 1);
                            break;
                        case AlphaDoPremultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToR8Premultiply, 1);
                            break;
                        case AlphaDoUnmultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToR8Unmultiply, 1);
                            break;
                    }
                    break;
                case GL_LUMINANCE_ALPHA:
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the red and alpha channels
                    // are chosen from the RGBA data.
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRA8, 2);
                            break;
                        case AlphaDoPremultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRA8Premultiply, 2);
                            break;
                        case AlphaDoUnmultiply:
                            doPacking<uint8_t>(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA8ToRA8Unmultiply, 2);
                            break;
                    } // switch (alphaOp)
                    break;
            } // switch (destinationFormat)
            break;
        } // case GL_UNSIGNED_BYTE:

        case GL_UNSIGNED_SHORT_4_4_4_4:
        {
            uint16_t* destination = static_cast<uint16_t*>(destinationData);
            switch (alphaOp)
            {
                case AlphaDoNothing:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort4444, 1);
                    break;
                case AlphaDoPremultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort4444Premultiply, 1);
                    break;
                case AlphaDoUnmultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort4444Unmultiply, 1);
                    break;
            }
            break;
        } // case GL_UNSIGNED_SHORT_4_4_4_4:

        case GL_UNSIGNED_SHORT_5_5_5_1:
        {
            uint16_t* destination = static_cast<uint16_t*>(destinationData);
            switch (alphaOp) {
                case AlphaDoNothing:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort5551, 1);
                    break;
                case AlphaDoPremultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort5551Premultiply, 1);
                    break;
                case AlphaDoUnmultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort5551Unmultiply, 1);
                    break;
            }
            break;
        } //case GL_UNSIGNED_SHORT_5_5_5_1:

        case GL_UNSIGNED_SHORT_5_6_5:
        {
            uint16_t* destination = static_cast<uint16_t*>(destinationData);
            switch (alphaOp)
            {
                case AlphaDoNothing:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort565, 1);
                    break;
                case AlphaDoPremultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort565Premultiply, 1);
                    break;
                case AlphaDoUnmultiply:
                    doPacking<uint16_t>(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA8ToUnsignedShort565Unmultiply, 1);
                    break;
            }
            break;
        } // case GL_UNSIGNED_SHORT_5_6_5:

        case GL_FLOAT:
        {
            // OpenGL ES, and therefore WebGL, require that the format and
            // internalformat be identical, which implies that the source and
            // destination formats will both be floating-point in this branch -- at
            // least, until WebKit supports floating-point image formats natively.
            ASSERT(sourceDataFormat == SourceFormatRGBA32F || sourceDataFormat == SourceFormatRGB32F
                   || sourceDataFormat == SourceFormatRA32F || sourceDataFormat == SourceFormatR32F
                   || sourceDataFormat == SourceFormatA32F);
            // Because WebKit doesn't use floating-point color channels for anything
            // internally, there's no chance we have to do a (lossy) unmultiply
            // operation.
            ASSERT(alphaOp == AlphaDoNothing || alphaOp == AlphaDoPremultiply);
            // For the source formats with an even number of channels (RGBA32F,
            // RA32F) it is guaranteed that the pixel data is tightly packed because
            // unpack alignment <= sizeof(float) * number of channels.
            float* destination = static_cast<float*>(destinationData);
            if (alphaOp == AlphaDoNothing
                && ((sourceDataFormat == SourceFormatRGBA32F && destinationFormat == GL_RGBA)
                || (sourceDataFormat == SourceFormatRA32F && destinationFormat == GL_LUMINANCE_ALPHA)))
            {
                // No conversion necessary.
                int numChannels = (sourceDataFormat == SourceFormatRGBA32F ? 4 : 2);
                memcpy(destinationData, sourceData, width * height * numChannels * sizeof(float));
                break;
            }

            switch (destinationFormat)
            {
                case GL_RGB:
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToRGB32F, 3);
                            break;
                        case AlphaDoPremultiply:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToRGB32FPremultiply, 3);
                            break;
                        default:
                            ASSERT_NOT_REACHED();
                    }
                    break;
                case GL_RGBA:
                    // AlphaDoNothing is handled above with fast path.
                    ASSERT(alphaOp == AlphaDoPremultiply);
                    doFloatingPointPacking(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA32FToRGBA32FPremultiply, 4);
                    break;
                case GL_ALPHA:
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the alpha channel is chosen
                    // from the RGBA data.
                    doFloatingPointPacking(sourceData, sourceDataFormat,
                            width, height, sourceUnpackAlignment,
                            destination, packOneRowOfRGBA32FToA32F, 1);
                    break;
                case GL_LUMINANCE:
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the red channel is chosen
                    // from the RGBA data.
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToR32F, 1);
                            break;
                        case AlphaDoPremultiply:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToR32FPremultiply, 1);
                            break;
                        default:
                            ASSERT_NOT_REACHED();
                    }
                    break;
                case GL_LUMINANCE_ALPHA:
                {
                    // From the desktop OpenGL conversion rules (OpenGL 2.1
                    // specification, Table 3.15), the red and alpha channels
                    // are chosen from the RGBA data.
                    switch (alphaOp)
                    {
                        case AlphaDoNothing:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToRA32F, 2);
                            break;
                        case AlphaDoPremultiply:
                            doFloatingPointPacking(sourceData, sourceDataFormat,
                                    width, height, sourceUnpackAlignment,
                                    destination, packOneRowOfRGBA32FToRA32FPremultiply, 2);
                            break;
                        default:
                            ASSERT_NOT_REACHED();
                    } // switch (alphaOp)
                    break;
                } // case GL_LUMINANCE_ALPHA:
            } // switch (destinationFormat)
            break;
        }
    } // switch (destinationType)
    return true;
}

bool WebGLGraphicsContext::validateVertexAttribfv(DCuint index, DCfloat* v, DCsizei size, DCsizei expectedSize)
{
    if (!v)
    {
        LOGE("validateVertexAttribfv : float array is NULL");
        return false;
    }

    if (size < expectedSize)
    {
        LOGE("validateVertexAttribfv : size is smaller than expected size");
        return false;
    }

    if (index >= (DCuint)m_maxVertexAttribs)
    {
        LOGE("validateVertexAttribfv : index is too large");
        return false;
    }

    return true;
}

void WebGLGraphicsContext::setVertexAttribValue(DCuint index, DCfloat v0, DCfloat v1, DCfloat v2, DCfloat v3)
{
    VertexAttribValue& attribValue = m_vertexAttribValue[index];
    attribValue.value[0] = v0;
    attribValue.value[1] = v1;
    attribValue.value[2] = v2;
    attribValue.value[3] = v3;
}

void WebGLGraphicsContext::setVertexAttribValues(DCuint index, DCsizei expectedSize, DCfloat* v)
{
    VertexAttribValue& attribValue = m_vertexAttribValue[index];
    attribValue.initValue();
    for (int ii = 0; ii < expectedSize; ++ii)
        attribValue.value[ii] = v[ii];
}

void WebGLGraphicsContext::vertexAttrib1f(DCuint index, DCfloat x)
{
    glVertexAttrib1f(index, x);
    setVertexAttribValue(index, x, 0.0, 0.0, 1.0);
}

void WebGLGraphicsContext::vertexAttrib1fv(DCuint index, Float32Array* values)
{
    if (!validateVertexAttribfv(index, values->data(), values->length(), 1))
    {
        LOGE("vertexAttrib1fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib1fv(index, values->data());
    setVertexAttribValues(index, 1, values->data());
}

void WebGLGraphicsContext::vertexAttrib1fv(DCuint index, DCfloat* values, DCsizei size)
{
    if (!validateVertexAttribfv(index, values, size, 1))
    {
        LOGE("vertexAttrib1fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib1fv(index, values);
    setVertexAttribValues(index, 1, values);
}

void WebGLGraphicsContext::vertexAttrib2f(DCuint index, DCfloat x, DCfloat y)
{
    glVertexAttrib2f(index, x, y);
    setVertexAttribValue(index,    x, y, 0.0, 1.0);
}

void WebGLGraphicsContext::vertexAttrib2fv(DCuint index, Float32Array* values)
{
    if (!validateVertexAttribfv(index, values->data(), values->length(), 2))
    {
        LOGE("vertexAttrib2fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib2fv(index, values->data());
    setVertexAttribValues(index, 2, values->data());
}

void WebGLGraphicsContext::vertexAttrib2fv(DCuint index, DCfloat* values, DCsizei size)
{
    if (!validateVertexAttribfv(index, values, size, 2))
    {
        LOGE("vertexAttrib2fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib2fv(index, values);
    setVertexAttribValues(index, 2, values);
}

void WebGLGraphicsContext::vertexAttrib3f(DCuint index, DCfloat x, DCfloat y, DCfloat z)
{
    glVertexAttrib3f(index, x, y, z);
    setVertexAttribValue(index, x, y, z, 1.0);
}

void WebGLGraphicsContext::vertexAttrib3fv(DCuint index, Float32Array* values)
{
    if (!validateVertexAttribfv(index, values->data(), values->length(), 3))
    {
        LOGE("vertexAttrib3fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib3fv(index, values->data());
    setVertexAttribValues(index, 3, values->data());
}

void WebGLGraphicsContext::vertexAttrib3fv(DCuint index, DCfloat* values, DCsizei size)
{
    if (!validateVertexAttribfv(index, values, size, 3))
    {
        LOGE("vertexAttrib3fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib3fv(index, values);
    setVertexAttribValues(index, 3, values);
}

void WebGLGraphicsContext::vertexAttrib4f(DCuint index, DCfloat x, DCfloat y, DCfloat z, DCfloat w)
{
    glVertexAttrib4f(index, x, y, z, w);
    setVertexAttribValue(index, x, y, z, w);
}

void WebGLGraphicsContext::vertexAttrib4fv(DCuint index, Float32Array* values)
{
    if (!validateVertexAttribfv(index, values->data(), values->length(), 4))
    {
        LOGE("vertexAttrib4fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib4fv(index, values->data());
    setVertexAttribValues(index, 4, values->data());
}

void WebGLGraphicsContext::vertexAttrib4fv(DCuint index, DCfloat* values, DCsizei size)
{
    if (!validateVertexAttribfv(index, values, size, 4))
    {
        LOGE("vertexAttrib4fv : validateVertexAttribfv check failed");
        return;
    }

    glVertexAttrib4fv(index, values);
    setVertexAttribValues(index, 4, values);
}

WebGLGetInfo WebGLGraphicsContext::getProgramParameter(WebGLProgram* program, DCenum pname)
{
    if (!program || !program->getObject())
    {
        LOGE("getProgramParameter : program is null or empty");
        return WebGLGetInfo();
    }

    DCint value = 0;
    switch (pname)
    {
        case GL_DELETE_STATUS:
            return WebGLGetInfo(program->isDeleted());
        case GL_VALIDATE_STATUS:
        case GL_LINK_STATUS:
            glGetProgramiv(program->getObject(), pname, &value);
            return WebGLGetInfo(static_cast<bool>(value));
        case GL_ATTACHED_SHADERS:
        case GL_ACTIVE_ATTRIBUTES:
        case GL_ACTIVE_UNIFORMS:
            glGetProgramiv(program->getObject(), pname, &value);
            return WebGLGetInfo(value);
        default:
            return WebGLGetInfo();
    }
}

WebGLGetInfo WebGLGraphicsContext::getShaderParameter(WebGLShader* shader, DCenum pname)
{
    if (!shader || !shader->getObject())
    {
        LOGE("getShaderParameter : shader is null or empty");
        return WebGLGetInfo();
    }

    DCint value = 0;
    switch (pname)
    {
        case GL_DELETE_STATUS:
            return WebGLGetInfo(shader->isDeleted());
        case GL_COMPILE_STATUS:
            glGetShaderiv(shader->getObject(), pname, &value);
            return WebGLGetInfo(static_cast<bool>(value));
        case GL_SHADER_TYPE:
            glGetShaderiv(shader->getObject(), pname, &value);
            return WebGLGetInfo(static_cast<DCuint>(value));
        default:
            return WebGLGetInfo();
    }
}

WebGLGetInfo WebGLGraphicsContext::getTexParameter(DCenum target, DCenum pname)
{
    WebGLTexture* tex = validateTextureBinding(target, false);

    if (!tex)
    {
        LOGE("getTexParameter : binding texture is null");
        return WebGLGetInfo();
    }

    DCint value = 0;
    switch (pname)
    {
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
            glGetTexParameteriv(target, pname, &value);
            return WebGLGetInfo(static_cast<DCuint>(value));
        default:
            return WebGLGetInfo();
    }
}

WebGLGetInfo WebGLGraphicsContext::getRenderbufferParameter(DCenum target, DCenum pname)
{
    if (target != GL_RENDERBUFFER)
    {
        LOGE("getRenderbufferParameter : target is not RENDERBUFFER");
        return WebGLGetInfo();
    }

    if (!m_renderbufferBinding || !m_renderbufferBinding->getObject())
    {
        LOGE("getRenderbufferParameter : binding renderbuffer is null or empty");
        return WebGLGetInfo();
    }

    DCint value = 0;
    switch (pname)
    {
        case GL_RENDERBUFFER_WIDTH:
        case GL_RENDERBUFFER_HEIGHT:
        case GL_RENDERBUFFER_RED_SIZE:
        case GL_RENDERBUFFER_GREEN_SIZE:
        case GL_RENDERBUFFER_BLUE_SIZE:
        case GL_RENDERBUFFER_ALPHA_SIZE:
        case GL_RENDERBUFFER_DEPTH_SIZE:
        case GL_RENDERBUFFER_STENCIL_SIZE:
            glGetRenderbufferParameteriv(target, pname, &value);
            return WebGLGetInfo(value);
        case GL_RENDERBUFFER_INTERNAL_FORMAT:
               glGetRenderbufferParameteriv(target, pname, &value);
            return WebGLGetInfo(static_cast<DCuint>(value));
        default:
            return WebGLGetInfo();
    }
}

WebGLGetInfo WebGLGraphicsContext::getFramebufferAttachmentParameter(DCenum target, DCenum attachment, DCenum pname)
{
    if (!validateFramebufferFuncParameters(target, attachment))
    {
        LOGE("getFramebufferAttachmentParameter : validateFramebufferFuncParameters check failed");
        return WebGLGetInfo();
    }

    if (!m_framebufferBinding || !m_framebufferBinding->getObject())// || m_framebufferBinding->isIncomplete(false))
    {
        LOGE("getFramebufferAttachmentParameter : binding framebuffer is null or empty");
        return WebGLGetInfo();
    }

    WebGLObject* object = m_framebufferBinding->getAttachment(attachment);
    if (!object)
    {
        if (pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE)
            return WebGLGetInfo(GL_NONE);

        // OpenGL ES 2.0 specifies INVALID_ENUM in this case, while desktop GL
        // specifies INVALID_OPERATION.
        LOGE("getFramebufferAttachmentParameter : attached object is null");
        return WebGLGetInfo();
    }

    if (!object->isTexture() && !object->isRenderBuffer())
    {
        LOGE("getFramebufferAttachmentParameter : attached object is not match");
        return WebGLGetInfo();
    }

    if (object->isTexture())
    {
        switch (pname)
        {
            case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
                return WebGLGetInfo(GL_TEXTURE);
            case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
                return WebGLGetInfo(reinterpret_cast<WebGLTexture*>(object));
            case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
            case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
                {
                    DCint value = 0;
                    if (attachment == GL_DEPTH_STENCIL_ATTACHMENT)
                        attachment = GL_DEPTH_ATTACHMENT; // Or STENCIL_ATTACHMENT, either works.
                    glGetFramebufferAttachmentParameteriv(target, attachment, pname, &value);
                    return WebGLGetInfo(value);
                }
            default:
                return WebGLGetInfo();
        }
    }
    else
    {
        switch (pname)
        {
            case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
                return WebGLGetInfo(GL_RENDERBUFFER);
            case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
                return WebGLGetInfo(reinterpret_cast<WebGLRenderBuffer*>(object));
            default:
                return WebGLGetInfo();
        }
    }
}

bool WebGLGraphicsContext::validateFramebufferFuncParameters(DCenum target, DCenum attachment)
{
    if (target != GL_FRAMEBUFFER)
    {
        LOGE("validateFramebufferFuncParameters : target is not FRAMEBUFFER");
        return false;
    }

    switch (attachment)
    {
        case GL_COLOR_ATTACHMENT0:
        case GL_DEPTH_ATTACHMENT:
        case GL_STENCIL_ATTACHMENT:
        case GL_DEPTH_STENCIL_ATTACHMENT:
            break;
        default:
            LOGE("validateFramebufferFuncParameters : attachment is not match");
            return false;
    }

    return true;
}

WebGLGetInfo WebGLGraphicsContext::getBooleanParameter(DCenum pname)
{
    DCboolean value = 0;
    glGetBooleanv(pname, &value);
    return WebGLGetInfo(static_cast<bool>(value));
}

WebGLGetInfo WebGLGraphicsContext::getBooleanArrayParameter(DCenum pname)
{
    if (pname != GL_COLOR_WRITEMASK)
    {
        return WebGLGetInfo(0, 0);
    }

    DCboolean value[4] = {0};
    glGetBooleanv(pname, value);
    bool boolValue[4];

    for (int ii = 0; ii < 4; ++ii)
        boolValue[ii] = static_cast<bool>(value[ii]);

    return WebGLGetInfo(boolValue, 4);
}

WebGLGetInfo WebGLGraphicsContext::getFloatParameter(DCenum pname)
{
    DCfloat value = 0;
    glGetFloatv(pname, &value);
    return WebGLGetInfo(value);
}

WebGLGetInfo WebGLGraphicsContext::getIntParameter(DCenum pname)
{
    DCint value = 0;
    glGetIntegerv(pname, &value);
    return WebGLGetInfo(value);
}

WebGLGetInfo WebGLGraphicsContext::getUnsignedIntParameter(DCenum pname)
{
    DCint value = 0;
    glGetIntegerv(pname, &value);
    return WebGLGetInfo(static_cast<DCuint>(value));
}

WebGLGetInfo WebGLGraphicsContext::getWebGLFloatArrayParameter(DCenum pname)
{
    DCfloat value[4] = {0};
    glGetFloatv(pname, value);
    unsigned length = 0;

    switch (pname)
    {
        case GL_ALIASED_POINT_SIZE_RANGE:
        case GL_ALIASED_LINE_WIDTH_RANGE:
        case GL_DEPTH_RANGE:
            length = 2;
            break;
        case GL_BLEND_COLOR:
        case GL_COLOR_CLEAR_VALUE:
            length = 4;
            break;
    }

    return WebGLGetInfo(Float32Array::create(value, length));
}

WebGLGetInfo WebGLGraphicsContext::getWebGLIntArrayParameter(DCenum pname)
{
    DCint value[4] = {0};
    glGetIntegerv(pname, value);
    unsigned length = 0;

    switch (pname)
    {
        case GL_MAX_VIEWPORT_DIMS:
            length = 2;
            break;
        case GL_SCISSOR_BOX:
        case GL_VIEWPORT:
            length = 4;
            break;
    }

    return WebGLGetInfo(Int32Array::create(value, length));
}

WebGLGetInfo WebGLGraphicsContext::getParameter(DCenum pname)
{
    std::string strValue;
    switch (pname)
    {
        case GL_ACTIVE_TEXTURE:
            return getUnsignedIntParameter(pname);
        case GL_ALIASED_LINE_WIDTH_RANGE:
            return getWebGLFloatArrayParameter(pname);
        case GL_ALIASED_POINT_SIZE_RANGE:
            return getWebGLFloatArrayParameter(pname);
        case GL_ALPHA_BITS:
            return getIntParameter(pname);
        case GL_ARRAY_BUFFER_BINDING:
            return WebGLGetInfo(m_boundArrayBuffer);
        case GL_BLEND:
            return getBooleanParameter(pname);
        case GL_BLEND_COLOR:
            return getWebGLFloatArrayParameter(pname);
        case GL_BLEND_DST_ALPHA:
            return getUnsignedIntParameter(pname);
        case GL_BLEND_DST_RGB:
            return getUnsignedIntParameter(pname);
        case GL_BLEND_EQUATION_ALPHA:
            return getUnsignedIntParameter(pname);
        case GL_BLEND_EQUATION_RGB:
            return getUnsignedIntParameter(pname);
        case GL_BLEND_SRC_ALPHA:
            return getUnsignedIntParameter(pname);
        case GL_BLEND_SRC_RGB:
            return getUnsignedIntParameter(pname);
        case GL_BLUE_BITS:
            return getIntParameter(pname);
        case GL_COLOR_CLEAR_VALUE:
            return getWebGLFloatArrayParameter(pname);
        case GL_COLOR_WRITEMASK:
            return getBooleanArrayParameter(pname);
        case GL_COMPRESSED_TEXTURE_FORMATS:
            // Defined as null in the spec
            return WebGLGetInfo();
        case GL_CULL_FACE:
            return getBooleanParameter(pname);
        case GL_CULL_FACE_MODE:
            return getUnsignedIntParameter(pname);
        case GL_CURRENT_PROGRAM:
            return WebGLGetInfo(m_currentProgram);
        case GL_DEPTH_BITS:
            return getIntParameter(pname);
        case GL_DEPTH_CLEAR_VALUE:
            return getFloatParameter(pname);
        case GL_DEPTH_FUNC:
            return getUnsignedIntParameter(pname);
        case GL_DEPTH_RANGE:
            return getWebGLFloatArrayParameter(pname);
        case GL_DEPTH_TEST:
            return getBooleanParameter(pname);
        case GL_DEPTH_WRITEMASK:
            return getBooleanParameter(pname);
        case GL_DITHER:
            return getBooleanParameter(pname);
        case GL_ELEMENT_ARRAY_BUFFER_BINDING:
            return WebGLGetInfo("not support in this version");
            //return WebGLGetInfo(m_boundVertexArrayObject,getElementArrayBuffer()));
        case GL_FRAMEBUFFER_BINDING:
            return WebGLGetInfo(m_framebufferBinding);
        case GL_FRONT_FACE:
            return getUnsignedIntParameter(pname);
        case GL_GENERATE_MIPMAP_HINT:
            return getUnsignedIntParameter(pname);
        case GL_GREEN_BITS:
            return getIntParameter(pname);
        case GL_LINE_WIDTH:
            return getFloatParameter(pname);
        case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
            return getIntParameter(pname);
        case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
            return getIntParameter(pname);
        case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
            return getIntParameter(pname);
        case GL_MAX_RENDERBUFFER_SIZE:
            return getIntParameter(pname);
        case GL_MAX_TEXTURE_IMAGE_UNITS:
            return getIntParameter(pname);
        case GL_MAX_TEXTURE_SIZE:
            return getIntParameter(pname);
        case GL_MAX_VARYING_VECTORS:
            return getIntParameter(pname);
        case GL_MAX_VERTEX_ATTRIBS:
            return getIntParameter(pname);
        case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
            return getIntParameter(pname);
        case GL_MAX_VERTEX_UNIFORM_VECTORS:
            return getIntParameter(pname);
        case GL_MAX_VIEWPORT_DIMS:
            return getWebGLIntArrayParameter(pname);
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
            // WebGL 1.0 specifies that there are no compressed texture formats.
            return WebGLGetInfo(static_cast<int>(0));
        case GL_NUM_SHADER_BINARY_FORMATS:
            // FIXME: should we always return 0 for this?
            return getIntParameter(pname);
        case GL_PACK_ALIGNMENT:
            return getIntParameter(pname);
        case GL_POLYGON_OFFSET_FACTOR:
            return getFloatParameter(pname);
        case GL_POLYGON_OFFSET_FILL:
            return getBooleanParameter(pname);
        case GL_POLYGON_OFFSET_UNITS:
            return getFloatParameter(pname);
        case GL_RED_BITS:
            return getIntParameter(pname);
        case GL_RENDERBUFFER_BINDING:
            return WebGLGetInfo(m_renderbufferBinding);
        case GL_RENDERER:
            return WebGLGetInfo(glGetString(GL_RENDERER));
        case GL_SAMPLE_BUFFERS:
            return getIntParameter(pname);
        case GL_SAMPLE_COVERAGE_INVERT:
            return getBooleanParameter(pname);
        case GL_SAMPLE_COVERAGE_VALUE:
            return getFloatParameter(pname);
        case GL_SAMPLES:
            return getIntParameter(pname);
        case GL_SCISSOR_BOX:
            return getWebGLIntArrayParameter(pname);
        case GL_SCISSOR_TEST:
            return getBooleanParameter(pname);
        case GL_SHADING_LANGUAGE_VERSION:
            strValue = std::string("DCanvas WebGL GLSL ES 1.0 (") + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) + std::string(")");
            return WebGLGetInfo(strValue);
        case GL_STENCIL_BACK_FAIL:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BACK_FUNC:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BACK_PASS_DEPTH_PASS:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BACK_REF:
            return getIntParameter(pname);
        case GL_STENCIL_BACK_VALUE_MASK:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BACK_WRITEMASK:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_BITS:
            return getIntParameter(pname);
        case GL_STENCIL_CLEAR_VALUE:
            return getIntParameter(pname);
        case GL_STENCIL_FAIL:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_FUNC:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_PASS_DEPTH_FAIL:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_PASS_DEPTH_PASS:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_REF:
            return getIntParameter(pname);
        case GL_STENCIL_TEST:
            return getBooleanParameter(pname);
        case GL_STENCIL_VALUE_MASK:
            return getUnsignedIntParameter(pname);
        case GL_STENCIL_WRITEMASK:
            return getUnsignedIntParameter(pname);
        case GL_SUBPIXEL_BITS:
            return getIntParameter(pname);
        case GL_TEXTURE_BINDING_2D:
            return WebGLGetInfo(m_textureUnits[m_activeTextureUnit].m_texture2DBinding);
        case GL_TEXTURE_BINDING_CUBE_MAP:
            return WebGLGetInfo(m_textureUnits[m_activeTextureUnit].m_textureCubeMapBinding);
        case GL_UNPACK_ALIGNMENT:
            return getIntParameter(pname);
        case UNPACK_FLIP_Y_WEBGL:
            return WebGLGetInfo((bool)m_unpackFlipY);
        case UNPACK_PREMULTIPLY_ALPHA_WEBGL:
            return WebGLGetInfo((bool)m_unpackPremultiplyAlpha);
        case UNPACK_COLORSPACE_CONVERSION_WEBGL:
            return WebGLGetInfo("not support");
            //return WebGLGetInfo(m_unpackColorspaceConversion);
        case GL_VENDOR:
            strValue = std::string("aliyun (") + std::string((char*)glGetString(GL_VENDOR)) + std::string(")");
            return WebGLGetInfo(strValue);
        case GL_VERSION:
            strValue = std::string("DCanvas WebGL 1.0 (") + std::string((char*)glGetString(GL_VERSION)) + std::string(")");
            return WebGLGetInfo(strValue);
        case GL_VIEWPORT:
            return getWebGLIntArrayParameter(pname);
        case GL_OES_standard_derivatives:
        //case Extensions3D::FRAGMENT_SHADER_DERIVATIVE_HINT_OES: // OES_standard_derivatives
            if (m_oesStandardDerivatives)
                return getUnsignedIntParameter(GL_OES_standard_derivatives);
            return WebGLGetInfo();
        //case GL_OES_vertex_array_object:
        //case Extensions3D::VERTEX_ARRAY_BINDING_OES: // OES_vertex_array_object
            //if (m_oesVertexArrayObject) {
        //        return WebGLGetInfo("not support in this version");
            /*
                if (!m_boundVertexArrayObject->isDefaultObject())
                    return WebGLGetInfo(PassRefPtr<WebGLVertexArrayObjectOES>(m_boundVertexArrayObject));
                return WebGLGetInfo();
            */
            //}
            return WebGLGetInfo();
        default:
            return WebGLGetInfo();
    }
}

WebGLGetInfo WebGLGraphicsContext::getVertexAttrib(DCuint index, DCenum pname)
{
    if (index >= (DCuint)m_maxVertexAttribs)
    {
        LOGE("getVertexAttrib : index is too large");
        return WebGLGetInfo();
    }
    //const WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(index);

    DCint value = 0;
    DCfloat* values;

    switch (pname)
    {
        case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
            if (!m_boundArrayBuffer || !m_boundArrayBuffer->getObject())
                return WebGLGetInfo();
            return WebGLGetInfo(m_boundArrayBuffer);
        case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
        case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
            glGetVertexAttribiv(index, pname, &value);
            return WebGLGetInfo((bool)value);
        case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        case GL_VERTEX_ATTRIB_ARRAY_TYPE:
            glGetVertexAttribiv(index, pname, &value);
            return WebGLGetInfo(value);
        case GL_CURRENT_VERTEX_ATTRIB:
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &value);
            values = new DCfloat[value];
            glGetVertexAttribfv(index, pname, values);
            return WebGLGetInfo(Float32Array::create(values, value));
        default:
            return WebGLGetInfo();
    }
}

DCsizeiptr WebGLGraphicsContext::getVertexAttribOffset(DCuint index, DCenum pname)
{
    DCvoid* pointer = 0;
    glGetVertexAttribPointerv(index, pname, &pointer);
    return (DCsizeiptr)pointer;
}

WebGLActiveInfo* WebGLGraphicsContext::getActiveAttrib(WebGLProgram* program, DCuint index)
{
    if (!program || !program->getObject())
    {
        LOGE("getActiveAttrib : program is null or empty");
        return WebGLActiveInfo::create(std::string("null"), (DCenum)0, (DCint)0);
    }

    DCint maxAttributeSize = 0;
    ::glGetProgramiv(program->getObject(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeSize);

    char name[maxAttributeSize]; // GL_ACTIVE_ATTRIBUTE_MAX_LENGTH includes null termination
    DCsizei nameLength = 0;
    DCint size = 0;
    DCenum type = 0;

    glGetActiveAttrib(program->getObject(), index, maxAttributeSize, &nameLength, &size, &type, name);

    if (!nameLength)
    {
        LOGE("getActiveAttrib : glGetActiveAttrib failed, return 0 length name");
    }

    return WebGLActiveInfo::create(std::string(name, nameLength), type, size);
}

WebGLActiveInfo* WebGLGraphicsContext::getActiveUniform(WebGLProgram* program, uint index)
{
    if (!program || !program->getObject())
    {
        LOGE("getActiveUniform : program is null or empty");
        return WebGLActiveInfo::create(std::string("null"), (DCenum)0, (DCint)0);
    }

    DCint maxUniformSize = 0;
    glGetProgramiv(program->getObject(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformSize);

    char name[maxUniformSize]; // GL_ACTIVE_UNIFORM_MAX_LENGTH includes null termination
    DCsizei nameLength = 0;
    DCint size = 0;
    DCenum type = 0;

    glGetActiveUniform(program->getObject(), index, maxUniformSize, &nameLength, &size, &type, name);

    if (!nameLength)
    {
        LOGE("getActiveUniform : glGetProgramiv falied, return 0 length name");
    }

    return WebGLActiveInfo::create(std::string(name, nameLength), type, size);
}

WebGLGetInfo WebGLGraphicsContext::getUniform(WebGLProgram* program, DCuint location)
{
    if (!program || !program->getObject())
    {
        LOGE("getUniform : program is null or empty");
        return WebGLGetInfo();
    }

    DCint activeUniforms = 0;
    glGetProgramiv(program->getObject(), GL_ACTIVE_UNIFORMS, &activeUniforms);

    for (DCint i = 0; i < activeUniforms; i++)
    {
        WebGLActiveInfo info = *(getActiveUniform(program, i));

        std::string end("[0]");
        // Strip "[0]" from the name if it's an array.
        if (info.size() > 1 && info.name().compare(info.name().length()-end.length(), end.length(), end) == 0)
            info.name() = info.name().substr(0, info.name().length() - 3);
        // If it's an array, we need to iterate through each element, appending "[index]" to the name.
        for (DCint index = 0; index < info.size(); ++index)
        {
            std::string name = info.name();
            if (info.size() > 1 && index >= 1)
            {
                name += '[';
                std::stringstream strs;
                strs<<index;
                name.append(strs.str());
                name += ']';
            }
            // Now need to look this up by name again to find its location
            DCint loc = ::glGetUniformLocation(program->getObject(), name.c_str());
            if ((DCuint)loc == location)
            {
                // Found it. Use the type in the ActiveInfo to determine the return type.
                DCenum baseType;
                DCuint length;
                switch (info.type())
                {
                    case GL_BOOL:
                        baseType = GL_BOOL;
                        length = 1;
                        break;
                    case GL_BOOL_VEC2:
                        baseType = GL_BOOL;
                        length = 2;
                        break;
                    case GL_BOOL_VEC3:
                        baseType = GL_BOOL;
                        length = 3;
                        break;
                    case GL_BOOL_VEC4:
                        baseType = GL_BOOL;
                        length = 4;
                        break;
                    case GL_INT:
                        baseType = GL_INT;
                        length = 1;
                        break;
                    case GL_INT_VEC2:
                        baseType = GL_INT;
                        length = 2;
                        break;
                    case GL_INT_VEC3:
                        baseType = GL_INT;
                        length = 3;
                        break;
                    case GL_INT_VEC4:
                        baseType = GL_INT;
                        length = 4;
                        break;
                    case GL_FLOAT:
                        baseType = GL_FLOAT;
                        length = 1;
                        break;
                    case GL_FLOAT_VEC2:
                        baseType = GL_FLOAT;
                        length = 2;
                        break;
                    case GL_FLOAT_VEC3:
                        baseType = GL_FLOAT;
                        length = 3;
                        break;
                    case GL_FLOAT_VEC4:
                        baseType = GL_FLOAT;
                        length = 4;
                        break;
                    case GL_FLOAT_MAT2:
                        baseType = GL_FLOAT;
                        length = 4;
                        break;
                    case GL_FLOAT_MAT3:
                        baseType = GL_FLOAT;
                        length = 9;
                        break;
                    case GL_FLOAT_MAT4:
                        baseType = GL_FLOAT;
                        length = 16;
                        break;
                    case GL_SAMPLER_2D:
                    case GL_SAMPLER_CUBE:
                        baseType = GL_INT;
                        length = 1;
                        break;
                    default:
                        // Can't handle this type
                        LOGE("getUniform : Type is not match");
                        return WebGLGetInfo();
                } // switch (info.type())

                switch (baseType)
                {
                    case GL_FLOAT:
                    {
                        DCfloat value[16] = {0};
                        ::glGetUniformfv(program->getObject(), location, value);

                        if (length == 1)
                            return WebGLGetInfo(value[0]);

                        return WebGLGetInfo(Float32Array::create(value, length));
                    }
                    case GL_INT:
                    {
                        DCint value[4] = {0};
                        ::glGetUniformiv(program->getObject(), location, value);

                        if (length == 1)
                            return WebGLGetInfo(value[0]);

                        return WebGLGetInfo(Int32Array::create(value, length));
                    }
                    case GL_BOOL:
                    {
                        DCint value[4] = {0};
                        ::glGetUniformiv(program->getObject(), location, value);

                        if (length > 1)
                        {
                            bool boolValue[16] = {0};
                            for (unsigned j = 0; j < length; j++)
                                boolValue[j] = static_cast<bool>(value[j]);
                            return WebGLGetInfo(boolValue, length);
                        }

                        return WebGLGetInfo(static_cast<bool>(value[0]));
                    }
                    default:
                        LOGE("getUniform : baseType is not match");

                } // switch (baseType)
            } // if ((DCuint)loc == location)
        } // for (DCint index = 0; index < info.size(); ++index)
    } // for (DCint i = 0; i < activeUniforms; i++)

    // If we get here, something went wrong in our unfortunately complex logic above
    LOGE("getUniform : not find correct result");
    return WebGLGetInfo();
}

DCenum WebGLGraphicsContext::checkFramebufferStatus(DCenum target)
{
    if (target != GL_FRAMEBUFFER)
    {
        LOGE("checkFramebufferStatus : target is not FRAMEBUFFER");
        return 0;
    }

    if (!m_framebufferBinding || !m_framebufferBinding->getObject())
        return GL_FRAMEBUFFER_COMPLETE;

    unsigned long result = glCheckFramebufferStatus(target);
    return result;
}

void WebGLGraphicsContext::readPixels(DCint x, DCint y, DCsizei width, DCsizei height,
                                      DCenum format, DCenum type, ArrayBufferView* pixels)
{
    if (!pixels)
    {
        LOGE("readPixels : ArrayBufferView is null");
        return;
    }

    if (!validateSize(width, height))
    {
        LOGE("readPixels : width and height is invalid");
        return;
    }

    switch (format)
    {
        case GL_ALPHA:
        case GL_RGB:
        case GL_RGBA:
            break;
        default:
            LOGE("readPixels : format is not match");
            return;
    }

    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            break;
        default:
            LOGE("readPixels : type is not match");
            return;
    }

    if (format != GL_RGBA || type != GL_UNSIGNED_BYTE)
    {
        LOGE("readPixels : unsupported format or type, only support GL_RGBA, GL_UNSIGNED_BYTE");
        return;
    }

    // Validate array type against pixel type.
    if (!pixels->isUnsignedByteArray())
    {
        LOGE("readPixels : ArrayBufferView is not Uint8Array");
        return;
    }

    if (m_framebufferBinding && !m_framebufferBinding->getObject())
    {
        LOGE("readPixels : binding framebuffer is null or empty");
        return;
    }
    // Calculate array size, taking into consideration of PACK_ALIGNMENT.
    DCuint totalBytesRequired;
    DCuint padding;

    if (!computeImageSizeInBytes(format, type, width, height,
                                m_packAlignment, &totalBytesRequired, &padding))
    {
        LOGE("readPixels : computeImageSizeInBytes check failed");
        return;
    }

    if (pixels->byteLength() < totalBytesRequired)
    {
        LOGE("readPixels : ArrayBuffer isn't large enough");
        return;
    }
    void* data = pixels->baseAddress();

    glReadPixels(x, y, width, height, format, type, data);

    // FIXME: remove this section when GL driver bug on Mac is fixed, i.e.,
    // when alpha is off, readPixels should set alpha to 255 instead of 0.
    if (!m_attributes->alpha())
    {
        unsigned char* pixels = reinterpret_cast<unsigned char*>(data);
        for (DCsizei iy = 0; iy < height; ++iy)
        {
            for (DCsizei ix = 0; ix < width; ++ix)
            {
                pixels[3] = 255;
                pixels += 4;
            }
            pixels += padding;
        }
    }
}

void WebGLGraphicsContext::initVertexAttrib0()
{
    WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(0);

    m_vertexAttrib0Buffer = createBuffer();
    bindBuffer(GL_ARRAY_BUFFER, m_vertexAttrib0Buffer);
    bufferData(GL_ARRAY_BUFFER, 0, GL_DYNAMIC_DRAW);
    vertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
    state.bufferBinding = m_vertexAttrib0Buffer;
    bindBuffer(GL_ARRAY_BUFFER, 0);
    enableVertexAttribArray(0);
    m_vertexAttrib0BufferSize = 0;
    m_vertexAttrib0BufferValue[0] = 0.0f;
    m_vertexAttrib0BufferValue[1] = 0.0f;
    m_vertexAttrib0BufferValue[2] = 0.0f;
    m_vertexAttrib0BufferValue[3] = 1.0f;
    m_forceAttrib0BufferRefill = false;
    m_vertexAttrib0UsedBefore = false;
}

bool WebGLGraphicsContext::simulateVertexAttrib0(DCsizei numVertex)
{
    const WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(0);
    const VertexAttribValue& attribValue = m_vertexAttribValue[0];
    if (!m_currentProgram)
        return false;

    bool usingVertexAttrib0 = m_currentProgram->isUsingVertexAttrib0();

    if (usingVertexAttrib0)
        m_vertexAttrib0UsedBefore = true;

    if (state.enabled && usingVertexAttrib0)
        return false;

    if (!usingVertexAttrib0 && !m_vertexAttrib0UsedBefore)
        return false;

    m_vertexAttrib0UsedBefore = true;
    bindBuffer(GL_ARRAY_BUFFER, m_vertexAttrib0Buffer);
    DCsizeiptr bufferDataSize = (numVertex + 1) * 4 * sizeof(DCfloat);

    if (bufferDataSize > m_vertexAttrib0BufferSize)
    {
        glBufferData(GL_ARRAY_BUFFER, bufferDataSize, 0, GL_DYNAMIC_DRAW);
        m_vertexAttrib0BufferSize = bufferDataSize;
        m_forceAttrib0BufferRefill = true;
    }

    if (usingVertexAttrib0
        && (m_forceAttrib0BufferRefill
        || attribValue.value[0] != m_vertexAttrib0BufferValue[0]
        || attribValue.value[1] != m_vertexAttrib0BufferValue[1]
        || attribValue.value[2] != m_vertexAttrib0BufferValue[2]
        || attribValue.value[3] != m_vertexAttrib0BufferValue[3]))
    {
        DCfloat* bufferData = new DCfloat[(numVertex + 1) * 4];

        for (DCsizei ii = 0; ii < numVertex + 1; ++ii)
        {
            bufferData[ii * 4] = attribValue.value[0];
            bufferData[ii * 4 + 1] = attribValue.value[1];
            bufferData[ii * 4 + 2] = attribValue.value[2];
            bufferData[ii * 4 + 3] = attribValue.value[3];
        }

        m_vertexAttrib0BufferValue[0] = attribValue.value[0];
        m_vertexAttrib0BufferValue[1] = attribValue.value[1];
        m_vertexAttrib0BufferValue[2] = attribValue.value[2];
        m_vertexAttrib0BufferValue[3] = attribValue.value[3];
        m_forceAttrib0BufferRefill = false;
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferDataSize, bufferData);
    }

    vertexAttribPointer(0, 4, GL_FLOAT, 0, 0, 0);
    return true;
}

void WebGLGraphicsContext::restoreStatesAfterVertexAttrib0Simulation()
{
    const WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(0);

    if (state.bufferBinding != m_vertexAttrib0Buffer)
    {
        bindBuffer(GL_ARRAY_BUFFER, state.bufferBinding);
        vertexAttribPointer(0, state.size, state.type, state.normalized, state.originalStride, state.offset);
    }

    bindBuffer(GL_ARRAY_BUFFER, m_boundArrayBuffer);
}

bool WebGLGraphicsContext::validateDrawMode(DCenum mode)
{
    switch (mode)
    {
        case GL_POINTS:
        case GL_LINE_STRIP:
        case GL_LINE_LOOP:
        case GL_LINES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLES:
            return true;
        default:
            return false;
    }
}

DCuint WebGLGraphicsContext::sizeInBytes(DCenum type)
{
    switch (type)
    {
        case GL_BYTE:
            return sizeof(DCbyte);
        case GL_UNSIGNED_BYTE:
            return sizeof(DCubyte);
        case GL_SHORT:
            return sizeof(DCshort);
        case GL_UNSIGNED_SHORT:
            return sizeof(DCushort);
        case GL_INT:
            return sizeof(DCint);
        case GL_UNSIGNED_INT:
            return sizeof(DCuint);
        case GL_FLOAT:
            return sizeof(DCfloat);
    }
    return 0;
}

bool WebGLGraphicsContext::validateRenderingState(DCint numElementsRequired)
{
    if (!m_currentProgram)
    {
        LOGE("validateRenderingState : current program is null");
        return false;
    }

    // Look in each enabled vertex attrib and check if they've been bound to a buffer.
    for (int i = 0; i < m_maxVertexAttribs; ++i)
    {
        const WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(i);
        //LOGE("validateRenderingState : VertexAttribState get");
        if (state.enabled
            && (!state.bufferBinding || !state.bufferBinding->getObject()))
        {
            LOGE("validateRenderingState : binding buffer is null or empty");
            return false;
        }
    }
    if (numElementsRequired <= 0)
        return true;

    // Look in each consumed vertex attrib (by the current program) and find the smallest buffer size
    int smallestNumElements = INT_MAX;
    int numActiveAttribLocations = m_currentProgram->numActiveAttribLocations();

    for (int i = 0; i < numActiveAttribLocations; ++i)
    {
        int loc = m_currentProgram->getActiveAttribLocation(i);

        if (loc >= 0 && loc < static_cast<int>(m_maxVertexAttribs))
        {
            const WebGLVertexArrayObjectOES::VertexAttribState& state = m_boundVertexArrayObject->getVertexAttribState(loc);
            if (state.enabled)
            {
                // Avoid off-by-one errors in numElements computation.
                // For the last element, we will only touch the data for the
                // element and nothing beyond it.
                int bytesRemaining = static_cast<int>(state.bufferBinding->byteLength() - state.offset);
                int numElements = 0;
                ASSERT(state.stride > 0);

                if (bytesRemaining >= state.bytesPerElement)
                    numElements = 1 + (bytesRemaining - state.bytesPerElement) / state.stride;
                if (numElements < smallestNumElements)
                    smallestNumElements = numElements;
            }
        }
    }

    if (smallestNumElements == INT_MAX)
        smallestNumElements = 0;

    return numElementsRequired <= smallestNumElements;
}

WebGLBuffer* WebGLGraphicsContext::validateBufferDataParameters(DCenum target, DCenum usage)
{
    WebGLBuffer* buffer = NULL;
    switch (target)
    {
        case GL_ELEMENT_ARRAY_BUFFER:
            buffer = m_boundVertexArrayObject->getElementArrayBuffer();
            break;
        case GL_ARRAY_BUFFER:
            buffer = m_boundArrayBuffer;
            break;
        default:
            LOGE("validateBufferDataParameters : target is not match");
            return NULL;
    }

    if (!buffer)
    {
        LOGE("validateBufferDataParameters : can't get the ArrayBuffer");
        return NULL;
    }

    switch (usage)
    {
        case GL_STREAM_DRAW:
        case GL_STATIC_DRAW:
        case GL_DYNAMIC_DRAW:
            return buffer;
        default:
            LOGE("validateBufferDataParameters : usage is not match");
            return NULL;
    }

    return NULL;
}

void WebGLGraphicsContext::swapBuffer()
{
    m_context->swapBuffer();
}

#if NATIVE_SURFACE
void WebGLGraphicsContext::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
    m_context->setPosition(x, y);
}

void WebGLGraphicsContext::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
    m_context->setSize(w, h);
}

void WebGLGraphicsContext::setAlpha(float alpha)
{
    m_context->setAlpha(alpha);
}
#endif

void WebGLGraphicsContext::getExtensionStrings()
{
    std::string extensionsString(reinterpret_cast<const char*>(::glGetString(GL_EXTENSIONS)));
    std::vector<std::string> availableExtensions;

    std::istringstream iss(extensionsString);
    std::string str;

    while(iss>>str)
    {
        m_availableExtensions.push_back(str);
    }
}

} // namespace DCanvas
