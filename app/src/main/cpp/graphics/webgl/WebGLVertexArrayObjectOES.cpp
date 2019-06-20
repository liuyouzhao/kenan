#include "defines.h"
#include "WebGLVertexArrayObjectOES.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLVertexArrayObjectOES"

namespace DCanvas
{

WebGLVertexArrayObjectOES* WebGLVertexArrayObjectOES::create(WebGLGraphicsContext* ctx, VaoType type)
{
    return new WebGLVertexArrayObjectOES(ctx, type);
}

WebGLVertexArrayObjectOES::WebGLVertexArrayObjectOES(WebGLGraphicsContext* ctx, VaoType type)
    : WebGLObject(ctx)
    , m_type(type)
    , m_hasEverBeenBound(false)
    , m_boundElementArrayBuffer(0)
{
    m_vertexAttribState.resize(ctx->getMaxVertexAttribs());

    //Extensions3D* extensions = context()->graphicsContext3D()->getExtensions();
    switch (m_type)
    {
        case VaoTypeDefault:
            break;
        default:
            //setObject(extensions->createVertexArrayOES());
            break;
    }
}

void WebGLVertexArrayObjectOES::deleteObjectImpl(DCPlatformObject& object)
{
    //Extensions3D* extensions = context()->graphicsContext3D()->getExtensions();
    switch (m_type)
    {
        case VaoTypeDefault:
            break;
        default:
            //extensions->deleteVertexArrayOES(object);
            break;
    }
}

} // namespace DCanvas
