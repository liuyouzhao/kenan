#include "defines.h"
#include "CanvasGradient.h"
#include <android/log.h>
#include <algorithm>
#include "Assert.h"

#undef LOG_TAG
#define  LOG_TAG    "CanvasGradient"

namespace DCanvas
{

bool CanvasGradient::s_bInited;
v8::Persistent<v8::ObjectTemplate>             CanvasGradient::s_class_ins;
v8::Persistent<v8::FunctionTemplate>           CanvasGradient::s_class_func_templ;

CanvasGradient::CanvasGradient(int canvasId)
{
    m_canvasId = canvasId;
    MemoryManager::getManager()->AddCanvasGradient(this);
    this->m_name = Style::TClassCanvasGradient;
}

CanvasGradient::CanvasGradient(const Point& p0, const Point& p1, int canvasId, Gl2d_Impl* gc)
    : m_radial(false)
    , m_p0(p0)
    , m_p1(p1)
    , m_r0(0)
    , m_r1(0)
    , m_aspectRatio(1)
    , m_stopsSorted(false)
    , m_lastStop(0)
    , m_spreadMethod(SpreadMethodPad)
    , m_hasNewStop(true)
    , m_target(NO_TARGET)
    , m_htex(NO_TEXTURE)
    , m_gc(gc)
    , m_canvasId(canvasId)
{
    MemoryManager::getManager()->AddCanvasGradient(this);
    this->m_name = Style::TClassCanvasGradient;
}

CanvasGradient::CanvasGradient(const Point& p0, float r0, const Point& p1, float r1, int canvasId, Gl2d_Impl* gc)
    : m_radial(true)
    , m_p0(p0)
    , m_p1(p1)
    , m_r0(r0)
    , m_r1(r1)
    , m_stopsSorted(false)
    , m_lastStop(0)
    , m_spreadMethod(SpreadMethodPad)
    , m_hasNewStop(true)
    , m_target(NO_TARGET)
    , m_htex(NO_TEXTURE)
    , m_gc(gc)
    , m_canvasId(canvasId)

{
    this->m_name = Style::TClassCanvasGradient;
    MemoryManager::getManager()->AddCanvasGradient(this);
}

CanvasGradient::~CanvasGradient()
{
    if (m_htex != NO_TEXTURE)
    {
        m_gc->Texture_Free(m_htex);
        m_gc->Target_Free(m_target);
    }
    //MemoryManager::getManager()->RemoveCanvasGradient(this);
}

bool CanvasGradient::HasInstance(v8::Handle<v8::Value> value)
{
    if(!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

void CanvasGradient::destroy()
{
    if(s_bInited)
    {
        s_class_ins.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

v8::Handle<v8::Value> CanvasGradient::wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc, int canvasId)
{
    HandleScope scope;

    char log[100];
    CanvasGradient* cg = NULL;
    
    if (4 == args.Length())
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!args[i]->IsNumber())
            {
                sprintf(log, "CanvasGradient wrap : argument %d is not v8 number", i+1);
                return THROW_EXCEPTION(TTypeError, log);
            }
        }

        float x0 = args[0]->NumberValue();
        float y0 = args[1]->NumberValue();
        float x1 = args[2]->NumberValue();
        float y1 = args[3]->NumberValue();

        cg = new CanvasGradient(Point(x0,y0), Point(x1,y1), canvasId, gc);
    }
    else if (6 == args.Length())
    {
        for (int i = 0; i < 6; ++i)
        {
            if (!args[i]->IsNumber())
            {
                sprintf(log, "CanvasGradient wrap : argument %d is not v8 number", i+1);
                return THROW_EXCEPTION(TTypeError, log);
            }
        }

        float x0 = args[0]->NumberValue();
        float y0 = args[1]->NumberValue();
        float r0 = args[2]->NumberValue();
        float x1 = args[3]->NumberValue();
        float y1 = args[4]->NumberValue();
        float r1 = args[5]->NumberValue();

        cg = new CanvasGradient(Point(x0, y0), r0, Point(x1, y1), r1, canvasId, gc);
    }
    else
    {
        return THROW_EXCEPTION(TTypeError,
            "CanvasGradient wrap : number of arguments Exception");
    }

    if(!s_bInited)
    {
        s_class_func_templ = v8::Persistent<FunctionTemplate>::New(FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("CanvasGradient"));
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins->Set("addColorStop", FunctionTemplate::New(CanvasGradient::addColorStop));
        s_class_ins -> SetInternalFieldCount(1);
        s_bInited = true;
    }

    v8::Handle<v8::Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(cg));

