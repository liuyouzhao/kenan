#include "defines.h"
#include "Path.h"
#include "Point.h"

#include <math.h>
#include <sstream>
#include "DMath.h"

#if USE_SKIA_ANDROID
#include "SkPath.h"
#include "SkRegion.h"
#endif

#undef LOG_TAG
#define  LOG_TAG    "Path"


static const float QUARTER = 0.552f; // approximation of control point positions on a bezier
                                     // to simulate a quarter of a circle.
namespace DCanvas
{

static void mapPoints(Point *p, int count, const AliTransformationMatrix& matrix)
{
    float x, y;
    for (int i = 0; i < count; ++i)
    {
        matrix.map(p[i].x(), p[i].y(), x, y);
        p[i].set(x, y);
    }
}

static void subdivide_quad_to(SkPath* path, Point pts[3],
                              int level , const AliTransformationMatrix& matrix)
{
    if (--level >= 0)
    {
        Point tmp[5];

        chopQuadAtHalf(pts, tmp);
        subdivide_quad_to(path, &tmp[0], level, matrix);
        subdivide_quad_to(path, &tmp[2], level, matrix);
    }
    else
    {
        mapPoints(&pts[1],2,matrix);
        path->quadTo(pts[1], pts[2]);
    }
}

static void subdivide_cubic_to(SkPath* path, Point pts[4],
                               int level , const AliTransformationMatrix& matrix)
{
    if (--level >= 0)
    {
        Point tmp[7];

        chopCubicAt(pts, tmp, 0.5f);
        subdivide_cubic_to(path, &tmp[0], level, matrix);
        subdivide_cubic_to(path, &tmp[3], level, matrix);
    }
    else
    {
        mapPoints(&pts[1],3,matrix);
        path->cubicTo(pts[1], pts[2], pts[3]);
    }
}
static void  Transform(SkPath* path, const AliTransformationMatrix &matrix)
{
    SkPath  tmp;
    char res[300];
    SkPath::Iter    iter(*path, false);
    SkPoint         pts[4];
    Point           fpts[4];
    SkPath::Verb    verb;

    while ((verb = iter.next(pts)) != SkPath::kDone_Verb)
    {
        switch (verb)
        {
            case SkPath::kMove_Verb:
                setfpts(&fpts[0],&pts[0],4);
                mapPoints(&fpts[0],1,matrix);
                tmp.moveTo(fpts[0]);
                break;
            case SkPath::kLine_Verb:
                setfpts(&fpts[0],&pts[0],4);
                mapPoints(&fpts[1],1,matrix);
                tmp.lineTo(fpts[1]);
                break;
            case SkPath::kQuad_Verb:
                setfpts(&fpts[0],&pts[0],4);
                subdivide_quad_to(&tmp, fpts, 2, matrix);
                break;
            case SkPath::kCubic_Verb:
                setfpts(&fpts[0],&pts[0],4);
                subdivide_cubic_to(&tmp, fpts, 2, matrix);
                break;
            case SkPath::kClose_Verb:
                tmp.close();
                break;
            default:
                SkASSERT(!"unknown verb");
                break;
        }
    }

    path->swap(tmp);
}

Path::Path()
{
    m_path = new SkPath;
}

Path::Path(const Path& other)
{
    m_path = new SkPath(*other.m_path);
}

Path::~Path()
{
    delete m_path;
}

Path& Path::operator=(const Path& other)
{
    *m_path = *other.m_path;
    return *this;
}

bool Path::isEmpty() const
{
    return m_path->isEmpty();
}

bool Path::hasCurrentPoint() const
{
    // we wants to know if we have any points, including any moveTos.
    // Skia's empty() will return true if it has just a moveTo, so we need to
    // call getPoints(NULL), which returns the number of points,
    // including moveTo.
    return m_path->getPoints(0, 0) > 0;
}

bool Path::contains(const Point& point, WindRule rule) const
{
    SkRegion    rgn, clip;

    int x = (int)floorf(point.x());
    int y = (int)floorf(point.y());
    clip.setRect(x, y, x + 1, y + 1);

    SkPath::FillType ft = m_path->getFillType();    // save
    m_path->setFillType(rule == RULE_NONZERO ? SkPath::kWinding_FillType : SkPath::kEvenOdd_FillType);

    bool contains = rgn.setPath(*m_path, clip);

    m_path->setFillType(ft);    // restore
    return contains;
}

void Path::translate(const FloatSize& size)
{
    m_path->offset(SkFloatToScalar(size.width()), SkFloatToScalar(size.height()));
}
void Path::scale(const float x, const float y)
{
/*
    SkMatrix matrix;
    matrix.setScale(x, y);
    m_path->transform(matrix);
*/
}

void Path::rotate(const float radian)
{
/*
    SkMatrix matrix;
    matrix.setRotate(radian * 180 /M_PI);
    m_path->transform(matrix);
*/
}
void Path::transform(float a, float b, float c, float d, float e, float f)
{
    SkMatrix matrix;
    matrix.setScaleX(a);
    matrix.setScaleY(d);
    matrix.setSkewY(c);
    matrix.setSkewX(b);
    matrix.setTranslateX(e);
    matrix.setTranslateY(f);

    //m_path->transform(matrix);
}

void Path::setTransform(const AliTransformationMatrix& tmatrix)
{
    if (m_matrix != tmatrix)
    {
        if (!tmatrix.isInvertible())
            return;
        Transform(m_path, m_matrix.inverse());

        Transform(m_path, tmatrix);
        m_matrix = tmatrix;
    }
}
FloatRect Path::boundingRect() const
{
    const SkRect& r = m_path->getBounds();
    return FloatRect(   SkScalarToFloat(r.fLeft),
                        SkScalarToFloat(r.fTop),
                        SkScalarToFloat(r.width()),
                        SkScalarToFloat(r.height()));
}

void Path::moveTo(const Point& point)
{
    m_path->moveTo(SkFloatToScalar(point.x()), SkFloatToScalar(point.y()));
}

void Path::moveTo(float x, float y)
{
    m_path->moveTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::addLineTo(const Point& p)
{
    m_path->lineTo(SkFloatToScalar(p.x()), SkFloatToScalar(p.y()));
}

void Path::addLineTo(const float x, const float y)
{
    m_path->lineTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::addQuadCurveTo(const Point& cp, const Point& ep)
{
    m_path->quadTo( SkFloatToScalar(cp.x()), SkFloatToScalar(cp.y()),
                    SkFloatToScalar(ep.x()), SkFloatToScalar(ep.y()));
}

void Path::addBezierCurveTo(const Point& p1, const Point& p2, const Point& ep)
{
    m_path->cubicTo(SkFloatToScalar(p1.x()), SkFloatToScalar(p1.y()),
                    SkFloatToScalar(p2.x()), SkFloatToScalar(p2.y()),
                    SkFloatToScalar(ep.x()), SkFloatToScalar(ep.y()));
}

void Path::addArcTo(const Point& p1, const Point& p2, float radius)
{
    m_path->arcTo(SkFloatToScalar(p1.x()), SkFloatToScalar(p1.y()),
                  SkFloatToScalar(p2.x()), SkFloatToScalar(p2.y()),
                  SkFloatToScalar(radius));
}

void Path::closeSubpath()
{
    m_path->close();
}

static const float gPI  = 3.14159265f;
static const float g2PI = 6.28318531f;
static const float g180OverPI = 57.29577951308f;

static float fast_mod(float angle, float max)
{
    if (angle >= max || angle <= -max)
    {
        angle = fmodf(angle, max);
    }
    return angle;
}

void Path::addArc(const Point& p, float r, float sa, float ea,
                  bool clockwise)
{
    SkScalar    cx = SkFloatToScalar(p.x());
    SkScalar    cy = SkFloatToScalar(p.y());
    SkScalar    radius = SkFloatToScalar(r);

    SkRect  oval;
    oval.set(cx - radius, cy - radius, cx + radius, cy + radius);

    float sweep = ea - sa;
    bool prependOval = false;

    /*  Note if clockwise and the sign of the sweep disagree. This particular
        logic was deduced from http://canvex.lazyilluminati.com/misc/arc.html
    */
    if (clockwise && (sweep > 0 || sweep < -g2PI))
    {
        sweep = fmodf(sweep, g2PI) - g2PI;
    }
    else if (!clockwise && (sweep < 0 || sweep > g2PI))
    {
        sweep = fmodf(sweep, g2PI) + g2PI;
    }

    // If the abs(sweep) >= 2PI, then we need to add a circle before we call
    // arcTo, since it treats the sweep mod 2PI. We don't have a prepend call,
    // so we just remember this, and at the end create a new path with an oval
    // and our current path, and then swap then.
    //
    if (sweep >= g2PI || sweep <= -g2PI)
    {
        prependOval = true;
//        SkDebugf("addArc sa=%g ea=%g cw=%d sweep %g treat as circle\n", sa, ea, clockwise, sweep);

        // now reduce sweep to just the amount we need, so that the current
        // point is left where the caller expects it.
        sweep = fmodf(sweep, g2PI);
    }

    sa = fast_mod(sa, g2PI);
    SkScalar startDegrees = SkFloatToScalar(sa * g180OverPI);
    SkScalar sweepDegrees = SkFloatToScalar(sweep * g180OverPI);

//    SkDebugf("addArc sa=%g ea=%g cw=%d sweep=%g ssweep=%g\n", sa, ea, clockwise, sweep, SkScalarToFloat(sweepDegrees));
    m_path->arcTo(oval, startDegrees, sweepDegrees, false);

    if (prependOval)
    {
        SkPath tmp;
        tmp.addOval(oval);
        tmp.addPath(*m_path);
        m_path->swap(tmp);
    }
}

void Path::addRect(const FloatRect& rect)
{
    m_path->addRect(rect);
}

void Path::addEllipse(const FloatRect& rect)
{
    m_path->addOval(rect);
}

void Path::clear()
{
    m_path->reset();
    m_matrix.makeIdentity();
}

PlatformPathPtr Path::getPath() const
{
    return m_path;
}

void Path::apply(void* info, PathApplierFunction function) const
{
    SkPath::Iter    iter(*m_path, false);
    SkPoint         pts[4];

    PathElement     elem;
    Point      fpts[3];

    for (;;)
    {
        switch (iter.next(pts))
        {
            case SkPath::kMove_Verb:
                elem.type = PathElementMoveToPoint;
                elem.points = setfpts(fpts, &pts[0], 1);
                break;
            case SkPath::kLine_Verb:
                elem.type = PathElementAddLineToPoint;
                elem.points = setfpts(fpts, &pts[1], 1);
                break;
            case SkPath::kQuad_Verb:
                elem.type = PathElementAddQuadCurveToPoint;
                elem.points = setfpts(fpts, &pts[1], 2);
                break;
            case SkPath::kCubic_Verb:
                elem.type = PathElementAddCurveToPoint;
                elem.points = setfpts(fpts, &pts[1], 3);
                break;
            case SkPath::kClose_Verb:
                elem.type = PathElementCloseSubpath;
                elem.points = setfpts(fpts, 0, 0);
                break;
            case SkPath::kDone_Verb:
                return;
        }
        function(info, &elem);
    }
}

/*void Path::transform(const AffineTransform& xform)
{
    m_path->transform(xform);
}*/

std::string Path::debugString() const
{
    std::ostringstream otr;

    SkPath::Iter iter(*m_path, false);
    SkPoint pts[4];

    int numPoints = m_path->getPoints(0, 0);
    SkPath::Verb verb;

    do
    {
        verb = iter.next(pts);
        switch (verb)
        {
            case SkPath::kMove_Verb:
                otr <<"M " <<pts[0].fX << pts[0].fY<<std::endl;
                numPoints -= 1;
                break;
            case SkPath::kLine_Verb:
              if (!iter.isCloseLine()) {
                    otr <<"L " << pts[1].fX << pts[1].fY<<std::endl;
                    numPoints -= 1;
                }
                break;
            case SkPath::kQuad_Verb:
                otr << "Q "<< pts[1].fX << pts[1].fY <<
                    pts[2].fX << pts[2].fY<<std::endl;
                numPoints -= 2;
                break;
            case SkPath::kCubic_Verb:
                otr <<"C " << pts[1].fX << pts[1].fY <<
                    pts[2].fX << pts[2].fY << pts[3].fX << pts[3].fY<<std::endl;
                numPoints -= 3;
                break;
            case SkPath::kClose_Verb:
                otr <<"Z "<<std::endl;
                break;
            case SkPath::kDone_Verb:
                break;
        }
    } while (verb != SkPath::kDone_Verb);

    // If you have a path that ends with an M, Skia will not iterate the
    // trailing M. That's nice of it, but Apple's paths output the trailing M
    // and we want out layout dumps to look like theirs
    if (numPoints)
    {
        //ASSERT(numPoints==1);
        LOGD("numPoints %d is 1??",numPoints);
        m_path->getLastPt(pts);
        otr <<"M " << pts[0].fX << pts[0].fY<< std::endl;
    }

    return otr.str();
}

Path Path::createRoundedRectangle(const FloatRect& rectangle, const FloatSize& roundingRadii)
{
    Path path;
    float x = rectangle.x();
    float y = rectangle.y();
    float width = rectangle.width();
    float height = rectangle.height();
    float rx = roundingRadii.width();
    float ry = roundingRadii.height();
    if (width <= 0.0f || height <= 0.0f)
        return path;

    float dx = rx, dy = ry;
    // If rx is greater than half of the width of the rectangle
    // then set rx to half of the width (required in SVG spec)
    if (dx > width * 0.5f)
        dx = width * 0.5f;

    // If ry is greater than half of the height of the rectangle
    // then set ry to half of the height (required in SVG spec)
    if (dy > height * 0.5f)
        dy = height * 0.5f;

    path.moveTo(Point(x + dx, y));

    if (dx < width * 0.5f)
        path.addLineTo(Point(x + width - rx, y));

    path.addBezierCurveTo(Point(x + width - dx * (1 - QUARTER), y), Point(x + width, y + dy * (1 - QUARTER)), Point(x + width, y + dy));

    if (dy < height * 0.5)
        path.addLineTo(Point(x + width, y + height - dy));

    path.addBezierCurveTo(Point(x + width, y + height - dy * (1 - QUARTER)), Point(x + width - dx * (1 - QUARTER), y + height), Point(x + width - dx, y + height));

    if (dx < width * 0.5)
        path.addLineTo(Point(x + dx, y + height));

    path.addBezierCurveTo(Point(x + dx * (1 - QUARTER), y + height), Point(x, y + height - dy * (1 - QUARTER)), Point(x, y + height - dy));

    if (dy < height * 0.5)
        path.addLineTo(Point(x, y + dy));

    path.addBezierCurveTo(Point(x, y + dy * (1 - QUARTER)), Point(x + dx * (1 - QUARTER), y), Point(x + dx, y));

    path.closeSubpath();

    return path;
}

Path Path::createRoundedRectangle(const FloatRect& rectangle,
                                  const FloatSize& topLeftRadius,
                                  const FloatSize& topRightRadius,
                                  const FloatSize& bottomLeftRadius,
                                  const FloatSize& bottomRightRadius)
{
    Path path;

    float width = rectangle.width();
    float height = rectangle.height();
    if (width <= 0.0 || height <= 0.0)
        return path;

    if (width < topLeftRadius.width() + topRightRadius.width()
     || width < bottomLeftRadius.width() + bottomRightRadius.width()
     || height < topLeftRadius.height() + bottomLeftRadius.height()
     || height < topRightRadius.height() + bottomRightRadius.height())
        // If all the radii cannot be accommodated, return a rect.
        return createRectangle(rectangle);

    float x = rectangle.x();
    float y = rectangle.y();

    path.moveTo(Point(x + topLeftRadius.width(), y));

    path.addLineTo(Point(x + width - topRightRadius.width(), y));

    path.addBezierCurveTo(Point(x + width - topRightRadius.width() * (1 - QUARTER), y), Point(x + width, y + topRightRadius.height() * (1 - QUARTER)), Point(x + width, y + topRightRadius.height()));

    path.addLineTo(Point(x + width, y + height - bottomRightRadius.height()));

    path.addBezierCurveTo(Point(x + width, y + height - bottomRightRadius.height() * (1 - QUARTER)), Point(x + width - bottomRightRadius.width() * (1 - QUARTER), y + height), Point(x + width - bottomRightRadius.width(), y + height));

    path.addLineTo(Point(x + bottomLeftRadius.width(), y + height));

    path.addBezierCurveTo(Point(x + bottomLeftRadius.width() * (1 - QUARTER), y + height), Point(x, y + height - bottomLeftRadius.height() * (1 - QUARTER)), Point(x, y + height - bottomLeftRadius.height()));

    path.addLineTo(Point(x, y + topLeftRadius.height()));

    path.addBezierCurveTo(Point(x, y + topLeftRadius.height() * (1 - QUARTER)), Point(x + topLeftRadius.width() * (1 - QUARTER), y), Point(x + topLeftRadius.width(), y));

    path.closeSubpath();

    return path;
}

Path Path::createRectangle(const FloatRect& rectangle)
{
    Path path;
    float x = rectangle.x();
    float y = rectangle.y();
    float width = rectangle.width();
    float height = rectangle.height();
    if (width <= 0.0f || height <= 0.0f)
        return path;

    path.moveTo(Point(x, y));
    path.addLineTo(Point(x + width, y));
    path.addLineTo(Point(x + width, y + height));
    path.addLineTo(Point(x, y + height));
    path.closeSubpath();

    return path;
}

Path Path::createEllipse(const Point& center, float rx, float ry)
{
    float cx = center.x();
    float cy = center.y();
    Path path;
    if (rx <= 0.0f || ry <= 0.0f)
        return path;

    float x = cx;
    float y = cy;

    unsigned step = 0, num = 100;
    bool running = true;
    while (running)
    {
        if (step == num)
        {
            running = false;
            break;
        }

        float angle = static_cast<float>(step) / static_cast<float>(num) * 2.0f * piFloat;
        x = cx + cosf(angle) * rx;
        y = cy + sinf(angle) * ry;

        step++;
        if (step == 1)
            path.moveTo(Point(x, y));
        else
            path.addLineTo(Point(x, y));
    }

    path.closeSubpath();

    return path;
}

Path Path::createCircle(const Point& center, float r)
{
    return createEllipse(center, r, r);
}

Path Path::createLine(const Point& start, const Point& end)
{
    Path path;
    if (start.x() == end.x() && start.y() == end.y())
        return path;

    path.moveTo(start);
    path.addLineTo(end);

    return path;
}

} // namespace DCanvas
