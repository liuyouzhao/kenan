#include "defines.h"
#include <string>
#include "WebSocketImpl.h"
#include "SocketClient.h"

#undef LOG_TAG
#define  LOG_TAG    "WebSocketImpl"

namespace DCanvas
{

Handle<v8::ObjectTemplate>                  WebSocketImpl::s_class_proto;
Handle<v8::ObjectTemplate>                  WebSocketImpl::s_objectTemplate;
Handle<FunctionTemplate>                    WebSocketImpl::s_functionTemplate;

WebSocketImpl::WebSocketImpl(char* argv):
    m_bClosed(false)
{
    std::string url = argv;

    if (url.length() == 0)
    {
        m_pSocketclt = new SocketClient(this);
    }
    else
    {
        std::string ip_port;
        std::string port;
        std::string ip;
        ip_port = url.substr(5);
        int n = ip_port.find(":");
        port = ip_port.substr(n + 1);
        ip = ip_port.substr(0, n);
        int nPort = atoi(port.c_str());
        m_pSocketclt = new SocketClient((char*)ip.c_str(), nPort, this);
    }
}

WebSocketImpl::~WebSocketImpl()
{
    if(m_pSocketclt)
    {
        delete m_pSocketclt;
        m_pSocketclt = NULL;
    }

    if(!m_js_onopen.IsEmpty())
    {
    	m_js_onopen.Dispose();
    	m_js_onopen.Clear();
    }

    if(!m_js_onerror.IsEmpty())
    {
	    m_js_onerror.Dispose();
	    m_js_onerror.Clear();
    }

    if(!m_js_onclose.IsEmpty())
    {
	    m_js_onclose.Dispose();
	    m_js_onclose.Clear();
    }

    if(!m_js_onmessage.IsEmpty())
   	{
       	m_js_onmessage.Dispose();
       	m_js_onmessage.Clear();
   	}
}

v8::Handle<v8::Value> WebSocketImpl::WebSocketCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!args.IsConstructCall())
    {
        return THROW_EXCEPTION(TError, "Cannot call constructor as function");
    }

    if (args.Length() != 1)
    {
        LOGE("Call WebSocketCreate not with 1 arg");
        return THROW_EXCEPTION(TRangeError, "Expected no arguments");
    }
    Local<Value> url = args[0];
    Local<String> url_str = url->ToString();

    char tmp_url[URL_MAX_LEN] = {0};
    int len = URL_MAX_LEN > url_str->Length() ? url_str->Length() : URL_MAX_LEN;
    url_str->WriteUtf8(tmp_url, len);

    if (len == 0)
        return THROW_EXCEPTION(TError, "constructor url length zero error");

    WebSocketImpl * t = new WebSocketImpl(tmp_url);
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(WebSocketImpl::WebSocketDestroy));

    self->SetInternalField(0, External::New(t));
    t->m_pSocketclt->scConnect();
    return self;
}

