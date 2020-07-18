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
    bool isRunning() {  return mRunning; }


private:
    RuntimeApi();
    int runCodeOnce(std::string code);
    static RuntimeApi *sThis;
    bool mRunning;

    std::map<std::string, RuntimeTask*> taskMap;
    RuntimeTask *engineTask;
};

}


#endif //KENAN_RUNTIMEAPI_H
