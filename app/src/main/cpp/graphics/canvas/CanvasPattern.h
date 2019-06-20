#ifndef CANVASPATTERN_H_INCLUDED
#define CANVASPATTERN_H_INCLUDED

#include "gl2d_impl.h"
#include "Style.h"
#include "ObjectWrap.h"
#include "Image.h"
//#include "MemoryManager.h"
#include <string>

namespace DCanvas
{

class CanvasPattern : Style
{
public:
    CanvasPattern(Image* image, const std::string& type, int canvasId, Gl2d_Impl* gc);
    ~CanvasPattern();
    static v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc, int canvasId);
    bool repeatX() const { return m_repeatX; }
    bool repeatY() const { return m_repeatY; }

    Image* image() const { return m_image; }
    std::string type() const { return m_type; }

    HTEXTURE texture() const { return m_htex; }
    void setTargetAndTexture(HTARGET htar, HTEXTURE htex);
    
    int getCanvasId() { return m_canvasId; }

    static bool HasInstance(v8::Handle<v8::Value> value);
    static void destroy();

private:
    Image*      m_image;
    HTARGET     m_target;
    HTEXTURE    m_htex;
    Gl2d_Impl*  m_gc;
    std::string m_type;
    bool        m_repeatX;
    bool        m_repeatY;
    int         m_canvasId;
    
    void    parseRepetitionType(const std::string&);

    static bool s_bInited;
    static v8::Persistent<v8::ObjectTemplate>               s_class_ins;
    static v8::Persistent<v8::FunctionTemplate>             s_class_func_templ;
};

}// namespace DCanvas

#endif // CANVASPATTERN_H
