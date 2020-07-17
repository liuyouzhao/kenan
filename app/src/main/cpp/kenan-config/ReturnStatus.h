//
// Created by hujia on 7/17/20.
//

#ifndef KENAN_RETURNSTATUS_H
#define KENAN_RETURNSTATUS_H

#include <string>

class ReturnStatus {
public:
    ReturnStatus(int stat = 0,
                 std::string msg = std::string(""),
                 std::string err = std::string("")) {
        status = stat;
        message = msg;
        error = err;
    }
    int status;
    std::string message;
    std::string error;
};

#endif //KENAN_RETURNSTATUS_H
