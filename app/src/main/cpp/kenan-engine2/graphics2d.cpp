/*
** gl's 2d Engine 1.00
** Copyright (C) 2011-2014, aliyun cloud apps
**
** Core System functions
*/
#include "gl2d_impl.h"
#include "MathExtras.h"

#if IN_LINUXx86
#include <stdlib.h>
#elif IN_ANDROID
#include <stdlib.h>

#include <android/log.h>
#undef LOG_TAG
#undef LOGI
#undef LOGE
#define  LOG_TAG    "aligl2_lib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif // IN_LINUXx86

//gl blending func
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305

int GL_BLENDING[][2] =
{
    {GL_ONE,                    GL_ONE},
    {GL_SRC_ALPHA,              GL_ONE_MINUS_SRC_ALPHA},
    {GL_SRC_COLOR,              GL_DST_ALPHA},
    {GL_ONE_MINUS_SRC_COLOR,    GL_DST_ALPHA},
    {GL_SRC_ALPHA,              GL_DST_ALPHA},
    {GL_ONE_MINUS_SRC_ALPHA,    GL_DST_ALPHA},
    {GL_DST_ALPHA,              GL_DST_ALPHA},
    {GL_ONE_MINUS_DST_ALPHA,    GL_DST_ALPHA},
    {GL_SRC_COLOR,              GL_ONE_MINUS_SRC_ALPHA},
    {GL_SRC_COLOR,              GL_ONE_MINUS_SRC_COLOR},
    {GL_SRC_COLOR,              GL_ONE_MINUS_DST_ALPHA},
    {GL_ONE,                    GL_ONE_MINUS_SRC_ALPHA},
    {GL_ONE_MINUS_SRC_ALPHA,    GL_ONE}
};

static bool IsSameScissorRect(int x, int y, int w, int h)
{
    GLint scissorBox[4];
    glGetIntegerv(GL_SCISSOR_BOX, scissorBox);
    if (scissorBox[0] == x && scissorBox[1] == y && scissorBox[2] == w && scissorBox[3] == h)
    {
        return true;
    }
    return false;
}

void Gl2d_Impl::_Init(int width, int height)
{
    nScreenWidth  = width;
    nScreenHeight = height;

#if NATIVE_SURFACE
	if(m_isNative)
	{
		eglDisplay = m_nativeEnv->getDisplay();
		eglConfig  = m_nativeEnv->getConfig();
		eglSurface = m_nativeEnv->getSurface();
		eglContext = m_nativeEnv->getContext();
	}
#endif

    // create all buffers' handles
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_textureCoordBuffer);
    glGenBuffers(1, &_colorBuffer);
    glGenBuffers(1, &_indexBuffer);

    // malloc all buffer data
    if(_vertex_data)	free(_vertex_data);
    if(_color_data)		free(_color_data);
    if(_texcoord_data)	free(_texcoord_data);
    _vertex_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_POINT2D);
    _color_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_COLOR);
    _texcoord_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_TEXCOORD);

    // bind index buffer and init the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    _index_quad = (GLushort *)malloc(sizeof(GLushort ) * GL2D_SIZE_INDICS_QUAD * VERTEX_BUFFER_SIZE / GL2D_PRIM_QUADS);
    GLushort once_index[] = {0, 1, 2, 0, 2, 3};

    /* Give the index data:
     *----------------------------------------------------------------------
     *    for(var i = 0; i < VERTEX_BUFFER_SIZE; i ++)
     *     {
            indices[i * HTSIZE_INDICS_QUAD + 0] = i * 4 + 0;
            indices[i * HTSIZE_INDICS_QUAD + 1] = i * 4 + 1;
            indices[i * HTSIZE_INDICS_QUAD + 2] = i * 4 + 2;

            indices[i * HTSIZE_INDICS_QUAD + 3] = i * 4 + 0;
            indices[i * HTSIZE_INDICS_QUAD + 4] = i * 4 + 2;
            indices[i * HTSIZE_INDICS_QUAD + 5] = i * 4 + 3;
        }
    *---------------------------------------------------------------------------
    */
    LOOP(i, VERTEX_BUFFER_SIZE / GL2D_PRIM_QUADS)
    {
        LOOP(j, 6)
        {
            _index_quad[i * GL2D_SIZE_INDICS_QUAD + j] = (GLushort )((i << 2) + once_index[j]);
        }
    }

    // write buffer data!
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort) *  GL2D_SIZE_INDICS_QUAD * VERTEX_BUFFER_SIZE / GL2D_PRIM_QUADS,
                 _index_quad, GL_STATIC_DRAW);

    // disable the depth test
    glDisable(GL_DEPTH_TEST);

    // Init the params of each buffer data
    CurTexture = NO_TEXTURE;
    CurPrimType = 1;
    CurBlendMode = -1;
    OldEffect = CurEffect = -1;
    CurSissorTop = -1;
    CurSissorLeft = -1;
    CurSissorW = 0;
    CurSissorH = 0;
    OldSissorLeft = CurSissorLeft;
    OldSissorTop = CurSissorTop;
    OldSissorW = CurSissorW;
    OldSissorH = CurSissorH;

    _vertex_length = 0;
    _texcoord_length = 0;
    _color_length = 0;

    nPrim = 0;

    /**
    **---------------------------------------------------------------------------------
    **  Init the fbos, set them -1GEFFECTS
    ***/
    LOOP(i, MAX_FBOS)
    {
        _uiFbos[i] = -1;
        _uiFbosTextures[i] = NO_TEXTURE;
    }

    LOOP(i, MAX_EFFECTS_NUMBER)
    {
        GEFFECTS[i] = NULL;
    }

    _fbos_length = 0;

    // get the current Fob that the screen fob
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_i32OriginalFbo);
    _i32CurrentFbo = _i32OriginalFbo;

    _matIsChanged = false;

    // Indentify the matrix
    _pfIdentity[0] = 1.0f; _pfIdentity[1] = 0.0f;
    _pfIdentity[2] = 0.0f; _pfIdentity[3] = 1.0f;
    _pfIdentity[4] = 0.0f; _pfIdentity[5] = 0.0f;

    _clipNum = 0;
    m_lineWidth = 1.0;
}

void CALL Gl2d_Impl::Gfx_SetClearColor(float r, float g, float b, float a)
{
    // gl set the clear color
    glClearColor(r, g, b, a);
}

void CALL Gl2d_Impl::Gfx_Clear()
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif
    /*
        Clears the color buffer.
    */
    if (CurSissorW > 0 && CurSissorH > 0 && CurSissorLeft != -1 && CurSissorTop != -1)
    {
        glScissor(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH);
        glEnable(GL_SCISSOR_TEST);
    }
    glClear(GL_COLOR_BUFFER_BIT);
}

