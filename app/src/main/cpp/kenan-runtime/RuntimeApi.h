//
// Created by hujia on 7/10/20.
//

#ifndef KENAN_RUNTIMEAPI_H
#define KENAN_RUNTIMEAPI_H

#include <string>
#include <map>

#include "V8Main.h"

using namespace kenan_v8bindings;

namespace kenan_runtime {

class RuntimeTask;
class RuntimeOptions;
class RuntimeMessageLoop;

class RuntimeApi {
public:
    static RuntimeApi *instance() {
        if(sThis == NULL)
            sThis = new RuntimeApi();
        return sThis;
    }
    int init(RuntimeOptions &runtimeOptions);
    int deinit();
    int onFrame();

    int runTask(std::string taskId, std::string file, bool rw);
    void stopTask(std::string taskId);
    bool hasTask(std::string taskId) {  return taskMap[taskId] != NULL; }

    RuntimeTask *getTask(std::string taskId) {  return taskMap[taskId]; }
    bool isRunning() {  return mRunning; }

    void sendUserMessage(std::string taskId, std::string target, std::string data);
    void broadcastUserMessage(std::string target, std::string data);

private:
    RuntimeApi();
    int runCodeOnce(std::string code);
    static RuntimeApi *sThis;
    bool mRunning;

    std::map<std::string, RuntimeTask*> taskMap;
    RuntimeTask *engineTask;

    pthread_mutex_t mutex;
};

}


#endif //KENAN_RUNTIMEAPI_H
