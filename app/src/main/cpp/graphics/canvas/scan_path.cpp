#include "defines.h"
#include "scan_path.h"
#include "Edge.h"
#include "SkPath.h"
#include <vector>

#undef  LOG_TAG
#define  LOG_TAG    "scan_path"

namespace DCanvas
{

#define IsZero(val) (val < 0.0000001 && val > -0.0000001)
#define IsNANP(val) ((val<-1.0) ? (val>-1.00000001):(val<-0.00000009))
#define SK_MinS32   0x80000001
#define SK_MaxS32   0x7FFFFFFF
#ifndef SkDECLAREPARAM
#define SkDECLAREPARAM(type, var)   , type var
#endif
#ifndef SkPARAM
#define SkPARAM(var)                , var
#endif
#define kEDGE_HEAD_Y    SK_MinS32
#define kEDGE_TAIL_Y    SK_MaxS32

typedef void (*CompensateFunc)(Point*, int r, int g, int b, int a,
                               Gl2d_Impl*, GraphicsContext::CanvasState*);

static PArray* head = NULL;
static PArray* curr = NULL;
static bool s_beginCompensate = false;
static bool s_pureCompensate = false;
static bool s_isClosePath = false;
static Point s_firstStrokePoint[2];
static Point s_lastStrokePoint[2];
static Point s_compensatePoint[4];
static CompensateFunc s_lastCompensateFunc;

static void cleanStrokePoint()
{
    s_firstStrokePoint[0] = Point(0, 0);
    s_firstStrokePoint[1] = Point(0, 0);
    s_lastStrokePoint[0] = Point(0, 0);
    s_lastStrokePoint[1] = Point(0, 0);
}

class Triple
{
public:
    Point points[3];
    Triple() { }
    Triple(const Triple& tri)
    {
        this->points[0] = tri.points[0];
        this->points[1] = tri.points[1];
        this->points[2] = tri.points[2];
    }
};

class Round
{
public:
    Point center[1];
    Round() { }
    Round(const Round& round)
    {
        this->center[0] = round.center[0];
    }
};

static std::vector<Triple> s_compensateTriples;
static std::vector<Round> s_compensateRounds;

static float rotationDirection(Point a, Point b)
{
    return a.x() * b.y() - a.y() * b.x();
}

static void calculateLineQuad(Point dst[4], Point src[2], float lineWidth)
{
    float x1 = src[0].x();
    float y1 = src[0].y();
    float x2 = src[1].x();
    float y2 = src[1].y();
    
    float ds = sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
    float sinTheta = (x2 - x1) / ds;
    
    float dy = 0.0;
    float dx = 0.0;
    
    if(!IsZero(x2 - x1))
    {
        dy = lineWidth / 2.0 * sinTheta;
        dx = -1 * dy * (y2 - y1) / (x2 - x1);
    }
    else
    {
        dy = 0;
        if(y2 > y1)
            dx = -lineWidth / 2.0;
        else
            dx = lineWidth / 2.0;
    }
    
    dst[0].setX(x1 + dx);  dst[0].setY(y1 + dy);
    dst[1].setX(x1 - dx);  dst[1].setY(y1 - dy);
    dst[2].setX(x2 - dx);  dst[2].setY(y2 - dy);
    dst[3].setX(x2 + dx);  dst[3].setY(y2 + dy);
}

static Point calculateLineIntersection(Point P1, Point P2, Point P3, Point P4)
{
    //t0 =  (x0(y3 – y2) + x2(y0 – y3) + x3(y2 – y0))
    //t1 = -(x0(y2 – y1) + x1(y0 – y2) + x2(y1 – y0))
    //D = x0(y3 – y2) + x1(y2 – y3) + x3(y1 – y0) + x2(y0 – y1)
    //if D equal zero, two lines are same line
    //x = x0 + t0 * (x1 – x0) / D;  y = y0 + t0 * (y1 – y0) / D;
    //or
    //x = x2 + t1 * (x3 – x2) / D;  y = y2 + t1 * (y3 – y2) / D;
    
    float t1 = P1.x() * (P4.y() - P3.y()) + P3.x() * (P1.y() - P4.y()) + P4.x() * (P3.y() - P1.y());
    float D = P1.x() * (P4.y() - P3.y()) + P2.x() * (P3.y() - P4.y()) + P4.x() * (P2.y() - P1.y()) + P3.x() * (P1.y() - P2.y());
    if (IsZero(D))
    {
        LOGE("calculateLineIntersection : the same two lines");
    }
    float x = P1.x() + t1 * (P2.x() - P1.x()) / D;
    float y = P1.y() + t1 * (P2.y() - P1.y()) / D;
    return Point(x, y);
}

void compensateRound(Point* points, int r, int g, int b, int a,
                     Gl2d_Impl* m_context, GraphicsContext::CanvasState* m_state)
{
    CanvasStyle old_style = m_state->fillStyle;
    m_state->fillStyle = m_state->strokeStyle;
    float radius = floor(m_state->lineWidth / 2.0f);
    Path p ;
    p.clear();
    p.addArc(points[0], radius, 0, M_PI * 2, true);
    FillPathWalkLine(p, m_context, m_state);
    m_state->fillStyle = old_style;
}

void compensateBevel(Point* points, int r, int g, int b, int a,
                     Gl2d_Impl* m_context, GraphicsContext::CanvasState* m_state)
{
    CanvasStyle old_style = m_state->fillStyle;
    m_state->fillStyle = m_state->strokeStyle;
    m_context->Gfx_RenderTripleEx(points[0].x(), points[0].y(), 
                                  points[1].x(), points[1].y(), 
                                  points[2].x(), points[2].y(),
                                  r, g, b, a, 1);
    m_state->fillStyle = old_style;
}

void compensateMiter(Point* points, int r, int g, int b, int a,
                     Gl2d_Impl* m_context, GraphicsContext::CanvasState* m_state)
{
    CanvasStyle old_style = m_state->fillStyle;
    m_state->fillStyle = m_state->strokeStyle;

    m_context->Gfx_RenderQuadEx(points[0].x(), points[0].y(), 
                                points[1].x(), points[1].y(), 
                                points[3].x(), points[3].y(),
                                points[2].x(), points[2].y(),
                                r, g, b, a, 1);
    m_state->fillStyle = old_style;
}

bool outMiterLimit(Point intersection, Point inner,
                   GraphicsContext::CanvasState* state)
{
    float miterDist = sqrt((intersection.x() - inner.x()) * (intersection.x() - inner.x())
                         + (intersection.y() - inner.y()) * (intersection.y() - inner.y()));
    //LOGE("miter : %f", miterDist * 2.0 / state->lineWidth);
    if(IsZero(state->miterLimit))
        return false;
        
    return miterDist * 2.0 / state->lineWidth  > state->miterLimit;
}

void renderWidthCompensate(const Point* fpts, GLContextRef gc,
                           GraphicsContext::CanvasState*& m_state,
                           CompensateFunc compensateFunc)
{
    Color strokeColor = m_state->strokeStyle.color();
    int alpha = strokeColor.alpha() * m_state->globalAlpha;
    
    if (m_state->shadowPath)
    {
        alpha = 255;
    }
    int r = strokeColor.red(), g = strokeColor.green(),
        b = strokeColor.blue(), a = alpha;
        

    Point dst[4], src[2];
    src[0] = fpts[0];
    src[1] = fpts[2];

    calculateLineQuad(dst, src, m_state->lineWidth);
    if (!s_pureCompensate)
    {
        gc->Gfx_RenderQuadEx(dst[0].x(), dst[0].y(),
                             dst[1].x(), dst[1].y(),
                             dst[2].x(), dst[2].y(),
                             dst[3].x(), dst[3].y(),
                             r, g, b, a, 1);
    }

    Point comsDrawPoint[4];
    if (s_beginCompensate)
    {
        float result = rotationDirection((s_lastStrokePoint[1] - s_lastStrokePoint[0]),
                                         (fpts[2] - fpts[0]));

        if (IsZero(result))
        {
            //do nothing
        }
        else if (result > 0)
        {
            //direction of rotation is right
            comsDrawPoint[0] = fpts[0];
            comsDrawPoint[1] = s_compensatePoint[2];
            comsDrawPoint[2] = dst[1];
            if (compensateFunc == compensateMiter)
            {
                //calculate Intersection
                comsDrawPoint[3] = calculateLineIntersection(s_compensatePoint[1],
                                                             s_compensatePoint[2],
                                                             dst[1], dst[2]);

                if (outMiterLimit(comsDrawPoint[3], comsDrawPoint[0], m_state))
                {
                    Triple triple;
                    triple.points[0] = comsDrawPoint[0];
                    triple.points[1] = comsDrawPoint[1];
                    triple.points[2] = comsDrawPoint[2];
                    s_compensateTriples.push_back(triple);
                }
                else
                {
                    compensateFunc(comsDrawPoint, r, g, b, a, gc, m_state);
                }
            }
            else if (compensateFunc == compensateBevel)
            {
                Triple triple;
                triple.points[0] = comsDrawPoint[0];
                triple.points[1] = comsDrawPoint[1];
                triple.points[2] = comsDrawPoint[2];
                s_compensateTriples.push_back(triple);
            }
            else if (compensateFunc == compensateRound)
            {
                Round round;
                round.center[0] = comsDrawPoint[0];
                s_compensateRounds.push_back(round);
            }
        }
        else 
        {
            //direction of rotation is left
            comsDrawPoint[0] = fpts[0];
            comsDrawPoint[1] = dst[0];
            comsDrawPoint[2] = s_compensatePoint[3];
            if (compensateFunc == compensateMiter)
            {
                //calculate Intersection
                comsDrawPoint[3] = calculateLineIntersection(s_compensatePoint[0],
                                                             s_compensatePoint[3],
                                                             dst[0], dst[3]);
                                                             
                if (outMiterLimit(comsDrawPoint[3], comsDrawPoint[0], m_state))
                {
                    Triple triple;
                    triple.points[0] = comsDrawPoint[0];
                    triple.points[1] = comsDrawPoint[1];
                    triple.points[2] = comsDrawPoint[2];
                    s_compensateTriples.push_back(triple);
                }
                else
                {
                    compensateFunc(comsDrawPoint, r, g, b, a, gc, m_state);
                }
            }
            else if (compensateFunc == compensateBevel)
            {
                Triple triple;
                triple.points[0] = comsDrawPoint[0];
                triple.points[1] = comsDrawPoint[1];
                triple.points[2] = comsDrawPoint[2];
                s_compensateTriples.push_back(triple);
            }
            else if (compensateFunc == compensateRound)
            {
                Round round;
                round.center[0] = comsDrawPoint[0];
                s_compensateRounds.push_back(round);
            }
        } // if rotation result
    }
    else
    {
        s_firstStrokePoint[0] = fpts[0];
        s_firstStrokePoint[1] = fpts[2];
        s_lastCompensateFunc = compensateFunc;
        s_beginCompensate = true;
    }

    s_lastStrokePoint[0] = fpts[0];
    s_lastStrokePoint[1] = fpts[2];
    
    s_compensatePoint[0] = dst[0];
    s_compensatePoint[1] = dst[1];
    s_compensatePoint[2] = dst[2];
    s_compensatePoint[3] = dst[3];
}

static void strokequad(Point fpts[3], int level, GLContextRef gc,
                       GraphicsContext::CanvasState*& m_state)
{
    if (level > 0 )
    {
        Point tmp[5];

        chopQuadAtHalf(fpts, tmp);
        strokequad(tmp, level - 1, gc, m_state);
        strokequad(&tmp[2], level - 1, gc, m_state);
    }
    else
    {
        renderWidthCompensate(fpts, gc, m_state, compensateBevel);
    }
}

static void strokecubic(const Point* fpts, int level, GLContextRef gc, GraphicsContext::CanvasState*& m_state)
{
    if (level > 0)
    {
        Point tmp[7];

        chopCubicAt(fpts, tmp, 0.5f);
        strokecubic(tmp, level - 1, gc, m_state);
        strokecubic(&tmp[3],  level - 1, gc, m_state);
    }
    else
    {
/*        Color strokeColor = m_state->strokeStyle.color();
        int alpha = strokeColor.alpha() * m_state->globalAlpha;
        if (m_state->shadowPath)
        {
            alpha = 255;
        }

        gc->Gfx_RenderLineEx(fpts[0].x(),
                             fpts[0].y(),
                             fpts[3].x(),
                             fpts[3].y(), strokeColor.red(),
                             strokeColor.green(), strokeColor.blue(), alpha);
*/
        Point points[3];
        points[0] = fpts[0];
        points[2] = fpts[3];
        renderWidthCompensate(points, gc, m_state, compensateBevel);
    }
}

static void fillcubic(const Point* pts, int level, GLContextRef gc, 
                      GraphicsContext::CanvasState*& state)
{
    if (level > 0)
    {
        Point tmp[7];

        chopCubicAt(pts, tmp, 0.5f);
        fillcubic(tmp, level - 1, gc, state);
        fillcubic(&tmp[3],  level - 1, gc, state);
    }
    else
    {
        /*Color strokeColor = state->strokeColor;
        int alpha = strokeColor.alpha();
                if (IsNANP(state->globalAlpha))
                    alpha = 255* state->globalAlpha;

        gc->Gfx_RenderTripleEx(p.x(),p.y(), pts[0].x(), pts[0].y(), pts[3].x(), pts[3].y(),
                                                strokeColor.red(),
                                            strokeColor.green(), strokeColor.blue(), alpha, 1);*/
        curr->next = new PArray();
        curr = curr->next;
        curr->p.set(pts[3].x(), pts[3].y());
    }
}

static void fillquad(Point pts[3], int level, GLContextRef gc,
                     GraphicsContext::CanvasState*& state)
{
    if (level > 0 )
    {
        Point tmp[5];

        chopQuadAtHalf(pts, tmp);
        fillquad(tmp, level - 1, gc, state);
        fillquad(&tmp[2], level - 1, gc, state);
    }
    else
    {
        /*
        Color strokeColor = state->strokeColor;
        int alpha = strokeColor.alpha();
                if (IsNANP(state->globalAlpha))
                    alpha = 255* state->globalAlpha;
        gc->Gfx_RenderTripleEx(p.x(),p.y(), pts[0].x(), pts[0].y(), pts[2].x(), pts[2].y(),
                                                strokeColor.red(),
                                            strokeColor.green(), strokeColor.blue(), alpha, 1);
        */
        curr->next = new PArray();
        curr = curr->next;
        curr->p.set(pts[2].x(), pts[2].y());
    }
}

void FillPathPoly(Path& m_path, Gl2d_Impl* gc,GraphicsContext::CanvasState* state)
{
    PlatformPathPtr path = m_path.getPath();
    Point lastPoint(-1,-1);

    SkPath::Iter    iter(*path, false);
    SkPoint pts[4];
    SkPath::Verb    verb;
    Point fpts[4];
    Color strokeColor = state->strokeStyle.color();
    int alpha = strokeColor.alpha();
    if (IsNANP(state->globalAlpha))
        alpha = 255* state->globalAlpha;
    curr = head = new PArray();

    while ((verb = iter.next(pts)) != SkPath::kDone_Verb)
    {
        switch(verb)
        {
            //moveto
            case SkPath::kMove_Verb:
                setfpts(&fpts[0],&pts[0],1);
                lastPoint.set(fpts[0].x(), fpts[0].y());

                curr->p.set(fpts[0].x(), fpts[0].y());
                break;
            //lineto
            case SkPath::kLine_Verb:
            {
                setfpts(&fpts[0],&pts[0],2);
                curr->next = new PArray();
                curr = curr->next;
                curr->p.set(fpts[1].x(), fpts[1].y());

                 break;
            }
            //arc
            case SkPath::kQuad_Verb:
            {
                setfpts(&fpts[0], &pts[0], 3);
                int d = compute_int_quad_dist(fpts);
                  /*  quadratics approach the line connecting their start and end points
                 4x closer with each subdivision, so we compute the number of
                 subdivisions to be the minimum need to get that distance to be less
                 than a pixel.
                 */
                int level = (33 - CLZ_portable(d)) >> 1;
                if (level > kMaxQuadSubdivideLevel)
                {
                    level = kMaxQuadSubdivideLevel;
                }
                   fillquad(fpts, level, gc, state);

                break;
            }
            case SkPath::kCubic_Verb:
            {
                setfpts(&fpts[0], &pts[0], 4);
                fillcubic(fpts,kMaxCubicSubdivideLevel, gc, state);
                break;
            }
            case SkPath::kClose_Verb:break;
            default:
                break;
        }
    }
    CPfill* pf = new CPfill();
    pf->polyfill(head , gc, state);
}

CPfill::CPfill()
{
    m_AET = 0;
    maxy = -1;
    miny = 3000;
}

CPfill::~CPfill()
{
    edge *p;
    while (m_AET)
    {
        p = m_AET;
        m_AET = m_AET->next;
        delete p;
    }
    if (m_edges)
        delete []m_edges;
}

void CPfill::creatET(PArray *point,int num)
{
    int m,k,i = 0;
    edge *p,*q;
    Point lowpoint, highpoint;
    m = maxy - miny + 1;
    m_edges = new edge*[m];
    
    for ( i = 0; i < m; ++i)
        m_edges[i]=0;

    PArray* cu = point;

    while (cu)
    {
        if (cu->next)
        {
            if (cu->p.y() < cu->next->p.y())
            {
                lowpoint = cu->p;
                highpoint = cu->next->p;
                k = cu->p.y() - miny;
            }
            else
            {
                lowpoint = cu->next->p;
                highpoint= cu->p;
                k = cu->next->p.y() - miny;
            }
        }
        else
        {
            if (cu->p.y() < point->p.y())
            {
                lowpoint = cu->p;
                highpoint = point->p;
                k = cu->p.y() - miny;
            }
            else
            {
                lowpoint = point->p;
                highpoint= cu->p;
                k = point->p.y() - miny;
            }
        }
        
        q = m_edges[k];
        if (q)
        {
            while (q->next)
                q = q->next;
        }

        p = new edge;
        p->y_max = highpoint.y();
        p->x = lowpoint.x();
        p->deltx = (highpoint.x() - lowpoint.x()) *1.0/(highpoint.y() - lowpoint.y());
        p->next = NULL;
        if (q)
            q->next = p;
        else
            m_edges[k] = p;
            
        cu = cu->next;
    }
}

void CPfill::creatAET(int y)
{
    edge *p,*q;
    p = m_AET;

    while (p)
    {
        p->x = p->x+p->deltx;
        p  =p->next;
    }

    p = q = m_AET;
    edge *t;
    while (p)
    {
        if (y >= p->y_max)
        {
            if (p == m_AET)
            {
                m_AET = m_AET->next;
                delete p;
                p = q = m_AET;
            }
            else
            {
                q->next = p->next;
                t = p;
                p = p->next;
                delete t;
            }
        }
        else
        {
            q = p;
            p = p->next;
        }
    }

    q = m_edges[y-miny];
    if (m_AET == NULL)
        m_AET = q;
    else
    {
        p = m_AET;
        while (p->next)
            p = p->next;
        p->next = q;
    }

    p = m_AET;
    m_AET = NULL;
    while (p)
    {
        q = p;
        p = p->next;
        m_AET = insert(m_AET,q);
    }
}

edge* CPfill::insert(edge *head,edge *p)
{
    edge *p1,*p2;
    if (head == 0 )
    {
        head = p;
        p->next = 0;
        return head;
    }
    
    if (head->x >= p->x)
    {
            p->next = head;
            head = p;
            return head;
    }
    p2 = p1 = head;
    while (p2->next && p2->x <p->x)
    {
            p1 = p2;
            p2 = p2->next;
    }
    
    if (p2->x < p->x)
    {
        p2->next = p;
        p->next = 0;
    }
    else
    {
        p->next = p2;
        p1->next = p;
    }
    return head;
}

void CPfill::polyfill(PArray *point, Gl2d_Impl* gc, GraphicsContext::CanvasState* state)
{

    Color fillStyle = state->fillStyle.color();
    int alpha = fillStyle.alpha();
    if (IsNANP((state->globalAlpha)))
        alpha = 255* state->globalAlpha;
    edge *p;
    int j,i;
    int num = 0;
    PArray* cu = point;

    while (cu)
    {
        ++num;
        if (cu->p.y() > maxy)
            maxy = cu->p.y();
        if (cu->p.y() < miny)
            miny = cu->p.y();
        cu = cu->next;
    }
    creatET(point,num);

    int curr_x;
    for (i = miny; i <= maxy; ++i)
    {
        creatAET(i);
        p = m_AET;
        j = 0;
        curr_x = 0;
        
        while (p)
        {
            if (j % 2 == 0)
                curr_x = p->x + 0.5;
            if (j % 2 == 1)
            {
                gc->Gfx_RenderLineEx(curr_x,  i,  (int)(p->x + 0.5),
                                     i, fillStyle.red(), fillStyle.green(),
                                     fillStyle.blue(), alpha);
            }
            
            p=p->next;
            j++;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void SkQuadClipper::setClip(const SkIRect& clip)
{
    // conver to scalars, since that's where we'll see the points
    fClip.set(clip);
}

SkQuadClipper::SkQuadClipper() {}
///////////////////////////////////////////////////////////////////////////////

/*  If we somehow returned the fact that we had to flip the pts in Y, we could
 communicate that to setQuadratic, and then avoid having to flip it back
 here (only to have setQuadratic do the flip again)
 */
bool SkQuadClipper::clipQuad(const SkPoint srcPts[3], SkPoint dst[3])
{
    bool reverse;

    // we need the data to be monotonically increasing in Y
    if (srcPts[0].fY > srcPts[2].fY)
    {
        dst[0] = srcPts[2];
        dst[1] = srcPts[1];
        dst[2] = srcPts[0];
        reverse = true;
    }
    else
    {
        memcpy(dst, srcPts, 3 * sizeof(SkPoint));
        reverse = false;
    }

    // are we completely above or below
    const SkScalar ctop = fClip.fTop;
    const SkScalar cbot = fClip.fBottom;
    if (dst[2].fY <= ctop || dst[0].fY >= cbot)
    {
        return false;
    }

    SkScalar t;
    SkPoint tmp[5]; // for SkChopQuadAt

    // are we partially above
    if (dst[0].fY < ctop)
    {
        if (chopMonoQuadAtY(dst, ctop, &t))
        {
            // take the 2nd chopped quad
            SkChopQuadAt(dst, tmp, t);
            dst[0] = tmp[2];
            dst[1] = tmp[3];
        }
        else
        {
            // if chopMonoQuadAtY failed, then we may have hit inexact numerics
            // so we just clamp against the top
            for (int i = 0; i < 3; i++)
            {
                if (dst[i].fY < ctop)
                {
                    dst[i].fY = ctop;
                }
            }
        }
    }

    // are we partially below
    if (dst[2].fY > cbot)
    {
        if (chopMonoQuadAtY(dst, cbot, &t))
        {
            SkChopQuadAt(dst, tmp, t);
            dst[1] = tmp[1];
            dst[2] = tmp[2];
        }
        else
        {
            // if chopMonoQuadAtY failed, then we may have hit inexact numerics
            // so we just clamp against the bottom
            for (int i = 0; i < 3; i++)
            {
                if (dst[i].fY > cbot)
                {
                    dst[i].fY = cbot;
                }
            }
        }
    }

    if (reverse)
    {
        SkTSwap<SkPoint>(dst[0], dst[2]);
    }
    return true;
}

static int buildEdges(Edge edge[], const SkPath& path,
                      const SkIRect* clipRect, Edge* list[], int shiftUp)
{
    Edge**          start = list;
    SkPath::Iter    iter(path, true);
    SkPoint         pts[4];
    SkPath::Verb    verb;

    SkQuadClipper qclipper;
    if (clipRect)
    {
        SkIRect r;
        r.set(clipRect->fLeft >> shiftUp, clipRect->fTop >> shiftUp,
              clipRect->fRight >> shiftUp, clipRect->fBottom >> shiftUp);
        qclipper.setClip(r);
    }

    while ((verb = iter.next(pts)) != SkPath::kDone_Verb)
    {
        switch (verb)
        {
            case SkPath::kLine_Verb:
                if (edge->setLine(pts[0], pts[1], clipRect, shiftUp))
                {
                    *list++ = edge;
                    edge = (Edge*)((char*)edge + sizeof(Edge));
                }
                break;
            case SkPath::kQuad_Verb:
            {
                SkPoint tmp[5], clippedPts[3];
                SkPoint* p = tmp;
                int     count = SkChopQuadAtYExtrema(pts, tmp);

                do
                {
                    const SkPoint* qpts = p;
                    if (clipRect)
                    {
                        if (!qclipper.clipQuad(p, clippedPts))
                        {
                            goto NEXT_CHOPPED_QUAD;
                        }
                        qpts = clippedPts;
                    }
                    if (((SkQuadraticEdge*)edge)->setQuadratic(qpts, shiftUp))
                    {
                        *list++ = edge;
                        edge = (Edge*)((char*)edge + sizeof(SkQuadraticEdge));
                    }
                    
                NEXT_CHOPPED_QUAD:
                    p += 2;
                    
                } while (--count >= 0);
                break;
            }
            case SkPath::kCubic_Verb:
            {
                SkPoint tmp[10];
                SkPoint* p = tmp;
                int     count = SkChopCubicAtYExtrema(pts, tmp);
                SkASSERT(count >= 0 && count <= 2);

                do
                {
                    if (((SkCubicEdge*)edge)->setCubic(p, clipRect, shiftUp))
                    {
                        *list++ = edge;
                        edge = (Edge*)((char*)edge + sizeof(SkCubicEdge));
                    }
                    p += 3;
                    
                } while (--count >= 0);
                break;
            }
        default:
            break;
        }
    }
    return (int)(list - start);
}

static int cheapWorstCaseEdgeCount(const SkPath& path, size_t* storage)
{
    int ptCount = path.getPoints(NULL, 0);
    // worst case is curve, close, curve, close, as that is
    //     2 lines per pt, or             : pts * 2
    //     2 quads + 1 line per 2 pts, or : pts * 3 / 2
    //     3 cubics + 1 line per 3 pts    : pts * 4 / 3
    int edgeCount = ptCount << 1;
    // worst storage, due to relative size of different edge types, is
    // quads * 3 / 2
    size_t quadSize = (ptCount * 3 >> 1) * sizeof(SkQuadraticEdge);
    *storage = quadSize;
    return edgeCount;
}

static void validateSort(const Edge* edge)
{
    int y = kEDGE_HEAD_Y;
    while (edge->fFirstY != SK_MaxS32)
    {
        y = edge->fFirstY;
        edge = edge->fNext;
    }
}

static inline void removeEdge(Edge* edge)
{
    edge->fPrev->fNext = edge->fNext;
    edge->fNext->fPrev = edge->fPrev;
}

static inline void swapEdges(Edge* prev, Edge* next)
{
    SkASSERT(prev->fNext == next && next->fPrev == prev);

    // remove prev from the list
    prev->fPrev->fNext = next;
    next->fPrev = prev->fPrev;

    // insert prev after next
    prev->fNext = next->fNext;
    next->fNext->fPrev = prev;
    next->fNext = prev;
    prev->fPrev = next;
}

static void backwardInsertEdgeBasedOnX(Edge* edge SkDECLAREPARAM(int, curr_y))
{
    SkFixed x = edge->fX;

    for (;;)
    {
        Edge* prev = edge->fPrev;

        // add 1 to curr_y since we may have added new edges (built from curves)
        // that start on the next scanline
        SkASSERT(prev && prev->fFirstY <= curr_y + 1);

        if (prev->fX <= x)
            break;

        swapEdges(prev, edge);
    }
}

static void insertNewEdges(Edge* newEdge, int curr_y)
{
    SkASSERT(newEdge->fFirstY >= curr_y);

    while (newEdge->fFirstY == curr_y)
    {
        Edge* next = newEdge->fNext;
        backwardInsertEdgeBasedOnX(newEdge  SkPARAM(curr_y));
        newEdge = next;
    }
}

static void validateEdgesForY(const Edge* edge, int curr_y)
{
    while (edge->fFirstY <= curr_y)
    {
        SkASSERT(edge->fPrev && edge->fNext);
        SkASSERT(edge->fPrev->fNext == edge);
        SkASSERT(edge->fNext->fPrev == edge);
        SkASSERT(edge->fFirstY <= edge->fLastY);

        SkASSERT(edge->fPrev->fX <= edge->fX);
        edge = edge->fNext;
    }
}

static int edgeCompare(const void* a, const void* b)
{
    const Edge* edgea = *(const Edge**)a;
    const Edge* edgeb = *(const Edge**)b;

    int valuea = edgea->fFirstY;
    int valueb = edgeb->fFirstY;

    if (valuea == valueb)
    {
        valuea = edgea->fX;
        valueb = edgeb->fX;
    }

    // this overflows if valuea >>> valueb or vice-versa
    //     return valuea - valueb;
    // do perform the slower but safe compares
    return (valuea < valueb) ? -1 : (valuea > valueb);
}

static Edge* sortEdges(Edge* list[], int count, Edge** last)
{
    qsort(list, count, sizeof(Edge*), edgeCompare);

    // now make the edges linked in sorted order
    for (int i = 1; i < count; i++)
    {
        list[i - 1]->fNext = list[i];
        list[i]->fPrev = list[i - 1];
    }

    *last = list[count - 1];
    return list[0];
}

static void walkEdges(Edge* prevHead, int start_y, int stop_y,
                      Gl2d_Impl* gc, GraphicsContext::CanvasState* state)
{
    validateSort(prevHead->fNext);

    int curr_y = start_y;
    // returns 1 for evenodd, -1 for winding, regardless of inverse-ness
    int windingMask = 1;

    Color fillStyle = state->fillStyle.color();
    int alpha = fillStyle.alpha() * state->globalAlpha;
    if (state->shadowPath)
        alpha = 255;
    if (state->lineWidth != 1.0)
    {
        gc->Gfx_SwapBuffer();
        gc->Gfx_SetLineWidth(1.0);
    }

    for (;;)
    {
        int     w = 0;
        int     left SK_INIT_TO_AVOID_WARNING;
        bool    in_interval = false;
        Edge* currE = prevHead->fNext;
        SkFixed prevX = prevHead->fX;

        validateEdgesForY(currE, curr_y);

        while (currE->fFirstY <= curr_y)
        {
            int x = (currE->fX + SK_Fixed1/2) >> 16;
            w += currE->fWinding;
            if ((w & windingMask) == 0) // we finished an interval
            {
                SkASSERT(in_interval);
                int width = x - left;
                SkASSERT(width >= 0);
                if (width)
                {
                    gc->Gfx_RenderLineEx(left, curr_y, x , curr_y,
                                         fillStyle.red(), fillStyle.green(),
                                         fillStyle.blue(), alpha);
                                         //here  blitter->blitH(left, curr_y, width);
                    /*Point dst[4], src[2];
                    src[0].setX(left);
                    src[0].setY(curr_y);
                    src[1].setX(x);
                    src[1].setY(curr_y);

                    calculateLineQuad(dst, src, 1.0);

                    gc->Gfx_RenderQuadEx(dst[0].x(), dst[0].y(),
                                         dst[1].x(), dst[1].y(),
                                         dst[2].x(), dst[2].y(),
                                         dst[3].x(), dst[3].y(),
                                         fillStyle.red(), fillStyle.green(),
                                         fillStyle.blue(), alpha, 1);
                    */
                }
                in_interval = false;
            }
            else if (!in_interval)
            {
                left = x;
                in_interval = true;
            }

            Edge* next = currE->fNext;
            SkFixed newX;

            if (currE->fLastY == curr_y)    // are we done with this edge?
            {
                if (currE->fCurveCount < 0)
                {
                    if (((SkCubicEdge*)currE)->updateCubic())
                    {
                        SkASSERT(currE->fFirstY == curr_y + 1);

                        newX = currE->fX;
                        goto NEXT_X;
                    }
                }
                else if (currE->fCurveCount > 0)
                {
                    if (((SkQuadraticEdge*)currE)->updateQuadratic())
                    {
                        newX = currE->fX;
                        goto NEXT_X;
                    }
                }
                removeEdge(currE);
            }
            else
            {
                SkASSERT(currE->fLastY > curr_y);
                newX = currE->fX + currE->fDX;
                currE->fX = newX;
            NEXT_X:
                if (newX < prevX)   // ripple currE backwards until it is x-sorted
                    backwardInsertEdgeBasedOnX(currE  SkPARAM(curr_y));
                else
                    prevX = newX;
            }
            currE = next;
            SkASSERT(currE);
        }

        curr_y += 1;
        if (curr_y >= stop_y)
            break;

        // now currE points to the first edge with a Yint larger than curr_y
        insertNewEdges(currE, curr_y);
    }
    if (state->lineWidth != 1.0)
    {
        gc->Gfx_SwapBuffer();
        gc->Gfx_SetLineWidth(state->lineWidth);
    }
}

void FillPathWalkLine(Path& mpath, Gl2d_Impl* gc, GraphicsContext::CanvasState* state)
{
    SkIRect ir;
    SkPath* path = mpath.getPath();
    path->getBounds().round(&ir);

    size_t  size;
    int     maxCount = cheapWorstCaseEdgeCount(*path, &size);

    SkAutoMalloc    memory(maxCount * sizeof(Edge*) + size);
    Edge**        list = (Edge**)memory.get();
    Edge*         initialEdge = (Edge*)(list + maxCount);
    int             count = buildEdges(initialEdge, *path, NULL, list, 0);
    SkASSERT(count <= maxCount);

    if (count < 2)
    {
        return;
    }
    Edge headEdge, tailEdge, *last;
    // this returns the first and last edge after they're sorted into a dlink list
    Edge* edge = sortEdges(list, count, &last);

    headEdge.fPrev = NULL;
    headEdge.fNext = edge;
    headEdge.fFirstY = kEDGE_HEAD_Y;
    headEdge.fX = SK_MinS32;
    edge->fPrev = &headEdge;

    tailEdge.fPrev = last;
    tailEdge.fNext = NULL;
    tailEdge.fFirstY = kEDGE_TAIL_Y;
    last->fNext = &tailEdge;

    // now edge is the head of the sorted linklist
    walkEdges(&headEdge, ir.fTop, ir.fBottom, gc, state);
}

void drawShapsArray(int r, int g, int b, int a, Gl2d_Impl* gc,
                    GraphicsContext::CanvasState* state)
{
    //draw Triple queue
    int len = s_compensateTriples.size();
    for (int i = 0; i < len; i++)
    {
        compensateBevel(s_compensateTriples[i].points,
                        r, g, b, a,
                        gc, state);
    }
    //clear Triple queue
    s_compensateTriples.clear();

    //draw Round queue
    len = s_compensateRounds.size();
    for (int i = 0; i < len; i++)
    {
        compensateRound(s_compensateRounds[i].center,
                        r, g, b, a,
                        gc, state);
    }
    //clear Round queue
    s_compensateRounds.clear();
}

void drawLineCap(Gl2d_Impl* context, GraphicsContext::CanvasState* state)
{
    if (s_firstStrokePoint[0] != s_lastStrokePoint[1])
    {
        float radius = state->lineWidth / 2.0;
        switch(state->lineCap)
        {
            case RoundCap:
            {
                CanvasStyle old_style = state->fillStyle;
                state->fillStyle = state->strokeStyle;
                Path p ;
                p.clear();
                p.addArc(Point(s_firstStrokePoint[0].x(),
                               s_firstStrokePoint[0].y()),
                               floor(radius), 0, M_PI * 2, true);
                               
                FillPathWalkLine(p, context, state);
                p.clear();
                p.addArc(Point(s_lastStrokePoint[1].x(),
                               s_lastStrokePoint[1].y()),
                               floor(radius), 0, M_PI * 2, true);
                               
                FillPathWalkLine(p, context, state);
                state->fillStyle = old_style;
                break;
            }
            case SquareCap:
            {
                Color strokeColor = state->strokeStyle.color();
                int alpha = strokeColor.alpha() * state->globalAlpha;
                if (state->shadowPath)
                {
                    alpha = 255;
                }
                
                int r = strokeColor.red();
                int g = strokeColor.green();
                int b = strokeColor.blue();

                float lineLen = sqrt(
                             (s_firstStrokePoint[0].x() - s_firstStrokePoint[1].x()) *
                             (s_firstStrokePoint[0].x() - s_firstStrokePoint[1].x()) +
                             (s_firstStrokePoint[0].y() - s_firstStrokePoint[1].y()) *
                             (s_firstStrokePoint[0].y() - s_firstStrokePoint[1].y()));
                
                float dx = (s_firstStrokePoint[0].x() - s_firstStrokePoint[1].x()) *
                           radius;
                float dy = (s_firstStrokePoint[0].y() - s_firstStrokePoint[1].y()) *
                           radius;
                
                float x = (s_firstStrokePoint[0].x() * lineLen + dx) / lineLen;
                float y = (s_firstStrokePoint[0].y() * lineLen + dy) / lineLen;
                
                Point dst[4], src[2];
                src[0] = Point(x, y);
                src[1] = Point(s_firstStrokePoint[0].x(), s_firstStrokePoint[0].y());
                calculateLineQuad(dst, src, state->lineWidth);
                
                context->Gfx_RenderQuadEx(dst[0].x(), dst[0].y(),
                                     dst[1].x(), dst[1].y(),
                                     dst[2].x(), dst[2].y(),
                                     dst[3].x(), dst[3].y(),
                                     r, g, b, alpha, 1);
                
                //context->Gfx_RenderLineEx(x, y, s_firstStrokePoint[0].x(), 
                //                          s_firstStrokePoint[0].y(), r, g, b, alpha);
                
                lineLen = sqrt(
                       (s_lastStrokePoint[0].x() - s_lastStrokePoint[1].x()) *
                       (s_lastStrokePoint[0].x() - s_lastStrokePoint[1].x()) +
                       (s_lastStrokePoint[0].y() - s_lastStrokePoint[1].y()) *
                       (s_lastStrokePoint[0].y() - s_lastStrokePoint[1].y()));
                       
                dx = (s_lastStrokePoint[1].x() - s_lastStrokePoint[0].x()) *
                           radius;
                dy = (s_lastStrokePoint[1].y() - s_lastStrokePoint[0].y()) *
                           radius;
                           
                x = (s_lastStrokePoint[1].x() * lineLen + dx) / lineLen;
                y = (s_lastStrokePoint[1].y() * lineLen + dy) / lineLen;
                
                src[0] = Point(x, y);
                src[1] = Point(s_lastStrokePoint[1].x(), s_lastStrokePoint[1].y());
                calculateLineQuad(dst, src, state->lineWidth);
                
                context->Gfx_RenderQuadEx(dst[0].x(), dst[0].y(),
                                     dst[1].x(), dst[1].y(),
                                     dst[2].x(), dst[2].y(),
                                     dst[3].x(), dst[3].y(),
                                     r, g, b, alpha, 1);
                //context->Gfx_RenderLineEx(x, y, s_lastStrokePoint[1].x(), 
                //                          s_lastStrokePoint[1].y(), r, g, b, alpha);
                break;
            }
            case ButtCap:
            {
                //nothing to do
                break;
            }
            default:
                LOGE("StrokePath : lineCap is not match");
        }
    }
}

void StrokePath(const Path& m_path, Gl2d_Impl* m_context, GraphicsContext::CanvasState* m_state)
{
    PlatformPathPtr path = m_path.getPath();
    Point lastPoint;

    SkPath::Iter    iter(*path, false);
    SkPoint pts[4];
    SkPath::Verb    verb;
    Point fpts[4];
    Color strokeColor = m_state->strokeStyle.color();
    int alpha = strokeColor.alpha() * m_state->globalAlpha;
    if (m_state->shadowPath)
    {
        alpha = 255;
    }
    
    int r = strokeColor.red();
    int g = strokeColor.green();
    int b = strokeColor.blue();
    
    cleanStrokePoint();

    while ((verb = iter.next(pts)) != SkPath::kDone_Verb)
    {
        switch(verb)
        {
            //moveto
            case SkPath::kMove_Verb:
                if (!s_isClosePath)
                    drawLineCap(m_context, m_state);
                    
                s_isClosePath = false;
                s_beginCompensate = false;
                
                drawShapsArray(r, g, b, alpha, m_context, m_state);
                
                break;
            //lineto
            case SkPath::kLine_Verb:
            {
                setfpts(&fpts[0],&pts[0],2);

                renderLineSelf(fpts[0].x(), fpts[0].y(), fpts[1].x(), fpts[1].y(),
                               r, g, b, alpha, m_context, m_state);
                /*m_context->Gfx_RenderLineEx(TRANSFORMX(fpts[0].x(), fpts[0].y(), cosa, sina),//lastPoint.x(),
                                            TRANSFORMY(fpts[0].x(), fpts[0].y(), cosa, sina),//lastPoint.y(),
                                            TRANSFORMX(fpts[1].x(), fpts[1].y(), cosa, sina),
                                            TRANSFORMY(fpts[1].x(), fpts[1].y(), cosa, sina), strokeColor.red(),
                                            strokeColor.green(), strokeColor.blue(), alpha);*/
                 break;
            }
            //arc
            case SkPath::kQuad_Verb:
            {
                setfpts(&fpts[0], &pts[0], 3);
                int d = compute_int_quad_dist(fpts);
                /*
                  quadratics approach the line connecting their start and end points
                  4x closer with each subdivision, so we compute the number of
                  subdivisions to be the minimum need to get that distance to be less
                  than a pixel.
                */
                int level = (33 - CLZ_portable(d)) >> 1;
                if (level > kMaxQuadSubdivideLevel)
                {
                    level = kMaxQuadSubdivideLevel;
                }
                strokequad(fpts, level, m_context, m_state);
                
                break;
            }
            case SkPath::kCubic_Verb:
            {
                setfpts(&fpts[0], &pts[0], 4);
                strokecubic(fpts, kMaxCubicSubdivideLevel, m_context, m_state);
                /*char res[300];
                sprintf(res,"kCubic_Verb x= %f, y =%f, x1 = %f, y1= %f, x2 = %f, y2 = %f, x3 = %f, y3 = %f\n",fpts[0].x(), fpts[0].y(), fpts[1].x(), fpts[1].y(), fpts[2].x(), fpts[2].y(), fpts[3].x(), fpts[3].y());
                LOGE(res);
                lastPoint = fpts[3];*/
                break;
            }
            case SkPath::kClose_Verb:
            {
                s_isClosePath = true;
                Point pArray[3];
                pArray[0] = s_firstStrokePoint[0];
                pArray[2] = s_firstStrokePoint[1];
                
                s_pureCompensate = true;
                renderWidthCompensate(pArray, m_context, 
                                      m_state, s_lastCompensateFunc);
                s_pureCompensate = false;
                break;
            }
            default:
                break;
        }
    }
    //m_context->Transform_enable();//has problem
    
    drawShapsArray(r, g, b, alpha, m_context, m_state);
    
    if (!s_isClosePath)
        drawLineCap(m_context, m_state);
}

void renderLineSelf(float x1, float y1, float x2, float y2, int r, int g, int b, int a, Gl2d_Impl* m_context, GraphicsContext::CanvasState* m_state)
{
     Point fpts[3];
     fpts[0] = Point(x1, y1);
     fpts[2] = Point(x2, y2);
     
     CompensateFunc func = compensateMiter;
     if (m_state->lineJoin == BevelJoin)
        func = compensateBevel;
     else if (m_state->lineJoin == RoundJoin)
        func = compensateRound;
        
     renderWidthCompensate(fpts, m_context, m_state, func);
    //m_context->Gfx_RenderLineEx(x1, y1, x2, y2, r, g, b, a);
}

}// namespace DCanvas
