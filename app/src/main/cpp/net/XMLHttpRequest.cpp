#include "defines.h"
#include "XMLHttpRequest.h"
#include "CallBackFuncs.h"
#include "DCSocket.h"
#include "CallBackFuncs.h"

#undef LOG_TAG
#define  LOG_TAG    "XMLHttpRequest"

namespace DCanvas
{

v8::Handle<v8::ObjectTemplate>                  XMLHttpRequest::s_class_proto;
v8::Handle<v8::ObjectTemplate>                  XMLHttpRequest::s_objectTemplate;
Handle<FunctionTemplate>                        XMLHttpRequest::s_functionTemplate;

XMLHttpRequest* XMLHttpRequest::create()
{
    return new XMLHttpRequest();
}

XMLHttpRequest::XMLHttpRequest()
    :m_responseText("")
{
    m_readyStateChangeFuncSetted = false;
}

XMLHttpRequest::~XMLHttpRequest()
{
    if (m_readyStateChangeFuncSetted)
        m_readyStateChangedFunc.Dispose();
}

void XMLHttpRequest::abort()
{
    //TODO to implement
}

std::string XMLHttpRequest::getAllResponseHeaders()
{
    std::string res("");
    std::hash_map<std::string, std::string>::iterator it = m_responseHeaders.begin();
    for (;it != m_responseHeaders.end();++it)
    {
        res += it->first + ": " + it->second + "\r\n";
    }
    return res;
}

std::string XMLHttpRequest::getResponseHeader(const std::string& name)
{

    return m_responseHeaders[name];
}

void XMLHttpRequest::open(const std::string& method, const std::string& url)
{
    open(method, url, true);
}

void XMLHttpRequest::open(const std::string& method, const std::string& url, bool async)
{
    State previousState = m_state;
    m_state = UNINTITIALIZED;
    m_method = onkownUpper(method);
    m_url = url;
    m_async = async;

    if (previousState != OPENED)
        changeState(OPENED);
    else
        m_state = OPENED;
}

void XMLHttpRequest::open(const std::string& method, const std::string& url, bool async, const std::string& user)
{
    open(method, url/*add user*/, async );
}

void XMLHttpRequest::open(const std::string& method, const std::string& url,
                            bool async, const std::string& user, const std::string& password)
{
    open(method, url/*add user and password*/, async);
}

void XMLHttpRequest::setRequestHeader(const std::string& name, const std::string& value)
{
    m_requestHeaders[name] = value;
}

static void* async_func(void* p )
{
    XMLHttpRequest* img = (XMLHttpRequest*)p;
    img->sendImpl();
    return NULL;
}

std::string XMLHttpRequest::buildQuery(std::string& method,
                                        std::string& path,
                                        std::string& host,
                                        std::string& param)
{
    std::string res("");
    res += method + " " + path + " HTTP/1.0\r\n"
        + "Host: " + host + "\r\n"
        + "User-Agent: XMLHttpRequest 1.0\r\n";

    std::hash_map<std::string, std::string>::iterator it = m_requestHeaders.begin();
    for (;it != m_requestHeaders.end();++it)
    {
        res += it->first + it->second + "\r\n";
    }
    res += "\r\n";

    if (m_sendData.length() > 0)
        res+= m_sendData;

    return res;
}

void XMLHttpRequest::sendImpl()
{
    DCSocket* tcp = DCSocket::create();
    int i =0;
    std::string method;
    int port = 80;
    std::string path;
    std::string param;
    std::string host;

    DCSocket::parse(m_url.c_str(), m_url.length(), method, host, port, path, param);

    tcp->setUrl((char*)host.c_str());
    tcp->setPort(port);

    tcp->dcconnect();
    if (path.size() == 0)
    path ="/";
    tcp->dcsend(buildQuery(m_method, path, host, param));
    changeState(SEND);

    char buf[1025];
    int htmlstart = 0;
    char * htmlcontent;
    int tmpres = 0;
    m_responseHeader = "";
    while ((tmpres = tcp->dcrecv(buf, 1024)) > 0)
    {
        if (htmlstart == 0)
        {
            htmlcontent = strstr(buf, "\r\n\r\n");
            if (htmlcontent == NULL)
            {
                for (i=0; i<tmpres; ++i)
                {
                    m_responseHeader += buf[i];
                }
            }
            else
            {
                for (i=0; buf+i != htmlcontent;++i)
                {
                    m_responseHeader += buf[i];
                }
                htmlstart = 1;
                htmlcontent += 4;
                for (char* s = htmlcontent; s != buf + tmpres; ++s)
                {
                    m_responseText += *s;
                }
                parseResponseHeader();
                changeState(RECEIVING);
            }
        }
        else
        {
            for (i = 0; i < tmpres; ++i)
                m_responseText += buf[i];
        }
    }

    changeState(DONE);
    delete tcp;
}

void XMLHttpRequest::send()
{
    if (!m_async)
    {
        sendImpl();
    }
    else
    {
        pthread_t thread;
        int result = pthread_create(&thread, NULL, async_func, (void*)this);
        if (result != 0)
        {
            LOGE("create thread xmlHttpRequest failed!");
        }
    }

}

void XMLHttpRequest::send(std::string& str)
{
    m_sendData  = str;
    send();
}

void XMLHttpRequest::changeState(State newState)
{
    if (m_state != newState)
    {
        m_state = newState;
        if (!m_async)
        {
            callOnreadyStateChange();
        }
        else
        {
            FuncData* fd = new FuncData();
            fd->data = callOnreadyStateChangeFunc;
            fd->param = this;
            CallBackFuncs::getFuncQueue()->AddFunc(fd);
        }
    }
}

void XMLHttpRequest::callOnreadyStateChange()
{
    if (!m_readyStateChangeFuncSetted)
    {
        LOGE("readyStateChanged func lost");
        return;
    }

    m_readyStateChangedFunc->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
}

void* XMLHttpRequest::callOnreadyStateChangeFunc(void* p)
{
    ((XMLHttpRequest*)p)->callOnreadyStateChange();
    return NULL;
}

static void getLine(std::string& line, int& i, const std::string &responseHeader)
{
    line = "";
    int len = responseHeader.length() -1;
    while (responseHeader[i] == '\r' && responseHeader[i+1] == '\n')
    {
        i += 2;
    }

    while (i < len)
    {
        if (responseHeader[i]=='\r' && responseHeader[i+1] == '\n')
        {
            break;
        }
        else
        {
            line += responseHeader[i++];
        }
    }
}

void XMLHttpRequest::parseResponseHeader()
{
    int i = 0;
    int len = m_responseHeader.length();
    std::string line("");

    getLine(line, i, m_responseHeader);
    int llen = line.length();
    int j = 0;
    while (j < llen && line[j++]!=' ')
    {
        ;//
    }

    m_status = 0;
    while (j < llen && line[j] !=' ')
    {
        m_status = m_status*10 + line[j++];
    }

    m_statusText = "";
    while (j < llen && line[j] != '\r')
        m_statusText += line[j++];
    ////m_status and m_statusText ok

    std::string key;
    std::string word;
    while (i < len - 4)
    {
        key = "";
        word = "";
        j = 0;
        getLine(line, i, m_responseHeader);
        llen = line.length();
        while (j < llen && line[j] != ':')
        {
            key += line[j++];
        }

        if (key == "Set-Cookie")
        {
            while (j < llen && line[j] != ':')key += line[j++];
        }

        while (j < llen)
        {
            word += line[j++];
        }
        m_responseHeaders.insert(std::make_pair(key, word));
    }
}

std::string XMLHttpRequest::onkownUpper(const std::string& method)
{
    std::string s(method);
    for (unsigned int i = 0; i < method.length(); ++i)
    {
        s[i] = method[i] >= 97 ? method[i] - 32 : method[i];
    }

    if (s == "COPY" || s ==  "DELETE" || s == "GET" || s == "HEAD"
        || s == "INDEX" || s == "LOCK" || s == "M-POST" || s == "MKCOL"
        || s == "MOVE" || s == "OPTIONS" || s == "POST" || s == "PROPFIND"
        || s == "PROPPATCH" || s == "PUT" || s == "UNLOCK")
    {
           return s;
    }
    return method;
}

v8::Handle<v8::Value> XMLHttpRequest::GetReadyState(v8::Local<v8::String> property,
                                                        const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(XMLHttpRequest, info);

    return v8::Int32::New(class_impl->m_state);
}

v8::Handle<v8::Value> XMLHttpRequest::GetStatus(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(XMLHttpRequest, info);

    return v8::Int32::New(class_impl->m_status);
}

v8::Handle<v8::Value> XMLHttpRequest::GetStatusText(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(XMLHttpRequest, info);

    return v8::String::New(class_impl->m_statusText.c_str());
}

v8::Handle<v8::Value> XMLHttpRequest::GetResponseText(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(XMLHttpRequest, info);

    return v8::String::New(class_impl->m_responseText.c_str());
}

v8::Handle<v8::Value> XMLHttpRequest::GetOnreadyStateChange(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void XMLHttpRequest::SetOnreadyStateChange(v8::Local<v8::String> property,
                                            v8::Local<v8::Value> value,
                                            const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(XMLHttpRequest, info);
    if (!value->IsFunction())
    {
        THROW_EXCEPTION(TTypeError,
                "Error:: xmlHttpRequest SetOnreadyStateChange function Exception!!");
        return;
    }
    class_impl->m_readyStateChangedFunc = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(value));
    class_impl->m_readyStateChangeFuncSetted = true;
}

void XMLHttpRequest::Set(v8::Local<v8::String> property,
                         v8::Local<v8::Value> value,
                         const AccessorInfo& info)
{
    HandleScope scope;
}

v8::Handle<v8::Value> XMLHttpRequest::Open(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());
    if (args.Length() == 3)
    {
        if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsBoolean())
        {
            return THROW_EXCEPTION(TTypeError,
                    "Error:: xmlHttpRequest open arguments wrong type Exception!!");
        }
        v8::String::Utf8Value method(args[0]->ToString());
        v8::String::Utf8Value url(args[1]->ToString());
        request->open(std::string(*method), std::string(*url), args[2]->BooleanValue());
    }
    else
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest open arguments wrong Exception!");
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> XMLHttpRequest::Send(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());
    if (args.Length() == 0)
    {
        request->send();
    }
    else
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest send arguments wrong Exception!");
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> XMLHttpRequest::GetAllResponseHeaders(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest GetAllResponseHeaders arguments wrong Exception!");
    }
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());

    return v8::String::New(request->getAllResponseHeaders().c_str());
}

