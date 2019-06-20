/*
** gl's 2d Engine 1.00
** Copyright (C) 2011-2014, aliyun cloud apps
**
** Common core implementation header
*/
#ifndef gl2d_IMPL_H
#define gl2d_IMPL_H

#include <stdio.h>
#include "gl2d.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "effect.h"
#include "AliTransformationMatrix.h"

#if IN_ANDROID
#if NATIVE_SURFACE
#include <stdlib.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <surfaceflinger/ISurface.h>
#include <surfaceflinger/SurfaceComposerClient.h>
#endif
#endif

#include "aligl.h"

#define VERTEX_BUFFER_SIZE       4000

#define NO_TEXTURE               0x7FFFFFFF
#define NO_TARGET                0x7FFFFFFF

#define MAX_FBOS                 22
/*
** gl2d primitive type constants
*/
#define GL2D_PRIM_LINES          2
#define GL2D_PRIM_TRIPLES        3
#define GL2D_PRIM_QUADS          4
#define GL2D_PRIM_POLYGON        5

#define GL2D_SIZE_POINT2D        3
#define GL2D_SIZE_TEXCOORD       2
#define GL2D_SIZE_COLOR          4
#define GL2D_SIZE_INDICS_QUAD    6

#define GL2D_MATRIX_ARRAY_LENGTH 6

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY             0
#define TEXCOORD_ARRAY           1
#define COLOR_ARRAY              2

#define MAX_EFFECTS_NUMBER       100
using namespace DCanvas;
/*
** gl2d Interface implementation
*/
class Gl2d_Impl : public Gl2d
#if NATIVE_SURFACE
, public aligl
#endif
{
public:
    Gl2d_Impl();
    virtual void        CALL    Release();

#if IN_WINx86
    virtual bool        CALL    System_Initiate(HWND window, HDC H,
                                                int width = 800, int height = 600);
#elif IN_LINUXx86
    virtual bool        CALL    System_Initiate(Window x11Window, Display* x11Display,
                                                int width = 800, int height = 600);
#elif IN_ANDROID
    virtual bool        CALL    System_Initiate(int x = 0, int y = 0,
                                                int width = 800, int height = 600,
                                                int layer = 0x40000000);
#if NATIVE_SURFACE
    virtual bool        CALL    System_InitiateNative(int x = 0, int y = 0,
                                                int width = 800, int height = 600,
                                                int layer = 0x40000000);
#endif
        //virtual bool            CALL    System_Initiate(int width = 800, int height = 600);
#endif
    virtual void        CALL    System_Shutdown();
    virtual void        CALL    System_ChangeSize(int x, int y, int width, int height);
    virtual char*       CALL    System_GetErrorMessage();

    virtual HEFFECT     CALL    Effect_Load(gl2EffectState shader);
    virtual void        CALL    Effect_Free(HEFFECT eff);
    virtual void        CALL    Effect_Free_All();
    virtual void        CALL    Effect_Active(HEFFECT eff, bool hasTex = false);
    virtual void        CALL    Effect_SetFloat(HEFFECT eff, float value, const char* name);
    virtual void        CALL    Effect_SetFloatv2(HEFFECT eff, float x, float y, const char* name);
    virtual void        CALL    Effect_SetInt(HEFFECT eff, int value, const char* name);

    virtual void        CALL    Gfx_SetClearColor(float r, float g, float b, float a);
    virtual void        CALL    Gfx_SetLineWidth(float width);
    virtual void        CALL    Gfx_Clear();

    virtual void        CALL    Gfx_RenderLine(float x1, float y1, float x2, float y2,
                                               float r, float g, float b, float a, float z=0.5f);
    virtual void        CALL    Gfx_RenderTriple(const gl2dTriple *triple);
    virtual void        CALL    Gfx_RenderQuad(const gl2dQuad *quad);

    virtual void        CALL    Gfx_RenderLineEx(float x1, float y1, float x2, float y2,
                                                 int r, int g, int b, int a);
    virtual void        CALL    Gfx_RenderTripleEx(float x1, float y1, float x2, float y2,
                                                   float x3, float y3, int r, int g, int b, int a,
                                                   int blend, HTEXTURE htex = NO_TEXTURE );
    virtual void        CALL    Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                                 float x3, float y3, float x4, float y4,
                                                 int r, int g, int b, int a, int blend,
                                                 HTEXTURE htex = NO_TEXTURE);
    virtual void        CALL    Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                                 float x3, float y3, float x4, float y4,
                                                 float tx[4], float ty[4],
                                                 int r[4], int g[4], int b[4], int a[4],
                                                 int blend, HTEXTURE htex);
    virtual void        CALL    Gfx_RenderRectEx(float left, float top, float right, float bottom,
                                                 int r, int g, int b, int a, int blend,
                                                 HTEXTURE htex = NO_TEXTURE );
    virtual void        CALL    Gfx_RenderRectExTexture(float left, float top, float right, float bottom,
                                                        float TL, float TT, float TR, float TB,
                                                        int r, int g, int b, int a, int blend,
                                                        HTEXTURE htex );
    virtual void        CALL    Gfx_SetMatrix(float* matrix);

    virtual void        CALL    Gfx_RenderRoundEx(float x0, float y0, float r0,
                                                  int r, int g, int b, int a, int blend);
    virtual void        CALL    Gfx_RenderTorusEx(float x0, float y0, float r0,
                                                  float x1, float y1, float r1,
                                                  int r[2], int g[2], int b[2], int a[2], int blend);

    virtual void        CALL    Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0);
    virtual void        CALL    Gfx_Scissor(int x=0, int y=0, int w=0, int h=0);
    virtual void        CALL    Gfx_CloseScissor();
    virtual void        CALL    Gfx_BeginSetClipPath();
    virtual void        CALL    Gfx_ClipPath();
    virtual void        CALL    Gfx_EndClipPath();
    virtual void        CALL    Gfx_BackClipping();
    virtual void        CALL    Gfx_RemoveClipping();
