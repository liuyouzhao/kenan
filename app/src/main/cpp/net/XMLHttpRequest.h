#ifndef XMLHTTPREQUEST_H
#define XMLHTTPREQUEST_H

#include <string>
#include <v8.h>
#include <hash_map>
#include "ObjectWrap.h"
#include <pthread.h>
#include <vector>

namespace DCanvas
{

class XMLHttpRequest : public ObjectWrap
{
    enum State
    {
        UNINTITIALIZED = 0, //initialized state
        OPENED = 1,         //opened but request has not been sent
        SEND = 2,           //send request but not receive response
        RECEIVING = 3,      //in processing request
        DONE = 4            //
    };

public:
    static v8::Handle<v8::Value> GetReadyState(v8::Local<v8::String> property,const AccessorInfo& info);
    static void Set(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    static v8::Handle<v8::Value> GetStatus(v8::Local<v8::String> property,const AccessorInfo& info);
    static v8::Handle<v8::Value> GetResponseText(v8::Local<v8::String> property,const AccessorInfo& info);
    static v8::Handle<v8::Value> GetStatusText(v8::Local<v8::String> property,const AccessorInfo& info);

    static v8::Handle<v8::Value> GetOnreadyStateChange(v8::Local<v8::String> property,const AccessorInfo& info);
    static void SetOnreadyStateChange(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    static v8::Handle<v8::Value> AjaxCreate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void AjaxDestroy(Persistent<Object> self, XMLHttpRequest* parameter);
    static Handle<FunctionTemplate> createAjax();
    static v8::Handle<v8::Value> Open(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Send(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetAllResponseHeaders(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetResponseHeader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetRequestHeader(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Abort(const v8::FunctionCallbackInfo<v8::Value>& args);

    XMLHttpRequest* create();
    ~XMLHttpRequest();
    void callOnreadyStateChange();
    void sendImpl();
    void abort();
    std::string getAllResponseHeaders();
    std::string getResponseHeader(const std::string& name);
    void open(const std::string& method, const std::string&);
    void open(const std::string& method, const std::string&, bool async);
    void open(const std::string& method, const std::string&, bool async, const std::string& user);
    void open(const std::string& method, const std::string&, bool async, const std::string& user, const std::string& password);
    void setRequestHeader(const std::string& name, const std::string& value);
    std::string onkownUpper(const std::string&);
    void send();
    void send(std::string&);
    static void* callOnreadyStateChangeFunc(void*);

private:
    void parseResponseHeader();
    std::string buildQuery(std::string& method, std::string& path, std::string& host, std::string& param);
    void changeState(State newState);
    XMLHttpRequest();
    std::hash_map<std::string, std::string> m_requestHeaders;
    std::hash_map<std::string, std::string> m_responseHeaders;
    State m_state;
    std::string m_url;
    std::string m_responseText;
    int m_status;
    std::string m_method;
    std::string m_statusText;
    std::string m_sendData;
    bool m_async;
    bool m_readyStateChangeFuncSetted;
    v8::Persistent<v8::Function> m_readyStateChangedFunc;

    std::string m_responseHeader;

    static v8::Handle<v8::ObjectTemplate>               s_class_proto;
    static v8::Handle<v8::ObjectTemplate>               s_objectTemplate;
    static Handle<FunctionTemplate>                     s_functionTemplate;
};

} // namespace DCanvas

#endif // XMLHTTPREQUEST_H
