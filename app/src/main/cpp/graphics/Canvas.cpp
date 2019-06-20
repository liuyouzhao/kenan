#include "defines.h"
#include "Canvas.h"
#include "GraphicsContext.h"
#include "WebGLGraphicsContext.h"
#include "V8WebGLGraphicsContext.h"
#include "RenderingContext.h"
#include "NativeEnv.h"
#include <v8.h>

#include <string>
#include <algorithm>

#include "MemoryManager.h"

#include <android/log.h>
#include <string>

#undef LOG_TAG
#define  LOG_TAG    "Canvas"

namespace DCanvas
{

Canvas::Canvas()
    : m_x(0)
    , m_y(0)
    , m_width(CLOUDX_SCREEN_WIDTH)
    , m_height(CLOUDX_SCREEN_HEIGHT)
    , m_context(NULL)
    , m_nativeEnv(NULL)
    , m_currentLayer(0x00020000)
    , m_id(-1)
{
}

Canvas::Canvas(int id, int w, int h)
    : m_id(id)
    , m_x(0)
    , m_y(0)
    , m_width(w)
    , m_height(h)
    , m_context(NULL)
    , m_nativeEnv(NULL)
    , m_currentLayer(0x00020000)
{
}

Canvas::Canvas(int id, int x, int y, int width, int height)
    : m_id(id)
    , m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_context(NULL)
    , m_nativeEnv(NULL)
    , m_currentLayer(0x00020000)
{
}

Canvas::Canvas(int x, int y, int width, int height)
    : m_id(-1)
    , m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_context(NULL)
    , m_nativeEnv(NULL)
    , m_currentLayer(0x00020000)
{
}

int Canvas::getId()
{
	return m_id;
}

Canvas::~Canvas()
{
    LOGD("destructor of canvas ");
    MemoryManager* mm = MemoryManager::getManager();
    mm->clearResourceWithCanvasId(m_id);
    //LOGD("clearResourceWithCanvasId of canvas ok");
    if (m_context)
    {
        //m_context->destroy();
        delete m_context;
        m_context = NULL;
    }

    if (m_nativeEnv)
    {
        delete m_nativeEnv;
        m_nativeEnv = NULL;
    }
    LOGD("destructor of canvas ok");
}

void  Canvas::onFrame()
{
    if (m_context)
        m_context->onFrame();
}

void Canvas::prepareToDrawException()
{
    GraphicsContext * gc = NULL;
    if (!m_context)
    {
        m_context = GraphicsContext::create(m_x, m_y, m_width, m_height, m_nativeEnv, m_id);
        gc = (GraphicsContext*)m_context;
        
        gc->setFont((char*)"20px monospace");
    }
    else 
    {
        gc = (GraphicsContext*)m_context;
    }
    gc->setFillStyle(Color("white"));
    gc->fillRect(0, 0, m_width, m_height);
    gc->setFillStyle(Color("black"));
}

void Canvas::drawException(const char* s, int width, int height)
{
    if (!m_context)
    {
        prepareToDrawException();
    }
    GraphicsContext * gc =(GraphicsContext*)m_context;

    gc->fillText((char*)s, width, height);
    onFrame();
}

RenderingContext* Canvas::getContext(const char* type)
{
	std::string webtype(type);
	if (m_context)
	{
		delete m_context;
		m_context = NULL;
	}
	
	if (webtype == "2d")
	{
		if (m_id != -1)
		{
			LOGD("java surface");
			m_context = GraphicsContext::create(m_x, m_y, m_width, m_height, m_nativeEnv, m_id);
		}else //native
		{
			LOGD("native surface");
			m_context = GraphicsContext::create(m_x, m_y, m_width, m_height, m_nativeEnv);
		}
		LOGD("graphics 2d m_context [%d]", (unsigned int)((GraphicsContext*)m_context)->GetContext());
		return m_context;
	}
	else if (webtype == "webgl")
	{
		LOGD("graphics 3d %d, %d, %d, %d", m_x, m_y, m_width, m_height);
		WebGLContextAttributes* webGLAttrs = WebGLContextAttributes::create();
		m_context = WebGLGraphicsContext::Create(m_x, m_y, m_width, m_height, m_nativeEnv);
		((WebGLGraphicsContext*)m_context)->setAttributes(webGLAttrs);
		return m_context;
	}
	return NULL;
}

RenderingContext* Canvas::getContext(const char* type, std::hash_map<std::string, bool> hashAttri)
{
	char* web = (char*)"webgl";
	int flg = 0;
	for (int i = 0; i < 5; ++i)
		if (web[i] != type[i])
			flg = 1;

	if (flg && type[0] == '2' && type[1] == 'd')
		flg = -1;

	if (m_context)
	{
		delete m_context;
		m_context = NULL;
	}

	if (flg == -1)
	{
		LOGD("graphics 2d");
		m_context = GraphicsContext::create(m_x, m_y, m_width, m_height, m_nativeEnv, m_id);

		return m_context;
	}
	else if (flg == 0)
	{
		LOGD("graphics 3d");
		WebGLContextAttributes* webGLAttrs = WebGLContextAttributes::create();

		std::hash_map<std::string, bool>::iterator itr = hashAttri.find("alpha");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setAlpha(itr->second);
		itr = hashAttri.find("depth");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setDepth(itr->second);
		itr = hashAttri.find("stencil");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setStencil(itr->second);
		itr = hashAttri.find("antialias");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setAntialias(itr->second);
		itr = hashAttri.find("premultipliedAlpha");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setPremultipliedAlpha(itr->second);
		itr = hashAttri.find("preserveDrawingBuffer");
		if (itr != hashAttri.end() && itr->second)	webGLAttrs->setPreserveDrawingBuffer(itr->second);

		m_context = WebGLGraphicsContext::Create(m_x, m_y, m_width, m_height, m_nativeEnv);

		((WebGLGraphicsContext*)m_context)->setAttributes(webGLAttrs);

		return m_context;
	}
	return NULL;
}

v8::Handle<v8::Value> Canvas::get_context(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::String::Utf8Value value(args[0]);

    char* web = (char*)"webgl";
    int flg = 0;
    for (int i = 0; i < 5; ++i)
        if (web[i] != (*value)[i])
            flg = 1;

    if (flg && (*value)[0] == '2' && (*value)[1] == 'd')
        flg = -1;

    if (m_context)
    {
        delete m_context;
    }

    if (flg == -1)
    {
    }
    else if (flg == 0)
    {
        LOGD("graphics 3d");

        WebGLContextAttributes* webGLAttrs = WebGLContextAttributes::create();
        if (args.Length() > 1 && args[1]->IsObject())
        {
            v8::Handle<v8::Object> jsAttrs = args[1]->ToObject();

            v8::Handle<v8::String> alpha = v8::String::New("alpha");
            if (jsAttrs->Has(alpha))
                webGLAttrs->setAlpha(jsAttrs->Get(alpha)->BooleanValue());

            v8::Handle<v8::String> depth = v8::String::New("depth");
            if (jsAttrs->Has(depth))
                webGLAttrs->setDepth(jsAttrs->Get(depth)->BooleanValue());

            v8::Handle<v8::String> stencil = v8::String::New("stencil");
            if (jsAttrs->Has(stencil))
                webGLAttrs->setStencil(jsAttrs->Get(stencil)->BooleanValue());

            v8::Handle<v8::String> antialias = v8::String::New("antialias");
            if (jsAttrs->Has(antialias))
                webGLAttrs->setAntialias(jsAttrs->Get(antialias)->BooleanValue());

            v8::Handle<v8::String> premultipliedAlpha = v8::String::New("premultipliedAlpha");
            if (jsAttrs->Has(premultipliedAlpha))
                webGLAttrs->setPremultipliedAlpha(jsAttrs->Get(premultipliedAlpha)->BooleanValue());

            v8::Handle<v8::String> preserveDrawingBuffer = v8::String::New("preserveDrawingBuffer");
            if (jsAttrs->Has(preserveDrawingBuffer))
                webGLAttrs->setPreserveDrawingBuffer(jsAttrs->Get(preserveDrawingBuffer)->BooleanValue());
        }

        m_context = WebGLGraphicsContext::Create(m_x, m_y, m_width, m_height, m_nativeEnv);
        ((WebGLGraphicsContext*)m_context)->setAttributes(webGLAttrs);

        //return ((WebGLGraphicsContext*)m_context)->wrap(args);
        return V8WebGLGraphicsContext::Wrap((WebGLGraphicsContext*)m_context);
    }

    return v8::Null();
}

void Canvas::resume()
{
    if (m_nativeEnv)
    {
        LOGD("m_current layer is %ud", m_currentLayer);
        m_nativeEnv->setLayer(m_currentLayer);
    }
}

void Canvas::pause()
{
    if (m_nativeEnv)
    {
        LOGD("add by tjj m_current layer is 0");
        m_nativeEnv->setLayer(0);
    }
}

void Canvas::setLayer(unsigned int lay)
{
    m_currentLayer = lay;
    m_nativeEnv->setLayer(lay);
}

}// namepace DCanvas
