#include "defines.h"
#include "ImageLoader.h"
#include "CloudxResourceLoader.h"
#include "DCSocket.h"
#include <utils/threads.h>
#include "ThreadPool.h"
#include <unistd.h>

#undef   LOG_TAG
#define  LOG_TAG    "ImageLoader"

namespace DCanvas
{

ImageLoader* ImageLoader::create()
{
    return new ImageLoader();
}

ImageLoader::ImageLoader()
{
    m_url = m_path = NULL;
}

ImageLoader::~ImageLoader()
{
    if(m_url)
    {
        delete[] m_url;
        m_url = NULL;
    }
}

void* ImageLoader::notifyFinished(void* data, int size, void* imgLoader)
{
    LOGD("ImageLoader LOG notifyFinished");
    if (imgLoader != NULL)
    {
        LOGE("imgLoader is not null");
        ((ImageLoader*)imgLoader)->m_img->setData(data, size);
    }

    return NULL;
}

bool ImageLoader::startThread()
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

void ImageLoader::load(char* src, int length, Image* img)
{
    int j = 0;
    convertRightUrl(src, length, j);
    convertRightPath(src, length, j);

    m_img = img;
    LOGD("url/  %s  /ok? %s", m_url, m_path);
    startThread();
}

/*
void ImageLoader::load(char* url, char* path)
{
    return load(url, 80, path);
}
*/

void* ImageLoader::thread_func(void* p )
{
    ThreadControler * tc = ThreadControler::getThreadControler();
    while(1)
    {
        if(tc->continueAndRun())
            break;
        usleep(1000);
    }

    LOGD("thread funcs");
    ImageLoader* loader = (ImageLoader*)p;
    LOGD("uuuue %s", loader->m_url);
    DCSocket* dcs = DCSocket::create(loader->m_url);
    dcs->setPtr(p);
    dcs->setCallBack((CALLBACK)ImageLoader::notifyFinished);
    dcs->setPort(loader->m_port);
    LOGD("set ok!");
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
