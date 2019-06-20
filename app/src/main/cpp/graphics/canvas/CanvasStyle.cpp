#include "defines.h"
#include "CanvasStyle.h"

#undef LOG_TAG
#define  LOG_TAG    "GraphicsContext"

namespace DCanvas
{

CanvasStyle::CanvasStyle(CanvasPattern* pattern)
    : m_type(TPattern)
    , m_pattern(pattern)
{
}

CanvasStyle::CanvasStyle(Color color)
    : m_type(TCurrentColor)
    , m_color(color)
{
}

CanvasStyle::CanvasStyle(CanvasGradient* gradient)
    : m_type(TGradient)
    , m_gradient(gradient)
{

}

}//namespace DCanvas
