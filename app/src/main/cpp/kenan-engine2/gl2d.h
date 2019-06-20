/*
** gl's 2d Engine 1.00
** Copyright (C) 2011-2014, aliyun cloud apps
**
** System layer API
*/
#ifndef GL2D_H
#define GL2D_H

#ifdef __WINx86__
#ifndef IN_WINx86
#define IN_WINx86   1
#endif
#else
#ifndef IN_WINx86
#define IN_WINx86   0
#endif
#endif // __WINx86__

#ifdef __LINUXx86__
#ifndef IN_LINUXx86
#define IN_LINUXx86  1
#endif
#else
#ifndef IN_LINUXx86
#define IN_LINUXx86  0
#endif
#endif // __LINUXx86__

#ifdef __ANDROID__
#ifndef IN_ANDROID
#define IN_ANDROID   1
#endif
#else
#ifndef IN_ANDROID
#define IN_ANDROID   0
#endif
#endif // __ANDROID__

#if IN_WINx86

#include <windows.h>

#elif IN_LINUXx86

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#elif IN_ANDROID

#endif // IN_WINx86

#define GL2D_VERSION 0x001

#define CALL

/*
** For loop
*/
#define LOOP(i,a) for(unsigned long i=0;i<a;++i)

#include <math.h>
/*
** Common data types
*/
#ifndef DWORD
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif

/*
** gl2d Handle types
*/
typedef DWORD HTEXTURE;
typedef DWORD HTARGET;
typedef DWORD HEFFECT;
typedef DWORD HSHADER;

/*
** Hardware color macros
*/
#define ARGB(a,r,g,b)    ((DWORD(a)<<24) + (DWORD(r)<<16) + (DWORD(g)<<8) + DWORD(b))
#define GETA(col)        ((col)>>24)
#define GETR(col)        (((col)>>16) & 0xFF)
#define GETG(col)        (((col)>>8) & 0xFF)
#define GETB(col)        ((col) & 0xFF)
#define SETA(col,a)        (((col) & 0x00FFFFFF) + (DWORD(a)<<24))
#define SETR(col,r)        (((col) & 0xFF00FFFF) + (DWORD(r)<<16))
#define SETG(col,g)        (((col) & 0xFFFF00FF) + (DWORD(g)<<8))
#define SETB(col,b)        (((col) & 0xFFFFFF00) + DWORD(b))

/*
** gl2d Blending constants
*/
#define    BLEND_COLORADD        1
#define    BLEND_COLORMUL        0
#define    BLEND_ALPHABLEND      2
#define    BLEND_ALPHAADD        0
#define    BLEND_ZWRITE          4
#define    BLEND_NOZWRITE        0

#define    BLEND_DEFAULT         1

/*
** gl2d System state constants
*/
enum gl2dBoolState
{
    GL2D_WINDOWED        = 11,   // bool        run in window?          (default: false)
    GL2D_ZBUFFER         = 12,   // bool        use z-buffer?           (default: false)
    GL2D_TEXTUREFILTER   = 13,   // bool        texture filtering?      (default: true)

    GL2D_USESOUND        = 18,   // bool        use BASS for sound?     (default: true)

    GL2D_DONTSUSPEND     = 24,   // bool        focus lost:suspend?     (default: false)
    GL2D_HIDEMOUSE       = 25,   // bool        hide system cursor?     (default: true)

