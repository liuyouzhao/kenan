//
// Created by hujia on 21/06/19.
//

#ifndef KENAN_V8MAIN_H
#define KENAN_V8MAIN_H

#include <v8.h>
#include <string>

using namespace v8;
class V8Main {

public:
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
    void firstRunJavascript(std::string javascript);
    void runJavascript(std::string javascript);
    void destroyV8Environment();
private:
    static V8Main *m_instance;

    Isolate *isolate;
    std::unique_ptr<v8::Platform> platform;
    v8::Isolate::CreateParams create_params;
    v8::Local<v8::Context> context;
    Persistent<Context> persistentContext;


    Local<Context> setupJavascriptEnvironment(Isolate *isolate, Local<Context> context);
};



#endif //KENAN_V8MAIN_H
