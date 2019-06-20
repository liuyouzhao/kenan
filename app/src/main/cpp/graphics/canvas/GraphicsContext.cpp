/********************************
 *
 *implementation for graphics
 *used to render on the screen
 * ********************************/
#include "defines.h"
#include "GraphicsContext.h"
#include "DirectCanvas.h"
#include "Color.h"
#include "Image.h"
#include "ImageData.h"

#include "Path.h"
#include "scan_path.h"

//#include "WebViewAliyun.h"

#undef LOG_TAG
#define LOG_TAG    "GraphicsContext"

namespace DCanvas
{

GraphicsContext* GraphicsContext::create(int canvasId)
{
    return new GraphicsContext(canvasId);
}

GraphicsContext* GraphicsContext::create(int x, int y, int width, int height, NativeEnv*& nativeEnv, int canvasId)
{
    LOGD("create GraphicsContext!");
    return new GraphicsContext(x, y, width, height, nativeEnv, canvasId);
}

GraphicsContext* GraphicsContext::create(int x, int y, int width, int height, NativeEnv*& nativeEnv)
{
    LOGD("create GraphicsContext!");
    return new GraphicsContext(x, y, width, height, nativeEnv);
}

GraphicsContext::GraphicsContext(int x, int y, int width, int height, NativeEnv*& nativeEnv)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
{
    while (!m_stateStack.empty())
        m_stateStack.pop();

    m_stateStack.push(m_currentState);
    m_context = new Gl2d_Impl();
    if (!nativeEnv)
    {
        LOGE("x %d, y %d, width %d, height %d", x, y, width, height);
        m_context->System_InitiateNative(x, y, width, height, 0x00020000);
        LOGD("-----------------GraphicsContext::init() finished!!");
        nativeEnv = m_context->getNativeEnv();
    }
    else
    {
        m_context->setNativeEnv(nativeEnv);
    }
    m_context->_Init(width, height);
    init(x, y, width, height);
}


GraphicsContext::GraphicsContext(int canvasId)
{
    while (!m_stateStack.empty())
        m_stateStack.pop();
    m_canvasId = canvasId;
    m_stateStack.push(m_currentState);
    init(0, 0, CLOUDX_SCREEN_WIDTH, CLOUDX_SCREEN_HEIGHT);
}

GraphicsContext::GraphicsContext(int x, int y, int width, int height, NativeEnv*& nativeEnv, int canvasId)
    : m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_canvasId(canvasId)
{
    while (!m_stateStack.empty())
        m_stateStack.pop();

    m_stateStack.push(m_currentState);
    m_context = new Gl2d_Impl();
    if (!nativeEnv)
    {
        LOGE("x %d, y %d, width %d, height %d", x, y, width, height);
        m_context->System_Initiate(x, y, width, height, 0x00020000);
        LOGD("-----------------GraphicsContext::init() finished!!");
        nativeEnv = m_context->getNativeEnv();
    }
    else
    {
        m_context->setNativeEnv(nativeEnv);
    }
    m_context->_Init(width, height);
    init(x, y, width, height);
}

void GraphicsContext::init(int x, int y, int width, int height)
{
    m_target = m_context->Target_Create(width, height);
    m_target_bak = m_context->Target_Create(width, height);

    m_effectWithTexture = m_context->Effect_Load(GL2D_NORMAL);
    m_effectNoTexture = m_context->Effect_Load(GL2D_NORMAL_NOTEXTURE);
    m_effectWithBlur = m_context->Effect_Load(GL2D_BLUR);

    m_curEff = m_effectNoTexture;
    for (int i = 0; i < 4; i ++)
    {
        m_CurRed[i] = m_CurGreen[i] = m_CurBlue[i] = m_CurAlpha[i] = 255;

    }
    m_CurTex = NO_TEXTURE;

    m_context->Effect_Active(m_effectNoTexture, false);

    m_Sprite = new Gl2dSprite(NO_TEXTURE, 0.0, 0.0, 1.0, 1.0, 0, 0, m_context);

    // create font
    // the 1024 is the max hash length
    m_font = DCFont::create(MAX_TEXTURE);
    for (int i = 0; i < MAX_TEXTURE; i ++)
        m_fontTex[i] = NO_TEXTURE;

    m_fontCounter = 0;
    m_bHasClip = false;
    m_cameraTexture = NO_TEXTURE;
    m_cameraWidth = 0;
    m_cameraHeight = 0;
}

GraphicsContext::CanvasState::CanvasState()
    : fillStyle(CanvasStyle(Color("black")))
    , strokeStyle(CanvasStyle(Color("black")))
    , globalAlpha(1.0)
    , lineWidth(1.0)
    , hasShadowBlur(false)
    , shadowPath(false)
    , lineCap(ButtCap)
    , lineJoin(MiterJoin)
    , miterLimit(0)
{
}

GraphicsContext::~GraphicsContext()
{
    LOGD("this is the destructor of graphicsContext!");
    destroy();
    LOGD("destructor of graphicsContext ok!");
}

///**
//* register to javascript call
//**/
//bool GraphicsContext::Regist(Persistent<v8::Context> _exec_context)
//{
//    return false;
//}

void GraphicsContext::setClearColor(float r, float g, float b, float a)
{
    m_context->Gfx_SetClearColor((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, (float)a / 255.0);
}

void GraphicsContext::clear()
{
    m_context->Gfx_Clear();
}

void GraphicsContext::drawBoxEx(float pos[4][2])
{
    if (m_CurTex == NO_TEXTURE)
    {
        m_context->Effect_Active(m_effectNoTexture, false);
    }
    else
    {
        m_context->Effect_Active(m_effectWithTexture, true);
    }
    float TL = 0.0f;
    float TT = 0.0f;
    float TR = 0.0f;
    float TB = 0.0f;
    float tx[4] = {TL, TR, TR, TL};
    float ty[4] = {TT, TT, TB, TB};

    m_context->Gfx_RenderQuadEx(pos[0][0], pos[0][1],
                            pos[1][0], pos[1][1],
                            pos[2][0], pos[2][1],
                            pos[3][0], pos[3][1],
                            tx, ty, m_CurRed, m_CurGreen, m_CurBlue, m_CurAlpha, 1, m_CurTex);

}

void GraphicsContext::drawImageData(ImageData* img, float x, float y, float w, float h, float dx, float dy, float dw, float dh)
{
    HTEXTURE htex = img->GetTexture();
    int width = img->Width();
    int height = img->Height();

    if (NO_TEXTURE == htex)
    {
        htex = m_context->Texture_Load(img->Data(), width, height);
        if (htex == NO_TEXTURE)
        {
            LOGE("wrong-----wrong-- wrong");
        }
        img->SetTexture(htex, m_context);
    }

    float tx1 = (float)x/(float)width;
    float tx2 = (float)(x+w)/(float)width;
    float ty1 = (float)y/(float)height;
    float ty2 = (float)(y+h)/(float)height;

    m_context->Effect_Active(m_effectWithTexture, true);
    Gl2dSprite* gl_Sprite = getSprite();
    gl_Sprite->SetTexture(htex);
    gl_Sprite->SetBlendMode(1);
    gl_Sprite->SetHotSpot(0,0);
    gl_Sprite->SetUV(tx1, ty1, tx2, ty2);

    Color fillColor = state().fillStyle.color();
    float alpha = fillColor.alpha() * state().globalAlpha;
    
    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();
        //init the target
        ////m_context->Gfx_BeginShadow(m_target, m_width, m_height);

        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectWithTexture, true);
        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render shadow
        m_context->Transform_disable();
        gl_Sprite->SetColor(1.0f, 1.0f, 1.0f, 1.0f, -1);
        gl_Sprite->RenderExIntWH(dx, dy, dw, dh, 0.0f);
        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();

        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                            dx, dy, dw, dh, state().shadowBlur,
                            m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                            state().shadowColor.red(), state().shadowColor.green(),
                            state().shadowColor.blue(),
                            state().shadowColor.alpha() * alpha / 255.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectWithTexture, true);
    }
    
    gl_Sprite->SetColor(1.0f, 1.0f, 1.0f, alpha / 255.0f, -1);

    gl_Sprite->RenderExIntWH(dx, dy, dw, dh, 0.0f);

    m_context ->Effect_Active(m_effectNoTexture, false);
}