void WebSocketImpl::WebSocketDestroy(Persistent<Object> self, WebSocketImpl* parameter)
{
    if (parameter)
    {
        delete parameter;
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
}

Handle<FunctionTemplate> WebSocketImpl::createWebSocket()
{
    s_functionTemplate = FunctionTemplate::New(WebSocketImpl::WebSocketCreate);
    s_functionTemplate->SetClassName(v8::String::New("WebSocket"));
    s_class_proto = s_functionTemplate ->PrototypeTemplate();

    s_class_proto->Set(v8::String::New("send"),FunctionTemplate::New(WebSocketImpl::Send));

    s_objectTemplate = s_functionTemplate->InstanceTemplate();
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate ->SetAccessor(v8::String::New("onopen"), WebSocketImpl::GetOnOpen, WebSocketImpl::SetOnOpen);
    s_objectTemplate ->SetAccessor(v8::String::New("onerror"), WebSocketImpl::GetOnError, WebSocketImpl::SetOnError);
    s_objectTemplate ->SetAccessor(v8::String::New("onclose"), WebSocketImpl::GetOnClose, WebSocketImpl::SetOnClose);
    s_objectTemplate ->SetAccessor(v8::String::New("onmessage"), WebSocketImpl::GetOnMessage, WebSocketImpl::SetOnMessage);
    s_objectTemplate ->SetAccessor(v8::String::New("bufferedAmount"), WebSocketImpl::GetBufferedAmount, WebSocketImpl::SetBufferedAmount);
    return s_functionTemplate;
}

Handle<v8::Value> WebSocketImpl::Send(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() == 0 || args.Length() > 1)
    {
        return v8::String::New("arguments exception");
    }
    WebSocketImpl* ws = ObjectWrap::Unwrap<WebSocketImpl>(args.This());
    if (ws->m_bClosed)
    {
        ws->m_pSocketclt->scConnect();
        ws->m_bClosed = false;
    }
    v8::Handle<v8::Value> data = args[0];
    v8::Handle<v8::String> strData = data->ToString();
    int len = MAXLEN < strData->Length() ? MAXLEN : strData->Length();
    char data_send[MAXLEN] = {0};
    strData->WriteUtf8(data_send, len);
    ws->m_pSocketclt->scSend(data_send, len);
    return v8::Undefined();
}

Handle<v8::Value> WebSocketImpl::Close(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() >= 1)
    {
        return v8::String::New("arguments exception");
    }
    WebSocketImpl* ws = ObjectWrap::Unwrap<WebSocketImpl>(args.This());
    ws->m_bClosed = true;
    ws->m_pSocketclt->disconnect();
    return v8::Undefined();
}

void WebSocketImpl::SetOnOpen(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    if (value->IsFunction())
    {
        class_impl->m_js_onopen = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        THROW_EXCEPTION(TError, "onopen function error");
        return;
    }
}

void WebSocketImpl::SetOnError(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    if (value->IsFunction())
    {
        class_impl->m_js_onerror = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        THROW_EXCEPTION(TError, "onError function error");
        return;
    }
}

void WebSocketImpl::SetOnClose(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    if (value->IsFunction())
    {
        class_impl->m_js_onclose = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        THROW_EXCEPTION(TError, "onclose function error");
        return;
    }
}

void WebSocketImpl::SetOnMessage(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    if (value->IsFunction())
    {
        class_impl->m_js_onmessage = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        THROW_EXCEPTION(TError, "onmessage function error");
        return;
    }
}

void WebSocketImpl::SetBufferedAmount(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    // do nothing
}

Handle<Value> WebSocketImpl::GetOnOpen(Local<String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    return class_impl->m_js_onopen;
}

Handle<Value> WebSocketImpl::GetOnError(Local<String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    return class_impl->m_js_onerror;
}

Handle<Value> WebSocketImpl::GetOnClose(Local<String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    return class_impl->m_js_onclose;
}

Handle<Value> WebSocketImpl::GetOnMessage(Local<String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    return class_impl->m_js_onmessage;
}

Handle<Value> WebSocketImpl::GetBufferedAmount(Local<String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(WebSocketImpl, info);

    Handle<Number> zero = Number::New(class_impl->m_pSocketclt->getBufferedAmount());

    return zero;
}

void WebSocketImpl::onOpen()
{
    HandleScope scope;
    m_js_onopen->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
}

void WebSocketImpl::onError()
{
    HandleScope scope;
    m_js_onerror->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
}

void WebSocketImpl::onClose()
{
    HandleScope scope;
    m_js_onclose->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
}

void WebSocketImpl::onMessage(const char* data, int len)
{
    HandleScope scope;
    Handle<Value> v8String[1];

    v8String[0] = String::New(data);
    m_js_onmessage->Call(v8::Context::GetCurrent()->Global(), 1, v8String);
}

} // namespace DCanvas
