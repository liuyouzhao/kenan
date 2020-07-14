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
    RuntimeMessage(std::string title,
                    void *data, void* (*cb)(void *),
                    DataDestructType dataDestructType = DataDestructType::NO_NEED);
    void *invoke();
    std::string title;
private:
    void *data;
    DataDestructType dataDestructType;
    void* (*messageCallbackFunction)(void *);
};

}
#endif //KENAN_RUNTIMEMESSAGE_H