#if !IN_ANDROID
    virtual void        CALL    Gfx_FlushRenders();
#endif
    virtual void        CALL    Gfx_SwapBuffer();
    virtual void        CALL    Gfx_SwapBufferToScreen();
    virtual HTARGET     CALL    Target_Create(int width, int height);
    virtual void        CALL    Target_Free(HTARGET target);
    virtual void        CALL    Target_FreeAll();
    virtual HTEXTURE    CALL    Target_GetTexture(HTARGET target);
    virtual void        CALL    Target_Active(HTARGET target, int width, int height);
    virtual void        CALL    Target_ResetToScreen();
    virtual void        CALL    Target_ReadPixels(int x, int y, int width, int height, GLuint* pPixelData );

    virtual HTEXTURE    CALL    Texture_Create(int width, int height);
    virtual HTEXTURE    CALL    Texture_Create();
    virtual HTEXTURE    CALL    Texture_Load(const unsigned int *data, int width, int height);
    virtual void        CALL    Texture_Free(HTEXTURE tex);
    virtual void        CALL    Texture_SaveToFile();

    virtual void        CALL    Transform_rotate(float d);
    virtual void        CALL    Transform_rotate3d(float rx, float ry, float rz);
    virtual void        CALL    Transform_rotate3d(float x, float y, float z, float angle);
    virtual void        CALL    Transform_rotateFromVector(float x, float y);

    virtual void        CALL    Transform_translate(float tx, float ty);
    virtual void        CALL    Transform_translate3d(float tx, float ty, float tz);

    virtual void        CALL    Transform_scale(float s);
    virtual void        CALL    Transform_scaleNonUniform(float sx, float sy);
    virtual void        CALL    Transform_scale3d(float sx, float sy, float sz);

    virtual void        CALL    Transform_reset(){mTransformMatrix.makeIdentity();}
    virtual void        CALL    Transform_enable(){bUseTransformation = true;}
    virtual void        CALL    Transform_disable(){bUseTransformation = false;}

    virtual void        CALL    Transform_useMatrix2d(float m11, float m12, float m21, float m22, float dx, float dy);
    virtual void        CALL    Transform_setMatrix(AliTransformationMatrix nm){mTransformMatrix = nm;}

    virtual AliTransformationMatrix
                        CALL    Transform_getMatrix(){return mTransformMatrix;}

    virtual HTEXTURE    CALL    Gfx_RenderShadow(HTARGET shadowTaget, HTARGET shadowTaget_bak,
                                                 HEFFECT effectBlur, int x, int y, int w, int h,
                                                 float blur, int targetWidth, int targetHeight,
                                                 int OffX, int OffY, int r, int g, int b, int a);

    //////// Implementation ////////
    void                _Init(int width, int height);
    void                _render_batch(bool bEndScene);
    void                _SetBlendMode(int blend);
    bool                _SetMatrix(float* matrix);
    void                _MulMatrix(float x, float y, float* matrix, float &x_out, float &y_out);
