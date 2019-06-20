#ifndef DIRECTCANVAS_H
#define DIRECTCANVAS_H
/**
* this class is the entrance of DCanvas
* use jni should use Init with no args
* EntranceJS->Run->OnFrame use java(GLSurfaceView)
* nodejs just use Init
**/

#include "CloudX.h"
//#include "MemoryManager.h"
#include "ThreadQueue.h"
#include "WebSocketImpl.h"
#include <string>
#include <queue>
#include "jni.h"

using namespace v8;

namespace DCanvas
{

class DirectCanvas
{

public:
    /*
    * Constructor
    */
    DirectCanvas();
    ~DirectCanvas();
    void setJNIEnv(JNIEnv*, jobject);
    void Init(int width, int height);
    void Run();
    void OnFrame();
    void OnFrame(int canvasId);
    void Destroy();
    void OnUnload();
    void EntranceJS(char *);
    void SetArguments(char *);
    char* GetArguments();
    void SetBasePath(std::string );
    void SetBaseUrl(std::string);
    bool HasDestroyed(){return m_hasDestroyed;}
	void SetUsed();
	bool GetReInited();

    void TouchStart(int x, int y);
    void TouchMove(int x, int y);
    void TouchEnd(int x, int y);
    void Zoom(float dist);

    static ThreadQueue<SocketPackage>* s_queue;
    
    void resume();
    void pause();

    static DirectCanvas* s_pDcanvas;
    void	 setCanvasIdAndParams(int id, int x, int y, int w, int h);
    void   clearCanvas(int canvasId);

private:

	JNIEnv* m_env;
	jobject m_obj;
    enum Events
    {
        TOUCHNONEEVENT,
        TOUCHSTARTEVENT,
        TOUCHENDEVENT,
        TOUCHMOVEEVENT,
        TOUCHZOOM
    };
    
    struct TouchEvent
    {
        Events m_e;
        int x;
        int y;    
        float dist;
    };

    CloudX*                         m_cloudX;
    bool 							m_hasDestroyed;
 	bool 							m_isReInited;
     
    //TouchEvent m_event[3];
    std::queue<TouchEvent> m_eventQueue;
    bool                            m_funcOnFrameok;
    std::string                     m_exceptionString;
    bool                            m_destroyWithException;
    
    bool                            m_onFrameSetted;
    v8::Persistent<v8::Function>    m_js_onFrame;
    bool                            m_onTouchStartSetted;
    v8::Persistent<v8::Function>    m_js_onTouchStart;
    bool                            m_onTouchEndSetted;
    v8::Persistent<v8::Function>    m_js_onTouchEnd;
    bool                            m_onTouchMoveSetted;
    v8::Persistent<v8::Function>     m_js_onTouchMove;
    friend class V8DirectCanvas;
};

//static DirectCanvas* s_pDcanvas;

} // namespace DCanvas

#endif
