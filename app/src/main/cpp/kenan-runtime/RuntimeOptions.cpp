//
// Created by hujia on 7/10/20.
//

#include "RuntimeOptions.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "RuntimeOptions"

namespace kenan_runtime {

RuntimeOptions::RuntimeOptions() {
    mainTaskId = std::string("main_task");
}

void RuntimeOptions::setWorkingDirectory(const char *dir) {
    workingDirectory = std::string(dir);
}

void RuntimeOptions::setDefaultScriptPath(const char *path) {
    defaultScriptPath = std::string(path);
}

void RuntimeOptions::setScreenSize(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void RuntimeOptions::setEntryScriptCode(std::string code) {
    entryScriptCode = code;
}

}