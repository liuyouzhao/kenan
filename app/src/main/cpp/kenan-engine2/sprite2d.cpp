#include "MathExtras.h"
#include "gl2d.h"
#include "gl2d_impl.h"
#include "sprite2d.h"

#if IN_LINUXx86
#include <string.h>
#elif IN_ANDROID
#include <string.h>
#endif

Gl2dSprite::Gl2dSprite( HTEXTURE texture, float texx1, float texy1, float texx2, float texy2,
                        int w, int h, Gl2d_Impl* gl2d )
{
    this->nwidth = w;
    this->nheight = h;
    this->gl2d = gl2d;
    float _w = (float)w / (float)gl2d->nScreenWidth;
    float _h = (float)h / (float)gl2d->nScreenHeight;

    width=_w; height=_h;

    hotX=0; hotY=0;
    bXFlip = false;
    bYFlip = false;
    quad.tex = texture;

    quad.v[0].tx = texx1; quad.v[0].ty = texy1;
    quad.v[1].tx = texx2; quad.v[1].ty = texy1;
    quad.v[2].tx = texx2; quad.v[2].ty = texy2;
    quad.v[3].tx = texx1; quad.v[3].ty = texy2;

    quad.v[0].z =
    quad.v[1].z =
    quad.v[2].z =
    quad.v[3].z = 0.5f;

    quad.v[0].r =
    quad.v[1].r =
    quad.v[2].r =
    quad.v[3].r = 1.0f;

    quad.v[0].g =
    quad.v[1].g =
    quad.v[2].g =
    quad.v[3].g = 1.0f;

    quad.v[0].b =
    quad.v[1].b =
    quad.v[2].b =
    quad.v[3].b = 1.0f;

    quad.v[0].a =
    quad.v[1].a =
    quad.v[2].a =
    quad.v[3].a = 1.0f;

    quad.blend = BLEND_DEFAULT;
}

Gl2dSprite::Gl2dSprite(HTEXTURE texture, float texx1, float texy1, float texx2, float texy2,
                       float w, float h, Gl2d_Impl* gl2d, int blend)
{
	this->nwidth = (int)w;
	this->nheight = (int)h;
    this->gl2d = gl2d;
    this->SetBlendMode(blend);
    float _w = (float)w / (float)gl2d->nScreenWidth;
	float _h = (float)h / (float)gl2d->nScreenHeight;

	width=_w; height=_h;

    hotX=0; hotY=0;
    bXFlip = false;
    bYFlip = false;
    quad.tex = texture;

    quad.v[0].tx = texx1; quad.v[0].ty = texy1;
    quad.v[1].tx = texx2; quad.v[1].ty = texy1;
    quad.v[2].tx = texx2; quad.v[2].ty = texy2;
    quad.v[3].tx = texx1; quad.v[3].ty = texy2;

    quad.v[0].z =
    quad.v[1].z =
    quad.v[2].z =
    quad.v[3].z = 0.5f;

    quad.v[0].r =
    quad.v[1].r =
    quad.v[2].r =
    quad.v[3].r = 1.0f;

    quad.v[0].g =
    quad.v[1].g =
    quad.v[2].g =
    quad.v[3].g = 1.0f;

    quad.v[0].b =
    quad.v[1].b =
    quad.v[2].b =
    quad.v[3].b = 1.0f;

    quad.v[0].a =
    quad.v[1].a =
    quad.v[2].a =
    quad.v[3].a = 1.0f;

    quad.blend=BLEND_DEFAULT;
}

Gl2dSprite::Gl2dSprite(const Gl2dSprite &spr)
{
    memcpy(this, &spr, sizeof(Gl2dSprite));
}

void Gl2dSprite::RenderInt(int x, int y)
{
    float _x = (float)x / (float)gl2d->nScreenWidth;
    float _y = (float)y / (float)gl2d->nScreenHeight;
    Render(_x,_y);
}

void Gl2dSprite::Render(float x, float y)
{
    float tempx1, tempy1, tempx2, tempy2;

    tempx1 = x - hotX;
    tempy1 = y - hotY;
    tempx2 = x + width - hotX;
    tempy2 = y + height - hotY;

    quad.v[0].x = tempx1; quad.v[0].y = tempy1;
    quad.v[1].x = tempx2; quad.v[1].y = tempy1;
    quad.v[2].x = tempx2; quad.v[2].y = tempy2;
    quad.v[3].x = tempx1; quad.v[3].y = tempy2;

    gl2d->Gfx_RenderQuad(&quad);
}

void Gl2dSprite::RenderExInt( int x, int y, float rot,
                              float hscale/*=1.0f*/, float vscale/*=0.0f*/ )
{
    RenderEx(x, y, rot, hscale, vscale);
}


