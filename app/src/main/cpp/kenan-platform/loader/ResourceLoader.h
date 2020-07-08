//
// Created by hujia on 19/07/19.
//

#ifndef KENAN_RESOURCELOADER_H
#define KENAN_RESOURCELOADER_H

#include <string>

namespace kenan_platform
{

class ResourceLoader {
public:
    /// load resource & release resource
    static std::string loadImageResource(std::string filePath);
    static std::string loadSoundResource(std::string filePath);
    static int releaseResource(std::string id);
};

}

#endif //KENAN_RESOURCELOADER_H
