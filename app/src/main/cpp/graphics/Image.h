#ifndef IMAGE_H
#define IMAGE_H

#include <v8.h>
#include "ObjectWrap.h"
#include <gl2d_impl.h>
#include <string>

using namespace v8;

namespace DCanvas
{

class Image
{
friend class V8Image;
public:
    Image();
    ~Image();

    void updata(int length);
    void onLoad();
    void setData(void* data, int size);
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
    void setSelf(v8::Persistent<v8::Object> self) { m_js_self = self; }


private:
    HTEXTURE                m_hTex;
    unsigned int *          m_data;
    char *                  m_src;
    int                     m_width;
    int                     m_height;
    Gl2d_Impl *             m_context;
    bool                    m_onLoadSeted;
    int                     m_canvasId;

    v8::Persistent<v8::Function>                        m_js_OnLoad;
    v8::Persistent<v8::Object>                          m_js_self;
};

} // namepace DCanvas

#endif  // IMAGE_H