void GraphicsContext::drawImage(Image* img, float x, float y, float w, float h, float dx, float dy, float dw, float dh)
{
    if (img->getImageData() == NULL)
    {
        return ;
    }
    int width = img->getWidth();
    int height = img->getHeight();

    HTEXTURE htex = img->getTexture();
    if (NO_TEXTURE == htex)
    {
        htex = m_context->Texture_Load(img->getImageData(), width, height);
        img->setTexture(htex, m_context, m_canvasId);
    }

    float tx1 = (float)x/(float)width;
    float tx2 = (float)(x+w)/(float)width;
    float ty1 = (float)y/(float)height;
    float ty2 = (float)(y+h)/(float)height;

    m_context->Effect_Active(m_effectWithTexture, true);
    Gl2dSprite* gl_Sprite = getSprite();
    gl_Sprite->SetTexture(htex);
    gl_Sprite->SetBlendMode(1);
    gl_Sprite->SetHotSpot(0,0);
    gl_Sprite->SetUV(tx1, ty1, tx2, ty2);

    Color fillColor = state().fillStyle.color();
    float alpha = fillColor.alpha() * state().globalAlpha;
    
    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();
        //init the target
        ////m_context->Gfx_BeginShadow(m_target, m_width, m_height);

        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectWithTexture, true);
        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render shadow
        m_context->Transform_disable();
        gl_Sprite->SetColor(1.0f, 1.0f, 1.0f, 1.0f, -1);
        gl_Sprite->RenderExIntWH(dx, dy, dw, dh, 0.0f);
        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();

        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                            dx, dy, dw, dh, state().shadowBlur,
                            m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                            state().shadowColor.red(), state().shadowColor.green(),
                            state().shadowColor.blue(),
                            state().shadowColor.alpha() * alpha / 255.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectWithTexture, true);
    }

    gl_Sprite->SetColor(1.0f, 1.0f, 1.0f, alpha / 255.0f, -1);
    gl_Sprite->RenderExIntWH(dx, dy, dw, dh, 0.0f);

    m_context ->Effect_Active(m_effectNoTexture, false);
}

/*void GraphicsContext::drawWebView(aliyun::WebViewAliyun* wv, bool isChanged, float x, float y, float w, float h, float dx, float dy, float dw, float dh)
{
	HTEXTURE hTex = wv->getCacheTexture();
	if(!isChanged && hTex != NO_TEXTURE && hTex != -1)
	{
		m_context->Transform_enable();
		drawTexture(hTex, wv->getWidth(), wv->getHeight(), x, y, w, h, dx, dy, dw, dh);
		m_context->Transform_disable();
		return;
	}

	const unsigned char* pixels = wv->getCurrentPixels();
	if(pixels == NULL)
	{
		return;
	}
	unsigned int* data = new unsigned int[wv->getWidth() * wv->getHeight()];
	// here need to unsigned char* ---> unsigned int*
	// because SkBitmap::kARGB_8888_Config, so we changed the sequence
	unsigned int r = 0, g = 0, b = 0, a = 0;
	int c = 0;
	for (int i = wv->getHeight() - 1; i >= 0; i --)
	{
		for(int j = 0; j < wv->getWidth(); j ++)
		{
			int n = i * wv->getWidth() + j;
			r = 0x000000ff & pixels[4 * n];
			g = 0x000000ff & pixels[4 * n + 1];
			b = 0x000000ff & pixels[4 * n + 2];
			a = 0x000000ff & pixels[4 * n + 3];
			data[c++] =  r | (g << 8) | (b << 16) | (a << 24);
		}
		//LOGE("%d, %d, %d, %d", r, g, b, a);
	}
	if(hTex != NO_TEXTURE || hTex != -1)
	{
		m_context->Texture_Free(hTex);
	}
	hTex = m_context->Texture_Load(data, wv->getWidth(), wv->getHeight());
	wv->setCacheTexture(hTex);

	m_context->Transform_enable();
	drawTexture(hTex, wv->getWidth(), wv->getHeight(), x, y, w, h, dx, dy, dw, dh);
	m_context->Transform_disable();

	delete[] data;
}
*/
void GraphicsContext::drawTexture(HTEXTURE htex, float width, float height,
                                  float x, float y, float w, float h,
                                  float dx, float dy, float dw, float dh)
{
    float tx1 = (float)x/(float)width;
    float tx2 = (float)(x+w)/(float)width;
    float ty1 = (float)y/(float)height;
    float ty2 = (float)(y+h)/(float)height;

    m_context->Effect_Active(m_effectWithTexture, true);
    Gl2dSprite* gl_Sprite = getSprite();
    gl_Sprite->SetTexture(htex);
    gl_Sprite->SetBlendMode(1);
    gl_Sprite->SetHotSpot(0,0);
    gl_Sprite->SetUV(tx1, 1.0-ty1, tx2, 1.0-ty2);

    gl_Sprite->SetColor(1.0f, 1.0f, 1.0f,
                 state().globalAlpha, -1);
    gl_Sprite->RenderExIntWH(dx, dy, dw, dh, 0.0f);

    m_context ->Effect_Active(m_effectNoTexture, false);
}

