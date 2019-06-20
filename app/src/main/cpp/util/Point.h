#ifndef POINT_H
#define POINT_H

#include <math.h>
namespace DCanvas
{

class Point
{
public:
    Point():m_x(0), m_y(0) { }
    ~Point() { }
    Point(float fx, float fy):m_x(fx), m_y(fy) { }

    static Point Make(float fx, float fy)
    {
        Point p(fx, fy);
        return p;
    }
    void set(float fx, float fy) { m_x = fx; m_y = fy; }
    void setX(float fx) { m_x = fx; }
    void setY(float fy) { m_y = fy; }
    void negate() { m_x = -m_x; m_y = -m_y; }
    float x()const { return this->m_x; }
    float y()const { return this->m_y; }

    Point operator-() const
    {
        Point neg;
        neg.m_x = -m_x;
        neg.m_y = -m_y;
        return neg;
    }
    void operator +=(const Point& v)
    {
        m_x += v.x();
        m_y += v.y();
    }
    void operator -=(const Point& v)
    {
        m_x -= v.x();
        m_y -= v.y();
    }
    bool equals(float fx, float fy) const
    {
        return m_x == fx && m_y == fy;
    }
    void move(float x, float y)
    {
        m_x += x;
        m_y += y;
    }

    friend bool operator == (const Point& a, const Point& b)
    {
        return a.x() == b.x() && a.y() == b.y();
    }

    friend bool operator != (const Point& a, const Point& b)
    {
        return a.x() != b.x() || a.y() != b.y();
    }

    friend Point operator - (const Point& a, const Point& b)
    {
        Point v;
        v.set(a.x() - b.x(), a.y() - b.y());
        return v;
    }
    friend Point operator + (const Point& a, const Point& b)
    {
        Point v;
        v.set(a.x() + b.x(), a.y() + b.y());
        return v;
    }
    static float DotProduct(const Point& a, const Point& b)
    {
        return a.x() * b.x() + a.y() * b.y();
    }
    static float CrossProduct(const Point& a, const Point& b)
    {
        return a.x() * b.y() - a.y() * b.x();
    }
    static float Distance(const Point& a, const Point& b)
    {
        return Length(a.x() - b.x(), a.y() - b.y());
    }
    static float Length(float fx, float fy)
    {
        return sqrt(fx * fx  + fy * fy);
    }

#if USE_SKIA_ANDROID
    Point(const SkPoint& p)
        : m_x(p.fX)
        , m_y(p.fY)
    {
    }

    operator SkPoint() const
    {
        SkPoint p = {m_x, m_y};
        return p;
    }
#endif //USE_SKIA_ANDROID

private:
    float m_x, m_y;
};

} // namespace DCanvas

#endif // POINT_H
