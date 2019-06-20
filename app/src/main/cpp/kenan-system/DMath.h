#ifndef DMATH_H
#define DMATH_H

#include <math.h>
#include "Assert.h"

#define max(a,b) ((a > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)
#define ABS(a)   ((a < 0) ? -a : a)
#define ave(a,b) ((a + b) * 0.5f)

namespace DCanvas
{

const float piFloat = 3.14159265358979323846f;

static int dceil(float x)
{
    return ceil(x);
}

/** Linearly interpolate between A and B, based on t.
    If t is 0, return A
    If t is 1, return B
    else interpolate.
    t must be [0..SK_Scalar1]
*/
static inline float Interp(float A, float B, float t)
{
    return A + ((B - A) * t);
}

inline bool isASCIISpace(char c) { return c <= ' ' && (c == ' ' || (c <= 0xD && c >= 0x9)); }
inline bool isASCIISpace(unsigned short c) { return c <= ' ' && (c == ' ' || (c <= 0xD && c >= 0x9)); }
inline bool isASCIISpace(int c) { return c <= ' ' && (c == ' ' || (c <= 0xD && c >= 0x9)); }

inline char toASCIILower(char c) { return c | ((c >= 'A' && c <= 'Z') << 5); }
inline unsigned short toASCIILower(unsigned short c) { return c | ((c >= 'A' && c <= 'Z') << 5); }
inline int toASCIILower(int c) { return c | ((c >= 'A' && c <= 'Z') << 5); }

inline bool isASCIIDigit(char c) { return (c >= '0') & (c <= '9'); }
inline bool isASCIIDigit(unsigned short c) { return (c >= '0') & (c <= '9'); }
inline bool isASCIIDigit(int c) { return (c >= '0') & (c <= '9'); }

inline bool isASCIIHexDigit(char c) { return (c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'f'); }
inline bool isASCIIHexDigit(unsigned short c) { return (c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'f'); }
inline bool isASCIIHexDigit(int c) { return (c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'f'); }

inline int toASCIIHexValue(char c)
{//FIXME aseert
    ASSERT(isASCIIHexDigit(c));
    return c < 'A' ? c - '0' : (c - 'A' + 10) & 0xF;
}
inline int toASCIIHexValue(unsigned short c)
{//FIXME aseert
    return c < 'A' ? c - '0' : (c - 'A' + 10) & 0xF;
}

inline int toASCIIHexValue(int c)
{ //FIXME aseert ASSERT(isASCIIHexDigit(c));
    return c < 'A' ? c - '0' : (c - 'A' + 10) & 0xF;
}

} // namespace DCanvas

#endif // DMATH_H