void GraphicsContext::drawImage(float left, float right, float top, float bottom, float uv[4][2])
{
    if (m_CurTex == NO_TEXTURE)
    {
        m_context->Effect_Active(m_effectNoTexture, false);
    }
    else
    {
        m_context->Effect_Active(m_effectWithTexture, true);
    }
    float tx[4] = {uv[0][0], uv[1][0], uv[2][0], uv[3][0]};
    float ty[4] = {uv[0][1], uv[1][1], uv[2][1], uv[3][1]};

    m_context->Gfx_RenderQuadEx(left, top, right, top, right, bottom,
                                left, bottom, tx, ty, m_CurRed, m_CurGreen,
                                m_CurBlue, m_CurAlpha, 1, m_CurTex);
}

void GraphicsContext::setColor(float r, float g, float b, float a)
{
    for (int i = 0; i < 4; i ++)
    {
        m_CurRed[i] = r;
        m_CurGreen[i] = g;
        m_CurBlue[i] = b;
        m_CurAlpha[i] = a;
    }
}

std::string GraphicsContext::getLineCap()
{
    if (ButtCap == state().lineCap)
        return "butt";
    else if (RoundCap == state().lineCap)
        return "round";
    else if (SquareCap == state().lineCap)
        return "square";
    return "";
}

std::string GraphicsContext::getLineJoin()
{
    if (BevelJoin == state().lineJoin)
        return "bevel";
    else if (RoundJoin == state().lineJoin)
        return "round";
    else if (MiterJoin == state().lineJoin)
        return "miter";
    return "";
}

float GraphicsContext::getMiterLimit()
{
    return state().miterLimit;
}

void GraphicsContext::setColorEx(float color[4][4])
{
    for (int i = 0; i < 4; i ++)
    {
        m_CurRed[i]     = color[i][0];
        m_CurGreen[i]   = color[i][1];
        m_CurBlue[i]    = color[i][2];
        m_CurAlpha[i]   = color[i][3];
    }
}

void GraphicsContext::clearImage()
{
    m_CurTex = NO_TEXTURE;
}

void GraphicsContext::swapBuffer()
{
    m_context->Gfx_SwapBuffer();
}

void GraphicsContext::swapBufferToScreen()
{
    m_context->Gfx_SwapBufferToScreen();
}

void GraphicsContext::fillRect(float x, float y, float width, float height)
{
    CanvasStyle fillStyle = state().fillStyle;

    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();
        //init the target
        ////m_context->Gfx_BeginShadow(m_target, m_width, m_height);

        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectNoTexture, false);
        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render shadow
        m_context->Transform_disable();
        Color fillColor = fillStyle.color();
        float alpha = fillColor.alpha() * state().globalAlpha;
        m_context->Gfx_RenderRectEx(x, y, x + width, y + height,
                                    0, 0, 0, 255, 1);
        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();

        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                                    x, y, width, height, state().shadowBlur,
                                    m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                                    state().shadowColor.red(), state().shadowColor.green(),
                                    state().shadowColor.blue(),
                                    state().shadowColor.alpha() * alpha / 255.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectNoTexture, false);
    }

    if (fillStyle.isCurrentColor())
    {
        Color fillColor = fillStyle.color();
        float alpha = fillColor.alpha() * state().globalAlpha;
        m_context->Gfx_RenderRectEx(x, y, x + width, y + height,
                                    fillColor.red(), fillColor.green(),
                                    fillColor.blue(), alpha, 1);
    }
    else if (fillStyle.isPattern())
    {
        drawTexture(getPatternTexture(fillStyle.canvasPattern()),
                                      m_context->nScreenWidth, m_context->nScreenHeight,
                                      x, y, width, height, x, y, width, height);
    }
    else if (fillStyle.isGradient())
    {
        if (fillStyle.canvasGradient()->isZeroSize())
            return;
        drawTexture(getGradientTexture(state().fillStyle),
                                       m_context->nScreenWidth, m_context->nScreenHeight,
                                       x, y, width, height, x, y, width, height);
    }
    else
    {
        THROW_EXCEPTION(TTypeError, "fillRect : fillStyle is not match");
    }
}

