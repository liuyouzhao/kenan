#include "defines.h"
#include "ImageImpl.h"
#include "Image.h"
#include "fs.h"
#include <stdlib.h>
#include <gl2d_impl.h>
#include "Base64.h"
#include "defines.h"

#include <pthread.h>

#include "CallBackFuncs.h"
#include "ThreadPool.h"

#include <vector>
#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "Image"

using namespace kenan_system;

namespace kenan_graphics
{
static void*  onloadFunc(void* p)
{
    Image* _p = (Image*)p;
    LOGD("image onloadFunc parameter 22 %p", p);
    _p->onLoad();
    return NULL;
}

Image::Image()
{
    //MemoryManager::getManager()->AddImage(this);
    m_data = NULL;
    m_src = NULL;
    m_hTex = NO_TEXTURE;
    m_width = 0;
    m_height = 0;
    m_isOnloadFuncExist = false;
    onLoadCallback = NULL;
    m_context = Gl2d_Impl::instance();
}

Image::~Image()
{
    if (NO_TEXTURE != m_hTex && m_context)
    {
        m_context->Texture_Free(m_hTex);
    }

    if (m_data)
    {
        delete[] m_data;
    }

    if (m_src)
    {
        delete[] m_src;
    }

    //if (m_onLoadSeted)
    //{
    //    m_js_OnLoad.Dispose();
    //}
    //MemoryManager::getManager()->RemoveImage(this);
}

void Image::freeTexture()
{
    if (NO_TEXTURE != m_hTex && m_context)
    {
        m_context->Texture_Free(m_hTex);
        m_hTex = NO_TEXTURE;
    }
}


void Image::doLoadImageData() {

    if(m_src == NULL) return;

    int len = strlen(m_src);

    if(len <= 0) return;

    const int supportMimeTypesNum = 2;
    const char *constBase64Headers[supportMimeTypesNum] = {
        "data:*;base64,",
        "data:image/png;base64,"
    };

    /// Is base64 image source
    for(int i = 0; i < supportMimeTypesNum; i ++) {
        const char *mimeTypeHeader = constBase64Headers[i];
        if(!strncmp(mimeTypeHeader, m_src, strlen(mimeTypeHeader))) {
            m_data = loadImageDataFromBase64String(m_src, m_width, m_height);

            LOGD("image-wh %d %d", m_width, m_height);
            if(m_data)
                onLoad();
            else
                LOGE("[ERROR] %s %d", __FUNCTION__, __LINE__);

            return;
        }
    }

    /// Is native file
    m_data = loadImageDataFromFile(m_src, m_width, m_height);
    if(m_data) {
        LOGD("image inner load OK");
        onLoad();
    }
}

void Image::setData(void* data, int size)
{
    this->m_data = dataFromstream((const char*)data, m_width, m_height, size);

    FuncData* fd = new FuncData();
    fd->data = onloadFunc;
    fd->param = this;
    LOGD("setData parameter 11 %p", this);
    CallBackFuncs::getFuncQueue()->AddFunc(fd);
}

void Image::setPixels(int *pixels, int w, int h) {
    memcpy(m_data, pixels, w * h * sizeof(int));
    m_width = w;
    m_height = h;
    convertTexture();
}

HTEXTURE Image::getTexture()
{
    return m_hTex;
}

void Image::setTexture(HTEXTURE ht , Gl2d_Impl* gc, int canvasId)
{
    m_hTex = ht;
    m_context = gc;
    m_canvasId = canvasId;
}

void Image::convertTexture() {
    if(m_hTex != NO_TEXTURE) {
        m_context->Texture_Free(m_hTex);
    }
    m_hTex = m_context->Texture_Load(m_data, m_width, m_height);
}

void Image::onLoad()
{
    /*
    * convert to texture
    */
    convertTexture();

    LOGE("Image::onLoad() check hook exist %d", m_isOnloadFuncExist);
    if (!m_isOnloadFuncExist)
        return;

    LOGE("Image::onLoad() really tiggered callback hook %p", onLoadCallback);
    if(onLoadCallback != NULL)
        onLoadCallback(this);
}

void Image::setSrc(std::string src)
{
	const char* srcfile = src.c_str();
	if (m_src)
	{
		delete m_src;
		m_src = NULL;
	}
	m_src = new char[src.length() +1];
	strcpy(m_src, srcfile);
	m_src[src.length()] = 0;
	//updata(src.length());
	doLoadImageData();
}

ReturnStatus Image::load(std::string src)
{
    static const int supportMimeTypesNum = 2;
    static const char *constBase64Headers[supportMimeTypesNum] = {
        "data:*;base64,",
        "data:image/png;base64,"
    };

	const char* srcfile = src.c_str();
	if (m_src)
	{
		delete m_src;
		m_src = NULL;
	}
	m_src = new char[src.length() +1];
	strcpy(m_src, srcfile);
	m_src[src.length()] = 0;

    if(src.length() <= 0) return ReturnStatus(-1, std::string("bad argument"), std::string("image src string len=0"));

    if(m_data) {
        unloadImageBuffer_Int32(&m_data);
    }

    /// Is base64 image source
    for(int i = 0; i < supportMimeTypesNum; i ++) {
        const char *mimeTypeHeader = constBase64Headers[i];
        if(!strncmp(mimeTypeHeader, m_src, strlen(mimeTypeHeader))) {
            m_data = loadImageDataFromBase64String(m_src, m_width, m_height);
            break;
        }
    }

    if(!m_data) {
        m_data = loadImageDataFromFile(m_src, m_width, m_height);
    }

    if(m_width <= 0 || m_height <= 0 || !m_data) {
        return ReturnStatus(-1, std::string("load failed"), std::string("load image error"));
    }
    convertTexture();
    return ReturnStatus(0, std::string("ok"));
}

} // namespace kenan_graphics
