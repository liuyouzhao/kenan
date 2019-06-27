//
// Created by hujia on 27/06/19.
//

#ifndef KENAN_PLATFORMCONFIG_H
#define KENAN_PLATFORMCONFIG_H

#include <string>

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

private:

    static PlatformConfig *s_self;

    std::string m_dataDir;
};



#endif //KENAN_PLATFORMCONFIG_H