void GraphicsContext::fillText(char* text, float x, float y)
{
    HTEXTURE hTex = NO_TEXTURE;
    int w = 0;
    int h = 0;
    int rw = 0;
    int rh = 0;
    int rtop = 0;
    int rbot = 0;
    m_font->setText(text);
    m_font->setStroke(false);
    
    if ((hTex = m_font->getFromHash(w, h, rw, rh)) == NO_TEXTURE)
    {
        if (m_fontCounter >= MAX_TEXTURE)
        {
            swapBuffer();
            for (int i = 0; i < MAX_TEXTURE; i ++)
            {
                if (m_fontTex[i] != NO_TEXTURE)
                    m_context->Texture_Free(m_fontTex[i]);
                m_fontTex[i] = NO_TEXTURE;
            }

            m_fontCounter = 0;
            m_font->release();
        }
        unsigned int* pixels = m_font->getPixelArray(w, h, rw, rh, rtop, rbot);

        hTex = m_context->Texture_Load((unsigned int*)pixels, w, h);
        m_font->put(hTex, w, h, rw, rh, m_fontCounter);
        m_fontTex[m_fontCounter] = hTex;
        m_fontCounter ++;
    }
    
    int baseline = m_font->getBaseLine();
    TextAlign align =  m_font->getAlign();
    switch (baseline)
    {
        case TTOP:
            break;

        case TMIDDLE:
            y = y - (rh/2);
            break;

        case TBOTTOM:
            y = y - (rh) + rbot;

            break;

        case TNONE:
            y = y - rh - rtop;
            break;
    }

    switch (align)
    {
        case TLEFT:
            break;
        case TRIGHT:
            x = x - rw;
            break;
        case TCENTER:
            x = x - (rw / 2);
            break;
        case TSTART:
        case TEND:
            break;//not implementation
    }

    m_context->Effect_Active(m_effectWithTexture, true);
    Gl2dSprite* gl_Sprite = getSprite();

    gl_Sprite->SetTexture(hTex);
    gl_Sprite->SetBlendMode(1);
    gl_Sprite->SetHotSpot(0,0);

    gl_Sprite->SetUV(0.0f, 0.0f, 1.0f, 1.0f);

    Color fillColor = state().fillStyle.color();
    float alpha = fillColor.alpha() * state().globalAlpha;
    
    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();
        //init the target
        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectWithTexture, true);

        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render to target
        m_context->Transform_disable();
        
        //set spride color
        gl_Sprite->SetColor(0.0f, 0.0f, 0.0f, 1.0f, -1);
        gl_Sprite->RenderExIntWH(x, y, w, h, 0.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();
        
        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                                    x, y, w, h, state().shadowBlur / 5,
                                    m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                                    state().shadowColor.red(), state().shadowColor.green(),
                                    state().shadowColor.blue(),
                                    state().shadowColor.alpha() * alpha / 255.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectWithTexture, true);
    }

    //set spride color
    gl_Sprite->SetColor(fillColor.red()/255.0f,
                        fillColor.green()/255.0f,
                        fillColor.blue()/255.0f,
                        alpha / 255.0f, -1);

    gl_Sprite->RenderExIntWH(x, y, w, h, 0.0f);

    m_context ->Effect_Active(m_effectNoTexture, false);
}

void GraphicsContext::strokeText(char* text, float x, float y)
{
    HTEXTURE hTex = NO_TEXTURE;
    int w = 0;
    int h = 0;
    int rw = 0;
    int rh = 0;
    int rtop = 0;
    int rbot = 0;
    m_font->setText(text);
    m_font->setStroke(true);
    
    if ((hTex = m_font->getFromHash(w, h, rw, rh, true)) == NO_TEXTURE)
    {
        if (m_fontCounter >= MAX_TEXTURE)
        {
            swapBuffer();
            for (int i = 0; i < MAX_TEXTURE; i ++)
            {
                if (m_fontTex[i] != NO_TEXTURE)
                    m_context->Texture_Free(m_fontTex[i]);
                m_fontTex[i] = NO_TEXTURE;
            }
            m_fontCounter = 0;
            m_font->release();
        }

        unsigned int* pixels = m_font->getPixelArray(w, h, rw, rh, rtop, rbot, true);

        hTex = m_context->Texture_Load((unsigned int*)pixels, w, h);
        m_font->put(hTex, w, h, rw, rh, m_fontCounter);
        m_fontTex[m_fontCounter] = hTex;
        m_fontCounter ++;
    }
    int baseline = m_font->getBaseLine();
    TextAlign align =  m_font->getAlign();

    switch (baseline)
    {
        case TTOP:
            break;
        case TMIDDLE:
            y = y - (rh/2);
            break;
        case TBOTTOM:
            y = y - (rh) + rbot;
            break;
        case TNONE:
            y = y - rh - rtop;
            break;
    }

    switch (align)
    {
        case TLEFT:
            break;
        case TRIGHT:
            x = x - rw;
            break;
        case TCENTER:
            x = x - (rw / 2);
            break;
        case TSTART:
        case TEND:
            break;//not implementation
    }

    m_context->Effect_Active(m_effectWithTexture, true);
    Gl2dSprite* gl_Sprite = getSprite();
    gl_Sprite->SetTexture(hTex);
    gl_Sprite->SetBlendMode(1);
    gl_Sprite->SetHotSpot(0,0);
    gl_Sprite->SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    
    Color strokeColor = state().strokeStyle.color();
    float alpha = strokeColor.alpha() * state().globalAlpha;
    
    if (state().hasShadowBlur)
    {
        //swap to default target
        m_context->Gfx_SwapBuffer();
        //init the target
        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectWithTexture, true);

        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render to target
        m_context->Transform_disable();
        gl_Sprite->SetColor(0.0f, 0.0f, 0.0f, 1.0f, -1);
        gl_Sprite->RenderExIntWH(x, y, w, h, 0.0f);

        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();
        
        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                                    x, y, w, h, state().shadowBlur / 5,
                                    m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                                    state().shadowColor.red(), state().shadowColor.green(),
                                    state().shadowColor.blue(), 
                                    state().shadowColor.alpha() * alpha / 255.0f);
    }
    
    gl_Sprite->SetColor(strokeColor.red()/255.0f,
                        strokeColor.green()/255.0f,
                        strokeColor.blue()/255.0f,
                        alpha / 255.0f, -1);
                        
    gl_Sprite->RenderExIntWH(x, y, w, h, 0.0f);

    m_context ->Effect_Active(m_effectNoTexture, false);
}

void GraphicsContext::destroy()
{
    //should delete before my delete
    if (m_font)
    {
        for (int i = 0; i < MAX_TEXTURE; i ++)
        {
            if (NO_TEXTURE != m_fontTex[i])
                m_context->Texture_Free(m_fontTex[i]);
        }
        m_font->release();
        m_fontCounter = 0;
        delete m_font;
        m_font = 0;
    }

    if (NO_TARGET != m_target)
        m_context->Target_Free(m_target);

    if (NO_TARGET != m_target_bak)
        m_context->Target_Free(m_target_bak);

    if (NO_TEXTURE != m_cameraTexture)
        m_context->Texture_Free(m_cameraTexture);

    if (m_Sprite)
        delete m_Sprite;

//must delete after other resources
    if (m_context)
    {
        m_context->System_Shutdown();
        delete m_context;
        m_context = NULL;
    }
    LOGD("gl2dimpl delete ok!");
}

