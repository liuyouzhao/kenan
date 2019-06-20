#ifndef FLOATSIZE_H
#define FLOATRECT_H

namespace DCanvas
{

class FloatSize
{
public:
    FloatSize() : m_width(0), m_height(0) { }
    FloatSize(float width, float height) : m_width(width), m_height(height) { }

    float width() const { return m_width; }
    float height() const { return m_height; }

    void setWidth(float width) { m_width = width; }
    void setHeight(float height) { m_height = height; }

    bool isEmpty() const { return m_width <= 0 || m_height <= 0; }

    FloatSize expandedTo(const FloatSize& other) const
    {
        return FloatSize(m_width > other.m_width ? m_width : other.m_width,
            m_height > other.m_height ? m_height : other.m_height);
    }

    FloatSize shrunkTo(const FloatSize& other) const
    {
       return FloatSize(m_width < other.m_width ? m_width : other.m_width,
           m_height < other.m_height ? m_height : other.m_height);
    }

private:
    float m_width, m_height;
};

inline FloatSize& operator+=(FloatSize& a, const FloatSize& b)
{
    a.setWidth(a.width() + b.width());
    a.setHeight(a.height() + b.height());
    return a;
}

inline FloatSize& operator-=(FloatSize& a, const FloatSize& b)
{
    a.setWidth(a.width() - b.width());
    a.setHeight(a.height() - b.height());
    return a;
}

inline FloatSize operator+(const FloatSize& a, const FloatSize& b)
{
    return FloatSize(a.width() + b.width(), a.height() + b.height());
}

inline FloatSize operator-(const FloatSize& a, const FloatSize& b)
{
    return FloatSize(a.width() - b.width(), a.height() - b.height());
}

inline FloatSize operator-(const FloatSize& size)
{
    return FloatSize(-size.width(), -size.height());
}

inline bool operator==(const FloatSize& a, const FloatSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}

inline bool operator!=(const FloatSize& a, const FloatSize& b)
{
    return a.width() != b.width() || a.height() != b.height();
}

} // namespace

#endif // FLOATRECT_H
