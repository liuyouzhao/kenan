#include "FloatRect.h"

#include <math.h>

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

namespace DCanvas
{

bool FloatRect::intersects(const FloatRect& other) const
{
    // Checking emptiness handles negative widths as well as zero.
    return !isEmpty() && !other.isEmpty()
        && x() < other.right() && other.x() < right()
        && y() < other.bottom() && other.y() < bottom();
}

bool FloatRect::contains(const FloatRect& other) const
{
    return x() <= other.x() && right() >= other.right()
        && y() <= other.y() && bottom() >= other.bottom();
}

void FloatRect::intersect(const FloatRect& other)
{
    float l = max(x(), other.x());
    float t = max(y(), other.y());
    float r = min(right(), other.right());
    float b = min(bottom(), other.bottom());

    // Return a clean empty rectangle for non-intersecting cases.
    if (l >= r || t >= b)
    {
        l = 0;
        t = 0;
        r = 0;
        b = 0;
    }

    m_location.setX(l);
    m_location.setY(t);
    m_size.setWidth(r - l);
    m_size.setHeight(b - t);
}

void FloatRect::unite(const FloatRect& other)
{
    // Handle empty special cases first.
    if (other.isEmpty())
        return;

    if (isEmpty())
    {
        *this = other;
        return;
    }

    float l = min(x(), other.x());
    float t = min(y(), other.y());
    float r = max(right(), other.right());
    float b = max(bottom(), other.bottom());

    m_location.setX(l);
    m_location.setY(t);
    m_size.setWidth(r - l);
    m_size.setHeight(b - t);
}

void FloatRect::scale(float sx, float sy)
{
    m_location.setX(x() * sx);
    m_location.setY(y() * sy);
    m_size.setWidth(width() * sx);
    m_size.setHeight(height() * sy);
}

FloatRect mapRect(const FloatRect& r, const FloatRect& srcRect, const FloatRect& destRect)
{
    if (srcRect.width() == 0 || srcRect.height() == 0)
        return FloatRect();

    float widthScale = destRect.width() / srcRect.width();
    float heightScale = destRect.height() / srcRect.height();
    return FloatRect(destRect.x() + (r.x() - srcRect.x()) * widthScale,
                     destRect.y() + (r.y() - srcRect.y()) * heightScale,
                     r.width() * widthScale, r.height() * heightScale);
}

#if USE_SKIA_ANDROID
FloatRect::FloatRect(const SkRect& r)
    : m_location(r.fLeft, r.fTop)
    , m_size(r.width(), r.height())
{
}

FloatRect::operator SkRect() const
{
    SkRect rect = { x(), y(), right(), bottom() };
    return rect;
}
#endif

} // namespace DCanvas
