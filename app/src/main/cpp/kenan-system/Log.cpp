//
// Created by hujia on 22/06/19.
//

#include "Log.h"
#include "defines.h"

#define LOG_TAG "[kenan]"

namespace kenan_system
{
    void Log::info(const char *tag, const char *content)
    {
        __LOGI(tag, "%s", content);
    }

    void Log::debug(const char *tag, const char *content)
    {
        __LOGD(tag, "%s", content);
    }

    void Log::error(const char *tag, const char *content)
    {
        __LOGE(tag, "%s", content);
    }
}