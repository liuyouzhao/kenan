//
// Created by hujia on 27/06/19.
//

#ifndef KENAN_PLATFORMCONFIG_H
#define KENAN_PLATFORMCONFIG_H

#include <string>
#if IN_ANDROID
#include <jni.h>
#elif IN_LINUX
#elif IN_WINDOWS
#endif

class PlatformConfig {
public:

    static PlatformConfig* instance()
    {
        if(s_self == NULL)
        {
            s_self = new PlatformConfig();
        }
        return s_self;
    }

    void setDataDir(std::string dir)
    {
        m_dataDir = dir;
    }

    const char *getDataDir()
    {
        return m_dataDir.c_str();
    }

#if IN_ANDROID
    void setPlatformEnv(JNIEnv *env)
    {
        this->m_env = env;
    }
    JNIEnv* env() { return m_env; }
#elif IN_LINUX
#elif IN_WINDOWS
#endif

private:
    static PlatformConfig *s_self;
    std::string m_dataDir;

#if IN_ANDROID
    JNIEnv* m_env;
#elif IN_LINUX
#elif IN_WINDOWS
#endif
};



#endif //KENAN_PLATFORMCONFIG_H
