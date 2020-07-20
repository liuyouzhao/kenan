//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_SALTHREADOPERATION_H
#define KENAN_SALTHREADOPERATION_H
#include <string>

namespace kenan_sal {
int SAL_taskStart(std::string tid, std::string file, bool rw);
int SAL_taskStart_Script(std::string tid, std::string script);
}

#endif //KENAN_SALTHREADOPERATION_H
