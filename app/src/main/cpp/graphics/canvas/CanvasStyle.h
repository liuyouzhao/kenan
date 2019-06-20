#ifndef CANVASSTYLE_H_INCLUDED
#define CANVASSTYLE_H_INCLUDED

#include "gl2d_impl.h"
#include "Image.h"
#include "Color.h"
#include "CanvasPattern.h"
#include "CanvasGradient.h"
#include "Assert.h"

namespace DCanvas
{

class CanvasStyle 
{
public:
    CanvasStyle(CanvasPattern*);
    CanvasStyle(Color);
    CanvasStyle(CanvasGradient*);

    bool  isCurrentColor() const { return m_type == TCurrentColor ||
                                          m_type == TCurrentColorWithOverrideAlpha; }

    bool  hasOverrideAlpha() const { return m_type == TCurrentColorWithOverrideAlpha; }
    bool  isPattern() const { return m_type == TPattern; }
    bool  isGradient() const { return m_type == TGradient; }

    Color color() const { return m_color; }

    HTEXTURE texture() const 
    {
         if (isPattern())
             return m_pattern->texture();
         else if (isGradient())
             return m_gradient->texture();
         else
             return NULL;
    }
    CanvasPattern* canvasPattern() const { return m_pattern; }
    CanvasGradient* canvasGradient() const { return m_gradient; }
    
    void setColor(Color cl) { m_color = cl; m_type = TCurrentColor; }
    void setPattern(CanvasPattern* cp) { m_pattern = cp; m_type = TPattern; }
    void setGradient(CanvasGradient* cg) { m_gradient = cg; m_type = TGradient; }

    enum StyleType 
    {   
        TGradient, 
        TPattern, 
        TCurrentColor, 
        TCurrentColorWithOverrideAlpha 
    };
   
private:

    StyleType m_type;

    CanvasPattern*  m_pattern;
    CanvasGradient* m_gradient;
    Color m_color;
};

}//namespace DCanvas

#endif // CANVASSTYLE_H
