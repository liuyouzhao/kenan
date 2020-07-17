//
// Created by hujia on 7/14/20.
//

#ifndef KENAN_RUNTIMELOOP_H
#define KENAN_RUNTIMELOOP_H

#include <queue>
#include "RuntimeMessage.h"

namespace kenan_runtime {

/***
* Main background Thread-Loop
**/
class RuntimeLoop {
public:
    static RuntimeLoop *instance() {
        if(sThis == 0)
            sThis = new RuntimeLoop();
        return sThis;
    }
    RuntimeLoop();

    void start(int threads);
    void stop();
    void sendMessage(RuntimeMessage &runtimeMessage);
    std::queue<RuntimeMessage> messageQueue;
private:
    static RuntimeLoop *sThis;
};


}
#endif //KENAN_RUNTIMELOOP_H
