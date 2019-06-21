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
        kenan_system::Log::error("Assert", "ASSERTION FAILED: %s\n", assertion);
        fprintf(stderr,"ASSERTION FAILED: %s\n", assertion);
    }
    else
    {
        kenan_system::Log::error("Assert", "SHOULD NEVER BE REACHED");
        fprintf(stderr,"SHOULD NEVER BE REACHED\n");
    }

    kenan_system::Log::error("Assert", "(%s:%d %s)\n", file, line, function);
    fprintf(stderr,"(%s:%d %s)\n", file, line, function);
}

} // namespace DCanvas
