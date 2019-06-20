#include "defines.h"
#include "ImageData.h"
#include "Color.h"
#include "gl2d_impl.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "ImageData"

namespace DCanvas
{
ImageData::ImageData(int w, int h)
{

    m_context = NULL;
    m_data = PixelArray::create(w * h * 4);
    for (int i = 0; i < w * h * 4; i += 4)
    {
        m_data->set(i,   (unsigned char)0);
        m_data->set(i + 1, (unsigned char)0);
        m_data->set(i + 2, (unsigned char)0);
        m_data->set(i + 3, (unsigned char)0);
    }
    init(w, h);
    //MemoryManager::getManager()->AddImageData(this);
}

ImageData::ImageData(int x, int y, int w, int h, Gl2d_Impl* con)
{
    m_context = con;
    m_data = PixelArray::create(w * h * 4);

    unsigned int *pix = new unsigned int[w * h];
    memset(pix, 0, sizeof(pix));
    if (m_context)
    {
        m_context->Gfx_SwapBuffer();
        m_context->Target_ReadPixels(x, 800-y-h, w , h, pix);
    }
    int k1 = (h-1) * w;
    int k2 = 0;
    for (int i = 0;i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            m_data->set(4 * (k2 + j),     (unsigned char)(pix[(k1 + j)] & 0x000000ff));
            m_data->set(4 * (k2 + j) + 1, (unsigned char)((pix[(k1 + j)]& 0x0000ff00)>>8));
            m_data->set(4 * (k2 + j) + 2, (unsigned char)((pix[(k1 + j)]& 0x00ff0000)>>16));
            m_data->set(4 * (k2 + j) + 3, (unsigned char)((pix[(k1 + j)]& 0xff000000)>>24));
        }
        k1 -= w;
        k2 += w;
    }
    delete[] pix;

    init(w, h);
    //MemoryManager::getManager()->AddImageData(this);
}

void ImageData::init(int w, int h)
{
    m_width = w;
    m_height = h;
    m_tex = NO_TEXTURE;
    m_gl_data = NULL;
    m_hasException = false;
}

void ImageData::setException()
{
    m_hasException = true;
}

ImageData::~ImageData()
{
    if (NO_TEXTURE != m_tex)
        m_context->Texture_Free(m_tex);

    if (m_gl_data)
    {
        delete[] m_gl_data;
        m_gl_data = NULL;
    }

    if (m_data)
    {
        delete m_data;
        m_data = NULL;
    }

    //MemoryManager::getManager()->RemoveImageData(this);
}

HTEXTURE ImageData::GetTexture()
{
    return m_tex;
}

unsigned int ImageData::GetSize()
{
    return m_width * m_height * 4;
}

void ImageData::SetTexture(HTEXTURE ht , Gl2d_Impl* gc)
{
    if (NO_TEXTURE != m_tex)
    {
        gc->Texture_Free(m_tex);
    }

    m_tex = ht;
    m_context = gc;
}

unsigned int* ImageData::Data()
{
    unsigned int r = 0, g = 0, b = 0, a = 0;

    if (NULL == m_gl_data)
    {
        m_gl_data = new unsigned int[m_width * m_height];
        for (int i = 0; i < m_width * m_height; ++i)
        {
            r = 0x000000ff & m_data->get(4 * i);
            g = 0x000000ff & m_data->get(4 * i + 1);
            b = 0x000000ff & m_data->get(4 * i + 2);
            a = 0x000000ff & m_data->get(4 * i + 3);
            m_gl_data[i] =  r | (g << 8) | (b << 16) | (a << 24);
        }
    }
    return m_gl_data;
}
} // namespace DCanvas
