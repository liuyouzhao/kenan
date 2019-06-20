#ifndef ASSERT_H
#define ASSERT_H

namespace DCanvas
{
#if defined(__GNUC__)
#define WTF_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#define WTF_PRETTY_FUNCTION __FUNCTION__
#endif

#define ASSERT(assertion) do \
    if (!(assertion)) { \
        ReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion); \
        CRASH(); \
    } \
while (0)

void ReportAssertionFailure(const char* file, int line, const char* function, const char* assertion);

#define CRASH() do { \
    *(int *)(uintptr_t)0xbbadbeef = 0; \
    ((void(*)())0)(); /* More reliable, but doesn't say BBADBEEF */ \
} while(false)

#define ASSERT_NOT_REACHED() do { \
    ReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, 0); \
    CRASH(); \
} while (0)

} // namespace DCanvas

#endif // ASSERT_H