//#if NATIVE_SURFACE
    //add to move position and resize
    void                 setPosition(int x, int y);
    void                 setSize(int w, int h);
    void                 setAlpha(float a);
//#endif // NATIVE_SURFACE
#if NATIVE_SURFACE
    NativeEnv*           getNativeEnv() const { return m_nativeEnv; }
    void                 setNativeEnv(NativeEnv* nativeEnv) { m_nativeEnv = nativeEnv; }
#endif //#if NATIVE_SURFACE
    // System States
    const char*          szIcon;
    char                 szWinTitle[256];
    int                  nScreenWidth;
    int                  nScreenHeight;
    int                  nScreenBPP;
    bool                 bWindowed;
    bool                 bZBuffer;
    bool                 bTextureFilter;
    bool                 bUseSound;
    int                  nSampleRate;
    int                  nFXVolume;
    int                  nMusVolume;
    int                  ngl2dFPS;
    bool                 bHideMouse;
    bool                 bDontSuspend;

    // Timer
    float                fTime;
    float                fDeltaTime;
    DWORD                nFixedDelta;
    int                  nFPS;
    DWORD                t0, t0fps, dt;
    int                  cfps;

    int                  nPrim;
    int                  CurPrimType;
    int                  CurBlendMode;
    HTEXTURE             CurTexture;
    HEFFECT              CurEffect;
    HEFFECT              OldEffect;
    int                  CurSissorTop;
    int                  CurSissorLeft;
    int                  CurSissorW;
    int                  CurSissorH;
    int                  OldSissorTop;
    int                  OldSissorLeft;
    int                  OldSissorW;
    int                  OldSissorH;

private:
    // EGL variables
    EGLDisplay           eglDisplay;
    EGLConfig            eglConfig;
    EGLSurface           eglSurface;
    EGLContext           eglContext;
    EGLNativeWindowType  eglWindow;

    DCanvas::AliTransformationMatrix mTransformMatrix;
    bool                 bUseTransformation;

    float                m_lineWidth;
    // the opengl buffer
    GLuint               _vertexBuffer; // Vertex buffer object handle
    GLuint               _indexBuffer;  // Index buffer object handle
    GLuint               _textureCoordBuffer;// texture coord buffer
    GLuint               _colorBuffer;        // color buffer

    // the opengl buffer memory
    GLfloat*             _vertex_data;    // the vertex buffer data
    GLfloat*             _texcoord_data;        // the texcoord buffer data
    GLfloat*             _color_data;        // the color buffer data
    GLushort *           _index_quad;    // the index data of quad

    // the effects
    Gl2DEffect*          GEFFECTS[MAX_EFFECTS_NUMBER];

    DWORD                _vertex_length;
    DWORD                _texcoord_length;
    DWORD                _color_length;

    // the opengl Fbo handle
    int                  _i32CurrentFbo;    // the current Fbo handle
    int                  _i32OriginalFbo;    // the Original Fbo handle

    // the opengl Fbo handle list
    GLuint               _uiFbos[MAX_FBOS];        // the fbos list
    GLuint               _uiFbosTextures[MAX_FBOS];// the textures of the Fbos
    GLuint               _uiRenderBuffer[MAX_FBOS]; // the color RGBA Render buffer
    DWORD                _fbos_length;

    // the clip count num
    int                  _clipNum;

    // if the matrix is changed
    GLboolean            _matIsChanged;

    // the matrix
    GLfloat              _pfIdentity[GL2D_MATRIX_ARRAY_LENGTH];

    bool 				 m_isNative;
};

typedef class Gl2d_Impl * GLContextRef;

#endif // gl2d_IMPL_H
