#include "defines.h"
#include "Font.h"

#undef  LOG_TAG
#define LOG_TAG "font"

namespace DCanvas
{

DCFontFile g_fontfile[MAX_FONTFILE_LENGTH] =
{
    {(char*)"sans-serif", (char*)"/system/fonts/DroidSans.ttf"},
    {(char*)"monospace",  (char*)"/system/fonts/DroidSansMono.ttf"},
    {(char*)"spirit-time", (char*)"/system/fonts/spirit_time.ttf"},
    {(char*)"sf-arch-rival", (char*)"/system/fonts/SF Arch Rival.ttf"},
    {NULL, NULL}
};

unsigned int* DCFont::getPixelArray(int &w, int &h, int &realW, int &realH,
                                    int& top, int & bot, bool isStroke)
{
    char* fontfile = m_curAttri._font;
    char* text = m_curAttri._text;
    int size = m_curAttri._size;
    int style = m_curAttri._style;
    w = size * strlen(text);
    h = size + FONTMAP_ADDS;
    
    if (text == NULL)
    {
        return NULL;
    }

    /// a fit-size bitmap
    if (m_colorArray)
    {
        delete[] m_colorArray;
        m_colorArray = NULL;
    }
#if HAS_SKIA
    SkPaint paint;
    paint.setARGB(255, 255, 255, 255);
    paint.setTextSize(size);

    /// here only support bold
    if (style >= 1)
        paint.setFakeBoldText(true);
    else
        paint.setFakeBoldText(false);

    if (isStroke)
    {
        paint.setFakeBoldText(true);
        paint.setStyle(SkPaint::kStroke_Style);
        m_curAttri._stroke = STROKE_STYLE;
    }
    else
    {
        paint.setStyle(SkPaint::kFill_Style);
    }

    if (fontfile != NULL)
    {
        paint.setTypeface(m_tf);
    }
    
    SkPaint::FontMetrics metrics;
    SkScalar spacing = paint.getFontMetrics(&metrics);

    h =  metrics.fBottom - metrics.fTop;
    top =  metrics.fAscent - metrics.fTop;
    bot =  metrics.fBottom - metrics.fDescent;
    m_colorArray = new unsigned int[w * h];
    memset(m_colorArray, 0, sizeof(m_colorArray[0]) * w * h);

    SkBitmap bmp;
    bmp.setConfig(SkBitmap::kARGB_8888_Config, w, h);
    bmp.allocPixels();
    bmp.setPixels(m_colorArray);

    /// create a canvas with this bitmap
    SkCanvas canvas(bmp);

	paint.setAntiAlias(true);

	//set filter
	//paint.setFilterBitmap(true);
	
    /// draw text into the bitmap
    canvas.drawText(text, strlen(text), 0, -metrics.fTop, paint);

    /// get the pixels
    unsigned char* pixels = (unsigned char*)bmp.getPixels();

    /// convert the char* to int*
    for (int i = 0, k = 0; i < w * h; ++i)
    {
        unsigned int A = 0x000000ff & (unsigned int)pixels[k++];
        unsigned int B = 0x000000ff & (unsigned int)pixels[k++];
        unsigned int G = 0x000000ff & (unsigned int)pixels[k++];
        unsigned int R = 0x000000ff & (unsigned int)pixels[k++];
        m_colorArray[i] = (unsigned int)((A << 24) | (R << 16) | (G << 8) | B);
    }
    
    realW = w;
    realH = h;
    for (int i = w - 1; i >= 0; --i)
    {
        bool has = false;
        for (int j = 0; j < h; ++j)
        {
            if (m_colorArray[j * w + i] > 0)
            {
                has = true;
                break;

            }
        }
        if (has)
        {
            realW = i + 1;
            break;
        }
    }
#endif
    return m_colorArray;
}

HTEXTURE DCFont::getFromHash(int &w, int &h, int &rw, int &rh,  bool isStroke)
{
    unsigned long key1 = hashString(m_curAttri.getKey(),
                strlen(m_curAttri.getKey()), FIRST_HASH_KEY);
    unsigned long key2 = hashString(m_curAttri.getKey(),
                strlen(m_curAttri.getKey()), SECOND_HASH_KEY);

    //find from hashtable
    for (int i = 0; i < MAX_TEXTURE; ++i)
    {
        if (m_hashTable[i]._id == key1 && m_hashTable[i]._id2 == key2)
        {
            w = m_hashTable[i]._w;
            h = m_hashTable[i]._h;
            rw = m_hashTable[i]._rw;
            rh = m_hashTable[i]._rh;
            return m_hashTable[i]._hTex;
        }
    }
    return NO_TEXTURE;
}

void DCFont::put(HTEXTURE hTex, int w, int h, int rw, int rh, int i)
{
    unsigned long key1 = hashString(m_curAttri.getKey(),
                strlen(m_curAttri.getKey()), FIRST_HASH_KEY);
    unsigned long key2 = hashString(m_curAttri.getKey(),
                strlen(m_curAttri.getKey()), SECOND_HASH_KEY);

    m_hashTable[i]._id = key1;
    m_hashTable[i]._id2 = key2;
    m_hashTable[i]._hTex = hTex;
    m_hashTable[i]._w = w;
    m_hashTable[i]._h = h;
    m_hashTable[i]._rw = rw;
    m_hashTable[i]._rh = rh;
    m_counter ++;
}

void DCFont::release()
{
    m_counter = 0;
    for (int i = 0; i < MAX_TEXTURE; ++i)
    {
        m_hashTable[i]._id = DEFAULT_ID;
        m_hashTable[i]._id2 = DEFAULT_ID;
    }
}

unsigned long DCFont::hashString(char* s, int len, int pram)
{
    unsigned long res = 0;
    for (; *s; ++s)
        res = pram * res + *s;

    return res;
}

void DCFont::ReleaseBro(DCFontNode* node)
{
}

void DCFont::SetStyle(char* font, int size, char* text, int s)
{
    m_curAttri._id = 0;
    m_curAttri._font = font;
    m_curAttri._size = size;
    m_curAttri._text = text;
    m_curAttri._style = s;
}


void DCFont::SetStyle(char* fontString)
{
#if HAS_SKIA
    std::string s(fontString);
    if (s.find(" bold") != std::string::npos || s.find("bold ") != std::string::npos)
    {
        //is bold
        m_curAttri._style |= 1;
    }
    if (s.find(" italic") != std::string::npos || s.find("italic ") != std::string::npos)
    {
        //is italic
        m_curAttri._style |= 2;
    }
    
    /// (2) check font
    for (int i = 0; i < MAX_FONTFILE_LENGTH; ++i)
    {
        if (g_fontfile[i].fontname == NULL)
            break;

        if (s.find(g_fontfile[i].fontname) != std::string::npos)
        {
            m_curAttri._font = g_fontfile[i].filepath;
//            if (m_tf)    delete m_tf;
			if (m_tf) m_tf->unref();
            m_tf = NULL;
            m_tf = SkTypeface::CreateFromFile(m_curAttri._font);
            break;
        }
    }
    
    if (s.find("px") == std::string::npos)
    {
        m_curAttri._size = 20;
        return;
    }
    
    /// (3) check size
    char* ptr = fontString;
    char px[20] = {0};
    char px2[20] = {0};
    bool hasNum = false;

    while (ptr != '\0')
    {
        if (*ptr == 'p' && *(ptr + 1) == 'x')
        {
            char* pn = ptr - 1;
            int i = 0;
            int j = 0;
            while (pn >= fontString && (*pn) <= '9' && (*pn) >= '0')
            {
                hasNum = true;
                px[i ++] = *pn--;
            }
            i --;
            while (i >= 0)
            {
                px2[j ++] = px[i --];
            }
            break;
        }
        ptr ++;
    }
    int size = 20;

    if (hasNum)
        sscanf(px2, "%d", &size);

    m_curAttri._size = size;
#endif
}

} // namespace DCanvas

