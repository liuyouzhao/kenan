#include "defines.h"
#include "ImageImpl.h"
#include "Image.h"
#include "fs.h"
#include <stdlib.h>
#include <gl2d_impl.h>
#include "Base64.h"
#include "defines.h"

#include <pthread.h>

#include "ImageLoader.h"
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
            return;
        }
    }

    /// Is native file
    m_data = loadImageDataFromFile(m_src, m_width, m_height);
    if(m_data)
        onLoad();
}

void Image::updata(int length)
{
    bool flg = false;
    int i = 0 , j = 0;
    LOGD("updata in: %s", m_src);
    char *basedata = (char*)"data:*;base64,";
    do
    {
        if (j == 13)
        {
            if (m_src[i] == basedata[j])
                flg = true;
            break;
        }
        else if (m_src[i] != basedata[j])
        {
            if (basedata[j] != '*')
                break;

            if (m_src[i] == ';')
            {
                ++j;
            }
            else
            {
                ++i;
            }
        }
        else
        {
            ++j;
            ++i;
        }
    } while (i < length);

    if (flg)
    {
        //is base64 data format
        std::vector<char> v;
        if (!base64Decode(&this->m_src[i+1], length-i-1, v))
        {
            LOGD("base64Decode faile!");
            return ;
        }
        this->m_data = dataFromstream(&v[0], m_width, m_height, v.size());
        onLoad();
    }
    else
    {
        LOGD("http request process entered");
        flg = 0;
        std::string path(this->m_src);
        std::string head("http://");
        std::string basepath(this->m_src);

        if (flg)/*native file format*/
        {
            this->m_data = dataFromFile(basepath.c_str(), m_width, m_height);
            onLoad();
        }
        else /*http async format*/
        {
            ImageLoader* loader = ImageLoader::create();
            LOGD("ImageLoader");
            loader->load((char*)basepath.c_str(), basepath.length(), this);
            LOGD("load end");
        }
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

void Image::onLoad()
{
    LOGE("Image::onLoad() check hook exist %d", m_isOnloadFuncExist);
    if (!m_isOnloadFuncExist)
        return ;

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

} // namespace DCanvas
