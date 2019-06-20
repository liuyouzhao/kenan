#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "Loader.h"
#include <pthread.h>
typedef void* (*RESOURCECALL)(void* data, int len, void* ptr);

namespace DCanvas
{

class ThreadControler
{
public:
    int  m_workThread;
    int  m_totalThread;
    pthread_mutex_t m_mutex;
    static ThreadControler* getThreadControler();
    void lock();
    void unlock();
    bool continueAndRun();

private:
    static ThreadControler* self;
    ThreadControler()
    {
        m_workThread = 0;
        m_totalThread = 100;
        pthread_mutex_init(&m_mutex, NULL);
    }
};

class ResourceLoader
{
public:
    virtual ~ResourceLoader();
    static ResourceLoader* create();
    //void load(char* url, int port, char* path);
    //void load(char* url, char* path);
    void load(char* url, int length);
    void setParam(void* param) { m_param = param; }
    void setCallBack(RESOURCECALL callback) { m_callback = (void*)callback; }
    char* getUrl() { return m_url; }
protected:
    ResourceLoader();
    static void* notifyFinished(void*, int, void*);
    static void* thread_func(void*);
    char* convertRightUrl(char*, int, int&);
    char* convertRightPath(char*, int, int);
    virtual bool  startThread();

    char*   m_url;
    int     m_port;
    char*   m_path;
    void*   m_param;
    void*   m_callback;
};

} // namespace DCanvas

#endif // RESOURCELOADER_H
