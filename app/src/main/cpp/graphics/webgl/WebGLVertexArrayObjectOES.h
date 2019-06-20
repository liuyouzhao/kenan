#ifndef WEBGLVERTEXARRAYOBJECTOES_H
#define WEBGLVERTEXARRAYOBJECTOES_H

#include "WebGLGraphicsContext.h"
#include "WebGLBuffer.h"
#include "WebGLObject.h"
#include <vector>

namespace DCanvas
{

class WebGLVertexArrayObjectOES : public WebGLObject
{
public:
    enum VaoType
    {
        VaoTypeDefault,
        VaoTypeUser,
    };

    virtual ~WebGLVertexArrayObjectOES() { deleteObject(); }

    static WebGLVertexArrayObjectOES* create(WebGLGraphicsContext*, VaoType);

    // Cached values for vertex attrib range checks
    struct VertexAttribState
    {
        VertexAttribState()
            : enabled(false)
            , bufferBinding(NULL)
            , bytesPerElement(0)
            , size(4)
            , type(GL_FLOAT)
            , normalized(false)
            , stride(16)
            , originalStride(0)
            , offset(0)
        {
        }

        bool enabled;
        WebGLBuffer* bufferBinding;
        DCsizei bytesPerElement;
        DCint size;
        DCenum type;
        bool normalized;
        DCsizei stride;
        DCsizei originalStride;
        DCintptr offset;
    };

    bool isDefaultObject() const { return m_type == VaoTypeDefault; }

    bool hasEverBeenBound() const { return getObject() && m_hasEverBeenBound; }
    void setHasEverBeenBound() { m_hasEverBeenBound = true; }

    WebGLBuffer* getElementArrayBuffer() const { return m_boundElementArrayBuffer; }
    void setElementArrayBuffer(WebGLBuffer* buffer) { m_boundElementArrayBuffer = buffer; }

    VertexAttribState& getVertexAttribState(int index) { return m_vertexAttribState[index]; }

private:
    WebGLVertexArrayObjectOES(WebGLGraphicsContext*, VaoType);

    virtual void deleteObjectImpl(DCPlatformObject&);

    virtual bool isVertexArray() const { return true; }

    VaoType m_type;
    bool m_hasEverBeenBound;
    WebGLBuffer* m_boundElementArrayBuffer;
    std::vector<VertexAttribState> m_vertexAttribState;
};

} // namespace DCanvas
#endif // WEBGLVERTEXARRAYOBJECTOES_H
