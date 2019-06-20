#ifndef SCRIPT_H
#define SCRIPT_H

#include <v8.h>
#include <gl2d_impl.h>

using namespace v8;

namespace DCanvas
{
class JsLoader;

class Script 
{

public:
    Script();
    ~Script();

    void updata(int length);
    void onLoad();
    void setData(void* data, int size);

    void SetSelf(v8::Persistent<v8::Object> self) { m_js_self = self; }

private:
    char *                          m_data;
    JsLoader*                       m_loader;
    v8::Handle<v8::String>          m_script_source;
    v8::Local<v8::Script>           m_script_runner;
    bool                            m_onLoadSeted;
    char *                          m_src;

    v8::Persistent<v8::Function>                        m_js_OnLoad;
    v8::Persistent<v8::Object>                          m_js_self;

friend class V8Script;
};

} // namepace DCanvas

#endif  // SCRIPT_H
