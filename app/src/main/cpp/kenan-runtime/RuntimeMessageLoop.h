//
// Created by hujia on 7/14/20.
//

#ifndef KENAN_RUNTIMELOOP_H
#define KENAN_RUNTIMELOOP_H

#include <queue>
#include "RuntimeMessage.h"

namespace kenan_runtime {

#define RUNTIME_LOOP_FUNCTION
/***
* background Thread-Loop
**/
class RuntimeMessageLoop {
public:
    RuntimeMessageLoop();

    RuntimeMessage pollMessage();
    bool getMessage(RuntimeMessage &message);

    void sendMessage(RuntimeMessage runtimeMessage);
    std::queue<RuntimeMessage> messageQueue;
private:
    pthread_mutex_t         competition;
    pthread_cond_t          cond;
    pthread_condattr_t      attr;
};

}
#endif //KENAN_RUNTIMELOOP_H
