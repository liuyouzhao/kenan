#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <vector>

namespace kenan_system
{

class Thread {
friend class ThreadPool;
public:
    Thread(int id, pthread_mutex_t mutex) {
        this->id = id;
        running = true;
        m_mutex = mutex;
    }

    bool isRunning();
private:
    int id;
    bool running;
    pthread_mutex_t             m_mutex;
};

class ThreadPool
{
public:
    static ThreadPool* instance();
    void start(int num, void* (*func)(void*));
    void stop();
private:
    static ThreadPool* s_self;
    ThreadPool();

    std::vector<Thread*>        m_threads;
    pthread_mutex_t             m_mutex;
};

}// namespace kenan_system

#endif // THREADPOOL_H