void Gl2dSprite::RenderExIntWH( int x, int y, int w, int h, float rot,
                                float hscale/*=1.0f*/, float vscale/*=0.0f*/ )
{
    this->nwidth = w;
    this->nheight = h;
    RenderEx(x, y, rot, hscale, vscale);
}

void Gl2dSprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
    float tx1, ty1, tx2, ty2;
    float sint, cost;

    float w_h = (float)gl2d->nScreenWidth / (float)gl2d->nScreenHeight;
    float h_w = (float)gl2d->nScreenHeight / (float)gl2d->nScreenWidth;

    if (vscale - 0.0f > 0.000001f || vscale - 0.0f < -0.000001f)
    {
        if(rot != 0.0f)
        {
            vscale = vscale*h_w;
        }
        else
        {
            vscale = vscale;
        }
    }
    else
    {
        vscale = hscale;
    }

    tx1 = -(float)nhotX * hscale;
    ty1 = -(float)nhotY * vscale;
    tx2 = ((float)nwidth - nhotX ) * hscale;
    ty2 = ((float)nheight - nhotY) * vscale;

    if (rot - 0.0f > 0.000001f || rot - 0.0f < -0.000001f)
    {
        cost = cosf(rot);
        sint = sinf(rot);

        quad.v[0].x  = (tx1*cost - ty1*sint) + x;
        quad.v[0].y  = (tx1*sint + ty1*cost) + y;

        quad.v[1].x  = (tx2*cost - ty1*sint) + x;
        quad.v[1].y  = (tx2*sint + ty1*cost) + y;

        quad.v[2].x  = (tx2*cost - ty2*sint) + x;
        quad.v[2].y  = (tx2*sint + ty2*cost) + y;

        quad.v[3].x  = (tx1*cost - ty2*sint) + x;
        quad.v[3].y  = (tx1*sint + ty2*cost) + y;
    }
    else
    {
        quad.v[0].x = tx1 + x; quad.v[0].y = y + ty1;
        quad.v[1].x = tx2 + x; quad.v[1].y = y + ty1;
        quad.v[2].x = tx2 + x; quad.v[2].y = y + ty2;
        quad.v[3].x = tx1 + x; quad.v[3].y = y + ty2;
    }

    int r[4] = {0};
    int g[4] = {0};
    int b[4] = {0};
    int a[4] = {0};
    float tx[4] = {0.0f};
    float ty[4] = {0.0f};
    LOOP(i, 4)
    {
        r[i] = quad.v[i].r * 255;
        g[i] = quad.v[i].g * 255;
        b[i] = quad.v[i].b * 255;
        a[i] = quad.v[i].a * 255;
        tx[i] = quad.v[i].tx;
        ty[i] = quad.v[i].ty;
    }

    gl2d->Gfx_RenderQuadEx(quad.v[0].x, quad.v[0].y, quad.v[1].x, quad.v[1].y,
                           quad.v[2].x, quad.v[2].y, quad.v[3].x, quad.v[3].y,
                           tx, ty, r, g, b, a, quad.blend, quad.tex );
}

void Gl2dSprite::RenderStretch(float x1, float y1, float x2, float y2)
{
    quad.v[0].x = x1; quad.v[0].y = y1;
    quad.v[1].x = x2; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x1; quad.v[3].y = y2;

    gl2d->Gfx_RenderQuad(&quad);
}

void Gl2dSprite::RenderStretchInt( int x1, int y1, int x2, int y2 )
{
    float _x1 = (float)x1 / (float)gl2d->nScreenWidth;
    float _y1 = (float)y1 / (float)gl2d->nScreenHeight;
    float _x2 = (float)x2 / (float)gl2d->nScreenWidth;
    float _y2 = (float)y2 / (float)gl2d->nScreenHeight;

    wrapOriginal(_x1, _y1);
    wrapOriginal(_x2, _y2);

    RenderStretch(_x1, _y1, _x2, _y2);
}

void Gl2dSprite::Render4VInt(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
    float _x0 = (float)x0 / (float)gl2d->nScreenWidth;
    float _y0 = (float)y0 / (float)gl2d->nScreenHeight;
    float _x1 = (float)x1 / (float)gl2d->nScreenWidth;
    float _y1 = (float)y1 / (float)gl2d->nScreenHeight;
    float _x2 = (float)x2 / (float)gl2d->nScreenWidth;
    float _y2 = (float)y2 / (float)gl2d->nScreenHeight;
    float _x3 = (float)x3 / (float)gl2d->nScreenWidth;
    float _y3 = (float)y3 / (float)gl2d->nScreenHeight;

    Render4V(_x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3);
}