void CALL Gl2d_Impl::Gfx_SetClipping(int x, int y, int w, int h)
{
    CurSissorTop = y;CurSissorLeft = x;
    CurSissorW = w;CurSissorH = h;

    _clipNum = _clipNum >= 2 ? 0 : _clipNum;
    if (_clipNum == 0)
    {
        OldSissorTop = CurSissorTop;    OldSissorLeft = CurSissorLeft;
        OldSissorW = CurSissorW;    OldSissorH = CurSissorH;
    }
    _clipNum ++;
}

void CALL Gl2d_Impl::Gfx_BackClipping()
{
    CurSissorTop = OldSissorTop;    CurSissorLeft = OldSissorLeft;
    CurSissorW = OldSissorW;    CurSissorH = OldSissorH;
}

void CALL Gl2d_Impl::Gfx_Scissor(int x, int y, int w, int h)
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif
    glScissor(x, y, w, h);
    glEnable(GL_SCISSOR_TEST);
}

void CALL Gl2d_Impl::Gfx_CloseScissor()
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif
    glDisable(GL_SCISSOR_TEST);
}

void CALL Gl2d_Impl::Gfx_RemoveClipping()
{
    glDisable(GL_SCISSOR_TEST);
}

#if IN_ANDROID
void CALL Gl2d_Impl::Gfx_SwapBuffer()
{
    /*
        Swap Buffers.
        Brings to the native display the current render surface.
    */
    _render_batch(false);
}

void CALL Gl2d_Impl::Gfx_SwapBufferToScreen()
{
    Gfx_SwapBuffer();
    //LOGD("graphics2d.cpp : %p, %p",m_nativeEnv->getDisplay(), m_nativeEnv->getSurface());
#if NATIVE_SURFACE
	if(m_isNative){
    	eglSwapBuffers(m_nativeEnv->getDisplay(), m_nativeEnv->getSurface());
	}
#endif
}
#else

void CALL Gl2d_Impl::Gfx_FlushRenders()
{
    /*
        Swap Buffers.
        Brings to the native display the current render surface.
    */
    _render_batch(false);
}

void CALL Gl2d_Impl::Gfx_SwapBuffer()
{
    eglSwapBuffers(this->eglDisplay, this->eglSurface);
}
#endif

void CALL Gl2d_Impl::Gfx_RenderLine(float x1, float y1, float x2, float y2,
                                    float r, float g, float b, float a, float z)
{
    if (CurPrimType != GL2D_PRIM_LINES || nPrim >= VERTEX_BUFFER_SIZE / GL2D_PRIM_LINES
    || CurTexture != (HTEXTURE)NO_TEXTURE || CurBlendMode != BLEND_DEFAULT
    || CurEffect != OldEffect || _matIsChanged)
    {
        _render_batch(false);

        _matIsChanged = false;

        // Update Scissor
        if (CurSissorW > 0 && CurSissorH > 0 && CurSissorLeft != -1 && CurSissorTop != -1)
        {
            if (!IsSameScissorRect(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH))
            {
                glScissor(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH);
            }
            glEnable(GL_SCISSOR_TEST);
        }

        OldEffect = CurEffect;
        GEFFECTS[OldEffect]->UseThisEffect();

        CurPrimType = GL2D_PRIM_LINES;
        if (CurBlendMode != BLEND_DEFAULT)
            _SetBlendMode(BLEND_DEFAULT);

        if (CurTexture)
        {
            //line cannot use texture!!
            CurTexture = NO_TEXTURE;
        }
    }

    int i = _vertex_length;

    _vertex_data[i] = x1;     _vertex_data[i+1] = y1;    _vertex_data[i+2] = 0;
    _vertex_data[i+3] = x2;   _vertex_data[i+4] = y2;    _vertex_data[i+5] = 0;
    _vertex_length += 6;

    i = _color_length;
    _color_data[i]     = r;  _color_data[i+1] = g;  _color_data[i+2] = b;  _color_data[i+3] = a;
    _color_data[i + 4] = r;  _color_data[i+5] = g;  _color_data[i+6] = b;  _color_data[i+7] = a;
    _color_length += 8;

    i = _texcoord_length;
    _texcoord_data[i]     = 0;
    _texcoord_data[i + 1] = 0;
    _texcoord_data[i + 2] = 0;
    _texcoord_data[i + 3] = 0;
    _texcoord_length += 4;

    nPrim ++;
}

void CALL Gl2d_Impl::Gfx_RenderTriple(const gl2dTriple *triple)
{
    if (CurPrimType != GL2D_PRIM_TRIPLES || nPrim >= VERTEX_BUFFER_SIZE / GL2D_PRIM_TRIPLES
    || CurTexture != triple->tex || CurBlendMode != triple->blend
    || CurEffect != OldEffect || _matIsChanged)
    {
        _render_batch(false);

        _matIsChanged = false;
        // Update Scissor
        if (CurSissorW > 0 && CurSissorH > 0 && CurSissorLeft != -1 && CurSissorTop != -1)
        {
            if (!IsSameScissorRect(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH))
            {
                glScissor(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH);
            }
            glEnable(GL_SCISSOR_TEST);
        }

        OldEffect = CurEffect;
        GEFFECTS[OldEffect]->UseThisEffect();

        CurPrimType = GL2D_PRIM_TRIPLES;
        if (CurBlendMode != triple->blend)
            _SetBlendMode(triple->blend);

        if (triple->tex != CurTexture)
        {
            CurTexture = triple->tex;
            //glBindTexture(GL_TEXTURE_2D, CurTexture);
        }
    }

    int vl = _vertex_length;
    int cl = _color_length;
    int tl = _texcoord_length;

    int j = 0;
    LOOP(n, 3)
    {
        _vertex_data[vl++] = triple->v[j].x;
        _vertex_data[vl++] = triple->v[j].y;
        _vertex_data[vl++] = triple->v[j].z;

        _color_data[cl++] = triple->v[j].r;
        _color_data[cl++] = triple->v[j].g;
        _color_data[cl++] = triple->v[j].b;
        _color_data[cl++] = triple->v[j].a;

        _texcoord_data[tl++] = triple->v[j].tx;
        _texcoord_data[tl++] = triple->v[j].ty;

        j ++;
    }
    _vertex_length += 9;
    _color_length += 12;
    _texcoord_length += 6;
    nPrim++;
}

