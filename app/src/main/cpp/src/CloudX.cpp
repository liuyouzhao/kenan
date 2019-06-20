#include "defines.h"
#include "CloudX.h"
#include "Sqlite3DB.h"
#include <android/log.h>


#undef LOG_TAG
#define  LOG_TAG    "CloudX"

namespace DCanvas
{
CloudX* CloudX::s_instance = NULL;

CloudX* CloudX::create()
{
    if (NULL == s_instance)
        s_instance = new CloudX();
    return s_instance;
}

CloudX::CloudX()
{
    LOGD("this is constructor of CloudX!");
    m_arguments = NULL;
    m_currentCanvas = NULL;
    m_db = NULL;
    m_basepath = "";
    
    m_canvasPool.clear();
    m_nCanvasNumber = 0;
    m_textCanvas = NULL;

}

CloudX::~CloudX()
{
    s_instance = NULL;
    if (m_arguments)
        delete m_arguments;

    if (CameraIns::hasCreated())
    {
        CameraIns* ins = CameraIns::create();
        delete ins;
    }

    if (m_textCanvas)
        delete m_textCanvas;
}

void CloudX::destroy()
{
    LOGD("this is cloudx destroy begin!");
    AudioIns::destroy();

    LOGD("delete canvas");
    for(unsigned int i = 0; i < m_canvasPool.size(); ++ i)
    {
        delete m_canvasPool[i];
    }
    m_canvasPool.clear();

    if (m_db)
        delete m_db;
    m_db = NULL;

    m_currentCanvas = NULL;

    LOGD("cloudx Destroy ok!");
}

void CloudX::onFrame()
{
    if (CameraIns::hasCreated())
    {
        CameraIns* ins = CameraIns::create();
        ins->callDatacb();
        ins->callTakePiccb();
    }

    for (int i = 0; i < m_canvasPool.size(); ++ i)
    {
        m_canvasPool[i]->onFrame();
    }
}

void CloudX::onFrame(int canvasId)
{
	
    Canvas* canvas = getCanvas(canvasId);
    canvas->onFrame();
}

void CloudX::deleteCanvas(int canvasId)
{
	LOGD("begin deleteCanvas");
    for (std::vector<Canvas*>::iterator it = m_canvasPool.begin(); it != m_canvasPool.end();)
    {
        Canvas* canvas = (Canvas*)*it;
        if (canvas->getId() == canvasId)
        {
            delete canvas;
            it = m_canvasPool.erase(it);
        }
        else {
            ++it;
        }
    }
    
}

char* CloudX::getArguments()
{
    return m_arguments;
}

void CloudX::setArguments(char* arg)
{
    unsigned int l = strlen(arg);

    if (m_arguments == NULL || strlen(m_arguments) < l)
    {
        if (m_arguments)delete m_arguments;
        m_arguments = new char[l+1];
    }
    memcpy(m_arguments, arg, l);
    m_arguments[l] = 0;
}

void CloudX::drawString(std::string s)
{
    if (!m_textCanvas)
        m_textCanvas = new Canvas();
    int len = s.length();
    int num = 0;
    int widthL = 38;
    std::string res = "";
    int offx = 10, offy = 45;
    m_textCanvas->prepareToDrawException();
    
    while (num * widthL < len)

    {
        res = s.substr(num * widthL, widthL * (num + 1));
        res += '\0';
        m_textCanvas->drawException(res.c_str(), 5, offy * num + 30);
        num ++;
    }
    
}

Canvas* CloudX::createCanvas(int id)
{

    Canvas* canvas = getCanvas(id);
    if (canvas == NULL)
    {LOGE("canvas is null error!");}
    return canvas;
}

Canvas* CloudX::createCanvas(int x, int y, int w, int h)
{

    Canvas *canvas = new Canvas(x, y, w, h);
    s_instance->appendCanvas(canvas);

    return canvas;
}

void CloudX::setCanvasIdAndParams(int id, int x, int y, int w, int h)
{
    Canvas *pCanvas = new Canvas(id, x, y, w, h);
    s_instance->appendCanvas(pCanvas);
  //  LOGE("add canvas ok id = %d", id);
}

Canvas* CloudX::createCanvas()
{
	Canvas* pCanvas = new Canvas();
    s_instance->appendCanvas(pCanvas);
    s_instance->setCanvas(pCanvas);
	return pCanvas;
}

void    CloudX::appendCanvas(Canvas* c)
{
	//LOGE("add canvas");
    m_canvasPool.push_back(c);
}

void CloudX::resume()
{
    if (m_currentCanvas) {
        m_currentCanvas->resume();
    }
}

void CloudX::pause()
{
    if (m_currentCanvas) {
        m_currentCanvas->pause();
    }
}

Canvas* CloudX::getCanvas(int canvasId)
{
    if (m_currentCanvas && m_currentCanvas->getId() == canvasId)
    {
        return m_currentCanvas;
    }

    for (int i = 0; i < m_canvasPool.size(); ++i)
    {
        if (m_canvasPool[i]->getId() == canvasId)
        {
            m_currentCanvas = m_canvasPool[i];
            return m_currentCanvas;
        }
    }

    return NULL;
}

}
