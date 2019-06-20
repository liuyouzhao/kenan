#ifndef PATH_H
#define PATH_H

#include <algorithm>
#include "Point.h"
#include "FloatRect.h"
#include "DMath.h"
#include "AliTransformationMatrix.h"

#include <string>

#if USE_SKIA_ANDROID
class SkPath;
typedef SkPath PlatformPath;
#endif
typedef PlatformPath* PlatformPathPtr;

namespace DCanvas
{

#define kMaxCubicSubdivideLevel 6
#define kMaxQuadSubdivideLevel  5

static void chopQuadAt(const Point* src/*3*/, Point* dst/*5*/, float t)
{
    float    abx = Interp(src[0].x(), src[1].x(), t);
    float    aby = Interp(src[0].y(), src[1].y(), t);
    float    bcx = Interp(src[1].x(), src[2].x(), t);
    float    bcy = Interp(src[1].y(), src[2].y(), t);
    float    abcx = Interp(abx, bcx, t);
    float    abcy = Interp(aby, bcy, t);

    dst[0] = src[0];
    dst[1].set(abx, aby);
    dst[2].set(abcx, abcy);
    dst[3].set(bcx, bcy);
    dst[4] = src[2];
}

static void chopCubicAt(const Point* src/*4*/, Point* dst/*7*/, float t)
{
    float    abx = Interp(src[0].x(), src[1].x(), t);
    float    aby = Interp(src[0].y(), src[1].y(), t);
    float    bcx = Interp(src[1].x(), src[2].x(), t);
    float    bcy = Interp(src[1].y(), src[2].y(), t);
    float    cdx = Interp(src[2].x(), src[3].x(), t);
    float    cdy = Interp(src[2].y(), src[3].y(), t);
    float    abcx = Interp(abx, bcx, t);
    float    abcy = Interp(aby, bcy, t);
    float    bcdx = Interp(bcx, cdx, t);
    float    bcdy = Interp(bcy, cdy, t);
    float    abcdx = Interp(abcx, bcdx, t);
    float    abcdy = Interp(abcy, bcdy, t);

    dst[0] = src[0];
    dst[1].set(abx, aby);
    dst[2].set(abcx, abcy);
    dst[3].set(abcdx, abcdy);
    dst[4].set(bcdx, bcdy);
    dst[5].set(cdx, cdy);
    dst[6] = src[3];
}

static void chopQuadAtHalf(const Point *src, Point *dst)
{
    float x01 = ave(src[0].x(), src[1].x());
    float y01 = ave(src[0].y(), src[1].y());
    float x12 = ave(src[1].x(), src[2].x());
    float y12 = ave(src[1].y(), src[2].y());

    dst[0] = src[0];
    dst[1].set(x01, y01);
    dst[2].set(ave(x01, x12), ave(y01, y12));
    dst[3].set(x12, y12);
    dst[4] = src[2];
}

static int compute_int_quad_dist(const Point pts[3]) {
    // compute the vector between the control point ([1]) and the middle of the
    // line connecting the start and end ([0] and [2])
    // we want everyone to be positive
    float dx =ABS( (pts[0].x() + pts[2].x())/2.0f - pts[1].x());
    float dy =ABS( (pts[0].y() + pts[2].y())/2.0f - pts[1].y());

    // convert to whole pixel values (use ceiling to be conservative)
    int idx = dceil(dx);
    int idy = dceil(dy);
    // use the cheap approx for distance
    if (idx > idy)
    {
        return idx + (idy >> 1);
    }
    else
    {
        return idy + (idx >> 1);
    }
}

static int CLZ_portable(uint32_t x)
{
    if (x == 0)
    {
        return 32;
    }

    int zeros = ((x >> 16) - 1) >> 31 << 4;
    x <<= zeros;

    int nonzero = ((x >> 24) - 1) >> 31 << 3;
    zeros += nonzero;
    x <<= nonzero;

    nonzero = ((x >> 28) - 1) >> 31 << 2;
    zeros += nonzero;
    x <<= nonzero;

    nonzero = ((x >> 30) - 1) >> 31 << 1;
    zeros += nonzero;
    x <<= nonzero;

    zeros += (~x) >> 31;
    return zeros;
}

#if USE_SKIA_ANDROID
static Point* setfpts(Point *dst, const SkPoint *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        (dst+i)->setX(SkScalarToFloat((src+i)->fX));
        (dst+i)->setY(SkScalarToFloat((src+i)->fY));
    }
    return dst;
}
#endif

enum WindRule
{
	RULE_NONZERO = 0,
	RULE_EVENODD = 1
};

enum PathElementType
{
	PathElementMoveToPoint,
	PathElementAddLineToPoint,
	PathElementAddQuadCurveToPoint,
	PathElementAddCurveToPoint,
	PathElementCloseSubpath
};

struct PathElement
{
	PathElementType type;
	Point* points;
};

typedef void (*PathApplierFunction)(void* info, const PathElement*);

class Path
{
public:
	Path();
	~Path();

	Path(const Path&);
	Path& operator=(const Path&);

	void swap(Path& other) { std::swap(m_path, other.m_path); }

	bool contains(const Point&, WindRule rule = RULE_NONZERO) const;
	//bool strokeContains(StrokeStyleApplier*, const Point&) const;
	FloatRect boundingRect() const;
   // FloatRect strokeBoundingRect(StrokeStyleApplier* = 0);

	void clear();
	bool isEmpty() const;
	// Gets the current point of the current path, which is conceptually the final point reached by the path so far.
	// Note the Path can be empty (isEmpty() == true) and still have a current point.
	bool hasCurrentPoint() const;

	void moveTo(const Point&);
	void moveTo(float x, float y);
	void addLineTo(const Point&);
	void addLineTo(const float x, const float y);
	void addQuadCurveTo(const Point& controlPoint, const Point& endPoint);
	void addBezierCurveTo(const Point& controlPoint1, const Point& controlPoint2, const Point& endPoint);
	void addArcTo(const Point&, const Point&, float radius);
	void closeSubpath();

	void addArc(const Point&, float radius, float startAngle, float endAngle, bool anticlockwise);
	void addRect(const FloatRect&);
	void addEllipse(const FloatRect&);

	void translate(const FloatSize&);
	void translate(const float x, const float y){this->translate(FloatSize(x, y));}
	void scale(const float x, const float y);
	void rotate(const float degrees);

	std::string debugString() const;

	PlatformPathPtr platformPath() const { return m_path; }

	static Path createRoundedRectangle(const FloatRect&, const FloatSize& roundingRadii);
	static Path createRoundedRectangle(const FloatRect&, const FloatSize& topLeftRadius, const FloatSize& topRightRadius, const FloatSize& bottomLeftRadius, const FloatSize& bottomRightRadius);
	static Path createRectangle(const FloatRect&);
	static Path createEllipse(const Point& center, float rx, float ry);
	static Path createCircle(const Point& center, float r);
	static Path createLine(const Point&, const Point&);

	void apply(void* info, PathApplierFunction) const;
	void transform(float a, float b, float c, float d, float e, float f);
	void setTransform(const AliTransformationMatrix&);
	PlatformPathPtr getPath() const;

	void addCircle(float x, float y, float r){m_path->addCircle(x,y,r);}

private:
	PlatformPathPtr m_path;
	AliTransformationMatrix m_matrix;
};

} // namespace DCanvas

#endif // PATH_H
