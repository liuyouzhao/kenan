//
// Created by hujia on 19/07/19.
//

#ifndef KENAN_RESOURCECONTROLLER_H
#define KENAN_RESOURCECONTROLLER_H


namespace kenan_platform
{

class ResourceController {
public:

    /// image
    static unsigned int* getImageData(unsigned int id, unsigned int& len);
    static unsigned int getImageWidth(unsigned int id);
    static unsigned int getImageHeight(unsigned int id);

    /// sound
    static int playSound(unsigned int id);
    static int stopSound(unsigned int id);
    static int pauseSound(unsigned int id);
    static int resumeSound(unsigned int id);
    static int setSoundVolume(unsigned int id, float left, float right);
};

}

#endif //KENAN_RESOURCECONTROLLER_H
