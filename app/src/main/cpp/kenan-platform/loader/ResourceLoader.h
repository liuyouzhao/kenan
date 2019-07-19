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
    static unsigned int loadImageResource(std::string filePath);
    static unsigned int loadSoundResource(std::string filePath);
    static unsigned int releaseResource(unsigned int id);
};

}

#endif //KENAN_RESOURCELOADER_H