v8::Handle<v8::Value> XMLHttpRequest::GetResponseHeader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest GetResponseHeader arguments wrong Exception!!");
    }
    if (!args[0]->IsString())
    {
        return THROW_EXCEPTION(TTypeError,
                "Error:: xmlHttpRequest GetResponseHeader arguments type wrong Exception!!");

    }
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());

    v8::String::Utf8Value name(args[0]->ToString());

    return v8::String::New(request->getResponseHeader(std::string(*name)).c_str());
}

v8::Handle<v8::Value> XMLHttpRequest::SetRequestHeader(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 2)
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest SetRequestHeader arguments wrong Exception!!");
    }
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());
    if (!args[0]->IsString() || args[1]->IsString())
    {
        return THROW_EXCEPTION(TTypeError,
                "Error:: xmlHttpRequest SetRequestHeader arguments type wrong Exception!!");
    }
    v8::String::Utf8Value key(args[0]->ToString());
    v8::String::Utf8Value word(args[1]->ToString());

    request->setRequestHeader(std::string(*key), std::string(*word));

    return v8::Undefined();
}

v8::Handle<v8::Value> XMLHttpRequest::Abort(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TRangeError,
                "Error:: xmlHttpRequest Abort arguments wrong Exception!!");
    }
    XMLHttpRequest *request = ObjectWrap::Unwrap<XMLHttpRequest>(args.This());

    request->abort();

    return v8::Undefined();
}

