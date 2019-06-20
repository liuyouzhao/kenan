#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>

namespace DCanvas
{

class ThreadPool
{
public:
    static ThreadPool* getThreadPool();
    static void     destroy();
    void      addThread(pthread_t);
    std::list<pthread_t> getThreads();

private:
    static ThreadPool* s_self;
    ThreadPool();

    std::list<pthread_t>    m_threads;
    pthread_mutex_t         m_mutex;
};

}// namespace DCanvas

#endif // THREADPOOL_H
