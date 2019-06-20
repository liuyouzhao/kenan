#ifndef WEBGLGETINFO_H
#define WEBGLGETINFO_H

#include "Float32Array.h"
#include "Int32Array.h"
#include "Uint8Array.h"
#include "WebGLBuffer.h"
#include "WebGLFrameBuffer.h"
#include "WebGLProgram.h"
#include "WebGLRenderBuffer.h"
#include "WebGLTexture.h"

#include <string>

namespace DCanvas
{

// A tagged union representing the result of get queries like
// getParameter (encompassing getBooleanv, getIntegerv, getFloatv) and
// similar variants. For reference counted types, increments and
// decrements the reference count of the target object.

class WebGLGetInfo
{
public:
    enum Type
    {
        kTypeBool,
        kTypeBoolArray,
        kTypeFloat,
        kTypeInt,
        kTypeNull,
        kTypeString,
        kTypeUnsignedInt,
        kTypeWebGLBuffer,
        kTypeWebGLFloatArray,
        kTypeWebGLFrameBuffer,
        kTypeWebGLIntArray,
        kTypeWebGLObjectArray,
        kTypeWebGLProgram,
        kTypeWebGLRenderBuffer,
        kTypeWebGLTexture,
        kTypeWebGLUnsignedByteArray,
        //kTypeWebGLVertexArrayObjectOES,
    };

    WebGLGetInfo(bool value);
    WebGLGetInfo(const bool* value, int size);
    WebGLGetInfo(float value);
    WebGLGetInfo(int value);
    // Represents the null value and type.
    WebGLGetInfo();
    WebGLGetInfo(const std::string& value);
    WebGLGetInfo(unsigned int value);
    WebGLGetInfo(WebGLBuffer* value);
    WebGLGetInfo(Float32Array* value);
    WebGLGetInfo(WebGLFrameBuffer* value);
    WebGLGetInfo(Int32Array* value);

    WebGLGetInfo(WebGLProgram* value);
    WebGLGetInfo(WebGLRenderBuffer* value);
    WebGLGetInfo(WebGLTexture* value);
    WebGLGetInfo(Uint8Array* value);
    //WebGLGetInfo(WebGLVertexArrayObjectOES value);

    virtual ~WebGLGetInfo();

    Type getType() const;

    bool getBool() const;
    const std::vector<bool>& getBoolArray() const;
    float getFloat() const;
    int getInt() const;
    const std::string& getString() const;
    unsigned int getUnsignedInt() const;
    WebGLBuffer* getWebGLBuffer() const;
    Float32Array* getWebGLFloatArray() const;
    WebGLFrameBuffer* getWebGLFrameBuffer() const;
    Int32Array* getWebGLIntArray() const;

    WebGLProgram* getWebGLProgram() const;
    WebGLRenderBuffer* getWebGLRenderBuffer() const;
    WebGLTexture* getWebGLTexture() const;
    Uint8Array* getWebGLUnsignedByteArray() const;
    //WebGLVertexArrayObjectOES getWebGLVertexArrayObjectOES() const;

private:
    Type m_type;
    bool m_bool;
    std::vector<bool> m_boolArray;
    float m_float;
    int m_int;
    std::string m_string;
    unsigned int m_unsignedInt;
    WebGLBuffer* m_webglBuffer;
    Float32Array* m_webglFloatArray;
    WebGLFrameBuffer* m_webglFrameBuffer;
    Int32Array* m_webglIntArray;

    WebGLProgram* m_webglProgram;
    WebGLRenderBuffer* m_webglRenderBuffer;
    WebGLTexture* m_webglTexture;
    Uint8Array* m_webglUnsignedByteArray;
    //WebGLVertexArrayObjectOES* m_webglVertexArrayObject;
};

} // namespace WebCore

#endif // WEBGLGETINFO_H