    GL2DBOOLSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum gl2dFuncState
{
    GL2D_FRAMEFUNC        = 1,   // bool*()     frame function          (default: NULL) (you MUST set this)
    GL2D_RENDERFUNC       = 2,   // bool*()     render function         (default: NULL)
    GL2D_FOCUSLOSTFUNC    = 3,   // bool*()     focus lost function     (default: NULL)
    GL2D_FOCUSGAINFUNC    = 4,   // bool*()     focus gain function     (default: NULL)
    GL2D_EXITFUNC         = 5,   // bool*()     exit function           (default: NULL)

    GL2DFUNCSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum gl2dIntState
{
    GL2D_SCREENWIDTH      = 8,    // int        screen width            (default: 800)
    GL2D_SCREENHEIGHT     = 9,    // int        screen height           (default: 600)
    GL2D_SCREENBPP        = 10,   // int        screen bitdepth         (default: 32) (desktop bpp in windowed mode)

    GL2D_SAMPLERATE       = 19,   // int        sample rate             (default: 44100)
    GL2D_FXVOLUME         = 20,   // int        global fx volume        (default: 100)
    GL2D_MUSVOLUME        = 21,   // int        global music volume     (default: 100)

    GL2D_FPS              = 23,   // int        fixed fps               (default: gl2dFPS_UNLIMITED)

    GL2DINTSTATE_FORCE_DWORD = 0x7FFFFFF
};

enum gl2dStringState
{
    GL2D_ICON             = 6,    // char*      icon resource           (default: NULL)
    GL2D_TITLE            = 7,    // char*      window title            (default: "gl2d")

    GL2D_INIFILE          = 15,   // char*      ini file                (default: NULL) (meaning no file)
    GL2D_LOGFILE          = 16,   // char*      log file                (default: NULL) (meaning no file)

    GL2DSTRINGSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum gl2EffectState
{
    GL2D_BLUR             = 17,   // blur effect
    GL2D_DARK             = 18,   // dark effect
    GL2D_BI               = 19,   // change to white black map
    GL2D_NORMAL_NOTEXTURE = 20,
    GL2D_NORMAL           = 21
};

/*
** Callback protoype used by gl2d
*/
typedef bool (*gl2dCallback)();

/*
** gl2d_FPS system state special constants
*/
#define GL2DFPS_UNLIMITED    0
#define GL2DFPS_VSYNC        -1

/*
** gl2d Primitive type constants
*/
#define GL2DPRIM_LINES       2
#define GL2DPRIM_TRIPLES     3
#define GL2DPRIM_QUADS       4

/*
** gl2d Vertex structure
*/
struct gl2dVertex
{
    float            x, y;        // screen position
    float            z;            // Z-buffer depth 0..1
    float            r, g, b, a;    // color
    float            tx, ty;        // texture coordinates
};

/*
** gl2d Triple structure
*/
struct gl2dTriple
{
    gl2dVertex        v[3];
    HTEXTURE          tex;
    int               blend;
};

/*
** gl2d Quad structure
*/
struct gl2dQuad
{
    gl2dVertex        v[4];
    HTEXTURE          tex;
    int               blend;
};

/*
** gl2d Rect structure
*/
struct gl2dRect
{
    float            left;
    float            top;
    float            right;
    float            bottom;

    void Set(float left, float top, float right, float bottom)
    {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }
};

/*
** gl2d Color structure
*/
struct gl2dColor
{
    float            r;
    float            g;
    float            b;
    float            a;
    gl2dColor()
    {}
    gl2dColor(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

/*
** gl2d Input Event type constants
*/
#define INPUT_KEYDOWN        1
#define INPUT_KEYUP          2
#define INPUT_MBUTTONDOWN    3
#define INPUT_MBUTTONUP      4
#define INPUT_MOUSEMOVE      5
#define INPUT_MOUSEWHEEL     6

/*
** gl2d Input Event flags
*/
#define GL2DINP_SHIFT        1
#define GL2DINP_CTRL         2
#define GL2DINP_ALT          4
#define GL2DINP_CAPSLOCK     8
#define GL2DINP_SCROLLLOCK   16
#define GL2DINP_NUMLOCK      32
#define GL2DINP_REPEAT       64

/*
** gl2d Interface class
*/
class Gl2d
{
public:
    virtual ~Gl2d() {};
    virtual void        CALL    Release() = 0;

#if IN_WINx86
    virtual bool        CALL    System_Initiate(HWND window, HDC H, int width = 800, int height = 600) = 0;
#elif IN_LINUXx86
    virtual bool        CALL    System_Initiate(Window x11Window, Display* x11Display, int width = 800, int height = 600) = 0;
#elif IN_ANDROID
    virtual bool        CALL    System_Initiate(int x = 0, int y = 0, int width = 800, int height = 600, int layer = 0x40000000) = 0;
  //virtual bool        CALL    System_Initiate(int width = 800, int height = 600) = 0;
#endif // IN_WINx86 

    virtual void        CALL    System_Shutdown() = 0;
    virtual char*       CALL    System_GetErrorMessage() = 0;
    
    virtual HEFFECT     CALL    Effect_Load(gl2EffectState shader)    = 0;
    virtual void        CALL    Effect_Free(HEFFECT eff) = 0;
    virtual void        CALL    Effect_Free_All() = 0;
    virtual void        CALL    Effect_Active(HEFFECT eff, bool hasTex = false) = 0;

    virtual void        CALL    Gfx_SetClearColor(float r, float g, float b, float a) = 0;
    virtual void        CALL    Gfx_SetLineWidth(float width) = 0;
    virtual void        CALL    Gfx_Clear() = 0;
    virtual void        CALL    Gfx_RenderLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float z=0.5f) = 0;
    virtual void        CALL    Gfx_RenderTriple(const gl2dTriple *triple) = 0;
    virtual void        CALL    Gfx_RenderQuad(const gl2dQuad *quad) = 0;

    virtual void        CALL    Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0) = 0;
    virtual void        CALL    Gfx_RemoveClipping() = 0;

#if !IN_ANDROID
    virtual void        CALL    Gfx_FlushRenders() = 0;
#endif
    virtual void        CALL    Gfx_SwapBuffer() = 0;

    virtual HTARGET     CALL    Target_Create(int width, int height) = 0;
    virtual void        CALL    Target_Free(HTARGET target) = 0;
    virtual void        CALL    Target_FreeAll() = 0;
    virtual HTEXTURE    CALL    Target_GetTexture(HTARGET target) = 0;
    virtual void        CALL    Target_Active(HTARGET target, int width, int height) = 0;
    virtual void        CALL    Target_ResetToScreen() = 0;

    virtual HTEXTURE    CALL    Texture_Create(int width, int height) = 0;
    virtual HTEXTURE    CALL    Texture_Load(const  unsigned int *data, int width, int height) = 0;
    virtual void        CALL    Texture_Free(HTEXTURE tex) = 0;
};

#endif // GL2D_H