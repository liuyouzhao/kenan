#include "defines.h"
#include "Assert.h"
#include "Log.h"
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "ASSERT"

namespace kenan_system
{

void ReportAssertionFailure(const char* file, int line, const char* function, const char* assertion)
{
    if (assertion)
    {
        __LOGE(TAG_ASSERT, "ASSERTION FAILED: %s\n", assertion);
        fprintf(stderr,"ASSERTION FAILED: %s\n", assertion);
    }
    else
    {
        __LOGE(TAG_ASSERT, "SHOULD NEVER BE REACHED");
        fprintf(stderr,"SHOULD NEVER BE REACHED\n");
    }

    __LOGE(TAG_ASSERT, "(%s:%d %s)\n", file, line, function);
    fprintf(stderr,"(%s:%d %s)\n", file, line, function);
}

} // namespace DCanvas
