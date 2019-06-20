#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <v8.h>
#include "ObjectWrap.h"

using namespace v8;

namespace DCanvas
{

#define URL_MAX_LEN 300

class SocketClient;
class SocketPackage;

class WebSocketImpl : ObjectWrap
{
public:
    WebSocketImpl(char* argv);
    ~WebSocketImpl();

    static Handle<FunctionTemplate> createWebSocket();

    static Handle<v8::Value> WebSocketCreate(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void WebSocketDestroy(Persistent<Object> self, WebSocketImpl* parameter);

    static Handle<v8::Value>     GetOnOpen(Local<v8::String> property,const AccessorInfo& info);
    static Handle<v8::Value>     GetOnError(Local<v8::String> property,const AccessorInfo& info);
    static Handle<v8::Value>     GetOnClose(Local<v8::String> property,const AccessorInfo& info);
    static Handle<v8::Value>     GetOnMessage(Local<v8::String> property,const AccessorInfo& info);
    static Handle<v8::Value>     GetBufferedAmount(Local<v8::String> property,const AccessorInfo& info);
    static void              SetOnOpen(Local<v8::String> property, Local<v8::Value> value, const AccessorInfo& info);
    static void              SetOnError(Local<v8::String> property, Local<v8::Value> value, const AccessorInfo& info);
    static void              SetOnClose(Local<v8::String> property, Local<v8::Value> value, const AccessorInfo& info);
    static void              SetOnMessage(Local<v8::String> property, Local<v8::Value> value, const AccessorInfo& info);
    static void              SetBufferedAmount(Local<v8::String> property, Local<v8::Value> value, const AccessorInfo& info);

    static Handle<v8::Value> Send(const v8::FunctionCallbackInfo<v8::Value>& args);
    static Handle<v8::Value> Close(const v8::FunctionCallbackInfo<v8::Value>& args);

    void                     onOpen();
    void                     onError();
    void                     onClose();
    void                     onMessage(const char* data, int len);

    SocketClient*            getSocket()    { return m_pSocketclt; };
private:
    Persistent<Function>                                m_js_onopen;
    Persistent<Function>                                m_js_onerror;
    Persistent<Function>                                m_js_onclose;
    Persistent<Function>                                m_js_onmessage;

    static Handle<ObjectTemplate>                       s_class_proto;
    static Handle<ObjectTemplate>                       s_objectTemplate;
    static Handle<FunctionTemplate>                     s_functionTemplate;

    SocketClient*                                       m_pSocketclt;
    bool                                                m_bClosed;
};

} // namespace

#endif // WEBSOCKET_H
