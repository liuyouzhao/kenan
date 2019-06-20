#include "WebGLContextAttributes.h"

namespace DCanvas
{
WebGLContextAttributes* WebGLContextAttributes::create()
{
    return new WebGLContextAttributes();
}

WebGLContextAttributes* WebGLContextAttributes::create(Attributes attributes)
{
    return new WebGLContextAttributes(attributes);
}

WebGLContextAttributes* WebGLContextAttributes::create(WebGLContextAttributes attributes)
{
    return new WebGLContextAttributes(attributes);
}

WebGLContextAttributes::WebGLContextAttributes()
{
}

WebGLContextAttributes::WebGLContextAttributes(Attributes attributes)
    : m_attrs(attributes)
{
}

WebGLContextAttributes::~WebGLContextAttributes()
{
}

bool WebGLContextAttributes::alpha() const
{
    return m_attrs.alpha;
}

void WebGLContextAttributes::setAlpha(bool alpha)
{
    m_attrs.alpha = alpha;
}

bool WebGLContextAttributes::depth() const
{
    return m_attrs.depth;
}

void WebGLContextAttributes::setDepth(bool depth)
{
    m_attrs.depth = depth;
}

bool WebGLContextAttributes::stencil() const
{
    return m_attrs.stencil;
}

void WebGLContextAttributes::setStencil(bool stencil)
{
    m_attrs.stencil = stencil;
}

bool WebGLContextAttributes::antialias() const
{
    return m_attrs.antialias;
}

void WebGLContextAttributes::setAntialias(bool antialias)
{
    m_attrs.antialias = antialias;
}

bool WebGLContextAttributes::premultipliedAlpha() const
{
    return m_attrs.premultipliedAlpha;
}

void WebGLContextAttributes::setPremultipliedAlpha(bool premultipliedAlpha)
{
    m_attrs.premultipliedAlpha = premultipliedAlpha;
}

bool WebGLContextAttributes::preserveDrawingBuffer() const
{
    return m_attrs.preserveDrawingBuffer;
}

void WebGLContextAttributes::setPreserveDrawingBuffer(bool preserveDrawingBuffer)
{
    m_attrs.preserveDrawingBuffer = preserveDrawingBuffer;
}

WebGLContextAttributes::Attributes WebGLContextAttributes::attributes() const
{
    return m_attrs;
}

} // namespace DCanvas
