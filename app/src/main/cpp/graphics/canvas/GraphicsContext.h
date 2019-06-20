/*******************
*this class is for drawing graphics
*******************/

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "Color.h"
#include <ObjectWrap.h>
#include <v8.h>
#include <gl2d_impl.h>
#include <sprite2d.h>
#include "Path.h"
#include "Font.h"
#include "ImageData.h"
#include "CanvasStyle.h"
#include "CameraIns.h"
#include "RenderingContext.h"

#include <stack>

namespace aliyun
{
	class WebViewAliyun;
}

namespace DCanvas
{

enum LineCap
{
    ButtCap   = 1,
    RoundCap  = 2,
    SquareCap = 3
};

enum LineJoin
{
    MiterJoin = 0,
    BevelJoin = 1,
    RoundJoin = 2
};

class Image;

class GraphicsContext : public RenderingContext
{
	friend class V8GraphicsContext;
public:
    virtual bool            is2d() const {return true;}
    virtual void            onFrame();
    virtual void            destroy();
    static GraphicsContext* create(int canvasId);
    static GraphicsContext* create(int x, int y, int width, int height, NativeEnv*& nativeEnv, int canvasId);
	static GraphicsContext* create(int x, int y, int width, int height, NativeEnv*& nativeEnv);
    virtual ~GraphicsContext();

    void            setClearColor(float r, float g, float b, float a);
    void            clear();
    void            drawBox(float left, float right, float top, float bottom);
    void            drawBoxEx(float pos[4][2]);
    void            drawImage(float left, float right, float top, float bottom, float uv[4][2]);

    void            setColor(float r, float g, float b, float a);
    void            setColorEx(float color[4][4]);
    void            clearImage();

    void            setFillStyle(Color cl){ state().fillStyle.setColor(cl); }
    void            setFillStyle(CanvasPattern* pt){ state().fillStyle.setPattern(pt); }
    void            setFillStyle(CanvasGradient* gd){ state().fillStyle.setGradient(gd); }

    void            setStrokeStyle(Color cl){ state().strokeStyle.setColor(cl); }
    void            setStrokeStyle(CanvasPattern* pt){ state().strokeStyle.setPattern(pt); }
    void            setStrokeStyle(CanvasGradient* gd){ state().strokeStyle.setGradient(gd); }

    void            swapBuffer();
    void            swapBufferToScreen();

//font
    void            setFont(char* font) { m_font->setFontString(font); }
    void            fillText(char* text, float x, float y);
    void            strokeText(char* text, float x, float y);
    void            setTextBaseLine(char* baseline) { m_font->setBaseLine(baseline); }
    void            setTextAlign(TextAlign align) { m_font->setAlign(align); }

//rect
    void            fillRect  (float x, float y, float width, float height);
    void            clearRect (float x, float y, float width, float height);
    void            strokeRect(float x, float y, float width, float height);

//draw img to dx(position x), dy(position y) as dw(width), dh(height)  from x(source image position x), y(source image position y) with w(width from x,y), h(height from x,y)
    void            drawImage(Image* img, float x, float y, float w, float h, float dx, float dy, float dw, float dh);
    void            drawImageData(ImageData* img, float x, float y, float w, float h, float dx, float dy, float dw, float dh);
    void            drawTexture(HTEXTURE htex, float width, float height, float x, float y, float w, float h, float dx, float dy, float dw, float dh);
    void            drawCameraPreview(CameraIns* ins, float x, float y, float w, float h);
    void			drawWebView(aliyun::WebViewAliyun* wv, bool isChanged, float x, float y, float w, float h, float dx, float dy, float dw, float dh);

// path functions
    void            fill();
    void            stroke();
    void            clip();

    void            beginPath();
    void            closePath();