    return scope.Close(class_obj);
}

void CanvasGradient::calculateGLQuad(gl2dQuad* quad, float sw, float sh)
{
    sortStopsIfNecessary();
    float tx = 0, ty = 0, ex = 0, ey = 0, lx0 = 0, ly0 = 0, lx1 = 0, ly1 = 0, x = 0, y = 0;
    if (m_p0.x() <= m_p1.x() && m_p0.y() <= m_p1.y())
    {
        tx = 0.0; ty = 0.0;
        ex = sw;  ey = sh;
        lx0 = 0.0; ly0 = sh;
        lx1 = sw;  ly1 = 0.0;
    }
    else if (m_p0.x() >= m_p1.x() && m_p0.y() >= m_p1.y())
    {
        ex = 0.0; ey = 0.0;
        tx = sw;  ty = sh;
        lx1 = 0.0; ly1 = sh;
        lx0 = sw;  ly0 = 0.0;
    }
    else if (m_p0.x() <= m_p1.x() && m_p0.y() >= m_p1.y())
    {
        tx = 0.0; ty = sh;
        ex = sw;  ey = 0.0;
        lx0 = 0.0; ly0 = 0.0;
        lx1 = sw;  ly1 = sh;
    }
    else if (m_p0.x() >= m_p1.x() && m_p0.y() <= m_p1.y())
    {
        ex = 0.0; ey = sh;
        tx = sw;  ty = 0.0;
        lx1 = 0.0; ly1 = 0.0;
        lx0 = sw;  ly0 = sh;
    }
    int size = m_stops.size();
    if (m_p0.x() == m_p1.x() && m_p0.y() != m_p1.y())
    {
        for (int i = 0; i < size; i++)
        {
            y = (m_p1.y() - m_p0.y()) * m_stops[i].stop + m_p0.y();
            quad[i].v[0].x = 0.0;
            quad[i].v[0].y = ty / sh;
            quad[i].v[1].x = 1.0;
            quad[i].v[1].y = ty / sh;
            quad[i].v[2].x = 1.0;
            quad[i].v[2].y = y / sh;
            quad[i].v[3].x = 0.0;
            quad[i].v[3].y = y / sh;
            tx = x;
            ty = y;
        }

        quad[size].v[0].x = 0.0;
        quad[size].v[0].y = ty / sh;
        quad[size].v[1].x = 1.0;
        quad[size].v[1].y = ty / sh;
        quad[size].v[2].x = 1.0;
        quad[size].v[2].y = ey / sh;
        quad[size].v[3].x = 0.0;
        quad[size].v[3].y = ey / sh;
    }
    else if (m_p0.x() != m_p1.x() && m_p0.y() == m_p1.y())
    {
        for (int i = 0; i < size; i++)
        {
            x = (m_p1.x() - m_p0.x()) * m_stops[i].stop + m_p0.x();
            quad[i].v[0].x = tx / sw;
            quad[i].v[0].y = 0.0;
            quad[i].v[1].x = tx / sw;
            quad[i].v[1].y = 1.0;
            quad[i].v[2].x = x / sw;
            quad[i].v[2].y = 1.0;
            quad[i].v[3].x = x / sw;
            quad[i].v[3].y = 0.0;
            tx = x;
            ty = y;
        }

        quad[size].v[0].x = tx / sw;
        quad[size].v[0].y = 0.0;
        quad[size].v[1].x = tx / sw;
        quad[size].v[1].y = 1.0;
        quad[size].v[2].x = ex / sw;
        quad[size].v[2].y = 1.0;
        quad[size].v[3].x = ex / sw;
        quad[size].v[3].y = 0.0;
    }
    else if (m_p0.x() != m_p1.x() && m_p0.y() != m_p1.y())
    {
        float k = (m_p1.y()-m_p0.y())/(m_p1.x()-m_p0.x());
        float temp_x = 0;
        for (int i = 0; i < size; i++)
        {
            x = (m_p1.x() - m_p0.x()) * m_stops[i].stop + m_p0.x();
            y = (m_p1.y() - m_p0.y()) * m_stops[i].stop + m_p0.y();
            temp_x = (k * k * lx0 - k * (ly0 - ty) + tx) / (k * k + 1);
            quad[i].v[0].x = temp_x / sw;
            quad[i].v[0].y = (temp_x/-k + ty + tx/k) / sh;
            temp_x = (k * k * lx1 - k * (ly1 - ty) + tx) / (k * k + 1);
            quad[i].v[1].x = temp_x / sw;
            quad[i].v[1].y = (temp_x/-k + ty + tx/k) / sh;
            temp_x = (k * k * lx1 - k * (ly1 - y) + x) / (k * k + 1);
            quad[i].v[2].x = temp_x / sw;
            quad[i].v[2].y = (temp_x / -k + y + x / k) / sh;
            temp_x = (k * k * lx0 - k *(ly0 - y) + x) / (k * k + 1);
            quad[i].v[3].x = temp_x / sw;
            quad[i].v[3].y = (temp_x/-k + y + x/k) / sh;

            tx = x;
            ty = y;
        }

        temp_x = (k * k *lx0 - k*(ly0-ty) + tx) / (k*k+1);
        quad[size].v[0].x = temp_x / sw;
        quad[size].v[0].y = (temp_x/-k + ty + tx/k) / sh;
        temp_x = (k * k * lx1 - k*(ly1-ty) + tx) / (k*k+1);
        quad[size].v[1].x = temp_x / sw;
        quad[size].v[1].y = (temp_x/-k + ty + tx/k) / sh;
        temp_x = (k * k * lx1 - k * (ly1 - ey) + ex) / (k * k + 1);
        quad[size].v[2].x = temp_x / sw;
        quad[size].v[2].y = (temp_x/-k + ey + ex/k) / sh;
        temp_x = (k * k * lx0 - k * (ly0 - ey) + ex) / (k * k + 1);
        quad[size].v[3].x = temp_x / sw;
        quad[size].v[3].y = (temp_x/-k + ey + ex/k) / sh;
    }
    else
    {
        LOGE("no match in calculateGLQuad!");
        return;
    }

    quad[0].v[0].r = quad[0].v[1].r = quad[0].v[2].r = quad[0].v[3].r = m_stops[0].red;
    quad[0].v[0].g = quad[0].v[1].g = quad[0].v[2].g = quad[0].v[3].g = m_stops[0].green;
    quad[0].v[0].b = quad[0].v[1].b = quad[0].v[2].b = quad[0].v[3].b = m_stops[0].blue;
    quad[0].v[0].a = quad[0].v[1].a = quad[0].v[2].a = quad[0].v[3].a = m_stops[0].alpha;

    quad[size].v[0].r = quad[size].v[1].r =
    quad[size].v[2].r = quad[size].v[3].r = m_stops[size-1].red;
    
    quad[size].v[0].g = quad[size].v[1].g =
    quad[size].v[2].g = quad[size].v[3].g = m_stops[size-1].green;
    
    quad[size].v[0].b = quad[size].v[1].b =
    quad[size].v[2].b = quad[size].v[3].b = m_stops[size-1].blue;
    
    quad[size].v[0].a = quad[size].v[1].a =
    quad[size].v[2].a = quad[size].v[3].a = m_stops[size-1].alpha;

    for (int i = 1; i < size; i++)
    {
        quad[i].v[0].r = quad[i].v[1].r = m_stops[i-1].red;
        quad[i].v[0].g = quad[i].v[1].g = m_stops[i-1].green;
        quad[i].v[0].b = quad[i].v[1].b = m_stops[i-1].blue;
        quad[i].v[0].a = quad[i].v[1].a = m_stops[i-1].alpha;

        quad[i].v[2].r = quad[i].v[3].r = m_stops[i].red;
        quad[i].v[2].g = quad[i].v[3].g = m_stops[i].green;
        quad[i].v[2].b = quad[i].v[3].b = m_stops[i].blue;
        quad[i].v[2].a = quad[i].v[3].a = m_stops[i].alpha;
    }

    for (int i = 0; i <= size; i++)
    {
        quad[i].v[0].x = quad[i].v[0].x * 2.0f - 1.0f;
        quad[i].v[1].x = quad[i].v[1].x * 2.0f - 1.0f;
        quad[i].v[2].x = quad[i].v[2].x * 2.0f - 1.0f;
        quad[i].v[3].x = quad[i].v[3].x * 2.0f - 1.0f;

        quad[i].v[0].y = 1.0f - quad[i].v[0].y * 2.0f;
        quad[i].v[1].y = 1.0f - quad[i].v[1].y * 2.0f;
        quad[i].v[2].y = 1.0f - quad[i].v[2].y * 2.0f;
        quad[i].v[3].y = 1.0f - quad[i].v[3].y * 2.0f;

        quad[i].v[0].tx =  quad[i].v[0].ty= 0.0;
        quad[i].v[1].tx =  quad[i].v[1].ty= 0.0;
        quad[i].v[2].tx =  quad[i].v[2].ty= 0.0;
        quad[i].v[3].tx =  quad[i].v[3].ty= 0.0;
        quad[i].v[0].z = quad[i].v[1].z = quad[i].v[2].z = quad[i].v[3].z = 0.5f;
        quad[i].blend = 1;
        quad[i].tex = NO_TEXTURE;
    }
}

