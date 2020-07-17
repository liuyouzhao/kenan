#include "ThreadPool.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "ThreadPool"

namespace kenan_system
{

bool Thread::isRunning() {
    IF_FAILED_EXIT(pthread_mutex_lock(&m_mutex), "pthread_mutex_lock failed")
    bool isRunning = running;
    IF_FAILED_EXIT(pthread_mutex_unlock(&m_mutex), "pthread_mutex_unlock failed")
    return isRunning;
}


ThreadPool* ThreadPool::s_self = NULL;
ThreadPool* ThreadPool::instance() {
    if(!s_self)
        s_self = new ThreadPool();
    return s_self;
}

ThreadPool::ThreadPool() {
    pthread_mutex_init(&m_mutex, NULL);
    m_threads.clear();
}

void ThreadPool::start(int num, void* (*func)(void*)) {

    int diff = num - m_threads.size();

    if(diff <= 0) {
        for(int i = 0; i < num; i ++) {
            Thread *t = m_threads[i];
            if(!t->running) {
                pthread_t thread;
                IF_FAILED_EXIT(pthread_create(&thread, NULL, func, t), "ThreadPool start failed");
                t->running = true;
            }
        }
        LOGD("ThreadPool refresh %d holding threads", num);
    }
    else {
        for(int i = 0; i < diff; i ++) {
            pthread_t thread;
            Thread *t = new Thread(i, m_mutex);
            IF_FAILED_EXIT(pthread_create(&thread, NULL, func, t), "ThreadPool start failed");
            m_threads.push_back(t);
        }
        LOGD("ThreadPool started %d new threads", m_threads.size());
    }
}

void ThreadPool::stop() {
    for(std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end(); ++iter) {
        Thread *thread = *iter;
        IF_FAILED_EXIT(pthread_mutex_lock(&m_mutex), "pthread_mutex_lock failed")
        thread->running = false;
        IF_FAILED_EXIT(pthread_mutex_unlock(&m_mutex), "pthread_mutex_unlock failed")
    }
}

}// namespace DCanvas