// js T constructor function, called when `new T(...)' in js
v8::Handle<v8::Value> XMLHttpRequest::AjaxCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return THROW_EXCEPTION(TError, "Cannot call constructor as function");

    // throw if we didn't get 0 args
    if (args.Length() != 0)
        return THROW_EXCEPTION(TRangeError, "Expected no arguments");

    // create the C++ Handle<T> to be wrapped
    XMLHttpRequest * t = new XMLHttpRequest();

    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(XMLHttpRequest::AjaxDestroy));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(t));

    LOGD("This is the Constructor of XMLHttpRequest!");
    return self;
}

/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void XMLHttpRequest::AjaxDestroy(Persistent<Object> self, XMLHttpRequest* parameter)
{

    //v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    if (parameter)
    {
        delete parameter;
        parameter = NULL;
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
}

Handle<FunctionTemplate> XMLHttpRequest::createAjax()
{
    s_functionTemplate = FunctionTemplate::New(XMLHttpRequest::AjaxCreate);
    s_functionTemplate->SetClassName(v8::String::New("XMLHttpRequest"));

    s_class_proto = s_functionTemplate ->PrototypeTemplate();
    s_class_proto ->Set(v8::String::New("open"),FunctionTemplate::New(XMLHttpRequest::Open));
    s_class_proto ->Set(v8::String::New("send"),FunctionTemplate::New(XMLHttpRequest::Send));
    s_class_proto ->Set(v8::String::New("getAllResponseHeaders"),FunctionTemplate::New(XMLHttpRequest::GetAllResponseHeaders));
    s_class_proto ->Set(v8::String::New("getResponseHeader"),FunctionTemplate::New(XMLHttpRequest::GetResponseHeader));
    s_class_proto ->Set(v8::String::New("setRequestHeader"),FunctionTemplate::New(XMLHttpRequest::SetRequestHeader));

    s_objectTemplate = s_functionTemplate->InstanceTemplate();
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate ->SetAccessor(v8::String::New("readyState"), XMLHttpRequest::GetReadyState, XMLHttpRequest::Set);
    s_objectTemplate ->SetAccessor(v8::String::New("status"), XMLHttpRequest::GetStatus, XMLHttpRequest::Set);
    s_objectTemplate ->SetAccessor(v8::String::New("statusText"), XMLHttpRequest::GetStatusText, XMLHttpRequest::Set);
    s_objectTemplate ->SetAccessor(v8::String::New("responseText"), XMLHttpRequest::GetResponseText, XMLHttpRequest::Set);
    s_objectTemplate ->SetAccessor(v8::String::New("onreadystatechange"),
                                    XMLHttpRequest::GetOnreadyStateChange,
                                    XMLHttpRequest::SetOnreadyStateChange);

    return s_functionTemplate;
}

} // namespace DCanvas
