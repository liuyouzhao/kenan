#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <v8.h>
#include "ObjectWrap.h"
#include "PixelArray.h"
#include <gl2d_impl.h>

namespace DCanvas
{

class ImageData
{
friend class V8ImageData;
public:
    ImageData(int width, int height);
    ImageData(int x, int y, int w, int h, Gl2d_Impl* con);
    ~ImageData();
    PixelArray* get_data() { return m_data; }
    unsigned int* Data();
    HTEXTURE GetTexture();
    void SetTexture(HTEXTURE ht, Gl2d_Impl* gc);

    void init(int w, int h);
    int Width() { return m_width; }
    int Height() { return m_height; }
    unsigned int GetSize();
    void setException();

private:
    int m_width;
    int m_height;
    int m_x;
    int m_y;

    v8::Persistent<v8::Object> m_js_self;
    void SetSelf(v8::Persistent<v8::Object> self) { m_js_self = self; }

    PixelArray*                 m_data;
    HTEXTURE                    m_tex;
    Gl2d_Impl *                 m_context;
    unsigned int*               m_gl_data;
    bool                        m_hasException;
};

} // namespace DCanvas

#endif // IMAGEDATA_H