    void            moveTo(float x, float y);
    void            lineTo(float x, float y);
    int             arc(float x, float y, float r, float sa, float ea, bool anticlockwise );
    void            quadraticCurveTo(float cpx, float cpy, float x, float y);
    void            bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

//state
    void            setGlobalAlphaState(float val);
    float           getGlobalAlphaState();

//transform
    void            translate(float x, float y);
    void            scale(float x, float y);
    void            rotate(float angle);
    void            transform(float m11, float m12, float m21, float m22, float dx, float dy);
    void            setTransform(float m11, float m12, float m21, float m22, float dx, float dy);

    void            setLineWidth(float val);
    float           getLineWidth(){return state().lineWidth;}

    void            setLineCap(char* val);
    std::string     getLineCap();

    void            setLineJoin(char* val);
    std::string     getLineJoin();

    void            setMiterLimit(float val);
    float           getMiterLimit();

    void            setShadowColor(Color c);
    Color           getShadowColor();
    void            setShadowBlur(float);
    float           getShadowBlur();
    void            setShadowOffsetX(float);
    float           getShadowOffsetX();
    void            setShadowOffsetY(float);
    float           getShadowOffsetY();

    bool            isPointInPath(const float x, const float y);

    void            save();
    void            restore();

    class CanvasState
    {
    public:
        CanvasState();
        //fill style
        CanvasStyle fillStyle;
        //stroke style
        CanvasStyle strokeStyle;

        // Alpha channel of all
        float globalAlpha;
        //
        short textAlignment;
        short textBaseline;

        float lineWidth;

        //shadow
        bool  hasShadowBlur;
        Color shadowColor;
        float shadowBlur;
        double shadowOffsetX;
        double shadowOffsetY;
        bool shadowPath;

        //transform
        AliTransformationMatrix  transform;

        //linecap
        LineCap lineCap;
        
        //lineJoin
        LineJoin lineJoin;
        float miterLimit;
    };

    Gl2dSprite* getSprite();

    CanvasState& state() { return m_stateStack.top(); }
    const CanvasState& state() const { return m_stateStack.top(); }

    GLContextRef GetContext() { return m_context; }

    int getCanvasId(){return m_canvasId;}    
    void setCanvasId(int canvasId){m_canvasId = canvasId;}
#if NATIVE_SURFACE
    virtual void setPosition(int x, int y);//get_gradient_texture
    virtual void setSize(int w , int h);
    virtual void setAlpha(float alpha);
#endif // NATIVE_SURFACE

private:
    GraphicsContext(int x, int y, int width, int height, NativeEnv*& nativeEnv);
    GraphicsContext(int canvasId);
    GraphicsContext(int x, int y, int width, int height, NativeEnv*& nativeEnv, int canvasId);

    HTEXTURE getPatternTexture(CanvasPattern* cp);
    HTEXTURE getGradientTexture(CanvasStyle& c_style);

    void init(int x, int y, int width, int height);

    GLContextRef            m_context;
    HTARGET                 m_target;
    HTARGET                 m_target_bak;

    // Effect0 no texture, 1 is with texture
    HEFFECT                 m_effectNoTexture;
    HEFFECT                 m_effectWithTexture;
    HEFFECT                 m_effectWithBlur;

    // the current effect
    HEFFECT                 m_curEff;

    // the current color
    int                     m_CurRed[4];
    int                     m_CurGreen[4];
    int                     m_CurBlue[4];
    int                     m_CurAlpha[4];

    // the current Texture
    HTEXTURE                m_CurTex;
    Gl2dSprite*             m_Sprite;

//state stack
    CanvasState             m_currentState;
    std::stack<CanvasState> m_stateStack;

//path
    Path                    m_path;

    DCFont*                 m_font;
    HTEXTURE                m_fontTex[MAX_TEXTURE];
    int                     m_fontCounter;
    bool                    m_bHasClip;

    int                     m_x;
    int                     m_y;
    int                     m_width;
    int                     m_height;

    HTEXTURE                m_cameraTexture;
    int                     m_cameraWidth;
    int                     m_cameraHeight;
    
    int                     m_canvasId;
};

} // namespace DCanvas

#endif // GRAPHICSCONTEXT_H