void GraphicsContext::clearRect(float x, float y, float width, float height)
{
    //
    ////remove all pixels to transparent black
    m_context->Gfx_Scissor(x, m_context->nScreenHeight - y - height, width, height);
    m_context->Gfx_Clear();
    m_context->Gfx_CloseScissor();
}

Gl2dSprite* GraphicsContext::getSprite()
{
    if (!m_Sprite)
    {
        LOGD("init spride! should be deleted later!");
        init(m_x, m_y, m_width, m_height);
    }

    return m_Sprite;
}

void GraphicsContext::save()
{
    ASSERT(m_stateStack.size() >= 1);
    state().transform = m_context->Transform_getMatrix();
    m_stateStack.push(state());
}

void GraphicsContext::restore()
{
    if (m_stateStack.size() <= 1)
    {
        LOGE("restore : m_stateStack.size() <= 1");
        return;
    }
    m_stateStack.pop();
    m_context->Transform_setMatrix(state().transform);
    if (m_bHasClip)
    {
        m_context->Gfx_EndClipPath();
        m_bHasClip = false;
    }
}

void GraphicsContext::strokeRect(float x, float y, float width, float height)
{
    CanvasStyle strokeStyle = state().strokeStyle;
    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();
        //init the target
        ////m_context->Gfx_BeginShadow(m_target, m_width, m_height);

        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectNoTexture, false);
        // clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();

        //render shadow
        Color m_strokeColor = strokeStyle.color();
        float alpha = m_strokeColor.alpha() * state().globalAlpha;

        float len = state().lineWidth / 2.0;
/*      m_context->Gfx_RenderLineEx(x - len, y, x + width + len, y, 0,0,0,255);
        m_context->Gfx_RenderLineEx(x + width, y, x + width, y + height, 0,0,0,255);
        m_context->Gfx_RenderLineEx(x + width + len, y + height, x - len, y+ height, 0,0,0,255);
        m_context->Gfx_RenderLineEx(x, y + height, x , y, 0,0,0,255);
        m_context->Gfx_SwapBuffer();
*/
        m_context->Transform_disable();
        state().shadowPath = true;
        Path p;
        p.clear();
        p.moveTo(x, y);
        p.addLineTo(x + width, y);
        p.addLineTo(x + width, y + height);
        p.addLineTo(x, y + height);
        p.addLineTo(x, y);
        p.closeSubpath();
        StrokePath(p, m_context, &state());
        m_context->Gfx_SwapBuffer();
        state().shadowPath = false;
        m_context->Transform_enable();
        
        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                                    x-len, y-len, width+2*len, height+2*len, state().shadowBlur,
                                    m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                                    state().shadowColor.red(), state().shadowColor.green(),
                                    state().shadowColor.blue(),
                                    state().shadowColor.alpha() * alpha / 255.0f);
                                   
        /*HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                                    0, 0, m_width, m_height, state().shadowBlur,
                                    m_width, m_height, state().shadowOffsetX, state().shadowOffsetY,
                                    state().shadowColor.red(), state().shadowColor.green(),
                                    state().shadowColor.blue(),
                                    state().shadowColor.alpha()*alpha/255);     */                      

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectNoTexture, false);
    }

    if (strokeStyle.isCurrentColor())
    {
        Path p;
        p.clear();
        p.moveTo(x, y);
        p.addLineTo(x + width, y);
        p.addLineTo(x + width, y + height);
        p.addLineTo(x, y + height);
        p.addLineTo(x, y);
        p.closeSubpath();
        StrokePath(p, m_context, &state());
        m_context->Gfx_SwapBuffer();
    }
    else if (strokeStyle.isPattern())
    {
        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha_temp = state().globalAlpha;
        state().globalAlpha = 0;
        Color m_strokeColor = strokeStyle.color();
        
        Path p;
        p.clear();
        p.moveTo(x, y);
        p.addLineTo(x + width, y);
        p.addLineTo(x + width, y + height);
        p.addLineTo(x, y + height);
        p.addLineTo(x, y);
        p.closeSubpath();
        StrokePath(p, m_context, &state());
        m_context->Gfx_SwapBuffer();
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha_temp;
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        
        drawTexture(getPatternTexture(strokeStyle.canvasPattern()),
                                      sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
                                      
        m_context->Gfx_EndClipPath();
    }
    else if (strokeStyle.isGradient())
    {
        if (strokeStyle.canvasGradient()->isZeroSize())
            return;

        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha_temp = state().globalAlpha;
        state().globalAlpha = 0;
        Color m_strokeColor = strokeStyle.color();
        
        Path p;
        p.clear();
        p.moveTo(x, y);
        p.addLineTo(x + width, y);
        p.addLineTo(x + width, y + height);
        p.addLineTo(x, y + height);
        p.addLineTo(x, y);
        p.closeSubpath();
        StrokePath(p, m_context, &state());
        m_context->Gfx_SwapBuffer();
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha_temp;
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        
        drawTexture(getGradientTexture(state().strokeStyle),
                                       sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
        m_context->Gfx_EndClipPath();
    }
    else
        LOGE("no match in stroke_rect");
}

void GraphicsContext::beginPath()
{
    m_path.clear();
}

void GraphicsContext::closePath()
{
     m_path.closeSubpath();
}

void GraphicsContext::moveTo(float x, float y)
{
    if (!std::isfinite(x) | !std::isfinite(y))
        return;
    //if (!state().m_invertibleCTM)
    //   return;
    m_path.moveTo(Point(x, y));
}

void GraphicsContext::lineTo(float x, float y)
{
    if (!std::isfinite(x) | !std::isfinite(y))
        return;
    //if (!state().m_invertibleCTM)
    //   return;
    
    if (!m_path.hasCurrentPoint())
        m_path.moveTo(x,  y);
    else
        m_path.addLineTo(x, y);
}

HTEXTURE GraphicsContext::getPatternTexture(CanvasPattern* cp)
{
    HTEXTURE htex = cp->texture();
    if (NO_TEXTURE != htex)
        return htex;
    Image* img = cp->image();

    if (img->getImageData() == NULL)
        return NO_TEXTURE;

    int sw = m_context->nScreenWidth;
    int sh = m_context->nScreenHeight;
    int iw = img->getWidth();
    int ih = img->getHeight();
    
    HTARGET target = m_context->Target_Create(sw, sh);
    //swap things before active target
    m_context->Gfx_SwapBuffer();
    m_context->Target_Active(target, sw, sh);

    HTEXTURE htexture = img->getTexture();
    if (NO_TEXTURE == htexture)
        htexture = m_context->Texture_Load(img->getImageData(), iw, ih);

    int nx = 1;
    int ny = 1;
    if (cp->repeatX())
        nx = ceilf((float)sw / (float)iw);
    if (cp->repeatY())
        ny = ceilf((float)sh / (float)ih);
        
    Gl2dSprite* gl_Sprite = getSprite();
    m_context->Effect_Active(m_effectWithTexture, true);

    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            gl_Sprite->SetTexture(htexture);
            gl_Sprite->SetBlendMode(1);
            gl_Sprite->SetHotSpot(0,0);
            gl_Sprite->SetUV(0, 0, 1.0, 1.0);
            gl_Sprite->SetColor(1.0f, 1.0f, 1.0f, 1.0f, -1);
            gl_Sprite->RenderExIntWH(iw*i, ih*j, iw, ih, 0.0f);
        }
    }

    m_context->Gfx_SwapBuffer();
    m_context->Effect_Active(m_effectNoTexture, false);
    htex = m_context->Target_GetTexture(target);
    m_context->Target_ResetToScreen();
    ///save textures, targetto pattern
    cp->setTargetAndTexture(target, htex);
    return htex;
}

