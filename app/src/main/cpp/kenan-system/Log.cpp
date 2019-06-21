//
// Created by hujia on 22/06/19.
//

#include "Log.h"
#include "defines.h"

namespace kenan_system
{
    void Log::info(const char *tag, const char *format,...)
    {
        va_list args;
        va_start(args, format);
        __LOGI(tag, format, args);
        va_end(args);
    }

    void Log::debug(const char *tag, const char *format,...)
    {
        va_list args;
        va_start(args, format);
        __LOGD(tag, format, args);
        va_end(args);
    }
    void Log::error(const char *tag, const char *format,...)
    {
        va_list args;
        va_start(args, format);
        __LOGE(tag, format, args);
        va_end(args);
    }
}