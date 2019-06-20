#ifndef SCAN_PATH_H
#define SCAN_PATH_H

#include "GraphicsContext.h"

#include "Color.h"
#include "Point.h"
#include "SkPath.h"
#include "Path.h"
#include "SkGeometry.h"

#include <gl2d_impl.h>

namespace DCanvas
{

class PArray
{
public:
    Point p;
    PArray* next;
    PArray() { next = NULL; }
};

struct edge
{
    int y_max;
    double x;
    double deltx;
    edge *next;
};

class CPfill
{
public:
    CPfill();
    ~CPfill();
    void polyfill(PArray *point, Gl2d_Impl* gc, GraphicsContext::CanvasState* state);

private:
    void creatET(PArray *point, int num);
    void creatAET(int y);
    edge* insert(edge *head,edge *p);
private:
    int maxy;
    int miny;
    edge **m_edges;
    edge * m_AET;
};

void FillPathPoly(Path& path, Gl2d_Impl* gc,
                  GraphicsContext::CanvasState* state);
void FillPathWalkLine(Path& mpath, Gl2d_Impl* gc,
                      GraphicsContext::CanvasState* state);
void StrokePath(const Path& m_path, Gl2d_Impl* m_context,
                GraphicsContext::CanvasState* m_state);

void renderLineSelf(float x1, float y1, float x2, float y2,
                    int r, int g, int b, int a, Gl2d_Impl* m_context,
                    GraphicsContext::CanvasState* m_state);

class SkQuadClipper
{
public:
    SkQuadClipper();

    void setClip(const SkIRect& clip);

    bool clipQuad(const SkPoint src[3], SkPoint dst[3]);

private:
    SkRect fClip;
};

static inline void clamp_le(SkScalar& value, SkScalar max)
{
    if (value > max)
    {
        value = max;
    }
}

static inline void clamp_ge(SkScalar& value, SkScalar min)
{
    if (value < min)
    {
        value = min;
    }
}

static bool chopMonoQuadAt(SkScalar c0, SkScalar c1, SkScalar c2,
                           SkScalar target, SkScalar* t)
{
    /* Solve F(t) = y where F(t) := [0](1-t)^2 + 2[1]t(1-t) + [2]t^2
     *  We solve for t, using quadratic equation, hence we have to rearrange
     * our cooefficents to look like At^2 + Bt + C
     */
    SkScalar A = c0 - c1 - c1 + c2;
    SkScalar B = 2*(c1 - c0);
    SkScalar C = c0 - target;

    SkScalar roots[2];  // we only expect one, but make room for 2 for safety
    int count = SkFindUnitQuadRoots(A, B, C, roots);
    if (count)
    {
        *t = roots[0];
        return true;
    }
    return false;
}

static bool chopMonoQuadAtY(SkPoint pts[3], SkScalar y, SkScalar* t)
{
    return chopMonoQuadAt(pts[0].fY, pts[1].fY, pts[2].fY, y, t);
}

} //namespace DCanvas

#endif //SCAN_PATH_H
