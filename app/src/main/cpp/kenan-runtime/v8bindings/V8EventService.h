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
class V8EventService {
public:
    V8EventService();
    static V8EventService* instance()
    {
        if(s_self == NULL)
            s_self = new V8EventService();
        return s_self;
    }

    void hookOnFinishResourceLoading(void* ptr);
    void bind(void* ptr, std::string mark);
    void unbind(void* ptr);

    void callAllReadyHooks(Isolate* isolate, Local<Context> context);

protected:

    std::vector< std::string > m_hookOnFinishResourceMarks;

    std::map<void*, std::string> m_resourceListenerMap;

    pthread_mutex_t         m_mutex;

    static V8EventService *s_self;


};
}


#endif //KENAN_V8EVENTTYPE_H
