#ifndef FONT_H
#define FONT_H

#include <android/log.h>

#if HAS_SKIA
#include "SkTypeface.h"
#endif
#include "gl2d_impl.h"

#define FIRST_HASH_KEY 131
#define SECOND_HASH_KEY 4111
#define DEFAULT_ID -999

namespace DCanvas
{

#define MAX_TEXT_LENGTH 256
#define MAX_KEY_LENGTH 1242
#define MAX_FONTFILE_LENGTH 128
#define MAX_FILENAME_LENGTH 128
#define FONTMAP_ADDS 4
#define MAX_TEXTURE 60
#define STROKE_STYLE 9999

struct DCFontFile
{
    char* fontname;
    char* filepath;
};

extern DCFontFile g_fontfile[MAX_FONTFILE_LENGTH];

enum BASELINETYPE
{
    TTOP = 0,
    TMIDDLE = 1,
    TBOTTOM = 2,
    TNONE = 3
};

enum TextAlign
{
    TLEFT   = 0,
    TCENTER = 1,
    TRIGHT  = 2,
    TSTART  = 3,
    TEND    = 4,
};

class DCFont
{

public:
    ~DCFont()
    {
        if (m_colorArray)
            delete[] m_colorArray;

        m_colorArray = NULL;
//        if (m_tf)
//            delete m_tf;
		if (m_tf)
			m_tf->unref();
        m_tf = NULL;
    }

    struct DCFontAttribute
    {
        int    _id;
        char*  _font;
        int    _size;
        char*  _text;
        int    _style;
        int    _stroke;
        char   _key[MAX_KEY_LENGTH];

        char* getKey()
        {
            if (_font != NULL)
                snprintf(_key, MAX_KEY_LENGTH, "%s,%s,%d,%d,%d", _font, _text, _size, _style,_stroke);
            else
                snprintf(_key, MAX_KEY_LENGTH, "%s,%d,%d,%d", _text, _size, _style,_stroke);
            return _key;
        }

        DCFontAttribute()
        {
            _id = 0;
            _font = NULL; _size = 0; _text = NULL; _style = 0;
            _stroke = 0;
        }
    };

    struct DCFontNode
    {
        unsigned long    _id;
        unsigned long    _id2;
        char*            _key;
        unsigned long    _hash;
        HTEXTURE         _hTex;
        int              _w;
        int              _h;
        int              _rw;
        int              _rh;
        
        //DCFontNode*    _brother;
        DCFontNode()
        {
            _id = DEFAULT_ID;
            _hTex = NO_TEXTURE;
            //_brother = NULL;
            _w = 0;
            _h = 0;
            _rw = _rh = 0;
            _key = (char*)"";
        }
    };

    /* Create the pointer    */
    static DCFont* create(unsigned long sTable) { return new DCFont(sTable); }

    /* Get the pixel array*/
    unsigned int* getPixelArray(int &w, int &h, int &realW, int &realH,
                                int& top, int& bot, bool isStroke = false);

    /* Put into hash table */
    void put(HTEXTURE hTex, int w, int h, int rw, int rh, int i);

    /* Need to Delete */
    bool isFull() { return m_counter >= MAX_TEXTURE; }

    /* Get the hTex from hash Table
     * if there is no, return -1
     */
    HTEXTURE getFromHash(int &w, int &h, int &rw, int &rh, bool isStroke = false);

    /* Release */
    void release();

    /* Set/Get font string*/
    void setFontString(char* font) { m_fontString = font; SetStyle(font); }
    char* getFontString() { return m_fontString; }

    void setBaseLine(char* baseline)
    {
        if (!strcmp(baseline, "bottom")) m_baseline = TBOTTOM;
        if (!strcmp(baseline, "top"))    m_baseline = TTOP;
        if (!strcmp(baseline, "middle")) m_baseline = TMIDDLE;
    }
    int getBaseLine() { return m_baseline; }

    void setAlign(TextAlign align)
    {
        m_align = align;
    }
    
    TextAlign getAlign() { return m_align; }
    
    /*Set Text*/
    void setText(char* text) { m_curAttri._text = text; }
    void setStroke(bool isStroke)
    {
        if (isStroke)
            m_curAttri._stroke = STROKE_STYLE;
        else
            m_curAttri._stroke = 0;
    }
    
protected:
    /* Caluculate the hash value */
    unsigned long hashString(char* s, int len, int pram);

    void ReleaseBro(DCFontNode* node);

    /* Set Styles    */
    //void SetStyle(DCFontAttribute* dcFontAttri) { m_curAttri = dcFontAttri; }
    void SetStyle(char* font, int size, char* text, int s);
    void SetStyle(char* fontString);

    /* Set Color     */
    void SetColor(int R, int G, int B, int A)
    {
        this->R = R;
        this->G = G;
        this->B = B;
        this->A = A;
    }

private:
    DCFont(int sTable)
    {
        m_sTable = sTable;
        R=G=B=0; A=255;
        m_fontString = (char*)"20px sans-serif";
        m_baseline = TBOTTOM;
        m_align = TLEFT;
        m_hTexNeedDel = NO_TEXTURE;
        m_del = 0;
        m_add = 0;
        m_counter = 0;

        m_colorArray = NULL;

        m_tf = SkTypeface::CreateFromFile(g_fontfile[0].filepath);

        SetStyle(m_fontString);
    }

    /// the table size
    unsigned long    m_sTable;

    /// the font table
    DCFontNode       m_hashTable[MAX_TEXTURE];

    /// the current attribute
    DCFontAttribute  m_curAttri;

    /// the font string
    char*            m_fontString;

    /// text base line
    BASELINETYPE     m_baseline;

    /// text align
    TextAlign        m_align;

    /// counter
    int              m_counter;
    int              m_del;
    int              m_add;

    /// the hTex need to delete before put
    HTEXTURE         m_hTexNeedDel;

    /// the colors
    int              R, G, B, A;

    /// create a canvas with this bitmap
    unsigned int*    m_colorArray;

    SkTypeface*      m_tf;
    SkTypeface*      m_ttf;
};
}// namespace DCanvas

#endif // FONT_H

