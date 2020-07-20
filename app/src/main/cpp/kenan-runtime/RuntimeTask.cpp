//
// Created by hujia on 7/18/20.
//
#include "defines.h"
#include "RuntimeTask.h"
#include "RuntimeMessageLoop.h"
#include "RuntimeMessage.h"
#include "UUID.h"
#include "SAL.h"
#include "SALFileOperation.h"
#include "SALTaskOperation.h"

using namespace kenan_system;

#undef LOG_TAG
#define  LOG_TAG    "RuntimeTask"

using namespace kenan_sal;

namespace kenan_runtime {

RuntimeTask *RuntimeTask::create(std::string id) {
    RuntimeTask *p = new RuntimeTask(id);
    return p;
}

void RuntimeTask::destroy(RuntimeTask *task) {
    delete task;
}

RuntimeTask::RuntimeTask(std::string id) {
    taskId = id;
    isRunning = false;
    over = false;
    v8MainContext = new V8Main();
    v8MainContext->initV8Environment();
    messageLoop = new RuntimeMessageLoop();
}

RuntimeTask::~RuntimeTask() {
    LOGD("-----------1");
    v8MainContext->destroyV8Environment();
    LOGD("-----------2");
    delete v8MainContext;
    LOGD("-----------3");
    delete messageLoop;
    LOGD("-----------4");
}

void RuntimeTask::sendMessage(RuntimeMessage &message) {
    messageLoop->sendMessage(message);
}

int RuntimeTask::setupScript(std::string script) {
    if(v8MainContext->firstRunJavascript(script)) {
        LOGE("%s first run code failed", __FUNCTION__);
        return -1;
    }
    LOGD("setupScript [OK] \n%s", script.c_str());
    isRunning = true;
    messageLoop->sendMessage(RuntimeMessage(UUID_gen_ulong(), S(M_TASK_ON_START), taskId, S("")));
    return 0;
}

int RuntimeTask::setup(std::string file, bool rw) {
    char *code = new char[TEXT_SCRIPT_BUFFER_LENGTH];
    memset(code, 0, TEXT_SCRIPT_BUFFER_LENGTH);
    int length = 0;

    int rt = SAL_systemReadFile(file.c_str(), code, length, TEXT_SCRIPT_BUFFER_LENGTH,
                        rw ? SAL_FOLDER_TYPE::RW_LOCATION : SAL_FOLDER_TYPE::RO_LOCATION);
    if(rt < 0) {
        LOGE("%s failed, failed read error", __FUNCTION__);
        return rt;
    }
    if(rt == 0) {
        LOGE("%s warning, empty code file", __FUNCTION__);
        return rt;
    }

    int ret = setupScript(std::string(code));
    delete[] code;
    return ret;
}

int RuntimeTask::frame() {
    return v8MainContext->onFrameCallback();
}

int RuntimeTask::poll() {
    RuntimeMessage message = messageLoop->pollMessage();
    return v8MainContext->onMessageCallback(message.mid, message.title, message.target, message.message);
}

void RuntimeTask::loop() {
    while(isRunning) {
        if(this->poll()) {
            LOGE("%s poll message callback has error, this task will stop", __FUNCTION__);
            isRunning = false;
        }
    }
    over = true;
}

}