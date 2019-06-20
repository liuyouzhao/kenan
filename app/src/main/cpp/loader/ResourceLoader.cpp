#include "defines.h"
#include "CloudxResourceLoader.h"

#include "DCSocket.h"
#include <utils/threads.h>
#include "ThreadPool.h"

#undef   LOG_TAG
#define  LOG_TAG    "ResourceLoader"

namespace DCanvas
{

ResourceLoader* ResourceLoader::create()
{
    return new ResourceLoader();
}

ResourceLoader::ResourceLoader()
{
    m_url = m_path = NULL;
}

ResourceLoader::~ResourceLoader()
{
    if(m_url)
    {
        delete[] m_url;
        m_url = NULL;
    }
}

void* ResourceLoader::notifyFinished(void* data, int len, void* ptr)
{
    LOGD("this is the callback func after receive data!");
    ResourceLoader* _p = (ResourceLoader*)ptr;

    if(_p->m_callback != NULL)
    {
        ((RESOURCECALL)(_p->m_callback))(data, len, _p->m_param);
    }
    return NULL;
}

char* ResourceLoader::convertRightUrl(char* src, int length, int& match)
{
    int i = 6, j = 0;
    while(src[i] == '/')
    {
        ++i;
    }
    j = i;
    while(j < length && src[j] != '/' && src[j] !=':')
    {
        ++j;
    }

    if(m_url)
    {
        delete[] m_url;
        m_url = NULL;
    }
    m_url = new char[j-i + 1];
    int k = 0;

    for(k = i; k < j; ++k)
        m_url[k-i] = src[k];

    m_url[k-i] = 0;

    int port = 0;
    if(src[j] == ':')
    {
        while(src[++j] != '/')
        {
            port = port * 10 + src[j]-'0';
        }
    }
    else
    {
        port = 80;
    }

    this->m_port = port;
    match = j;
    return m_url;
}

char* ResourceLoader::convertRightPath(char* src, int length, int j)
{
    if(m_path)
    {
        delete[] m_path;
        m_path = NULL;
    }
    m_path = new char[length - j + 1];
    bzero(m_path, length - j + 1);
    memcpy(m_path, src + j + 1,  length - j);
    return m_path;
}

bool ResourceLoader::startThread()
{
    pthread_t thread;
    int result = pthread_create(&thread, NULL, thread_func, (void*)this);
    if(result)
    {
        LOGE("create thread failed!");
        return false;
    }
    ThreadPool* tp = ThreadPool::getThreadPool();
    tp->addThread(thread);

    return true;
}

void ResourceLoader::load(char* src, int length)
{
    int j = 0;
    convertRightUrl(src, length, j);
    convertRightPath(src, length, j);

    startThread();
}

/*void ResourceLoader::load(char* url, char* path)
{
    return load(url, 80, path);
}*/

void* ResourceLoader::thread_func(void* p )
{
    ResourceLoader* loader = (ResourceLoader*)p;

    DCSocket* dcs = DCSocket::create(loader->m_url);
    dcs->setPtr(p);
    dcs->setCallBack((CALLBACK)ResourceLoader::notifyFinished);
    dcs->setPort(loader->m_port);
    dcs->recieve(loader->m_path);
    return NULL;
}

ThreadControler* ThreadControler::self = NULL;
ThreadControler* ThreadControler::getThreadControler()
{
    if(!self)
        self = new ThreadControler();
    return self;
}

void ThreadControler::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void ThreadControler::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

bool ThreadControler::continueAndRun()
{
    bool res = false;
    lock();
    if(m_workThread < m_totalThread)
    {
        res = true;
        m_workThread++;
    }
    unlock();
    return res;
}

} // namespace DCanvas