void CALL Gl2d_Impl::Gfx_RenderQuad(const gl2dQuad *quad)
{
    if (CurPrimType != GL2D_PRIM_QUADS || nPrim >= VERTEX_BUFFER_SIZE / GL2D_PRIM_QUADS
    || CurTexture != quad->tex || CurBlendMode != quad->blend || CurEffect != OldEffect)
    {
        _render_batch(false);

        // Update Scissor
        if (CurSissorW > 0 && CurSissorH > 0 && CurSissorLeft != -1 && CurSissorTop != -1)
        {
            if (!IsSameScissorRect(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH))
            {
                glScissor(CurSissorLeft, CurSissorTop, CurSissorW, CurSissorH);
            }
            glEnable(GL_SCISSOR_TEST);
        }

        OldEffect = CurEffect;
        GEFFECTS[OldEffect]->UseThisEffect();

        CurPrimType = GL2D_PRIM_QUADS;
        if (CurBlendMode != quad->blend)
            _SetBlendMode(quad->blend);

        if (quad->tex != CurTexture)
        {
            CurTexture = quad->tex;
            //glBindTexture(GL_TEXTURE_2D, CurTexture);
        }
    }

    int vl = _vertex_length;
    int cl = _color_length;
    int tl = _texcoord_length;

    int j = 0;
    LOOP(n, 4)
    {
        _vertex_data[vl++] = quad->v[j].x;
        _vertex_data[vl++] = quad->v[j].y;
        _vertex_data[vl++] = quad->v[j].z;

        _color_data[cl++] = quad->v[j].r;
        _color_data[cl++] = quad->v[j].g;
        _color_data[cl++] = quad->v[j].b;
        _color_data[cl++] = quad->v[j].a;

        _texcoord_data[tl++] = quad->v[j].tx;
        _texcoord_data[tl++] = quad->v[j].ty;

        j ++;
    }
    _vertex_length += 12;
    _color_length += 16;
    _texcoord_length += 8;
    nPrim++;
}

void CALL Gl2d_Impl::Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                      float x3, float y3, float x4, float y4,
                                      float tx[4], float ty[4], int r[4], int g[4], int b[4], int a[4],
                                      int blend, HTEXTURE htex /*= NO_TEXTURE*/ )
{
    // now multiply the matrix
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out, x4_out, y4_out;

        mTransformMatrix.map(x1,y1,x1_out,y1_out);
        mTransformMatrix.map(x2,y2,x2_out,y2_out);
        mTransformMatrix.map(x3,y3,x3_out,y3_out);
        mTransformMatrix.map(x4,y4,x4_out,y4_out);
        x1 = x1_out;    y1 = y1_out;
        x2 = x2_out;    y2 = y2_out;
        x3 = x3_out;    y3 = y3_out;
        x4 = x4_out;    y4 = y4_out;
    }
    gl2dQuad quad;
    quad.v[0].x = (float)x1 / (float)nScreenWidth;        quad.v[0].y = (float)y1 / (float)nScreenHeight;
    quad.v[1].x = (float)x2 / (float)nScreenWidth;        quad.v[1].y = (float)y2 / (float)nScreenHeight;
    quad.v[2].x = (float)x3 / (float)nScreenWidth;        quad.v[2].y = (float)y3 / (float)nScreenHeight;
    quad.v[3].x = (float)x4 / (float)nScreenWidth;        quad.v[3].y = (float)y4 / (float)nScreenHeight;

    quad.v[0].x = quad.v[0].x * 2.0f - 1.0f;
    quad.v[1].x = quad.v[1].x * 2.0f - 1.0f;
    quad.v[2].x = quad.v[2].x * 2.0f - 1.0f;
    quad.v[3].x = quad.v[3].x * 2.0f - 1.0f;

    quad.v[0].y = 1.0f - quad.v[0].y * 2.0f;
    quad.v[1].y = 1.0f - quad.v[1].y * 2.0f;
    quad.v[2].y = 1.0f - quad.v[2].y * 2.0f;
    quad.v[3].y = 1.0f - quad.v[3].y * 2.0f;

    LOOP(i, 4)
    {
        quad.v[i].tx = tx[i];
        quad.v[i].ty = ty[i];
        quad.v[i].r = r[i] / 255.0f;
        quad.v[i].g = g[i] / 255.0f;
        quad.v[i].b = b[i] / 255.0f;
        quad.v[i].a = a[i] / 255.0f;
        quad.v[i].z = 0.5f;
    }
    quad.blend = blend;
    quad.tex = htex;

    Gfx_RenderQuad(&quad);
}

void CALL Gl2d_Impl::Gfx_RenderQuadEx(float x1, float y1, float x2, float y2,
                                      float x3, float y3, float x4, float y4,
                                      int r, int g, int b, int a, int blend, HTEXTURE htex)
{
    // now multiply the matrix
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out, x4_out, y4_out;
        mTransformMatrix.map(x1,y1,x1_out,y1_out);
        mTransformMatrix.map(x2,y2,x2_out,y2_out);
        mTransformMatrix.map(x3,y3,x3_out,y3_out);
        mTransformMatrix.map(x4,y4,x4_out,y4_out);
        x1 = x1_out;    y1 = y1_out;
        x2 = x2_out;    y2 = y2_out;
        x3 = x3_out;    y3 = y3_out;
        x4 = x4_out;    y4 = y4_out;
    }
    gl2dQuad quad;
    quad.v[0].x = (float)x1 / (float)nScreenWidth;        quad.v[0].y = (float)y1 / (float)nScreenHeight;
    quad.v[1].x = (float)x2 / (float)nScreenWidth;        quad.v[1].y = (float)y2 / (float)nScreenHeight;
    quad.v[2].x = (float)x3 / (float)nScreenWidth;        quad.v[2].y = (float)y3 / (float)nScreenHeight;
    quad.v[3].x = (float)x4 / (float)nScreenWidth;        quad.v[3].y = (float)y4 / (float)nScreenHeight;

    quad.v[0].x = quad.v[0].x * 2.0f - 1.0f;
    quad.v[1].x = quad.v[1].x * 2.0f - 1.0f;
    quad.v[2].x = quad.v[2].x * 2.0f - 1.0f;
    quad.v[3].x = quad.v[3].x * 2.0f - 1.0f;

    quad.v[0].y = 1.0f - quad.v[0].y * 2.0f;
    quad.v[1].y = 1.0f - quad.v[1].y * 2.0f;
    quad.v[2].y = 1.0f - quad.v[2].y * 2.0f;
    quad.v[3].y = 1.0f - quad.v[3].y * 2.0f;

    LOOP(i, 4)
    {
        quad.v[i].tx =  quad.v[i].ty= 0.0;
        quad.v[i].r = r / 255.0f;
        quad.v[i].g = g / 255.0f;
        quad.v[i].b = b / 255.0f;
        quad.v[i].a = a / 255.0f;
        quad.v[i].z = 0.5f;
    }
    quad.blend = blend;
    quad.tex = htex;
    Gfx_RenderQuad(&quad);
}

void CALL Gl2d_Impl::Gfx_RenderLineEx(float x1, float y1, float x2, float y2, int r, int g, int b, int a)
{
    // now multiply the matrix
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out;
        mTransformMatrix.map(x1, y1, x1_out, y1_out);
        mTransformMatrix.map(x2, y2, x2_out, y2_out);
        x1 = x1_out;    y1 = y1_out;
        x2 = x2_out;    y2 = y2_out;
    }

    float _x1 = (float)x1 / (float)nScreenWidth;
    float _x2 = (float)x2 / (float)nScreenWidth;
    float _y1 = (float)y1 / (float)nScreenHeight;
    float _y2 = (float)y2 / (float)nScreenHeight;

    _x1 = _x1 * 2.0f - 1.0f;
    _x2 = _x2 * 2.0f - 1.0f;
    _y1 = 1.0f - _y1 * 2.0f;
    _y2 = 1.0f - _y2 * 2.0f;

    float _r = (float)r / 255.0;
    float _g = (float)g / 255.0;
    float _b = (float)b / 255.0;
    float _a = (float)a / 255.0;;

    Gfx_RenderLine(_x1, _y1, _x2, _y2, _r, _g, _b, _a);
}

