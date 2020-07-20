//
// Created by hujia on 7/10/20.
//
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/** kenan components */
#include "defines.h"
#include "PlatformConfig.h"
#include "CallBackFuncs.h"
#include "gl2d_impl.h"

#include "RuntimeApi.h"
#include "RuntimeOptions.h"
#include "RuntimeMessage.h"
#include "RuntimeMessageLoop.h"
#include "RuntimeTask.h"

#include "UUID.h"

#undef LOG_TAG
#define  LOG_TAG    "kenan_runtime::RuntimeApi"

using namespace kenan_v8bindings;
using namespace kenan_system;

#define MAIN_GRAPHICS_ID "main_graphics"

namespace kenan_runtime {
RuntimeApi *RuntimeApi::sThis = NULL;
RuntimeApi::RuntimeApi() {
    mRunning = false;
}

int RuntimeApi::init(RuntimeOptions &opts) {

    int ret = 0;
    /**
    (1) Init graphics
    */
    LOGD("[RuntimeOptions] screen size: %d %d", opts.screenWidth, opts.screenHeight);
    Gl2d_Impl::instance()->_Init(opts.screenWidth, opts.screenHeight);

    /**
    (2) Set platform configuration
    */
    PlatformConfig::instance()->setDataDir(opts.workingDirectory);

    /**
    (3) Start run javascript code
    */
    if(opts.entryScriptCode.length() > 0) {
        LOGD("Run entryScriptCode");
        engineTask = RuntimeTask::create(S("kenan-engine-task"));
        if(engineTask->setupScript(opts.entryScriptCode)) {
            LOGE("RuntimeApi::init failed, code setup failed");
            return -1;
        }
    }
    else {
        LOGE("RuntimeApi::init failed, no code here, call either setDefaultScriptPath or setEntryScriptCode in opts");
        ret = -1;
    }

    if(!ret) mRunning = true;
    else mRunning = false;
    return ret;
}

int RuntimeApi::onFrame() {
    if(!mRunning)
        return -1;

    if(engineTask->frame()) {
        LOGE("%s error", __FUNCTION__);
        exit(-1);
    }
    CallBackFuncs::getFuncQueue()->CallAllFunc();
    return 0;
}

int RuntimeApi::deinit() {
    RuntimeTask::destroy(engineTask);
}

int RuntimeApi::runTask(std::string taskId, std::string file, bool rw) {

    LOGD("runTask %s %s", taskId.c_str(), file.c_str());

    RuntimeTask *task = RuntimeTask::create(taskId);

    LOGD("RuntimeTask created %p ==1", task);
    LOGD("RuntimeTask created %p ==2", task);
    LOGD("RuntimeTask created %p ==3", task);
    LOGD("RuntimeTask created %p ==4", task);
    LOGD("RuntimeTask created %p ==5", task);
    usleep(1000 * 500);
    if(task->setup(file, rw)) {
        RuntimeTask::destroy(task);
        LOGE("%s task setup error", __FUNCTION__);
        return -1;
    }
    taskMap[taskId] = task;

    LOGD("Run task will begin %s %s", taskId.c_str(), file.c_str());

    task->loop();
    return 0;
}

void RuntimeApi::stopTask(std::string taskId) {
    RuntimeTask *task = taskMap[taskId];
    if(!task)
        return;
    task->stop();
    RuntimeMessage message(UUID_gen_ulong(), std::string(M_TASK_ON_STOP));
    task->sendMessage(message);
    while(!RuntimeTask::overDestroy(task)) {
        usleep(1000 * 200);
    }
}

}