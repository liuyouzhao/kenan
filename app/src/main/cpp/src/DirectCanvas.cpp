#include <v8.h>
#include "defines.h"
#include "V8ArrayType.h"
#include "DirectCanvas.h"
#include "Canvas.h"
#include "CanvasGradient.h"
#include "V8Image.h"
#include "Script.h"
#include "V8GlobalFunctions.h"
#include "WebSocketImpl.h"
#include "SocketClient.h"
#include "fs.h"
#include "XMLHttpRequest.h"

#include <gl2d_impl.h>
#include <binder/ProcessState.h>
#include "Float32Array.h"
#include "Uint16Array.h"
#include "Int16Array.h"
#include "Int32Array.h"
#include "Uint32Array.h"
#include "ArrayBuffer.h"
#include "Int8Array.h"
#include "Uint8Array.h"
#include "Float64Array.h"
#include "WebGLObject.h"

#include "CallBackFuncs.h"
#include "ThreadPool.h"
#include <pthread.h>
#include "V8CloudX.h"
//#include "V8WebView.h"
#include "V8Script.h"
#include <string>
#include <android/log.h>
#include "gl2d_impl.h"

using namespace v8;

#undef LOG_TAG
#define  LOG_TAG    "DirectCanvas"

namespace DCanvas
{
DirectCanvas* DirectCanvas::s_pDcanvas = NULL;
ThreadQueue<SocketPackage>* DirectCanvas::s_queue = NULL;

DirectCanvas::DirectCanvas()
{
    while (!m_eventQueue.empty())
        m_eventQueue.pop();
    m_cloudX = CloudX::create();
    s_queue = new ThreadQueue<SocketPackage>();
    m_destroyWithException = false;
    m_hasDestroyed = false;
    m_isReInited = true;
    s_pDcanvas = this;
    LOGD("construct DirectCanvas ok!");
}

DirectCanvas::~DirectCanvas()
{
	LOGD("begin to destructor");


    /*destroy must before delete*/
    //Destroy();
	s_pDcanvas = NULL;
    LOGD("end of all destructor!");
}

void DirectCanvas::setJNIEnv(JNIEnv* env, jobject obj)
{
 /*   m_env = env;
    m_obj = obj;
    jclass clazz = env->GetObjectClass(obj);
	if (NULL == clazz)
	{
		LOGE("can not find class BrowserFrame!!");
	}
	jmethodID getcName = env->GetMethodID(clazz, "createWebViewCanvas", "(IIII)V");
	if (NULL == getcName)
	{
		LOGE("can not find method createWindow!!");
	}
	env->CallVoidMethod(obj, getcName);
     if (env->ExceptionCheck() != 0)
    {
        LOGE("*** Uncaught exception returned from Java call!\n");
        env->ExceptionDescribe();
    }*/
}

void DirectCanvas::setCanvasIdAndParams(int id, int x, int y, int w, int h)
{
    m_cloudX->setCanvasIdAndParams(id, x, y, w, h);
}

void DirectCanvas::Init(int width, int height)
{
    
}

void DirectCanvas::OnFrame(int canvasId)
{
 /*other thread functions*/
    CallBackFuncs::getFuncQueue()->CallAllFunc();
    m_cloudX->onFrame(canvasId);

    MemoryManager::getManager()->clearFrameGarbagePoint();
    SocketPackage* sp = (SocketPackage*)s_queue->Out();
    if (sp != NULL)
    {
        sp->handleMessage();
        delete sp;
        sp = NULL;
    }

}

void DirectCanvas::OnFrame()
{
    TryCatch try_catch;
    v8::Local<v8::String> name;
    while (!m_eventQueue.empty())
    {
        TouchEvent te = m_eventQueue.front();
        m_eventQueue.pop();
        switch (te.m_e) /*handle touch event*/
        {
            case TOUCHNONEEVENT:
                break;

            case TOUCHSTARTEVENT:
                name = v8::String::New("ontouchstart");
                break;

            case TOUCHMOVEEVENT:
                name = v8::String::New("ontouchmove");
                break;

            case TOUCHENDEVENT:
                name = v8::String::New("ontouchend");
                break;

            case TOUCHZOOM:
                name = v8::String::New("ontouchzoom");
                break;

            default:
                break;
        }

        if (TOUCHNONEEVENT != te.m_e)
        {
            //v8::Local<v8::Value> m_js_value;
            //m_js_value = m_exe_context->Global()->Get(name);
            //v8::Local<v8::Function> m_js_function = Local<Function>::Cast(m_js_value);
            /*if (m_js_function->IsFunction())
            {
                Handle<Value>  argv[2] ;
                if (te.m_e != TOUCHZOOM)
                {
                    argv[0] = Integer::New(te.x);
                    argv[1] = Integer::New(te.y);
                    m_js_function->Call(m_exe_context->Global(), 2, argv) ;
                }else
                {
                    argv[0] = Number::New(te.dist);
                    m_js_function->Call(m_exe_context->Global(), 1, argv) ;
                }
            }*/
            //m_event[i].m_e = TOUCHNONEEVENT;
        }
    }
    /*other thread functions*/
    CallBackFuncs::getFuncQueue()->CallAllFunc();

    /*if (m_funcOnFrameok && !m_js_OnFrame->IsFunction())
    {
        m_js_OnFrameName = v8::String::New("onframe");
        m_js_OnFrameValue = m_exe_context->Global()->Get(m_js_OnFrameName);
        m_js_OnFrame = Persistent<Function>::New(Local<Function>::Cast(m_js_OnFrameValue));
        if (!m_js_OnFrame->IsFunction())
            LOGE("how could it be!");
    }
    if (m_funcOnFrameok)
        m_js_OnFrame->Call(m_exe_context->Global(), 0, NULL);
      */  
    m_cloudX->onFrame();

    MemoryManager::getManager()->clearFrameGarbagePoint();
    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        LOGE("has exception!");
        LOGE("%s", *exception);
        LOGE("line %d ", message->GetLineNumber());
        char num[20];
        sprintf(num, " line: %d", message->GetLineNumber());
        m_exceptionString = std::string(*exception) + std::string(num);
        //Destroy();
        m_destroyWithException= true;
    }