void CALL Gl2d_Impl::Gfx_RenderTripleEx( float x1, float y1, float x2, float y2,
                                         float x3, float y3, int r, int g, int b, int a,
                                         int blend, HTEXTURE htex)
{
    // now multiply the matrix
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out;
        mTransformMatrix.map(x1, y1, x1_out, y1_out);
        mTransformMatrix.map(x2, y2, x2_out, y2_out);
        mTransformMatrix.map(x3, y3, x3_out, y3_out);
        x1 = x1_out;    y1 = y1_out;
        x2 = x2_out;    y2 = y2_out;
        x3 = x3_out;    y3 = y3_out;
    }

    gl2dTriple triple;
    triple.v[0].x = (float)x1 / (float)nScreenWidth;
    triple.v[0].y = (float)y1 / (float)nScreenHeight;

    triple.v[1].x = (float)x2 / (float)nScreenWidth;
    triple.v[1].y = (float)y2 / (float)nScreenHeight;

    triple.v[2].x = (float)x3 / (float)nScreenWidth;
    triple.v[2].y = (float)y3 / (float)nScreenHeight;

    triple.v[0].r = (float)r / 255.0f;    triple.v[0].g = (float)g / 255.0f;
    triple.v[1].r = (float)r / 255.0f;    triple.v[1].g = (float)g / 255.0f;
    triple.v[2].r = (float)r / 255.0f;    triple.v[2].g = (float)g / 255.0f;

    triple.v[0].b = (float)b / 255.0f;    triple.v[0].a = (float)a / 255.0f;
    triple.v[1].b = (float)b / 255.0f;    triple.v[1].a = (float)a / 255.0f;
    triple.v[2].b = (float)b / 255.0f;    triple.v[2].a = (float)a / 255.0f;

    triple.v[0].x = triple.v[0].x * 2.0f - 1.0f;
    triple.v[1].x = triple.v[1].x * 2.0f - 1.0f;
    triple.v[2].x = triple.v[2].x * 2.0f - 1.0f;

    triple.v[0].y = 1.0f - triple.v[0].y * 2.0f;
    triple.v[1].y = 1.0f - triple.v[1].y * 2.0f;
    triple.v[2].y = 1.0f - triple.v[2].y * 2.0f;

    triple.v[0].z = triple.v[1].z = triple.v[2].z = 0.5f;

    triple.blend = blend;
    triple.tex = htex;

    Gfx_RenderTriple(&triple);
}

void CALL Gl2d_Impl::Gfx_RenderRectEx( float left, float top, float right, float bottom,
                                       int r, int g, int b, int a, int blend, HTEXTURE htex )
{
    gl2dQuad quad;
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out, x4_out, y4_out;
        mTransformMatrix.map(left, top, x1_out,y1_out);
        mTransformMatrix.map(right, top, x2_out,y2_out);
        mTransformMatrix.map(right, bottom, x3_out,y3_out);
        mTransformMatrix.map(left, bottom, x4_out,y4_out);
        quad.v[0].x = (float)x1_out / (float)nScreenWidth;    quad.v[0].y = (float)y1_out / (float)nScreenHeight;
        quad.v[1].x = (float)x2_out / (float)nScreenWidth;    quad.v[1].y = (float)y2_out / (float)nScreenHeight;
        quad.v[2].x = (float)x3_out / (float)nScreenWidth;    quad.v[2].y = (float)y3_out / (float)nScreenHeight;
        quad.v[3].x = (float)x4_out / (float)nScreenWidth;    quad.v[3].y = (float)y4_out / (float)nScreenHeight;
    }
    else
    {
        quad.v[0].x = (float)left / (float)nScreenWidth;    quad.v[0].y = (float)top / (float)nScreenHeight;
        quad.v[1].x = (float)right / (float)nScreenWidth;   quad.v[1].y = (float)top / (float)nScreenHeight;
        quad.v[2].x = (float)right / (float)nScreenWidth;   quad.v[2].y = (float)bottom / (float)nScreenHeight;
        quad.v[3].x = (float)left / (float)nScreenWidth;    quad.v[3].y = (float)bottom / (float)nScreenHeight;
    }

    quad.v[0].x = quad.v[0].x * 2.0f - 1.0f;
    quad.v[1].x = quad.v[1].x * 2.0f - 1.0f;
    quad.v[2].x = quad.v[2].x * 2.0f - 1.0f;
    quad.v[3].x = quad.v[3].x * 2.0f - 1.0f;

    quad.v[0].y = 1.0f - quad.v[0].y * 2.0f;
    quad.v[1].y = 1.0f - quad.v[1].y * 2.0f;
    quad.v[2].y = 1.0f - quad.v[2].y * 2.0f;
    quad.v[3].y = 1.0f - quad.v[3].y * 2.0f;
    LOOP(i, 4)
    {
        quad.v[i].tx =  quad.v[i].ty= 0.0;
        quad.v[i].r = r / 255.0f;
        quad.v[i].g = g / 255.0f;
        quad.v[i].b = b / 255.0f;
        quad.v[i].a = a / 255.0f;
        quad.v[i].z = 0.5f;
    }
    quad.blend = blend;
    quad.tex = htex;

    Gfx_RenderQuad(&quad);
}