void CanvasGradient::resortRadialColorStops()
{
    sortStopsIfNecessary();
    if (m_r0 > m_r1)
    {
        invertStops();
    }
}

v8::Handle<v8::Value> CanvasGradient::addColorStop(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (2 != args.Length())
        return THROW_EXCEPTION(TError,
            "CanvasGradient addColorStop : number of arguments Exception");

    CanvasGradient* cg = ObjectWrap::Unwrap<CanvasGradient>(args.This());

    if (!args[0]->IsNumber() || !args[1]->IsString())
        return THROW_EXCEPTION(TTypeError,
            "CanvasGradient addColorStop : type of arguments Exception");

    float value = args[0]->NumberValue();
    int  length = args[1]->ToString()->Length();
    v8::String::Utf8Value col(args[1]->ToString());
    cg->addColorStop(value, Color(*col));

    return v8::Undefined();
}

void CanvasGradient::addColorStop(float value, const Color& color)
{
    float r;
    float g;
    float b;
    float a;
    color.getRGBA(r, g, b, a);
    ColorStop cs = ColorStop(value, r, g, b, a);
    
    for (unsigned int i = 0; i < m_stops.size(); i++)
    {
        if (m_stops[i] == cs)
            return;
    }

    CanvasGradient* cg = this;
    if (NO_TEXTURE !=  m_htex)
    {
        CanvasGradient* tmp = new CanvasGradient(m_canvasId);
        (*tmp) = (*this);

        m_htex = NO_TEXTURE;
    }

    m_hasNewStop = true;
    m_stops.push_back(cs);

    m_stopsSorted = false;
}