    SocketPackage* sp = (SocketPackage*)s_queue->Out();
    if (sp != NULL)
    {
        sp->handleMessage();
        delete sp;
        sp = NULL;
    }

}

// destroy before destructor must do
void DirectCanvas::Destroy()
{

    if (m_hasDestroyed)
    {
        return ;
    }
    
    if (!m_destroyWithException)
        MemoryManager::getManager()->clearFrameGarbagePoint();

    ThreadPool* tp = ThreadPool::getThreadPool();
    std::list<pthread_t> ptl = tp->getThreads();
    for (std::list<pthread_t>::iterator it = ptl.begin(); it != ptl.end(); ++it)//waiting for all thread down
    {
        pthread_join(*it, NULL);
    }
    
    ThreadPool::destroy();
    CallBackFuncs::destroy();
    MemoryManager* mm = MemoryManager::getManager();
    mm->Destroy();

    V8ArrayBufferView::TemplateDestroy();
    V8ArrayBuffer::TemplateDestroy();
    WebGLObject::destroy();

    if (m_cloudX)
    {
        m_cloudX->destroy();
    }

    if (m_cloudX)
    {
        delete m_cloudX;
        m_cloudX = NULL;
    }

    if (s_queue)
    {
        s_queue->DeleteAll();
        delete s_queue;
    }

    V8CloudX::UnRegist();
    
    m_hasDestroyed = true;


    //m_js_OnFrame.Dispose();
    //m_exe_context.Dispose();

    //delete m_context_scope;
    //V8::Dispose();
    //m_funcOnFrameok = false;
}

void DirectCanvas::SetArguments(char *ch)
{
    m_cloudX->setArguments(ch);
}

char* DirectCanvas::GetArguments()
{
    if (m_destroyWithException)
        return (char*)"Has Exception";
    return m_cloudX->getArguments();
}

void DirectCanvas::OnUnload()
{
    if (m_destroyWithException)
        return;

    /*TryCatch try_catch;
    v8::Local<v8::Value> js_value;
    js_value = m_exe_context->Global()->Get(v8::String::New("onunload"));
    v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);

    //run function onunload
    if (js_function->IsFunction())
    {
        js_function->Call(m_exe_context->Global(), 0, NULL) ;
    }

    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(try_catch.Exception());
        LOGE("has exception!");
        LOGE(*exception);
    }*/
}