HTEXTURE GraphicsContext::getGradientTexture(CanvasStyle& c_style)
{
    CanvasGradient* cg = c_style.canvasGradient();
    HTEXTURE htex = MemoryManager::getManager()->FindCanvasGradient(cg);

    if (NO_TEXTURE != htex)
        return htex;

    float sw = m_context->nScreenWidth;
    float sh = m_context->nScreenHeight;

    HTARGET target = m_context->Target_Create(sw, sh);
    LOGD("create target!");
    //swap things before active target
    m_context->Gfx_SwapBuffer();
    m_context->Target_Active(target, sw, sh);
    m_context->Effect_Active(m_effectNoTexture, false);

    if (cg->isRadial())
    {
        m_context->Transform_disable();
        //radial gradient
        //resort colorStops
        cg->resortRadialColorStops();

        int size = cg->colorStopsSize();
        std::vector<ColorStop> stops = cg->getColorStops();
        //get Parameters
        float r0 = cg->startRadius();
        float r1 = cg->endRadius();
        float x0 = cg->p0().x();
        float y0 = cg->p0().y();
        float x1 = cg->p1().x();
        float y1 = cg->p1().y();

        //drawScreenRect
        m_context->Gfx_RenderRectEx(0, 0, sw, sh,
                                    255 * stops[size - 1].red,
                                    255 * stops[size - 1].green,
                                    255 * stops[size - 1].blue,
                                    255*stops[size-1].alpha, 1);

        //drawInnerRound
        float r = r0 + (r1-r0)*stops[0].stop;
        float x = x0 + (x1-x0)*stops[0].stop;
        float y = y0 + (y1-y0)*stops[0].stop;
        m_context->Gfx_RenderRoundEx(x, y, r, 255*stops[0].red,
                                     255*stops[0].green, 255*stops[0].blue,
                                     255*stops[0].alpha, 1);

        //drawMiddleTorus
        float tr, tx, ty;
        int red[2],green[2],blue[2],alpha[2];
        
        for (int i=0;i<size-1;i++)
        {
            tr = r;
            tx = x;
            ty = y;

            r = r0 + (r1-r0)*stops[i+1].stop;
            x = x0 + (x1-x0)*stops[i+1].stop;
            y = y0 + (y1-y0)*stops[i+1].stop;

            red[0]   = 255*stops[i].red;     red[1]   = 255*stops[i+1].red;
            green[0] = 255*stops[i].green;   green[1] = 255*stops[i+1].green;
            blue[0]  = 255*stops[i].blue;    blue[1]  = 255*stops[i+1].blue;
            alpha[0] = 255*stops[i].alpha;   alpha[1] = 255*stops[i+1].alpha;

            m_context->Gfx_RenderTorusEx(tx, ty, tr, x, y, r,
                                         red, green, blue, alpha, 1);
        }
        m_context->Transform_enable();
    }
    else
    {
        //linear gradient
        gl2dQuad quad[cg->colorStopsSize()+1];
        cg->calculateGLQuad(quad, sw, sh);
        for (int i = 0; i <= cg->colorStopsSize(); i++)
        {
            m_context->Gfx_RenderQuad(&quad[i]);
        }
    }
    m_context->Gfx_SwapBuffer();
    htex = m_context->Target_GetTexture(target);
    m_context->Target_ResetToScreen();
    ///save textures, targetto pattern
    cg->setTargetAndTexture(target, htex);
    return htex;
}