void CanvasGradient::addColorStop(const ColorStop& stop)
{
    ColorStop cs = stop;
    for (unsigned int i = 0; i < m_stops.size(); i++)
    {
        if (m_stops[i] == cs)
            return;
    }

    if (NO_TEXTURE != texture())
    {
        CanvasGradient* tmp = new CanvasGradient(m_canvasId);
        (*tmp) = (*this);
        m_htex = NO_TEXTURE;
    }

    m_hasNewStop = true;
    m_stops.push_back(stop);

    m_stopsSorted = false;
}

static inline bool compareStops(const ColorStop& a, const ColorStop& b)
{
    return a.stop < b.stop;
}

void CanvasGradient::sortStopsIfNecessary()
{
    if (m_stopsSorted)
        return;

    m_stopsSorted = true;

    if (!m_stops.size())
        return;

    // Shortcut for the ideal case (ordered 2-stop gradient)
    if (m_stops.size() == 2 && compareStops(*m_stops.begin(), *m_stops.end()))
        return;

    std::stable_sort(m_stops.begin(), m_stops.end(), compareStops);
}

void CanvasGradient::getColor(float value, float* r, float* g, float* b, float* a) const
{
    ASSERT(value >= 0);
    ASSERT(value <= 1);

    if (m_stops.empty())
    {
        *r = 0;
        *g = 0;
        *b = 0;
        *a = 0;
        return;
    }

    if (!m_stopsSorted)
    {
        if (m_stops.size())
            std::stable_sort(m_stops.begin(), m_stops.end(), compareStops);

        m_stopsSorted = true;
    }

    if (value <= 0 || value <= m_stops[0].stop)
    {
        *r = m_stops.front().red;
        *g = m_stops.front().green;
        *b = m_stops.front().blue;
        *a = m_stops.front().alpha;

        return;
    }

    if (value >= 1 || value >= m_stops.back().stop)
    {
        *r = m_stops.back().red;
        *g = m_stops.back().green;
        *b = m_stops.back().blue;
        *a = m_stops.back().alpha;

        return;
    }

    // Find stop before and stop after and interpolate.
    int stop = findStop(value);
    const ColorStop& lastStop = m_stops[stop];
    const ColorStop& nextStop = m_stops[stop + 1];
    float stopFraction = (value - lastStop.stop) / (nextStop.stop - lastStop.stop);

    *r = lastStop.red + (nextStop.red - lastStop.red) * stopFraction;
    *g = lastStop.green + (nextStop.green - lastStop.green) * stopFraction;
    *b = lastStop.blue + (nextStop.blue - lastStop.blue) * stopFraction;
    *a = lastStop.alpha + (nextStop.alpha - lastStop.alpha) * stopFraction;
}

