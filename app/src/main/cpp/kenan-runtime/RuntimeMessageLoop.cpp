//
// Created by hujia on 7/14/20.
//

#include "RuntimeMessageLoop.h"
#include "defines.h"

#include "ThreadPool.h"

#include <pthread.h>

#undef LOG_TAG
#define  LOG_TAG    "kenan_runtime::RuntimeMessageLoop"

using namespace kenan_system;

namespace kenan_runtime {

#define DEFAULT_TIMEOUT 1000 * 1000 * 60

void RuntimeMessageLoop::sendMessage(RuntimeMessage runtimeMessage) {
    IF_FAILED_EXIT(pthread_mutex_lock(&competition), "pthread_lock failed")

    messageQueue.push(runtimeMessage);

    IF_FAILED_EXIT(pthread_cond_signal(&cond), "pthread cond signal failed")
    IF_FAILED_EXIT(pthread_mutex_unlock(&competition), "pthread unlock failed")
}

RuntimeMessageLoop::RuntimeMessageLoop() {
    pthread_mutex_init(&competition, 0);
    pthread_condattr_init( &attr);
    pthread_cond_init( &cond, &attr);
}

RuntimeMessage RuntimeMessageLoop::pollMessage() {
    IF_FAILED_EXIT(pthread_mutex_lock(&competition), "pthread_lock failed")

    while(this->messageQueue.size() == 0) {
        IF_FAILED_EXIT(pthread_cond_wait(&cond, &competition), "pthread cond signal failed")
    }
    RuntimeMessage message = this->messageQueue.front();
    IF_FAILED_EXIT(pthread_mutex_unlock(&competition), "pthread unlock failed")

    return message;
}

bool RuntimeMessageLoop::getMessage(RuntimeMessage &msg) {
    bool has = false;
    IF_FAILED_EXIT(pthread_mutex_lock(&competition), "pthread_lock failed")

    if(this->messageQueue.size() > 0) {
        RuntimeMessage message = this->messageQueue.front();
        msg = message;
        has = true;
    }
    IF_FAILED_EXIT(pthread_mutex_unlock(&competition), "pthread unlock failed")
    return has;
}

}