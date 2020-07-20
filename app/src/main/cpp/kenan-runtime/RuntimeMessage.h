//
// Created by hujia on 7/14/20.
//

#ifndef KENAN_RUNTIMEMESSAGE_H
#define KENAN_RUNTIMEMESSAGE_H

#include <string>

namespace kenan_runtime {

#define M_TASK_ON_START "task_on_start"
#define M_TASK_ON_STOP "task_on_stop"

enum DataDestructType {
    NO_NEED,
    DELETE,
    DELETE_ARRAY,
    FREE
};

class RuntimeMessage {
public:
    RuntimeMessage( unsigned long id,
                    std::string ti,
                    std::string ta,
                    std::string msg = std::string("")):

                    mid(id),
                    title(ti),
                    target(ta),
                    message(msg) {}

    RuntimeMessage( unsigned long id,
                    std::string ti):
                    mid(id),
                    title(ti) {}

    std::string title;
    std::string target;
    std::string message;
    unsigned long mid;
};

}
#endif //KENAN_RUNTIMEMESSAGE_H