int CanvasGradient::findStop(float value) const
{
    ASSERT(value >= 0);
    ASSERT(value <= 1);
    ASSERT(m_stopsSorted);

    int numStops = m_stops.size();
    ASSERT(numStops >= 2);
    ASSERT(m_lastStop < numStops - 1);

    int i = m_lastStop;
    if (value < m_stops[i].stop)
        i = 1;
    else
        i = m_lastStop + 1;

    for (; i < numStops - 1; ++i)
        if (value < m_stops[i].stop)
            break;

    m_lastStop = i - 1;
    return m_lastStop;
}

bool CanvasGradient::hasAlpha() const
{
    for (size_t i = 0; i < m_stops.size(); i++) {
        if (m_stops[i].alpha < 1)
            return true;
    }

    return false;
}

void CanvasGradient::setSpreadMethod(GradientSpreadMethod spreadMethod)
{
    // FIXME: Should it become necessary, allow calls to this method after m_gradient has been set.
    //ASSERT(m_gradient == 0);
    m_spreadMethod = spreadMethod;
}

void CanvasGradient::setTargetAndTexture(HTARGET htar, HTEXTURE htex)
{
    m_target = htar;
    m_htex = htex;
    m_hasNewStop = false;
}

bool CanvasGradient::isRoundIntersect() const
{
    float d = Point::Distance(m_p0, m_p1);
    float diff = abs(m_r0 - m_r1);

    if (d > diff)
        return true;
    else
        return false;
}

void CanvasGradient::invertStops()
{
    ColorStop temp;
    int size = m_stops.size();
    int num = size / 2;

    for (int i = 0; i < num; ++i)
    {
        temp = m_stops[i];
        m_stops[i] = m_stops[size - 1 - i];
        m_stops[size - 1 - i] = temp;
    }
    m_stopsSorted = false;
}

bool CanvasGradient::isNeedUpdateTexture() const
{
    if (m_hasNewStop || m_htex == NO_TEXTURE)
        return true;
    else
        return false;
}

} //namespace DCanvas

