#ifndef JSLOADER_H
#define JSLOADER_H

#include <pthread.h>
#include <string>
#include "CloudxResourceLoader.h"

namespace DCanvas
{

class Script;

class JsLoader : public ResourceLoader
{
public:
    virtual ~JsLoader();
    static   JsLoader* create();
    void     load(char* url, int length, void (*cb)(void* data, int len));
    char*    getData() { return m_data; }
    int      getLength() { return m_nLength; }

    void onJsDataCallback(void* data, int len)
    {
        __on_js_data_callback(data, len);
    }
protected:
    static void* notifyFinished(void*, int, void* ptr);
    static void* thread_func(void*);
    virtual bool  startThread();
    JsLoader();
private:
    char*       m_data;
    int         m_nLength;
    void (*__on_js_data_callback)(void* data, int len);
};

} // namespace DCanvas

#endif // JSLOADER_H