void GraphicsContext::fill()
{
    CanvasStyle fillStyle = state().fillStyle;
    
    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();

        m_context->Target_Active(m_target,  m_width, m_height);
        m_context->Effect_Active(m_effectNoTexture, false);

        //get Path's bounding rect
        FloatRect boundingRect = m_path.boundingRect();
        float rx = boundingRect.x();
        float ry = boundingRect.y();
        float rw = boundingRect.width();
        float rh = boundingRect.height();

        // clear rect
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();
        //m_context->Gfx_RenderRectEx(rx, ry, rx + rw, ry + rh, 0, 0, 0, 0, 1);

        //render shadow
        m_context->Transform_disable();
        state().shadowPath = true;
        FillPathWalkLine(m_path, m_context, &state());
        state().shadowPath = false;
        m_context->Gfx_SwapBuffer();
        m_context->Transform_enable();
        
        float shadowAlpha = state().shadowColor.alpha() * 
                            state().globalAlpha * fillStyle.color().alpha() / 255.0f;

        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                    rx, ry, rw, rh,
                    state().shadowBlur, m_width, m_height,
                    state().shadowOffsetX, state().shadowOffsetY,
                    state().shadowColor.red(), state().shadowColor.green(),
                    state().shadowColor.blue(), shadowAlpha);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectNoTexture, false);
    }
    
    m_context->Transform_disable();
    
    //FillPath(m_path, m_context,state());
    
    if (fillStyle.isCurrentColor())
    {
        Path path = m_path;
        path.setTransform(m_context->Transform_getMatrix());
        FillPathWalkLine(path, m_context, &state());
    }
    else if (fillStyle.isPattern())
    {
        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha = state().globalAlpha;
        state().globalAlpha = 0;
        FillPathWalkLine(m_path, m_context, &state());
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha;
        
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        //from getPatternTexture
        m_context->Transform_enable();
        drawTexture(getPatternTexture(fillStyle.canvasPattern()),
                                      sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
        m_context->Transform_disable();
        m_context->Gfx_EndClipPath();
    }
    else if (fillStyle.isGradient())
    {
        if (fillStyle.canvasGradient()->isZeroSize())
            return;

        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha = state().globalAlpha;
        state().globalAlpha = 0;
        FillPathWalkLine(m_path, m_context, &state());
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha;
        
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        //from getPatternTexture
        m_context->Transform_enable();
        drawTexture(getGradientTexture(state().fillStyle),
                                       sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
        m_context->Transform_disable();
        m_context->Gfx_EndClipPath();
    }
    else
    {
        LOGE("no match in fill_rect");
    }

    m_context->Transform_enable();
}

void GraphicsContext::stroke()
{
    CanvasStyle strokeStyle = state().strokeStyle;

    if (state().hasShadowBlur)
    {
        //shadowBlur target is not ok
        m_context->Gfx_SwapBuffer();

        m_context->Target_Active(m_target, m_width, m_height);
        m_context->Effect_Active(m_effectNoTexture, false);
        
        float len = state().lineWidth / 2.0 + 0.5;
        //get bounding rect
        FloatRect boundingRect = m_path.boundingRect();
        float rx = boundingRect.x() - len;
        float ry = boundingRect.y() - len;
        float rw = boundingRect.width() + len * 2.0;
        float rh = boundingRect.height() + len * 2.0;

        //clear & draw
        m_context->Gfx_SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_context->Gfx_Clear();
        
        //render shadow
        m_context->Transform_disable();
        state().shadowPath = true;
        StrokePath(m_path, m_context, &state());
        m_context->Gfx_SwapBuffer();
        state().shadowPath = false;
        m_context->Transform_enable();
        
        float shadowAlpha = state().shadowColor.alpha() * 
                            state().globalAlpha * strokeStyle.color().alpha() / 255.0f;

        //render to screen
        HTEXTURE tex = m_context->Gfx_RenderShadow(m_target, m_target_bak, m_effectWithBlur,
                0, 0, m_width, m_height,
                state().shadowBlur, m_width, m_height,
                state().shadowOffsetX, state().shadowOffsetY,
                state().shadowColor.red(), state().shadowColor.green(),
                state().shadowColor.blue(), shadowAlpha);

        m_context->Gfx_SwapBuffer();
        m_context->Effect_Active(m_effectNoTexture, false);
    }
    
    //m_context->Transform_disable();
    //m_path.setTransform(m_context->Transform_getMatrix());

    if (strokeStyle.isCurrentColor())
    {
        StrokePath(m_path, m_context, &state());
    }
    else if (strokeStyle.isPattern())
    {
        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha = state().globalAlpha;
        state().globalAlpha = 0;
        StrokePath(m_path, m_context, &state());
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha;
        
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        m_context->Transform_enable();
        drawTexture(getPatternTexture(strokeStyle.canvasPattern()),
                    sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
        m_context->Transform_disable();
        m_context->Gfx_EndClipPath();
    }
    else if (strokeStyle.isGradient())
    {
        if (strokeStyle.canvasGradient()->isZeroSize())
            return;

        m_context->Gfx_BeginSetClipPath();
        
        //must change alpha to 0, when cliping
        float alpha = state().globalAlpha;
        state().globalAlpha = 0;
        StrokePath(m_path, m_context, &state());
        m_context->Gfx_ClipPath();
        state().globalAlpha = alpha;
        
        int sw = m_context->nScreenWidth;
        int sh = m_context->nScreenHeight;
        m_context->Transform_enable();
        drawTexture(getGradientTexture(state().strokeStyle),
                    sw, sh, 0, 0, sw, sh, 0, 0, sw, sh);
        m_context->Transform_disable();
        m_context->Gfx_EndClipPath();
    }
    else
        LOGE("no match in stroke_rect");
        
    //m_context->Transform_enable();
}

void GraphicsContext::clip()
{
    m_context->Gfx_BeginSetClipPath();
    
    //must change alpha to 0, when cliping
    float alpha = state().globalAlpha;
    state().globalAlpha = 0;
    bool hasShadow = state().hasShadowBlur;
    state().hasShadowBlur = false;
    fill();
    m_context->Gfx_ClipPath();
    state().hasShadowBlur = hasShadow;
    state().globalAlpha = alpha;
    m_bHasClip = true;
}

void GraphicsContext::setGlobalAlphaState(float val)
{
    state().globalAlpha = val;
}

float GraphicsContext::getGlobalAlphaState()
{
    return state().globalAlpha;
}

void GraphicsContext::setLineWidth(float val)
{
    if (val == state().lineWidth)
        return ;

    m_context->Gfx_SwapBuffer();
    state().lineWidth = val;
    m_context -> Gfx_SetLineWidth(val);
}

void GraphicsContext::setLineCap(char* val)
{
    std::string s(val);
    if (s == ("butt"))
    {
        state().lineCap = ButtCap;
    }
    else if (s == ("round"))
    {
        state().lineCap = RoundCap;
    }
    else if (s == "square")
    {
        state().lineCap = SquareCap;
    }
}

void GraphicsContext::setLineJoin(char* val)
{
    std::string s(val);
    if (s == ("miter"))
    {
        state().lineJoin = MiterJoin;
    }
    else if (s == ("round"))
    {
        state().lineJoin = RoundJoin;
    }
    else if (s == "bevel")
    {
        state().lineJoin = BevelJoin;
    }
}

void GraphicsContext::setMiterLimit(float val)
{
    state().miterLimit = val;
}

void GraphicsContext::translate(float x, float y)
{
    //m_path.translate(x, y);
    m_context->Transform_translate(x, y);
}

void GraphicsContext::scale(float x, float y)
{
    //m_path.scale(x, y);
    m_context->Transform_scaleNonUniform(x,y);
}

void GraphicsContext::rotate(float ro)
{
    //m_path.rotate(ro);
    m_context->Transform_rotate(ro * 180 /M_PI);
}

void GraphicsContext::transform(float m11, float m12,
                                float m21, float m22,
                                float dx,  float dy)
{
    m_context->Transform_useMatrix2d(m11, m12, m21, m22, dx, dy);
}

void GraphicsContext::setTransform(float m11, float m12,
                                   float m21, float m22,
                                   float dx,  float dy)
{
    m_context->Transform_reset();
    m_context->Transform_useMatrix2d(m11, m12, m21, m22, dx, dy);
}

int GraphicsContext::arc(float x, float y, float r, float sa, float ea, bool anticlockwise )
{
    if (!std::isfinite(x) | !std::isfinite(y) |
        !std::isfinite(r) | !std::isfinite(sa) | !std::isfinite(ea))
        return -1;
        
    if (r < 0)
    {
        LOGE("radius navigate error!");
        return -1;
    }

    if (!r || sa == ea)
    {
        // A subpath consisting of a single empty arc should draw nothing.
        // However, an empty arc may comprise one point in a non-empty
        // subpath, so do not ignore empty arcs entirely.
        Point pt(x + r * cosf(sa), y + r * sinf(sa));
        if (!m_path.hasCurrentPoint())
            m_path.moveTo(pt);
        else
            m_path.addLineTo(pt);
        return 0;
    }
    // If 'sa' and 'ea' differ by more than 2Pi, just add a circle starting/ending at 'sa'
    
    if (abs(sa - ea) >= 2 * piFloat)
    {
        if(anticlockwise)
        {
            m_path.addArc(Point(x, y), r, 0, 2 * piFloat, anticlockwise);
            return 2;
        }
        else 
        {
            m_path.addArc(Point(x, y), r, 0, - 2 * piFloat, anticlockwise);
            return 2;
        }
    }

    m_path.addArc(Point(x, y), r, sa, ea, anticlockwise);
    return 1;
}

void GraphicsContext::quadraticCurveTo(float cpx, float cpy, float x, float y)
{

     if (!std::isfinite(cpx) | !std::isfinite(cpy) |
         !std::isfinite(x) | !std::isfinite(y))
        return;
        
   // if (!state().m_invertibleCTM)
   //     return;
    if (!m_path.hasCurrentPoint())
        m_path.moveTo(Point(cpx, cpy));

    m_path.addQuadCurveTo(Point(cpx, cpy), Point(x, y));
}

void GraphicsContext::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    if (!std::isfinite(cp1x) | !std::isfinite(cp1y) |
        !std::isfinite(cp2x) | !std::isfinite(cp2y) |
        !std::isfinite(x) | !std::isfinite(y))
        return;
        
   // if (!state().m_invertibleCTM)
    //    return;
    if (!m_path.hasCurrentPoint())
        m_path.moveTo(Point(cp1x, cp1y));

    m_path.addBezierCurveTo(Point(cp1x, cp1y), Point(cp2x, cp2y), Point(x, y));
}

bool GraphicsContext::isPointInPath(const float x, const float y)
{
    return m_path.contains(Point(x, y));
}

void GraphicsContext::setShadowColor(Color c)
{
    state().shadowColor = c;
    state().hasShadowBlur = true;
}

Color GraphicsContext::getShadowColor()
{
    return state().shadowColor;
}

void GraphicsContext::setShadowBlur(float blur)
{
    state().shadowBlur = blur;
    //state().hasShadowBlur = true;
}

float GraphicsContext::getShadowBlur()
{
    return state().shadowBlur;
}

void GraphicsContext::setShadowOffsetX(float offx)
{
    state().shadowOffsetX = offx;
    //state().hasShadowBlur = true;
}

float GraphicsContext::getShadowOffsetX()
{
    return state().shadowOffsetX;
}

void GraphicsContext::setShadowOffsetY(float offy)
{
    state().shadowOffsetY = offy;
    //state().hasShadowBlur = true;
}

float GraphicsContext::getShadowOffsetY()
{
    return state().shadowOffsetY;
}

#if NATIVE_SURFACE
void GraphicsContext::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
    m_context->setPosition(x, y);
}

