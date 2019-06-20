#ifndef CANVASGRADIENT_H
#define CANVASGRADIENT_H

#include "Color.h"
#include "Style.h"
#include "Point.h"
#include "ObjectWrap.h"
#include "gl2d_impl.h"
//#include "MemoryManager.h"

#include <vector>

namespace DCanvas
{

class Color;
enum GradientSpreadMethod
{
    SpreadMethodPad = 1,
    SpreadMethodReflect = 2,
    SpreadMethodRepeat = 3
};

class ColorStop
{
public:
    float stop;
    float red;
    float green;
    float blue;
    float alpha;

    ColorStop() : stop(0), red(0), green(0), blue(0), alpha(0) { }
    ColorStop(float s, float r, float g, float b, float a) : stop(s), red(r), green(g), blue(b), alpha(a) { }
    ~ColorStop() { }
    
    friend bool operator == (const ColorStop& a, const ColorStop& b)
    {
        if (a.stop  == b.stop  &&
            a.red   == b.red   &&
            a.green == b.green &&
            a.blue  == b.blue  &&
            a.alpha == b.alpha)
            return true;
        else
            return false;
    }
    friend bool operator != (const ColorStop& a, const ColorStop& b)
    {
        if (a.stop  != b.stop  ||
            a.red   != b.red   ||
            a.green != b.green ||
            a.blue  != b.blue  ||
            a.alpha != b.alpha)
            return true;
        else
            return false;
    }
};

class CanvasGradient : public Style
{
public:
    CanvasGradient(int canvasId);
    CanvasGradient(const Point& p0, const Point& p1, int canvasId, Gl2d_Impl* gc);
    CanvasGradient(const Point& p0, float r0, const Point& p1, float r1, int canvasId, Gl2d_Impl* gc);
    ~CanvasGradient();

    static v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc, int canvasId);
    static v8::Handle<v8::Value> addColorStop(const v8::FunctionCallbackInfo<v8::Value> &args);

    HTEXTURE texture() const { return m_htex; }
    void setTargetAndTexture(HTARGET htar, HTEXTURE htex);
    void calculateGLQuad(gl2dQuad* quad, float sw, float sh);
    void resortRadialColorStops();
    int colorStopsSize() const { return m_stops.size(); }

    bool isNeedUpdateTexture() const;

    void addColorStop(const ColorStop&);
    void addColorStop(float, const Color&);

    void getColor(float value, float* r, float* g, float* b, float* a) const;
    bool hasAlpha() const;

    bool isRadial() const { return m_radial; }
    bool isZeroSize() const { return (m_p0.x() == m_p1.x() && m_p0.y() == m_p1.y() && (!m_radial || m_r0 == m_r1)) || (isRadial() && isRoundIntersect()); }
    bool isRoundIntersect() const;
    void sortStopsIfNecessary();

    const Point& p0() const { return m_p0; }
    const Point& p1() const { return m_p1; }

    void setP0(const Point& p) { m_p0 = p; }
    void setP1(const Point& p) { m_p1 = p; }

    float startRadius() const { return m_r0; }
    float endRadius() const { return m_r1; }

    void setStartRadius(float r) { m_r0 = r; }
    void setEndRadius(float r) { m_r1 = r; }

    float aspectRatio() const { return m_aspectRatio; }

    void setStopsSorted(bool s) { m_stopsSorted = s; }
    std::vector<ColorStop> getColorStops() { return m_stops; }

    void setSpreadMethod(GradientSpreadMethod);
    GradientSpreadMethod spreadMethod() { return m_spreadMethod; }

    friend bool operator == (const CanvasGradient& a, const CanvasGradient& b)
    {
        if (a.m_radial    == b.m_radial    &&
            a.m_p0        == b.m_p0        &&
            a.m_p1        == b.m_p1        &&
            a.m_r0        == b.m_r0        &&
            a.m_r1        == b.m_r1        &&
            a.m_stops     == b.m_stops     &&
            a.m_canvasId  == b.m_canvasId)
            return true;
        else
            return false;
    }

    friend bool operator != (const CanvasGradient& a, const CanvasGradient& b)
    {
        if (a == b)
            return false;
        else
            return true;
    }

    void operator =(const CanvasGradient &b)
    {
           m_radial    = b.m_radial;
           m_p0        = b.m_p0;
           m_p1        = b.m_p1;
           m_r0        = b.m_r0;
           m_r1        = b.m_r1;
           m_stops     = b.m_stops;
           m_htex      = b.m_htex;
           m_gc        = b.m_gc;
           m_target    = b.m_target;
           m_canvasId  = b.m_canvasId;
    }

    static bool HasInstance(v8::Handle<v8::Value> value);
    static void destroy();
	
	int getCanvasId(){ return m_canvasId;}
	
private:
    int     findStop(float value) const;
    void    invertStops();

    bool    m_radial;
    Point   m_p0;
    Point   m_p1;
    float   m_r0;
    float   m_r1;
    float   m_aspectRatio; // For elliptical gradient, width / height.
    mutable std::vector<ColorStop> m_stops;
    mutable bool            m_stopsSorted;
    mutable int             m_lastStop;
    GradientSpreadMethod    m_spreadMethod;

    bool m_hasNewStop;

    HTARGET     m_target;
    HTEXTURE    m_htex;
    Gl2d_Impl*  m_gc;
    
    int   m_canvasId;
     
    static bool s_bInited;
    static v8::Persistent<v8::ObjectTemplate>               s_class_ins;
    static v8::Persistent<v8::FunctionTemplate>             s_class_func_templ;
};

} //namespace  DCanvas

#endif // CANVASGRADIENT_H
