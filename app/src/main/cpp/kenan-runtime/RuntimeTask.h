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
public:
    static RuntimeTask *create(std::string id);
    static void destroy(RuntimeTask *task);

    int setup(std::string file, bool rw = false);
    int setupScript(std::string script);
    int poll();
    int frame();
    void sendMessage(RuntimeMessage &message);

private:
    RuntimeTask(std::string id);
    ~RuntimeTask();

    std::string taskId;

    V8Main *v8MainContext;
    RuntimeMessageLoop *messageLoop;

    bool isRunning;
};

}


#endif //KENAN_RUNTIMETASK_H
