//
// Created by hujia on 7/14/20.
//

#include "RuntimeLoop.h"
#include "defines.h"

#include "ThreadPool.h"

#include <pthread.h>

#undef LOG_TAG
#define  LOG_TAG    "kenan_runtime::RuntimeLoop"

using namespace kenan_system;

namespace kenan_runtime {

#define DEFAULT_TIMEOUT 1000 * 1000 * 60

pthread_mutex_t         competition;
pthread_cond_t          cond;
pthread_condattr_t      attr;

static void __oneLoop() {
    IF_FAILED_EXIT(pthread_mutex_lock(&competition), "pthread_lock failed")

    while(RuntimeLoop::instance()->messageQueue.size() == 0) {
        IF_FAILED_EXIT(pthread_cond_wait(&cond, &competition), "pthread cond signal failed")
    }

    RuntimeMessage message = RuntimeLoop::instance()->messageQueue.front();
    RuntimeLoop::instance()->messageQueue.pop();
    message.invoke();

    IF_FAILED_EXIT(pthread_mutex_unlock(&competition), "pthread unlock failed")
}

static void* __runtime_Thread_BGMain(void *thread) {
    Thread *currentThread = (Thread*)thread;
    while(currentThread->isRunning()) {
        __oneLoop();
    }
    return NULL;
}

RuntimeLoop *RuntimeLoop::sThis = NULL;

void RuntimeLoop::sendMessage(RuntimeMessage &runtimeMessage) {
    IF_FAILED_EXIT(pthread_mutex_lock(&competition), "pthread_lock failed")

    messageQueue.push(runtimeMessage);

    IF_FAILED_EXIT(pthread_cond_signal(&cond), "pthread cond signal failed")
    IF_FAILED_EXIT(pthread_mutex_unlock(&competition), "pthread unlock failed")
}

RuntimeLoop::RuntimeLoop() {
    pthread_mutex_init(&competition, 0);
    pthread_condattr_init( &attr);
    pthread_cond_init( &cond, &attr);
}

void RuntimeLoop::start(int threads) {
    threads = threads <= 1 ? 2 : threads;
    threads = threads > GLOBAL_MAX_THREADS_NUM ? GLOBAL_MAX_THREADS_NUM : threads;
    ThreadPool::instance()->start(threads, __runtime_Thread_BGMain);
}

void RuntimeLoop::stop() {
    ThreadPool::instance()->stop();
}

}