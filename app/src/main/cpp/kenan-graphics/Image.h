#ifndef IMAGE_H
#define IMAGE_H

#include <gl2d_impl.h>
#include <string>
#include "ReturnStatus.h"

namespace kenan_graphics
{

class Image
{
public:
    Image();
    ~Image();
    ReturnStatus load(std::string src);
    void onLoad();
    void setData(void* data, int size);
    void setPixels(int *pixels, int w, int h);
    void setSrc(std::string src);
    std::string getSrc()	{	return std::string(m_src); }
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    int getCanvasId() { return m_canvasId; }
    char* logSrc() { return m_src; }
    HTEXTURE getTexture();
    void setTexture(HTEXTURE ht, Gl2d_Impl* gc, int canvasId);
    void freeTexture();
    unsigned int* getImageData() { return m_data; }

    inline void setOnLoadCallback(void (*cb)(void*)) {  onLoadCallback = cb;    }
    inline bool hasOnLoadFunc() {    return m_isOnloadFuncExist; }
    inline void setOnLoadFuncExist(bool yesOrNo) {   m_isOnloadFuncExist = yesOrNo;  }
private:
    void doLoadImageData();
    void convertTexture();
    HTEXTURE                m_hTex;
    unsigned int *          m_data;
    char *                  m_src;
    int                     m_width;
    int                     m_height;
    Gl2d_Impl *             m_context;
    bool                    m_isOnloadFuncExist;
    int                     m_canvasId;

    void (*onLoadCallback)(void *key);
};

} // namepace DCanvas

#endif  // IMAGE_H
