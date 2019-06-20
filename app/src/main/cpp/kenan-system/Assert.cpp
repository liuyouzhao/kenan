#include "defines.h"
#include "Assert.h"
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "ASSERT"

namespace DCanvas
{

void ReportAssertionFailure(const char* file, int line, const char* function, const char* assertion)
{
    if (assertion)
    {
        LOGE("ASSERTION FAILED: %s\n", assertion);
        fprintf(stderr,"ASSERTION FAILED: %s\n", assertion);
    }
    else
    {
        LOGE("SHOULD NEVER BE REACHED");
        fprintf(stderr,"SHOULD NEVER BE REACHED\n");
    }

    LOGE("(%s:%d %s)\n", file, line, function);
    fprintf(stderr,"(%s:%d %s)\n", file, line, function);
}

} // namespace DCanvas
