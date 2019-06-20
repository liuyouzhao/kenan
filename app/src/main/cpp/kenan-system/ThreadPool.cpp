#include "ThreadPool.h"

namespace DCanvas
{

ThreadPool* ThreadPool::s_self = NULL;
ThreadPool* ThreadPool::getThreadPool()
{
    if(!s_self)
        s_self = new ThreadPool();
    return s_self;
}

ThreadPool::ThreadPool()
{
    pthread_mutex_init(&m_mutex, NULL);
    m_threads.clear();
}

void ThreadPool::destroy()
{
    if (s_self)
        delete s_self;
    s_self = NULL;
}

std::list<pthread_t> ThreadPool::getThreads()
{
    return m_threads;
}

void ThreadPool::addThread(pthread_t t)
{
    pthread_mutex_lock(&m_mutex);
    m_threads.push_back(t);
    pthread_mutex_unlock(&m_mutex);
}

}// namespace DCanvas
