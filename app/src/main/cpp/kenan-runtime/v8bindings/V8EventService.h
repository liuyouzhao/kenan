//
// Created by hujia on 26/06/19.
//

#ifndef KENAN_V8EVENTTYPE_H
#define KENAN_V8EVENTTYPE_H

#include <string>
#include <map>
#include <vector>
#include <pthread.h>
#include <v8.h>

using namespace v8;

namespace kenan_v8bindings
{
#define V8E_GRAPHICS_IMAGE_ONLOAD "graphics_image_onload"
#define V8E_SUCCESS "success"
#define V8E_FAILED "failed"

class V8Event {
public:
    V8Event(std::string t = std::string("default"),
            std::string a0 = std::string(""),
            std::string a1 = std::string(""),
            std::string a2 = std::string(""),
            std::string a3 = std::string("")) {
        title = t; arg0 = a0; arg1 = a1; arg2 = a2; arg3 = a3;
    }
    std::string title;
    std::string arg0;
    std::string arg1;
    std::string arg2;
    std::string arg3;
};

class V8EventService {
public:
    V8EventService();
    static V8EventService* instance()
    {
        if(s_self == NULL)
            s_self = new V8EventService();
        return s_self;
    }

    void postEvent(V8Event event) {
        pthread_mutex_lock(&m_mutex);
        m_hookOnFinishResourceMarks.push_back(event);
        pthread_mutex_unlock(&m_mutex);
    }

    void hookOnFinishResourceLoading(void* ptr);
    void bind(void* ptr, std::string mark);
    void unbind(void* ptr);

    void callAllReadyHooks(Isolate* isolate, Local<Context> context);

protected:

    std::vector< V8Event > m_hookOnFinishResourceMarks;

    std::map<void*, std::string> m_resourceListenerMap;

    pthread_mutex_t         m_mutex;

    static V8EventService *s_self;
};
}


#endif //KENAN_V8EVENTTYPE_H
