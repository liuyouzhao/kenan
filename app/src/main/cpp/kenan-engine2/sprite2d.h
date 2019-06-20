/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** hgeSprite helper class header
*/

#ifndef ALISPRITE_H
#define ALISPRITE_H

#include "gl2d.h"
#include "gl2d_impl.h"

/*
** HGE Sprite class
*/
class Gl2dSprite
{
public:
    Gl2dSprite(HTEXTURE texture,
               float texx1, float texy1,
               float texx2, float texy2,
               float w, float h, Gl2d_Impl* gl2d, int blend = 1);

    Gl2dSprite(HTEXTURE texture,
               float texx1, float texy1,
               float texx2, float texy2,
               int w, int h,  Gl2d_Impl* gl2d);

    Gl2dSprite(const Gl2dSprite &spr);
    ~Gl2dSprite() {}

    void        Render(float x, float y);
    void        RenderInt(int x,int y);
    void        RenderEx(float x, float y, float rot,
                         float hscale=1.0f, float vscale=0.0f);

    void        RenderExInt(int x, int y, float rot,
                            float hscale=1.0f, float vscale=0.0f);

    void        RenderExIntWH( int x, int y, int w, int h, float rot,
                               float hscale =1.0f, float vscale = 0.0f );

    void        RenderStretch(float x1, float y1, float x2, float y2);
    void        RenderStretchInt(int x1, int y1, int x2, int y2);

    void        Render4V(float x0, float y0, float x1, float y1, float x2,
                         float y2, float x3, float y3);

    void        Render4VInt(int x0, int y0, int x1, int y1, int x2,
                            int y2, int x3, int y3);

    void        SetTexture(HTEXTURE tex);
    void        SetUV(float tu1, float tv1, float tu2, float tv2);
    void        SetTextureRect(float x, float y, float w, float h,
                               float tex_width, float tex_height);

    void        SetColor(float r, float g, float b, float a, int i);
    void        SetZ(float z, int i=-1);
    void        SetBlendMode(int blend) { quad.blend = blend; }
    void        SetHotSpotInt(int x, int y);
    void        SetHotSpot(float x, float y);
    void        SetFlip(bool bX, bool bY);

    HTEXTURE    GetTexture() const { return quad.tex; }
    void        GetTextureRect(float *x, float *y, float *w, float *h) const;

    gl2dColor   GetColor(int i = 0) const;

    float       GetZ(int i=0) const { return quad.v[i].z; }
    int         GetBlendMode() const { return quad.blend; }
    void        GetHotSpot(float *x, float *y) const { *x=hotX; *y=hotY; }
    void        GetFlip(bool *bX, bool *bY) const { *bX=bXFlip; *bY=bYFlip; }

    gl2dRect*   GetBoundingBox(float x, float y, gl2dRect *rect) const;

    float       GetWidth() const { return width; }
    float       GetHeight() const { return height; }

protected:
    Gl2dSprite();
    Gl2d_Impl   *gl2d;

    gl2dQuad    quad;
    float       tx, ty, width, height;
    int         nwidth, nheight;
    float       tex_width, tex_height;
    float       hotX, hotY;
    int         nhotX, nhotY;
    bool        bXFlip, bYFlip;
};

#endif // ALISPRITE_H