void CALL Gl2d_Impl::Gfx_RenderRectExTexture( float left, float top, float right, float bottom,
                                              float TL, float TT, float TR, float TB,
                                              int r, int g, int b, int a, int blend, HTEXTURE htex )
{
    gl2dQuad quad;
    if (bUseTransformation)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out, x4_out, y4_out;
        mTransformMatrix.map(left, top, x1_out, y1_out);
        mTransformMatrix.map(right, top, x2_out, y2_out);
        mTransformMatrix.map(right, bottom, x3_out, y3_out);
        mTransformMatrix.map(left, bottom, x4_out, y4_out);
        quad.v[0].x = (float)x1_out / (float)nScreenWidth;    quad.v[0].y = (float)y1_out / (float)nScreenHeight;
        quad.v[1].x = (float)x2_out / (float)nScreenWidth;    quad.v[1].y = (float)y2_out / (float)nScreenHeight;
        quad.v[2].x = (float)x3_out / (float)nScreenWidth;    quad.v[2].y = (float)y3_out / (float)nScreenHeight;
        quad.v[3].x = (float)x4_out / (float)nScreenWidth;    quad.v[3].y = (float)y4_out / (float)nScreenHeight;
    }
    else
    {
        quad.v[0].x = (float)left / (float)nScreenWidth;    quad.v[0].y = (float)top / (float)nScreenHeight;
        quad.v[1].x = (float)right / (float)nScreenWidth;   quad.v[1].y = (float)top / (float)nScreenHeight;
        quad.v[2].x = (float)right / (float)nScreenWidth;   quad.v[2].y = (float)bottom / (float)nScreenHeight;
        quad.v[3].x = (float)left / (float)nScreenWidth;    quad.v[3].y = (float)bottom / (float)nScreenHeight;
    }

    quad.v[0].x = quad.v[0].x * 2.0f - 1.0f;
    quad.v[1].x = quad.v[1].x * 2.0f - 1.0f;
    quad.v[2].x = quad.v[2].x * 2.0f - 1.0f;
    quad.v[3].x = quad.v[3].x * 2.0f - 1.0f;

    quad.v[0].y = 1.0f - quad.v[0].y * 2.0f;
    quad.v[1].y = 1.0f - quad.v[1].y * 2.0f;
    quad.v[2].y = 1.0f - quad.v[2].y * 2.0f;
    quad.v[3].y = 1.0f - quad.v[3].y * 2.0f;

    quad.v[0].tx = TL;    quad.v[0].ty = 1.0f - TT;
    quad.v[1].tx = TR;    quad.v[1].ty = 1.0f - TT;
    quad.v[2].tx = TR;    quad.v[2].ty = 1.0f - TB;
    quad.v[3].tx = TL;    quad.v[3].ty = 1.0f - TB;

    LOOP(i, 4)
    {
        quad.v[i].r = r / 255.0f;
        quad.v[i].g = g / 255.0f;
        quad.v[i].b = b / 255.0f;
        quad.v[i].a = a / 255.0f;
        quad.v[i].z = 0.5f;
    }
    quad.blend = blend;
    quad.tex = htex;

    Gfx_RenderQuad(&quad);
}

void CALL Gl2d_Impl::Gfx_RenderRoundEx(float x0, float y0, float r0, int r, int g, int b, int a, int blend)
{
    gl2dTriple triple;
    float x = 0, y = 0;
    float tx = r0 * cosTable[ROUND_VERTEX_NUMBER-1] + x0;
    float ty = r0 * sinTable[ROUND_VERTEX_NUMBER-1] + y0;

    for (int i=0;i<ROUND_VERTEX_NUMBER;i++)
    {
        x = r0*cosTable[i] + x0;
        y = r0*sinTable[i] + y0;
        if (bUseTransformation)
        {
            float x0_out, y0_out, x_out, y_out, tx_out, ty_out;
            mTransformMatrix.map(x0, y0, x0_out, y0_out);
            mTransformMatrix.map(tx, ty, tx_out, ty_out);
            mTransformMatrix.map(x, y, x_out, y_out);

            triple.v[0].x = x0_out / nScreenWidth;    triple.v[0].y = y0_out / nScreenHeight;
            triple.v[1].x = tx_out / nScreenWidth;    triple.v[1].y = ty_out / nScreenHeight;
            triple.v[2].x = x_out / nScreenWidth;     triple.v[2].y = y_out / nScreenHeight;
        }
        else
        {
            triple.v[0].x = x0 / nScreenWidth;    triple.v[0].y = y0 / nScreenHeight;
            triple.v[1].x = tx / nScreenWidth;    triple.v[1].y = ty / nScreenHeight;
            triple.v[2].x = x / nScreenWidth;     triple.v[2].y = y / nScreenHeight;
        }

        tx = x;
        ty = y;
        triple.v[0].x = triple.v[0].x * 2.0f - 1.0f;
        triple.v[1].x = triple.v[1].x * 2.0f - 1.0f;
        triple.v[2].x = triple.v[2].x * 2.0f - 1.0f;

        triple.v[0].y = 1.0f - triple.v[0].y * 2.0f;
        triple.v[1].y = 1.0f - triple.v[1].y * 2.0f;
        triple.v[2].y = 1.0f - triple.v[2].y * 2.0f;

        triple.v[0].r = (float)r / 255.0f;    triple.v[0].g = (float)g / 255.0f;
        triple.v[1].r = (float)r / 255.0f;    triple.v[1].g = (float)g / 255.0f;
        triple.v[2].r = (float)r / 255.0f;    triple.v[2].g = (float)g / 255.0f;

        triple.v[0].b = (float)b / 255.0f;    triple.v[0].a = (float)a / 255.0f;
        triple.v[1].b = (float)b / 255.0f;    triple.v[1].a = (float)a / 255.0f;
        triple.v[2].b = (float)b / 255.0f;    triple.v[2].a = (float)a / 255.0f;

        triple.v[0].z = triple.v[1].z = triple.v[2].z = 0.5f;
        triple.blend = blend;
        triple.tex = NO_TEXTURE;
        Gfx_RenderTriple(&triple);
    }
}

