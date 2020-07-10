//
// Created by hujia on 21/06/19.
//

#ifndef KENAN_V8MAIN_H
#define KENAN_V8MAIN_H

#include "libplatform/libplatform.h"
#include <v8.h>
#include <string>

using namespace v8;

namespace kenan_v8bindings {
class V8Main {

public:
    enum V8CodeState
    {
        NOT_READY,
        COMPILED,
        RUN_FIRST,
        RUNNING
    };

    static V8Main *instance()
    {
        if(m_instance == NULL)
        {
            m_instance = new V8Main();
            return m_instance;
        }
        return m_instance;
    }

    void initV8Environment();
    int firstRunJavascript(std::string javascript);
    void runJavascript(std::string javascript);

    void onFrameUpdateCallback();

    void destroyV8Environment();
private:
    static V8Main *m_instance;

    Isolate *isolate;
    std::unique_ptr<v8::Platform> platform;
    v8::Isolate::CreateParams create_params;
    v8::Local<v8::Context> context;
    Persistent<Context> persistentContext;

    void setupJavascriptGlobalObjects(Isolate *isolate, Local<Context> context);
    Local<Context> createJavascriptContext(Isolate *isolate);

    V8CodeState codeState;
};

}

#endif //KENAN_V8MAIN_H
