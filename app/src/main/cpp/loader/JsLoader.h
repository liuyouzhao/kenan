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
    void     load(char* url, int length, Script* pScript);
    char*    getData() { return m_data; }
    int      getLength() { return m_nLength; }
protected:
    static void* notifyFinished(void*, int, void* ptr);
    static void* thread_func(void*);
    virtual bool  startThread();
    JsLoader();
private:
    char*       m_data;
    int         m_nLength;
    Script*     m_pScript;
};

} // namespace DCanvas

#endif // JSLOADER_H
