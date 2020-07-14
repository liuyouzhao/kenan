//
// Created by hujia on 7/14/20.
//

#include "RuntimeMessage.h"

namespace kenan_runtime {

RuntimeMessage::RuntimeMessage(std::string title, void *data, void* (*cb)(void *), DataDestructType dataDestructType) {
    this->title = title;
    this->data = data;
    this->dataDestructType = dataDestructType;
    messageCallbackFunction = cb;
}

void *RuntimeMessage::invoke() {
    if(messageCallbackFunction != NULL) {
        messageCallbackFunction(data);

        switch(dataDestructType) {
        case DataDestructType::NO_NEED:
        break;
        case DataDestructType::DELETE:
            delete data;
        break;
        case DataDestructType::DELETE_ARRAY:
            delete[] data;
        break;
        case DataDestructType::FREE:
            free(data);
        break;
        }
    }
}

}