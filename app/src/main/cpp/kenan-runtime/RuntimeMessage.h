//
// Created by hujia on 7/14/20.
//

#ifndef KENAN_RUNTIMEMESSAGE_H
#define KENAN_RUNTIMEMESSAGE_H

#include <string>

namespace kenan_runtime {

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
                    std::string msg):
                    mid(id),
                    title(ti),
                    target(ta),
                    message(msg)

    {}
    std::string title;
    std::string target;
    std::string message;
    unsigned long mid;
};

}
#endif //KENAN_RUNTIMEMESSAGE_H