#include "defines.h"
#include "Script.h"
#include "fs.h"
#include <stdlib.h>
#include <utils/threads.h>
#include "JsLoader.h"
#include "CallBackFuncs.h"
#include "ThreadPool.h"
#include <vector>
#include <string>
#include <stdio.h>

#undef LOG_TAG
#define  LOG_TAG    "Script"

namespace DCanvas
{

static void*  onloadFunc(void* p)
{
    Script* _p = (Script*)p;
    LOGD("Script onloadFunc parameter 22 %p", p);
    _p->onLoad();
    return NULL;
}

Script::Script()
{
    //MemoryManager::getManager()->AddImage(this);
    m_data = NULL;
    m_src = NULL;
    m_onLoadSeted = false;
}

Script::~Script()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = NULL;
    }

    if (m_src)
    {
        delete[] m_src;
        m_src = NULL;
    }

    if (m_onLoadSeted)
    {
        m_js_OnLoad.Dispose();
    }
    //MemoryManager::getManager()->RemoveImage(this);
}

void Script::updata(int length)
{
    LOGD("load start");
    std::string path(this->m_src);
    std::string head("http://");
    std::string basepath(this->m_src);

//    if(0 != path.compare(0, head.length(), head)) /*native file format add base path*/
//    {
//        CloudX* cx = CloudX::create();
//        basepath = cx->getBasePath() + path;
//        if (access(basepath.c_str(),0) == -1)
//        {
//            basepath = cx->getBaseUrl() + path;
//        }
//    }

    JsLoader* loader = JsLoader::create();
    LOGD("ScriptLoader");
    loader->load((char*)basepath.c_str(), basepath.length(), this);
    LOGD("load end");
}

void Script::setData(void* data, int size)
{
    SAFE_DELETE(this->m_data);
    this->m_data = new char[size + 1];
    memcpy(m_data, data, size);
    m_data[size] = '\0';
    //SAFE_DELETE(data);

    FuncData* fd = new FuncData();
    fd->data = onloadFunc;
    fd->param = this;
    LOGD("setData parameter 11 %p", this);
    CallBackFuncs::getFuncQueue()->AddFunc(fd);
    //onLoad();
}

void Script::onLoad()
{
    if (!m_onLoadSeted)
        return ;

    if (!m_js_OnLoad->IsFunction())
    {
        LOGE("lost onload exception");
        return ;
    }
    m_js_OnLoad->Call(m_js_self, 0, NULL);
}

/*
void Script::SetSrc(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Script, info);
    //copy value to srcfile;
    v8::String::Utf8Value  utfValue(value);
    char *srcfile = *(utfValue);

    if (class_impl->m_src && 0 == strcmp(class_impl->m_src,srcfile))
        return ;
    if (class_impl->m_src)
    {
        delete class_impl->m_src;
        class_impl->m_src = NULL;
    }
    class_impl->m_src = new char[utfValue.length() +1];
    strcpy(class_impl->m_src, srcfile);
    class_impl->m_src[utfValue.length()] = 0;
    class_impl->updata(utfValue.length());
}

v8::Handle<v8::Value> Script::GetSrc(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Script, info);
	if (!class_impl->m_src)
		return v8::Undefined();
    return v8::String::New(class_impl->m_src);
}

void Script::SetOnLoad(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(Script, info);

    if (value->IsFunction())
    {
        if (class_impl->m_onLoadSeted)
        {
            class_impl->m_js_OnLoad.Dispose();
            class_impl->m_js_OnLoad.Clear();
        }

        class_impl->m_js_OnLoad = Persistent<Function>::New(Local<Function>::Cast(value));
    }
    else
    {
        LOGE("onload function error!");
        return;
    }
    class_impl->m_onLoadSeted = true;
}

v8::Handle<v8::Value>  Script::GetOnLoad(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;

    return v8::Undefined();
}

// js T constructor function, called when `new T(...)' in js
v8::Handle<v8::Value> Script::ScriptCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return ThrowException(v8::String::New("Cannot call constructor as function"));

    // throw if we didn't get 0 args
    if (args.Length() != 0)
        return ThrowException(v8::String::New("Expected no arguments"));

    // create the C++ Handle<T> to be wrapped
    Script * t = new Script();

    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(Script::ScriptDestroy));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(t));

    t->SetSelf(self);

    LOGD("This is the Constructor of Script!");
    return self;
}
*/
/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
/*
void Script::ScriptDestroy(Persistent<Object> self, Script* parameter)
{
    //  v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    LOGD("Script destroy");
    if (parameter)
    {
        MemoryManager* mm = MemoryManager::getManager();
        //mm->DeleteImage(parameter);
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
    LOGD("Script destroy end!");
}

void saveToNative(char* data)
{
    FILE *stream_dst;
    stream_dst = fopen( "/sdcard/main.js", "ab+" );
    if( stream_dst==NULL ){
        return;
    }
    fprintf(stream_dst, "%s", data);
    fclose( stream_dst );
}


v8::Handle<v8::Value> Script::RunScript(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    Script* pScript = ObjectWrap::Unwrap<Script>(args.This());

    if(pScript->m_data == NULL)
    {
        return ThrowException(v8::String::New("Run single script Error, the data is NULL!"));
    }
    pScript->m_script_source = v8::String::New(pScript->m_data);

    TryCatch try_catch;

    //step 1: compile the js file
    pScript->m_script_runner =  v8::Script::Compile(pScript->m_script_source);
    if (pScript->m_script_runner.IsEmpty())
    {
        LOGE("single script something was wrong while compiling!");
        return v8::Undefined();
    }
    //step 2: run the compiled js file
    pScript->m_script_runner->Run();

    // to see if have any exception
    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value exception(try_catch.Exception());
        v8::Local<v8::Message> message = try_catch.Message();
        v8::String::Utf8Value smessage(message->Get());
        LOGE("has exception!");
        LOGE(*exception);
        LOGE("line %d ", message->GetLineNumber());
    }
    return v8::Undefined();
}

Handle<FunctionTemplate> Script::createScript()
{
    s_functionTemplate = FunctionTemplate::New(Script::ScriptCreate);
    s_functionTemplate->SetClassName(v8::String::New("Script"));
    s_classProto = s_functionTemplate ->PrototypeTemplate();

    s_objectTemplate = s_functionTemplate->InstanceTemplate();
    s_objectTemplate ->SetInternalFieldCount(1);
    s_objectTemplate ->SetAccessor(v8::String::New("src"),    Script::GetSrc,    Script::SetSrc);
    s_objectTemplate ->SetAccessor(v8::String::New("onload"), Script::GetOnLoad, Script::SetOnLoad);
    s_objectTemplate->Set("runScript", FunctionTemplate::New(Script::RunScript));

    return s_functionTemplate;
}

bool Script::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("Script::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}
*/
} // namespace DCanvas
