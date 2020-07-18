//
// Created by hujia on 7/10/20.
//
#include <string.h>
#include <stdio.h>

/** kenan components */
#include "defines.h"
#include "V8MainEngine.h"
#include "PlatformConfig.h"
#include "CallBackFuncs.h"
#include "gl2d_impl.h"

#include "RuntimeApi.h"
#include "RuntimeOptions.h"
#include "RuntimeMessageLoop.h"

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
        ret = runCodeOnce(opts.entryScriptCode);
    }
    else if(opts.defaultScriptPath.length() > 0) {
        LOGD("Run defaultScriptPath");
    }
    else {
        LOGE("RuntimeApi::init failed, no code here, call either setDefaultScriptPath or setEntryScriptCode in opts");
        ret = -1;
    }

    RuntimeMessageLoop *graphicsLoop = new RuntimeMessageLoop();
    threadsLoopMap[std::string(MAIN_GRAPHICS_ID)] = graphicsLoop;
    threadsV8Map[std::string(MAIN_GRAPHICS_ID)] = V8MainEngine::instance();

    if(!ret) mRunning = true;
    else mRunning = false;
    return ret;
}

int RuntimeApi::runCodeOnce(std::string code) {
    V8MainEngine::instance()->initV8Environment();
    if(V8MainEngine::instance()->firstRunJavascript(code)) {
        LOGE("RuntimeApi::runCodeOnce failed");
        return -1;
    }
    return 0;
}

int RuntimeApi::onFrame() {
    if(!mRunning)
        return -1;
    V8MainEngine::instance()->onFrameUpdateCallback();
    CallBackFuncs::getFuncQueue()->CallAllFunc();
    return 0;
}

int RuntimeApi::deinit() {
    V8MainEngine::instance()->destroyV8Environment();
}

}