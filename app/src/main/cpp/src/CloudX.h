/***
*entrance of DCanvas.
*manage event media canvas (memory?)
*
****/

#ifndef CLOUDX_H
#define CLOUDX_H

#include "v8.h"
#include "ObjectWrap.h"
#include "Canvas.h"
#include "CameraIns.h"
#include "AudioIns.h"
#include "SoundPoolIns.h"
#include "Sqlite3DB.h"
#include <string>
#include <vector>

namespace DCanvas
{
#define MAX_CANVAS 999
#define MULTY_CANVAS
class CloudX
{
	friend class V8CloudX;
public:
    static  CloudX* create();
    ~CloudX();
    Canvas* createCanvas(int);
    Canvas* createCanvas(int, int, int, int);
    Canvas* createCanvas();
    void    destroy();
    char*   getArguments();
    void    setArguments(char*);
    void    onFrame();
    void    onFrame(int canvasId);
    void    setCanvas(Canvas* c){ m_currentCanvas = c;}
    void    appendCanvas(Canvas* c);
    void    setBasePath(std::string path){m_basepath = path;}
    void    setBaseUrl(std::string url){m_baseurl = url;}
    Canvas* getCanvas(){return m_currentCanvas;}
    std::string  getBasePath(){return m_basepath;}
    std::string  getBaseUrl(){return m_baseurl;}
    void    drawString(std::string);
    void    resume();
    void    pause();
    void    deleteCanvas(int canvasId);
    void    setCanvasIdAndParams(int id, int x, int y, int w, int h);
private:
    CloudX();
    Canvas* getCanvas(int canvasId);
    static CloudX*  s_instance;
    std::vector<Canvas*> m_canvasPool;
    int             m_nCanvasNumber;
    Canvas*         m_currentCanvas;
    Canvas*         m_textCanvas;
    SQLite3DB*      m_db;
    
    //arguments for js to get the state
    char*           m_arguments;
    std::string     m_basepath;
    std::string     m_baseurl;

};

}
#endif
