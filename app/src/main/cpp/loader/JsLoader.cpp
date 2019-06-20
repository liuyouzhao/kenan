#include "defines.h"
#include "JsLoader.h"
#include "CloudxResourceLoader.h"
#include "DCSocket.h"
#include <utils/threads.h>
#include "ThreadPool.h"
#include <unistd.h>
#include "Script.h"

#undef   LOG_TAG
#define  LOG_TAG    "JsLoader"

namespace DCanvas
{

JsLoader::JsLoader()
{
    m_url = NULL;
    m_port = NULL;
    m_path = NULL;
    m_param = NULL;
    m_callback = NULL;
}

JsLoader::~JsLoader()
{
}

JsLoader* JsLoader::create()
{
    return new JsLoader();
}

void JsLoader::load(char *url, int length, Script* pScript)
{
    LOGD("load!!");
    ResourceLoader::load(url, length);
    m_pScript = pScript;
}

bool JsLoader::startThread()
{
    LOGD("startThread");
    pthread_t thread;
    int result = pthread_create(&thread, NULL, JsLoader::thread_func, (void*)this);
    if(result)
    {
        LOGE("create thread failed!");
        return false;
    }
    ThreadPool* tp = ThreadPool::getThreadPool();
    tp->addThread(thread);

    return true;
}

void *JsLoader::notifyFinished(void* data, int len, void *ptr)
{
    if(ptr != NULL)
    {
        JsLoader* pSelf = (JsLoader*)ptr;
        LOGD("[notifyFinished]:%s", pSelf->m_url);
        pSelf->m_data = (char*)data;
        pSelf->m_nLength = len;
        pSelf->m_pScript->setData(data, len);
        LOGD("set data end!");
    }

    return NULL;
}

void* JsLoader::thread_func(void* p )
{
    ThreadControler * tc = ThreadControler::getThreadControler();
    while(1)
    {
        if(tc->continueAndRun())
            break;
        usleep(1000);
    }

    JsLoader* loader = (JsLoader*)p;

    DCSocket* dcs = DCSocket::create(loader->m_url);
    dcs->setPtr(p);
    dcs->setCallBack((CALLBACK)JsLoader::notifyFinished);
    dcs->setPort(loader->m_port);
    dcs->recieve(loader->m_path);
    delete loader;
    delete dcs;
    LOGD("end thread func!");
    tc->lock();
    tc->m_workThread -- ;
    LOGD("workthread is %d", tc->m_workThread);
    tc->unlock();
    return NULL;
}

} // namespace DCanvas
