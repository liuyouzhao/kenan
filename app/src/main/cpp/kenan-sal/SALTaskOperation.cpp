//
// Created by hujia on 7/18/20.
//
#include "defines.h"
#include "Log.h"
#include "SAL.h"
#include "SALTaskOperation.h"

#undef LOG_TAG
#define  LOG_TAG    "SALTaskOperation"

namespace kenan_sal {
int SAL_taskStart(std::string tid, std::string file, bool rw) {
    LOGD("SAL_taskStart %s", file.c_str());
    return si_taskOp.SI_taskStart(tid.c_str(), file.c_str(), rw ? 1 : 0);
}

int SAL_taskStart_Script(std::string tid, std::string script) {
    LOGD("%s NOT implemented.", __FUNCTION__);
    return 0;
}

}