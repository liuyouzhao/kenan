//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_RUNTIMETASK_H
#define KENAN_RUNTIMETASK_H

#include <string>
#include <map>

#include "V8Main.h"

using namespace kenan_v8bindings;

namespace kenan_runtime {
class RuntimeMessage;
class RuntimeMessageLoop;

class RuntimeTask {
friend class RuntimeApi;
public:
    static RuntimeTask *create(std::string id);
    static void destroy(RuntimeTask *task);
    static bool overDestroy(RuntimeTask *task) {
        if(task->isOver()) {
            RuntimeTask::destroy(task);
            return true;
        }
        return false;
    }
    bool isOver() { return over; }

    int setup(std::string file, bool rw = false);
    int setupScript(std::string script);

    void loop();
    void stop() {   isRunning = false; }
    int frame();
    void sendMessage(RuntimeMessage &message);

private:
    RuntimeTask(std::string id);
    ~RuntimeTask();
    int poll(bool block = true);

    std::string taskId;

    V8Main *v8MainContext;
    RuntimeMessageLoop *messageLoop;

    bool isRunning;
    bool over;
};

}


#endif //KENAN_RUNTIMETASK_H
