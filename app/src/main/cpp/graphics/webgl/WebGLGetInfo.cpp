#include "WebGLGetInfo.h"
#include "Assert.h"

namespace DCanvas
{

WebGLGetInfo::WebGLGetInfo(bool value)
    : m_type(kTypeBool)
    , m_bool(value)
{
}

WebGLGetInfo::WebGLGetInfo(const bool* value, int size)
    : m_type(kTypeBoolArray)
{
    if (!value || size <=0)
        return;

    m_boolArray.resize(size);

    for (int ii = 0; ii < size; ++ii)
        m_boolArray[ii] = value[ii];
}

WebGLGetInfo::WebGLGetInfo(float value)
    : m_type(kTypeFloat)
    , m_float(value)
{
}

WebGLGetInfo::WebGLGetInfo(int value)
    : m_type(kTypeInt)
    , m_int(value)
{
}

WebGLGetInfo::WebGLGetInfo()
    : m_type(kTypeNull)
{
}

WebGLGetInfo::WebGLGetInfo(const std::string& value)
    : m_type(kTypeString)
    , m_string(value)
{
}

WebGLGetInfo::WebGLGetInfo(unsigned int value)
    : m_type(kTypeUnsignedInt)
    , m_unsignedInt(value)
{
}

WebGLGetInfo::WebGLGetInfo(WebGLBuffer* value)
    : m_type(kTypeWebGLBuffer)
    , m_webglBuffer(value)
{
}

WebGLGetInfo::WebGLGetInfo(Float32Array* value)
    : m_type(kTypeWebGLFloatArray)
    , m_webglFloatArray(value)
{
}

WebGLGetInfo::WebGLGetInfo(WebGLFrameBuffer* value)
    : m_type(kTypeWebGLFrameBuffer)
    , m_webglFrameBuffer(value)
{
}

WebGLGetInfo::WebGLGetInfo(Int32Array* value)
    : m_type(kTypeWebGLIntArray)
    , m_webglIntArray(value)
{
}

WebGLGetInfo::WebGLGetInfo(WebGLProgram* value)
    : m_type(kTypeWebGLProgram)
    , m_webglProgram(value)
{
}

WebGLGetInfo::WebGLGetInfo(WebGLRenderBuffer* value)
    : m_type(kTypeWebGLRenderBuffer)
    , m_webglRenderBuffer(value)
{
}

WebGLGetInfo::WebGLGetInfo(WebGLTexture* value)
    : m_type(kTypeWebGLTexture)
    , m_webglTexture(value)
{
}

WebGLGetInfo::WebGLGetInfo(Uint8Array* value)
    : m_type(kTypeWebGLUnsignedByteArray)
    , m_webglUnsignedByteArray(value)
{
}

WebGLGetInfo::~WebGLGetInfo()
{
}

WebGLGetInfo::Type WebGLGetInfo::getType() const
{
    return m_type;
}

bool WebGLGetInfo::getBool() const
{
    ASSERT(getType() == kTypeBool);
    return m_bool;
}

const std::vector<bool>& WebGLGetInfo::getBoolArray() const
{
    ASSERT(getType() == kTypeBoolArray);
    return m_boolArray;
}

float WebGLGetInfo::getFloat() const
{
    ASSERT(getType() == kTypeFloat);
    return m_float;
}

int WebGLGetInfo::getInt() const
{
    ASSERT(getType() == kTypeInt);
    return m_int;
}

const std::string& WebGLGetInfo::getString() const
{
    ASSERT(getType() == kTypeString);
    return m_string;
}

unsigned int WebGLGetInfo::getUnsignedInt() const
{
    ASSERT(getType() == kTypeUnsignedInt);
    return m_unsignedInt;
}

WebGLBuffer* WebGLGetInfo::getWebGLBuffer() const
{
    ASSERT(getType() == kTypeWebGLBuffer);
    return m_webglBuffer;
}

Float32Array* WebGLGetInfo::getWebGLFloatArray() const
{
    ASSERT(getType() == kTypeWebGLFloatArray);
    return m_webglFloatArray;
}

WebGLFrameBuffer* WebGLGetInfo::getWebGLFrameBuffer() const
{
    ASSERT(getType() == kTypeWebGLFrameBuffer);
    return m_webglFrameBuffer;
}

Int32Array* WebGLGetInfo::getWebGLIntArray() const
{
    ASSERT(getType() == kTypeWebGLIntArray);
    return m_webglIntArray;
}

WebGLProgram* WebGLGetInfo::getWebGLProgram() const
{
    ASSERT(getType() == kTypeWebGLProgram);
    return m_webglProgram;
}

WebGLRenderBuffer* WebGLGetInfo::getWebGLRenderBuffer() const
{
    ASSERT(getType() == kTypeWebGLRenderBuffer);
    return m_webglRenderBuffer;
}

WebGLTexture* WebGLGetInfo::getWebGLTexture() const
{
    ASSERT(getType() == kTypeWebGLTexture);
    return m_webglTexture;
}

Uint8Array* WebGLGetInfo::getWebGLUnsignedByteArray() const
{
    ASSERT(getType() == kTypeWebGLUnsignedByteArray);
    return m_webglUnsignedByteArray;
}

} // namespace DCanvas