void GraphicsContext::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
    m_context->setSize(w, h);
}

void GraphicsContext::setAlpha(float alpha)
{
    m_context->setAlpha(alpha);
}
#endif


void GraphicsContext::drawCameraPreview(CameraIns* ins, float x, float y, float w, float h)
{
    if (ins->isPreview())
    {
        unsigned int* data = ins->getPreviewData(m_cameraWidth, m_cameraHeight);
        if (!data)
        {
            LOGE("get preview data error  it is ok but should be deleted later!");
            return;
        }
        if (NO_TEXTURE != m_cameraTexture)
            m_context->Texture_Free(m_cameraTexture);

        m_cameraTexture = NO_TEXTURE;
        m_cameraTexture = m_context->Texture_Load(data, m_cameraWidth, m_cameraHeight);

        if (data)
            delete[] data;
    }

    if (NO_TEXTURE != m_cameraTexture)
    {
        if (w == h && w < 0)
        {
            w = m_cameraWidth;
            h = m_cameraHeight;
        }
        drawTexture(m_cameraTexture, m_cameraWidth, m_cameraHeight,
                    0, 0, m_cameraWidth, m_cameraHeight, x, y, w, h);
    }
}

void GraphicsContext::onFrame()
{
    swapBuffer();
}
} // namespace DCanvas