void CALL Gl2d_Impl::Gfx_RenderTorusEx(float x0, float y0, float r0, float x1, float y1, float r1,
                                       int r[2], int g[2], int b[2], int a[2], int blend)
{
    float dis = sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );
    float diff = abs( r0-r1 );
    if (dis > diff || (x0==x1 && y0==y1 && r0==r1))
        return;

    bool isOutward;
    if (r0<r1)
        isOutward = true;
    else isOutward =false;

    gl2dTriple triple;
    float tx0 = r0 * cosTable[ROUND_VERTEX_NUMBER-1] + x0;
    float ty0 = r0 * sinTable[ROUND_VERTEX_NUMBER-1] + y0;
    float tx1 = r1 * cosTable[ROUND_VERTEX_NUMBER-1] + x1;
    float ty1 = r1 * sinTable[ROUND_VERTEX_NUMBER-1] + y1;
    float nx0 = 0, ny0 = 0, nx1 = 0, ny1 = 0;

    for (int i = 0; i < ROUND_VERTEX_NUMBER; i++)
    {
        nx0 = r0*cosTable[i] + x0;
        ny0 = r0*sinTable[i] + y0;
        if (isOutward)
        {
            if (bUseTransformation)
            {
                float nx0_out, ny0_out, tx0_out, ty0_out, tx1_out, ty1_out;
                mTransformMatrix.map(nx0, ny0, nx0_out, ny0_out);
                mTransformMatrix.map(tx0, ty0, tx0_out, ty0_out);
                mTransformMatrix.map(tx1, ty1, tx1_out, ty1_out);

                triple.v[0].x = nx0_out / nScreenWidth;    triple.v[0].y = ny0_out / nScreenHeight;
                triple.v[1].x = tx0_out / nScreenWidth;    triple.v[1].y = ty0_out / nScreenHeight;
                triple.v[2].x = tx1_out / nScreenWidth;    triple.v[2].y = ty1_out / nScreenHeight;
            }
            else
            {
                triple.v[0].x = nx0 / nScreenWidth;    triple.v[0].y = ny0 / nScreenHeight;
                triple.v[1].x = tx0 / nScreenWidth;    triple.v[1].y = ty0 / nScreenHeight;
                triple.v[2].x = tx1 / nScreenWidth;    triple.v[2].y = ty1 / nScreenHeight;
            }
        }
        else
        {
            if (bUseTransformation)
            {
                float nx0_out, ny0_out, tx0_out, ty0_out, tx1_out, ty1_out;
                mTransformMatrix.map(nx0, ny0, nx0_out, ny0_out);
                mTransformMatrix.map(tx0, ty0, tx0_out, ty0_out);
                mTransformMatrix.map(tx1, ty1, tx1_out, ty1_out);

                triple.v[0].x = tx0_out / nScreenWidth;    triple.v[0].y = ty0_out / nScreenHeight;
                triple.v[1].x = nx0_out / nScreenWidth;    triple.v[1].y = ny0_out / nScreenHeight;
                triple.v[2].x = tx1_out / nScreenWidth;    triple.v[2].y = ty1_out / nScreenHeight;
            }
            else
            {
                triple.v[0].x = tx0 / nScreenWidth;    triple.v[0].y = ty0 / nScreenHeight;
                triple.v[1].x = nx0 / nScreenWidth;    triple.v[1].y = ny0 / nScreenHeight;
                triple.v[2].x = tx1 / nScreenWidth;    triple.v[2].y = ty1 / nScreenHeight;
            }
        }
        tx0 = nx0;
        ty0 = ny0;

        triple.v[0].x = triple.v[0].x * 2.0f - 1.0f;
        triple.v[1].x = triple.v[1].x * 2.0f - 1.0f;
        triple.v[2].x = triple.v[2].x * 2.0f - 1.0f;

        triple.v[0].y = 1.0f - triple.v[0].y * 2.0f;
        triple.v[1].y = 1.0f - triple.v[1].y * 2.0f;
        triple.v[2].y = 1.0f - triple.v[2].y * 2.0f;

        triple.v[0].r = (float)r[0] / 255.0f;    triple.v[0].g = (float)g[0] / 255.0f;
        triple.v[1].r = (float)r[0] / 255.0f;    triple.v[1].g = (float)g[0] / 255.0f;
        triple.v[2].r = (float)r[1] / 255.0f;    triple.v[2].g = (float)g[1] / 255.0f;

        triple.v[0].b = (float)b[0] / 255.0f;    triple.v[0].a = (float)a[0] / 255.0f;
        triple.v[1].b = (float)b[0] / 255.0f;    triple.v[1].a = (float)a[0] / 255.0f;
        triple.v[2].b = (float)b[1] / 255.0f;    triple.v[2].a = (float)a[1] / 255.0f;

        triple.v[0].z = triple.v[1].z = triple.v[2].z = 0.5f;
        triple.blend = blend;
        triple.tex = NO_TEXTURE;
        Gfx_RenderTriple(&triple);

        nx1 = r1*cosTable[i] + x1;
        ny1 = r1*sinTable[i] + y1;
        if (isOutward)
        {
            if (bUseTransformation)
            {
                float nx1_out, ny1_out, tx0_out, ty0_out, tx1_out, ty1_out;
                mTransformMatrix.map(nx1, ny1, nx1_out, ny1_out);
                mTransformMatrix.map(tx0, ty0, tx0_out, ty0_out);
                mTransformMatrix.map(tx1, ty1, tx1_out, ty1_out);

                triple.v[0].x = tx1_out / nScreenWidth;    triple.v[0].y = ty1_out / nScreenHeight;
                triple.v[1].x = nx1_out / nScreenWidth;    triple.v[1].y = ny1_out / nScreenHeight;
                triple.v[2].x = tx0_out / nScreenWidth;    triple.v[2].y = ty0_out / nScreenHeight;
            }
            else
            {
                triple.v[0].x = tx1 / nScreenWidth;    triple.v[0].y = ty1 / nScreenHeight;
                triple.v[1].x = nx1 / nScreenWidth;    triple.v[1].y = ny1 / nScreenHeight;
                triple.v[2].x = tx0 / nScreenWidth;    triple.v[2].y = ty0 / nScreenHeight;
            }
        }
        else
        {
            if (bUseTransformation)
            {
                float nx1_out, ny1_out, tx0_out, ty0_out, tx1_out, ty1_out;
                mTransformMatrix.map(nx1, ny1, nx1_out, ny1_out);
                mTransformMatrix.map(tx0, ty0, tx0_out, ty0_out);
                mTransformMatrix.map(tx1, ty1, tx1_out, ty1_out);

                triple.v[0].x = nx1_out / nScreenWidth;    triple.v[0].y = ny1_out / nScreenHeight;
                triple.v[1].x = tx1_out / nScreenWidth;    triple.v[1].y = ty1_out / nScreenHeight;
                triple.v[2].x = tx0_out / nScreenWidth;    triple.v[2].y = ty0_out / nScreenHeight;
            }
            else
            {
                triple.v[0].x = nx1 / nScreenWidth;    triple.v[0].y = ny1 / nScreenHeight;
                triple.v[1].x = tx1 / nScreenWidth;    triple.v[1].y = ty1 / nScreenHeight;
                triple.v[2].x = tx0 / nScreenWidth;    triple.v[2].y = ty0 / nScreenHeight;
            }
        }
        tx1 = nx1;
        ty1 = ny1;

        triple.v[0].x = triple.v[0].x * 2.0f - 1.0f;
        triple.v[1].x = triple.v[1].x * 2.0f - 1.0f;
        triple.v[2].x = triple.v[2].x * 2.0f - 1.0f;

        triple.v[0].y = 1.0f - triple.v[0].y * 2.0f;
        triple.v[1].y = 1.0f - triple.v[1].y * 2.0f;
        triple.v[2].y = 1.0f - triple.v[2].y * 2.0f;

        triple.v[0].r = (float)r[1] / 255.0f;    triple.v[0].g = (float)g[1] / 255.0f;
        triple.v[1].r = (float)r[1] / 255.0f;    triple.v[1].g = (float)g[1] / 255.0f;
        triple.v[2].r = (float)r[0] / 255.0f;    triple.v[2].g = (float)g[0] / 255.0f;

        triple.v[0].b = (float)b[1] / 255.0f;    triple.v[0].a = (float)a[1] / 255.0f;
        triple.v[1].b = (float)b[1] / 255.0f;    triple.v[1].a = (float)a[1] / 255.0f;
        triple.v[2].b = (float)b[0] / 255.0f;    triple.v[2].a = (float)a[0] / 255.0f;

        triple.v[0].z = triple.v[1].z = triple.v[2].z = 0.5f;
        triple.blend = blend;
        triple.tex = NO_TEXTURE;
        Gfx_RenderTriple(&triple);
    }
}

void CALL Gl2d_Impl::Gfx_SetMatrix( float* matrix )
{
    this->_matIsChanged = _SetMatrix(matrix);
}

