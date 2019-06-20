#include "defines.h"
#include <iterator>
#include <android/log.h>

#undef LOG_TAG
#define  LOG_TAG    "MemoryManager"

namespace DCanvas
{

MemoryManager* MemoryManager::s_self = NULL;
unsigned int   MemoryManager::s_imagedata_size = 0;
unsigned int   MemoryManager::s_memory_count = 0;
MemoryManager* MemoryManager::getManager()
{
    if (NULL == s_self)
    {
        LOGD("new MemoryManager");
        s_self = new MemoryManager();
    }

    return s_self;
}

void MemoryManager::AddImage(Image* img)
{
    m_imageObjs.insert(img);
}

void MemoryManager::RemoveImage(Image *img)
{
    m_imageObjs.erase(img);
}

void MemoryManager::DeleteImage(Image* img)
{
    for (std::set<Image*>::iterator it = m_imageObjs.begin(); it != m_imageObjs.end(); )
    {
        Image* now = *it;
        if(img == now)
        {
            LOGD("DeleteImage find delete from js");
            delete now;
            m_imageObjs.erase(it++);
            break;
        }else {
            ++it;
        }
    }
}

void MemoryManager::AddImageData(ImageData* imgData)
{
    s_imagedata_size += imgData->GetSize();
    if (s_imagedata_size >= MAXIMAGEDATA)
    {
        LOGD("\"imagedata out of memory!\" s_imagedata size %d", s_imagedata_size);
        imgData->setException();
    }

    m_imageDataObjs.insert(imgData);
}

void MemoryManager::RemoveImageData(ImageData *imgData)
{
    m_imageDataObjs.erase(imgData);
}

void MemoryManager::DeleteImageData(ImageData* imgData)
{
    for (std::set<ImageData*>::iterator it = m_imageDataObjs.begin();
         it != m_imageDataObjs.end(); ++it)
    {
        ImageData* now = *it;
        if(imgData == now)
        {
            LOGD("DeleteimgData find delete from js!");
            s_imagedata_size -= imgData->GetSize();
            delete imgData;
            m_imageDataObjs.erase(imgData);
            break;
        }
    }
}

CanvasPattern* MemoryManager::FindCanvasPattern(Image* img, std::string type, int canvasId)
{
    for (std::set<CanvasPattern*>::iterator it = m_canvasPatternObjs.begin();
         it != m_canvasPatternObjs.end(); ++it)
    {
        CanvasPattern* now = *it;
        if (now->image() == img && now->type() == type &&
            now->getCanvasId() == canvasId)
        {
            return now;
        }
    }
    return NULL;
}

void MemoryManager::AddCanvasPattern(CanvasPattern* cp)
{
    m_canvasPatternObjs.insert(cp);
}

void MemoryManager::RemoveCanvasPattern(CanvasPattern* cp)
{
    m_canvasPatternObjs.erase(cp);
}

HTEXTURE MemoryManager::FindCanvasGradient(CanvasGradient* cg)
{
    cg->sortStopsIfNecessary();
    for (std::set<CanvasGradient*>::iterator it = m_canvasGradientObjs.begin();
         it != m_canvasGradientObjs.end(); ++it)
    {
        CanvasGradient* now = *it;
        now->sortStopsIfNecessary();
        if ((*cg) == (*now) && now->texture() != NO_TEXTURE)
        {
            if (NO_TEXTURE == cg->texture())
            {
                addFrameGarbagePoint(cg);
            }

            return now->texture();
        }
    }
    return NO_TEXTURE;
}

void MemoryManager::AddCanvasGradient(CanvasGradient* cg)
{
    m_canvasGradientObjs.insert(cg);
}

void MemoryManager::RemoveCanvasGradient(CanvasGradient* cg)
{
    m_canvasGradientObjs.erase(cg);
}

void MemoryManager::Destroy()
{
    for (std::set<Image*>::iterator it = m_imageObjs.begin();
         it != m_imageObjs.end(); ++it)
    {
        if (NULL != *it)
            delete (Image*)(*it);
    }

    for (std::set<ImageData*>::iterator it = m_imageDataObjs.begin();
         it != m_imageDataObjs.end(); ++it)
    {
        if (NULL != *it)
        {
            s_imagedata_size -= (*it)->GetSize();
            delete (ImageData*)(*it);
        }
    }

    for (std::set<CanvasPattern*>::iterator it = m_canvasPatternObjs.begin();
         it != m_canvasPatternObjs.end(); ++it)
    {
        if (NULL != *it)
            delete (CanvasPattern*)(*it);
    }

    for (std::set<CanvasGradient*>::iterator it = m_canvasGradientObjs.begin();
         it != m_canvasGradientObjs.end(); ++it)
    {
        if (NULL != *it)
            delete (CanvasGradient*)(*it);
    }

    m_imageDataObjs.clear();
    m_imageObjs.clear();
    m_canvasPatternObjs.clear();
    m_canvasGradientObjs.clear();
    m_garbageArray.clear();
    s_imagedata_size = 0;
}

MemoryManager::~MemoryManager()
{
    Destroy();
    s_self = NULL;
}


void MemoryManager::addFrameGarbagePoint(CanvasGradient *p)
{
    m_garbageArray.insert(p);
}

void MemoryManager::clearFrameGarbagePoint()
{
    for (std::set<CanvasGradient*>::iterator it = m_garbageArray.begin();
         it != m_garbageArray.end(); ++it)
    {
        if (NULL != *it)
        {
            delete (CanvasGradient*)(*it);
        }
    }
    m_garbageArray.clear();
}

void MemoryManager::clearResourceWithCanvasId(int canvasId)
{
LOGD("begin clearResourceWithCanvasId %d", canvasId);
	for (std::set<CanvasPattern*>::iterator it = m_canvasPatternObjs.begin();
        it != m_canvasPatternObjs.end();)
    {
     	CanvasPattern* cp = (CanvasPattern*)*it;
     	if (cp->getCanvasId() == canvasId)
     	{
     		delete cp;
            m_canvasPatternObjs.erase(it++);
     	} else {
     		++it;
     	}
    }
    for (std::set<CanvasGradient*>::iterator it = m_canvasGradientObjs.begin();
    		it != m_canvasGradientObjs.end();)
    {
    	CanvasGradient* cp = (CanvasGradient*)*it;
     	if (cp->getCanvasId() == canvasId)
     	{
     		delete cp;
            m_canvasGradientObjs.erase(it++);
     	} else {
     		++it;
     	}
    }

    for (std::set<Image*>::iterator it = m_imageObjs.begin();
    		it != m_imageObjs.end(); it++)
    {
        Image* img = (Image*)*it;
        if (img->getCanvasId() == canvasId)
        {
            img->freeTexture();
        }
    }
    LOGD("end clearResourceWithCanvasId %d", canvasId);
}

} // namespace DCanvas