void DirectCanvas::EntranceJS(char * jsfile)
{
    unsigned int flen = 0;
    FILE* p = fopen(jsfile,"rw");
    if (!p)
        return;
    fseek(p, 0, SEEK_END);
    flen = ftell(p);
    fseek(p,0,SEEK_SET);

    char * source = new char[flen + 1];
    source[flen] = 0;

    fread(source, flen, 1, p);
    fclose(p);

    //m_script_source = v8::String::New(source);
    delete[] source;
}

void DirectCanvas::TouchStart(int x, int y)
{
/*
    m_event[0].m_e = TOUCHSTARTEVENT;
    m_event[0].x = x;
    m_event[0].y = y;
*/
    TouchEvent te;
    te.m_e = TOUCHSTARTEVENT;
    te.x = x;
    te.y = y;
    m_eventQueue.push(te);
}

void DirectCanvas::TouchMove(int x, int y)
{
/*
    m_event[1].m_e = TOUCHMOVEEVENT;
    m_event[1].x = x;
    m_event[1].y = y;
*/
    TouchEvent te;
    te.m_e = TOUCHMOVEEVENT;
    te.x = x;
    te.y = y;
    m_eventQueue.push(te);
}

void DirectCanvas::TouchEnd(int x, int y)
{
    TouchEvent te;
    te.m_e = TOUCHENDEVENT;
    te.x = x;
    te.y = y;
    m_eventQueue.push(te);
}

void DirectCanvas::Zoom(float dist)
{
/*
    m_event[1].m_e = TOUCHZOOM;
    m_event[1].dist = dist;
*/    
    TouchEvent te;
    te.m_e = TOUCHZOOM;
    te.dist = dist;
    m_eventQueue.push(te);
}
void DirectCanvas::resume()
{
    m_cloudX->resume();
}

void DirectCanvas::pause()
{
    m_cloudX->pause();
}

void DirectCanvas::SetUsed()
{
	m_isReInited = false; 
}

bool DirectCanvas::GetReInited()
{
	return m_isReInited;
}

void DirectCanvas::clearCanvas(int canvasId)
{
	LOGD("begin clearCanvas");
	m_cloudX->deleteCanvas(canvasId);
	LOGD("clearCanvas ok!");
}

/*
void DirectCanvas::_setCallBack()
{
    LOGD("setcallback");
    m_js_OnFrameName = v8::String::New("onframe");
    m_js_OnFrameValue = m_exe_context->Global()->Get(m_js_OnFrameName);

    if (!m_js_OnFrameValue.IsEmpty() && m_js_OnFrameValue->IsFunction())
    {
        m_js_OnFrame = Persistent<Function>::New(Local<Function>::Cast(m_js_OnFrameValue));

        if (!m_js_OnFrame->IsFunction())
        {
            ///////////////////check for js is any function called OnFrame
            LOGE("OnFrame, js_func_val->IsFunction check failed!") ;
        }
        else
        {
            m_funcOnFrameok = true;
        }
    }
}

void DirectCanvas::SetBasePath(std::string basepath)
{
    m_basepath = basepath;
    m_cloudX->setBasePath(basepath);
    LOGD("set base path %s",basepath.c_str());
}


void  DirectCanvas::SetBaseUrl(std::string baseurl)
{
    m_baseurl = baseurl;
    m_cloudX->setBaseUrl(baseurl);
    LOGD("set base url %s",baseurl.c_str());
}
*/
/***
*
    for array
    v8::Local<v8::Value> vl = m_exe_context->Global()->Get(v8::String::New("test"));
    v8::Local<Function> func = v8::Local<Function>::Cast(vl);
    Handle<Value> argv[1];
    Local<Object> obj = v8::Object::New();
    char data[10];
    for(int i=0;i<10;++i)
        data[i] = i;
    argv[0] = obj;
    obj->SetIndexedPropertiesToExternalArrayData(data, kExternalByteArray, 10);
    func->Call(m_exe_context->Global(), 1, argv);
*/
}