HTARGET CALL Gl2d_Impl::Target_Create(int width, int height)
{
    GLuint    _auiFbo;
    GLuint    _auiTexture;
    //GLuint  _auiRGBABuffer;

    glGenFramebuffers(1, &_auiFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _auiFbo);

    glGenTextures(1, &_auiTexture);

    glBindTexture(GL_TEXTURE_2D, _auiTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _auiTexture, 0);

    // Clear the color buffer for this FBO
    glClear(GL_COLOR_BUFFER_BIT);

    // save the fbo and the texture
    _uiFbos[_fbos_length++] = _auiFbo;
    _uiFbosTextures[_auiFbo] = _auiTexture;
    //_uiRenderBuffer[_auiFbo] = _auiRGBABuffer;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, _i32OriginalFbo);

    return (HTARGET)_auiFbo;
    //return 0;
}

void CALL Gl2d_Impl::Target_Active(HTARGET target, int width, int height)
{
     /*
         To do anything with a frame buffer object we need to bind it. In the case
         below we are binding our frame buffer object to the frame buffer.
    */
    glBindFramebuffer(GL_FRAMEBUFFER, target);

    /*
         If everything went ok then we can render to the texture.
    */
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glViewport(0, 0, width, height);
    }
    _i32CurrentFbo = target;
}

void CALL Gl2d_Impl::Target_ResetToScreen()
{

    glBindFramebuffer(GL_FRAMEBUFFER, _i32OriginalFbo);

    glViewport(0, 0, nScreenWidth, nScreenHeight);

    _i32CurrentFbo = _i32OriginalFbo;
}

void CALL Gl2d_Impl::Target_ReadPixels( int x, int y, int width, int height, GLuint* pPixelData )
{
    glReadPixels(x, y, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

}

void CALL Gl2d_Impl::Target_Free(HTARGET target)
{
    GLuint deleteTarget = target;
    glDeleteTextures(1, &_uiFbosTextures[target]);
    glDeleteRenderbuffers(1, &_uiRenderBuffer[target]);
    glDeleteFramebuffers(1, &deleteTarget);
}

void CALL Gl2d_Impl::Target_FreeAll()
{
    LOOP(i, _fbos_length)
    {
        GLuint _deleteTarget = _uiFbos[i];
        glDeleteTextures(1, &_uiFbosTextures[_deleteTarget]);
        glDeleteRenderbuffers(1, &_uiRenderBuffer[_deleteTarget]);
        glDeleteFramebuffers(1, &_deleteTarget);
    }
}

void CALL Gl2d_Impl::Gfx_BeginSetClipPath()
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif

    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_STENCIL_TEST);
}

void CALL Gl2d_Impl::Gfx_ClipPath()
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void CALL Gl2d_Impl::Gfx_EndClipPath()
{
#if IN_ANDROID
    this->Gfx_SwapBuffer();
#else
    this->Gfx_FlushRenders();
#endif
    glDisable(GL_STENCIL_TEST);
}

HTEXTURE CALL Gl2d_Impl::Target_GetTexture(HTARGET target)
{
    /*
        Firstly, to do anything with a frame buffer object we need to bind it. In the case
        below we are binding our frame buffer object to the frame buffer.
    */
    return _uiFbosTextures[target];
}

HTEXTURE CALL Gl2d_Impl::Texture_Create(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
    return (HTEXTURE)texture;
}

HTEXTURE CALL Gl2d_Impl::Texture_Create()
{
	GLuint texture;
	glGenTextures(1, &texture);
    return (HTEXTURE)texture;
}

HTEXTURE CALL Gl2d_Impl::Texture_Load(const  unsigned int *data, int width, int height)
{
    GLuint texture;
    glGenTextures(1, &texture);
    //glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glDisable(GL_TEXTURE_2D);

    return (HTEXTURE)texture;
}


void CALL Gl2d_Impl::Texture_SaveToFile()
{
    //notImplement();
}

void CALL Gl2d_Impl::Texture_Free(HTEXTURE tex)
{
    GLuint texture = tex;
    // Frees the texture
    glDeleteTextures(1, &texture);
}