void Gl2dSprite::Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    quad.v[0].x = x0; quad.v[0].y = y0;
    quad.v[1].x = x1; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x3; quad.v[3].y = y3;

    gl2d->Gfx_RenderQuad(&quad);
}

void Gl2dSprite::SetFlip(bool bX, bool bY)
{
    float tx, ty;

    if(bX != bXFlip)
    {
        tx = quad.v[0].tx; quad.v[0].tx = quad.v[1].tx; quad.v[1].tx = tx;
        ty = quad.v[0].ty; quad.v[0].ty = quad.v[1].ty; quad.v[1].ty = ty;
        tx = quad.v[3].tx; quad.v[3].tx = quad.v[2].tx; quad.v[2].tx = tx;
        ty = quad.v[3].ty; quad.v[3].ty = quad.v[2].ty; quad.v[2].ty = ty;
        bXFlip = !bXFlip;
    }

    if(bY != bYFlip)
    {
        tx = quad.v[0].tx; quad.v[0].tx = quad.v[3].tx; quad.v[3].tx = tx;
        ty = quad.v[0].ty; quad.v[0].ty = quad.v[3].ty; quad.v[3].ty = ty;
        tx = quad.v[1].tx; quad.v[1].tx = quad.v[2].tx; quad.v[2].tx = tx;
        ty = quad.v[1].ty; quad.v[1].ty = quad.v[2].ty; quad.v[2].ty = ty;
        bYFlip = !bYFlip;
    }
}

void Gl2dSprite::SetTexture(HTEXTURE tex)
{
    quad.tex=tex;
}

void Gl2dSprite::SetTextureRect(float x, float y, float w, float h, float tex_width, float tex_height)
{
    float tx1, ty1, tx2, ty2;
    bool bX,bY;

    tx = x;
    ty = y;
    width = w;
    height = h;

    tx1 = tx / tex_width;
    ty1 = ty / tex_height;
    tx2 = (tx + w) / tex_width;
    ty2 = (ty + h) / tex_height;

    quad.v[0].tx = tx1; quad.v[0].ty = ty1;
    quad.v[1].tx = tx2; quad.v[1].ty = ty1;
    quad.v[2].tx = tx2; quad.v[2].ty = ty2;
    quad.v[3].tx = tx1; quad.v[3].ty = ty2;

    bX = bXFlip;
    bY = bYFlip;
    bXFlip = false;
    bYFlip = false;
    SetFlip(bX, bY);

    /////////////////// consider animation frame somehow! ////////////////
}

void Gl2dSprite::SetColor(float r, float g, float b, float a, int i)
{
    if(i!=-1)
    {
        quad.v[i].r=r;quad.v[i].g=g;quad.v[i].b=b;quad.v[i].a=a;
    }
    else
    {
        quad.v[0].r = quad.v[1].r = quad.v[2].r = quad.v[3].r = r;
        quad.v[0].g = quad.v[1].g = quad.v[2].g = quad.v[3].g = g;
        quad.v[0].b = quad.v[1].b = quad.v[2].b = quad.v[3].b = b;
        quad.v[0].a = quad.v[1].a = quad.v[2].a = quad.v[3].a = a;
    }
}

void Gl2dSprite::SetZ(float z, int i)
{
    if(i!=-1) quad.v[i].z=z;
    else
    {
        quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z=z;
    }
}

void Gl2dSprite::SetHotSpotInt( int x, int y )
{
    float _x = (float) x / (float)nwidth * width;
    float _y = (float) y / (float)nheight * height;
    hotX = _x;
    hotY = _y;
    nhotX = x;
    nhotY = y;
}

void Gl2dSprite::SetHotSpot( float x, float y )
{
    hotX = x;
    hotY = y;
}

void Gl2dSprite::SetUV(float tu1, float tv1, float tu2, float tv2)
{
    quad.v[0].tx = tu1;    quad.v[0].ty = tv1;
    quad.v[1].tx = tu2;    quad.v[1].ty = tv1;
    quad.v[2].tx = tu2;    quad.v[2].ty = tv2;
    quad.v[3].tx = tu1;    quad.v[3].ty = tv2;
}

void Gl2dSprite::GetTextureRect(float *x, float *y, float *w, float *h) const
{
    *x = tx;
    *y = ty;
    *w = width;
    *h = height;
}

gl2dColor Gl2dSprite::GetColor(int i) const
{
    return gl2dColor(quad.v[i].r, quad.v[i].g, quad.v[i].b, quad.v[i].a);
}

gl2dRect* Gl2dSprite::GetBoundingBox(float x, float y, gl2dRect *rect) const
{
    rect->Set(x - hotX, y - hotY, x - hotX + width, y - hotY + height);
    return rect;
}
