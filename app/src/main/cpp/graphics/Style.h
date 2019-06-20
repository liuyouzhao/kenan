#ifndef STYLE_H
#define STYLE_H

#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{

class Style:public ObjectWrap
{
public:
    enum TypeClassName
    {
        TClassStyle,
        TClassCanvasGradient,
        TClassCanvasPattern
    };

    Style() { m_name = TClassStyle; }
    ~Style() {}



    virtual TypeClassName name()
    {
        return m_name;
    }

protected:
    TypeClassName m_name;
};

} // namespace DCanvas

#endif // STYLE_H