//////// Implementation ////////
void Gl2d_Impl::_render_batch(bool bEndScene)
{
    if (!nPrim) return;

    if (CurTexture != (HTEXTURE)NO_TEXTURE)
    {
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, CurTexture);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    /*
    Enable the custom vertex attribute at index VERTEX_ARRAY.
    We previously binded that index to the variable in our shader "vec4 MyVertex;"
    */
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glEnableVertexAttribArray(VERTEX_ARRAY);
    glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass the texture coordinates data
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glEnableVertexAttribArray(COLOR_ARRAY);
    glVertexAttribPointer(COLOR_ARRAY, 4, GL_FLOAT, GL_FALSE, 0, 0);

    //
    glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
    glEnableVertexAttribArray(TEXCOORD_ARRAY);
    glVertexAttribPointer(TEXCOORD_ARRAY, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDisable(GL_CULL_FACE);

    switch(CurPrimType)
    {
    case GL2D_PRIM_QUADS:
        {
            //pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, nPrim<<2, 0, nPrim<<1);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_QUADS*sizeof(GLfloat)*GL2D_SIZE_POINT2D,
                         _vertex_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_QUADS*sizeof(GLfloat)*GL2D_SIZE_COLOR,
                         _color_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_QUADS*sizeof(GLfloat)*GL2D_SIZE_TEXCOORD,
                         _texcoord_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
            glDrawElements(GL_TRIANGLES, nPrim * 6, GL_UNSIGNED_SHORT, 0);

        }

        break;

    case GL2D_PRIM_TRIPLES:
        {
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_TRIPLES*sizeof(GLfloat)*GL2D_SIZE_POINT2D,
                         _vertex_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_TRIPLES*sizeof(GLfloat)*GL2D_SIZE_COLOR,
                         _color_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
            glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_TRIPLES*sizeof(GLfloat)*GL2D_SIZE_TEXCOORD,
                         _texcoord_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
            glDrawArrays(GL_TRIANGLES, 0, nPrim * GL2D_PRIM_TRIPLES);

        }

        break;

    case GL2D_PRIM_LINES:
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_LINES*sizeof(GLfloat)*GL2D_SIZE_POINT2D,
                     _vertex_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_LINES*sizeof(GLfloat)*GL2D_SIZE_COLOR,
                     _color_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, nPrim*GL2D_PRIM_LINES*sizeof(GLfloat)*GL2D_SIZE_TEXCOORD,
                     _texcoord_data, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glDrawArrays(GL_LINES, 0, nPrim * GL2D_PRIM_LINES);
        break;
    }

    nPrim=0;

    /// renew the buffer data
    //SAFE_DELETE(_vertex_data);
    //SAFE_DELETE(_texcoord_data);
    //SAFE_DELETE(_color_data);

    _vertex_length = 0;
    _texcoord_length = 0;
    _color_length = 0;
    //_vertex_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_POINT2D);
    //_color_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_COLOR);
    //_texcoord_data = (GLfloat *)malloc(sizeof(GLfloat) * VERTEX_BUFFER_SIZE * GL2D_SIZE_TEXCOORD);
}

void Gl2d_Impl::Gfx_SetLineWidth(float width)
{
    m_lineWidth = width;
    glLineWidth(width);
}

void Gl2d_Impl::_SetBlendMode(int blend)
{
    CurBlendMode = blend;
    if (blend <= 0)
    {
        glDisable(GL_BLEND);
        return;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_BLENDING[blend][0], GL_BLENDING[blend][1]);
}

bool  Gl2d_Impl::_SetMatrix(float* matrix)
{
    bool changed = false;
    LOOP(i, GL2D_MATRIX_ARRAY_LENGTH)
    {
        changed = _pfIdentity[i] == matrix[i] ? false : true;
        _pfIdentity[i] = matrix[i];
    }
    //glUniformMatrix4fv( _i32ShaderMatrix, 1, GL_FALSE, _pfIdentity);
    return changed;
}

void Gl2d_Impl::_MulMatrix( float x, float y, float* matrix, float &x_out, float &y_out )
{
    x_out = x*matrix[0] + y*matrix[2] + matrix[4];
    y_out = x*matrix[1] + y*matrix[3] + matrix[5];
}

void CALL Gl2d_Impl::Transform_rotate(float d)
{
    mTransformMatrix.rotate(d);
}

void CALL Gl2d_Impl::Transform_rotate3d(float rx, float ry, float rz)
{
    mTransformMatrix.rotate3d(rx, ry, rz);
}

void CALL Gl2d_Impl::Transform_rotate3d(float x, float y, float z, float angle)
{
    mTransformMatrix.rotate3d(x, y, z, angle);
}

void CALL Gl2d_Impl::Transform_rotateFromVector(float x, float y)
{
    mTransformMatrix.rotateFromVector(x, y);
}

void CALL Gl2d_Impl::Transform_translate(float tx, float ty)
{
    mTransformMatrix.translate(tx, ty);
}

void CALL Gl2d_Impl::Transform_translate3d(float tx, float ty, float tz)
{
    mTransformMatrix.translate3d(tx, ty, tz);
}

void CALL Gl2d_Impl::Transform_scale(float s)
{
    mTransformMatrix.scale(s);
}

void CALL Gl2d_Impl::Transform_scaleNonUniform(float sx, float sy)
{
    mTransformMatrix.scaleNonUniform(sx, sy);
}

void CALL Gl2d_Impl::Transform_scale3d(float sx, float sy, float sz)
{
    mTransformMatrix.scale3d(sx, sy, sz);
}

void CALL Gl2d_Impl::Transform_useMatrix2d(float m11, float m12, float m21, float m22,
                                           float dx, float dy)
{
    mTransformMatrix.multiply(m11,m12,m21,m22,dx,dy);
}


HTEXTURE CALL Gl2d_Impl::Gfx_RenderShadow(HTARGET shadowTaget, HTARGET shadowTaget_bak,
                                          HEFFECT effectWithBlur, int x, int y, int w, int h,
                                          float blur, int targetWidth, int targetHeight,
                                          int OffX, int OffY,
                                          int r, int g, int b, int a)
{
    float xmax = x + w;
    float ymax = y + h;

    float shadowLeft = x;
    float shadowTop = y;
    int shadowWidth = w;
    int shadowHeight = h;

    Effect_Active(effectWithBlur, true);
    Effect_SetFloatv2(effectWithBlur, targetWidth, targetHeight, BLUE_TEX_SIZE);

    HTEXTURE shadowTexture = Target_GetTexture(shadowTaget);

    int bluri = (int)(blur + 0.5) * 20;

    float deltaX = w * 1.0 * bluri / (w + 2 * bluri) - 0.5;
    float deltaY = h * 1.0 * bluri / (h + 2 * bluri) - 0.5;

    shadowLeft += deltaX;
    shadowTop += deltaY;
    shadowWidth -= 2 * deltaX;
    shadowHeight -= 2 * deltaY;

    float tx1 = (float)x / (float)targetWidth;
    float tx2 = (float)xmax / (float)targetWidth;
    float ty1 = (float)y / (float)targetHeight;
    float ty2 = (float)ymax / (float)targetHeight;

    Target_Active(shadowTaget_bak, targetWidth, targetHeight);

    Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    Gfx_Clear();

    bool isUseTransform = bUseTransformation;
    bUseTransformation = false;

    Gfx_RenderRectExTexture(shadowLeft,                 // left
                            shadowTop,                  // top
                            shadowLeft + shadowWidth,   // right
                            shadowTop  + shadowHeight,  // bottom
                            tx1, ty1, tx2, ty2,
                            r, g, b, 255,
                            1, shadowTexture );
#ifdef __LINUXx86__
    Gfx_FlushRenders();
#endif
    Gfx_SwapBuffer();

    shadowTexture = Target_GetTexture(shadowTaget_bak);

    Target_ResetToScreen();

    float left , top, right, bottom;
    left = x - bluri;
    top = y - bluri;
    right = xmax + bluri;
    bottom = ymax + bluri;

    tx1 = (float)(x) / (float)targetWidth;
    tx2 = (float)xmax / (float)targetWidth;
    ty1 = (float)(y) / (float)targetHeight;
    ty2 = (float)ymax / (float)targetHeight;
//for offx and offy not transform
    if (isUseTransform)
    {
        float x1_out, y1_out, x2_out, y2_out, x3_out, y3_out, x4_out, y4_out;
        mTransformMatrix.map(left, top, x1_out, y1_out);
        mTransformMatrix.map(right, top, x2_out, y2_out);
        mTransformMatrix.map(right, bottom, x3_out, y3_out);
        mTransformMatrix.map(left, bottom, x4_out, y4_out);

        float tx[4], ty[4];
        int rr[4], gg[4], bb[4], aa[4];

        tx[0] = tx1;            tx[1] = tx2;
        tx[2] = tx2;            tx[3] = tx1;
        ty[0] = 1.0 - ty1;      ty[1] = 1.0 - ty1;
        ty[2] = 1.0 - ty2;      ty[3] = 1.0 - ty2;

        for (int ii = 0; ii < 4; ++ii)
        {
            rr[ii] = r;
            gg[ii] = g;
            bb[ii] = b;
            aa[ii] = a;
        }
        bUseTransformation = false;
        Gfx_RenderQuadEx(x1_out + OffX, y1_out + OffY, x2_out+ OffX, y2_out + OffY,
                         x3_out + OffX, y3_out + OffY, x4_out + OffX, y4_out + OffY,
                         tx, ty, rr, gg, bb, aa, 1, (HTEXTURE)shadowTexture);
    }
    else
    {
        Gfx_RenderRectExTexture(left + OffX ,       // left
                                top + OffY,         // top
                                right + OffX,       // right
                                bottom + OffY,      // bottom
                                tx1, ty1, tx2, ty2,
                                r, g, b, a,
                                1, shadowTexture );
    }
    bUseTransformation = isUseTransform;

    return shadowTexture;
}

