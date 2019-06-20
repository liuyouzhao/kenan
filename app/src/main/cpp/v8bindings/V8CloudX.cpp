#include "defines.h"
#include "V8CloudX.h"
#include "V8Canvas.h"
#include "Sqlite3DB.h"
#include <android/log.h>
#include "DirectCanvas.h"

#undef LOG_TAG
#define  LOG_TAG    "CloudX"

namespace DCanvas
{

v8::Handle<v8::ObjectTemplate>                  V8CloudX::class_proto;
v8::Handle<v8::Object>                          V8CloudX::class_obj;
v8::Persistent<v8::FunctionTemplate>            V8CloudX::class_func_templ;// = FunctionTemplate::New();

V8CloudX::V8CloudX()
{
}

V8CloudX::~V8CloudX()
{
}

v8::Persistent<v8::Object> V8CloudX::Wrap()
{
	v8::HandleScope handle_scope;
	class_func_templ = v8::Persistent<v8::FunctionTemplate>::New(FunctionTemplate::New());
    class_func_templ->SetClassName(v8::String::New("cloudlet"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("createCanvas", FunctionTemplate::New(V8CloudX::CreateCanvas));
    class_proto->Set("getCanvas", FunctionTemplate::New(V8CloudX::GetCanvas));
    class_proto->Set("getCamera", FunctionTemplate::New(V8CloudX::GetCamera));
    class_proto->Set("getAudio", FunctionTemplate::New(V8CloudX::GetAudio));
    class_proto->Set("getArguments", v8::FunctionTemplate::New(V8CloudX::GetArguments));
    class_proto->Set("createSoundPool", FunctionTemplate::New(V8CloudX::CreateSoundPool));
    class_proto->Set("getDataBase", FunctionTemplate::New(V8CloudX::GetDataBase));

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins -> SetInternalFieldCount(1);
    ClassImpl* pImpl = ClassImpl::create();

    Local<Object> obj = class_ins->NewInstance();
    obj->SetInternalField(0, External::New(pImpl));
    
    return v8::Persistent<v8::Object>::New(obj);
}

bool V8CloudX::Regist(Persistent<v8::Context> _exe_context)
{
    v8::Handle<v8::FunctionTemplate>     class_templ;
    v8::Handle<v8::ObjectTemplate>       class_proto ;
    v8::Handle<v8::Object>               obj_class_obj ;

    class_templ = FunctionTemplate::New();
    class_templ->SetClassName(v8::String::New("cloudlet"));
    class_proto = class_templ->PrototypeTemplate();

    class_proto->Set("createCanvas", FunctionTemplate::New(V8CloudX::CreateCanvas));
    class_proto->Set("getCamera", FunctionTemplate::New(V8CloudX::GetCamera));
    class_proto->Set("getAudio", FunctionTemplate::New(V8CloudX::GetAudio));
    class_proto->Set("getArguments", v8::FunctionTemplate::New(V8CloudX::GetArguments));
    class_proto->Set("createSoundPool", FunctionTemplate::New(V8CloudX::CreateSoundPool));
    class_proto->Set("getDataBase", FunctionTemplate::New(V8CloudX::GetDataBase));

    Handle<ObjectTemplate> class_ins = class_templ->InstanceTemplate();
    class_ins -> SetInternalFieldCount(1);
    ClassImpl* pImpl = ClassImpl::create();

    Local<Object> obj = class_ins->NewInstance();
    obj->SetInternalField(0, External::New(pImpl));
    _exe_context->Global()->Set(v8::String::New("CloudX"), obj);

    return true;
}

bool V8CloudX::UnRegist()
{
	return true;
}

v8::Handle<v8::Value> V8CloudX::CreateCanvas(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    ClassImpl* pImpl = ObjectWrap::Unwrap<ClassImpl>(args.This());

    if(!DirectCanvas::s_pDcanvas)
    {
    	return THROW_EXCEPTION(TError, "CloudX has been deleted");
    }

    void *ptr;
    if (args.Length() == 0)
    {
        ptr = pImpl->createCanvas();
        LOGD("create Canvas ok");
    }
    else if (args.Length() == 4)
    {
        int x = args[0]->Int32Value();
        int y = args[1]->Int32Value();
        int w = args[2]->Int32Value();
        int h = args[3]->Int32Value();
        
        ptr = pImpl->createCanvas(x, y, w, h);
    }
    else 
    {
        LOGD("create Canvas witd error arguments");
        return v8::Undefined();
    }
    return V8Canvas::Wrap(ptr);
}

v8::Handle<v8::Value> V8CloudX::GetCanvas(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    ClassImpl* pImpl = ObjectWrap::Unwrap<ClassImpl>(args.This());

    if(!DirectCanvas::s_pDcanvas)
    {
    	return THROW_EXCEPTION(TError, "CloudX has been deleted");
    }

    void *ptr;
    if (args.Length() == 0)
    {
        ptr = pImpl->createCanvas();
        LOGD("create Canvas ok");
    }
    else if (args.Length() == 1)
    {
        int id = args[0]->Int32Value();
        /*int y = args[1]->Int32Value();
        int w = args[2]->Int32Value();
        int h = args[3]->Int32Value();
        */
        ptr = pImpl->createCanvas(id);
    }
    else 
    {
        LOGD("create Canvas witd error arguments");
        return v8::Undefined();
    }
    return V8Canvas::Wrap(ptr);
}

v8::Handle<v8::Value> V8CloudX::GetCamera(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    return CameraIns::wrap(args);
}

v8::Handle<v8::Value> V8CloudX::GetAudio(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    return AudioIns::wrap(args);
}

v8::Handle<v8::Value> V8CloudX::CreateSoundPool(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    return SoundPoolIns::wrap(args);
}

v8::Handle<v8::Value> V8CloudX::GetArguments(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() >= 1)
        return THROW_EXCEPTION(TError, "CloudX GetArguments arguments exception");

    CloudX* cx = ObjectWrap::Unwrap<CloudX>(args.This());

    char* argument_str = cx->getArguments();
    if (NULL == argument_str)
        return v8::String::New("arguments has not set");

    Handle<Array> array = Array::New(1);
    array->Set(Integer::New(0), v8::String::New(argument_str));
    return array;

}

v8::Handle<v8::Value> V8CloudX::SetArguments(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 1)
        return THROW_EXCEPTION(TError, "CloudX::SetArguments arguments Exception!!");

    CloudX* cx = ObjectWrap::Unwrap<CloudX>(args.This());

    v8::String::Utf8Value value(args[0]);
    cx->setArguments(*(value));

    return v8::Undefined();
}

v8::Handle<v8::Value> V8CloudX::GetDataBase(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
        return THROW_EXCEPTION(TError, "GetDataBase arguments Exception!");

    CloudX* cx = ObjectWrap::Unwrap<CloudX>(args.This());
    if (!cx->m_db)
    {
        LOGD("create db");
        cx->m_db = SQLite3DB::createDB();
    }
    return cx->m_db->wrap();
}

}
