#include "defines.h"
#include "DirectCanvas.h"
#include "V8ArrayType.h"
#include "Canvas.h"
#include "CanvasGradient.h"
#include "V8Image.h"
#include "Script.h"
#include "V8GlobalFunctions.h"
#include "WebSocketImpl.h"
#include "SocketClient.h"
#include "fs.h"
#include "XMLHttpRequest.h"
#include "Float32Array.h"
#include "Uint16Array.h"
#include "Int16Array.h"
#include "Int32Array.h"
#include "Uint32Array.h"
#include "ArrayBuffer.h"
#include "Int8Array.h"
#include "Uint8Array.h"
#include "Float64Array.h"
#include "V8DirectCanvas.h"
#include "V8CloudX.h"
#include "V8Script.h"
#include <android/log.h>
#undef LOG_TAG
#define LOG_TAG    "V8DirectCanvas"

namespace DCanvas
{
v8::Persistent<v8::ObjectTemplate>             V8DirectCanvas::class_proto;
v8::Handle<v8::Object>                         V8DirectCanvas::class_obj;
v8::Persistent<v8::FunctionTemplate>           V8DirectCanvas::class_func_templ;// = FunctionTemplate::New();

void V8DirectCanvas::Wrap(v8::Handle<v8::Object> global, DirectCanvas* pImpl)
{
    v8::HandleScope handle_scope;

    if (class_func_templ.IsEmpty()) 
    {
		class_func_templ = v8::Persistent<v8::FunctionTemplate>::New(FunctionTemplate::New());
		class_func_templ->SetClassName(v8::String::New("DirectCanvas"));
		class_proto = v8::Persistent<v8::ObjectTemplate>::New(class_func_templ->PrototypeTemplate());

		//set functions
		class_proto->Set("log",             v8::FunctionTemplate::New(log));

		class_proto->Set("Image",           V8Image::CreateImage());
		class_proto->Set("Script",          V8Script::createScript());
		class_proto->Set("cloudlet", 		V8CloudX::Wrap());
		class_proto->Set("XMLHttpRequest",  XMLHttpRequest::createAjax());
		class_proto->Set("WebSocket",       WebSocketImpl::createWebSocket());
		//m_global_templ->Set("WebView",         V8WebView::CreateWebView());
	//TODO  split them
		class_proto->Set("Float32Array",    V8ArrayType<Float32Array>::CreateArray());
		class_proto->Set("Int32Array",      V8ArrayType<Int32Array>::CreateArray());
		class_proto->Set("Uint16Array",     V8ArrayType<Uint16Array>::CreateArray());
		class_proto->Set("Uint8Array",      V8ArrayType<Uint8Array>::CreateArray());
		class_proto->Set("Uint32Array",     V8ArrayType<Uint32Array>::CreateArray());
		class_proto->Set("Int16Array",      V8ArrayType<Int16Array>::CreateArray());
		class_proto->Set("Int8Array",       V8ArrayType<Int8Array>::CreateArray());
		class_proto->Set("Float64Array",    V8ArrayType<Float64Array>::CreateArray());
		class_proto->Set("ArrayBuffer",     V8ArrayBuffer::CreateArray());
		
		/*
		 *maybe should move onframe to cloudx
		 * */
	   

		class_proto->Set("setUsed", v8::FunctionTemplate::New(V8DirectCanvas::SetUsed));
		class_proto->Set("onFrame", v8::FunctionTemplate::New(V8DirectCanvas::OnFrame));
		class_proto->Set("onDestroy", v8::FunctionTemplate::New(V8DirectCanvas::OnDestroy));
    }
    v8::Handle<v8::ObjectTemplate>    class_ins = class_func_templ->InstanceTemplate();
    class_ins-> SetInternalFieldCount(1);
    class_ins->SetAccessor(v8::String::New("IsReInited"), V8DirectCanvas::GetIsReInited, V8DirectCanvas::SetIsReInited);
    
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(pImpl));
     
    global->Set(v8::String::New("cloudapp"), class_obj); 
    //return v8::Local<v8::Object>::New(class_obj);//v8::Persistent<v8::Object>::New(class_obj);
    LOGD("DirctCanvas Wrap ok!");
}

v8::Handle<v8::Value> V8DirectCanvas::OnDestroy(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::HandleScope scope;
    
    DirectCanvas* dc = ObjectWrap::Unwrap<DirectCanvas>(args.This());
	dc->Destroy();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8DirectCanvas::OnFrame(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::HandleScope scope;
    
    DirectCanvas* dc = ObjectWrap::Unwrap<DirectCanvas>(args.This());
    dc->OnFrame();
    
    return v8::Undefined();
}

void V8DirectCanvas::SetOnFrame(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(DirectCanvas, info);
    if (value->IsFunction())
    {
        if (class_impl->m_onFrameSetted == true)
        {
            class_impl->m_js_onFrame.Dispose();
            class_impl->m_js_onFrame.Clear();
        }
        class_impl->m_js_onFrame = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(value));
    }
    else
    {
        LOGE("onframe function error");
        return;
    }
}

v8::Handle<v8::Value>  V8DirectCanvas::GetOnFrame(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8DirectCanvas::SetOnTouchStart(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(DirectCanvas, info);
    if (value->IsFunction())
    {
        if (class_impl->m_onTouchStartSetted == true)
        {
            class_impl->m_js_onTouchStart.Dispose();
            class_impl->m_js_onTouchStart.Clear();
        }
        class_impl->m_js_onTouchStart = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(value));
    }
    else
    {
        LOGE("setontouchstart function error");
        return;
    }
}

v8::Handle<v8::Value>  V8DirectCanvas::GetOnTouchStart(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8DirectCanvas::SetOnTouchEnd(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(DirectCanvas, info);
    if (value->IsFunction())
    {
        if (class_impl->m_onTouchEndSetted == true)
        {
            class_impl->m_js_onTouchEnd.Dispose();
            class_impl->m_js_onTouchEnd.Clear();
        }
        class_impl->m_js_onTouchEnd = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(value));
    }
    else
    {
        LOGE("setontouch end function error");
        return;
    }
}

v8::Handle<v8::Value>  V8DirectCanvas::GetOnTouchEnd(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8DirectCanvas::SetOnTouchMove(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(DirectCanvas, info);
    if (value->IsFunction())
    {
        if (class_impl->m_onTouchMoveSetted == true)
        {
            class_impl->m_js_onTouchMove.Dispose();
            class_impl->m_js_onTouchMove.Clear();
        }
        class_impl->m_js_onTouchMove = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(value));
    }
    else
    {
        LOGE("set oin touch move function error");
        return;
    }
}

v8::Handle<v8::Value>  V8DirectCanvas::GetOnTouchMove(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

v8::Handle<v8::Value> V8DirectCanvas::GetIsReInited(v8::Local<v8::String> property,const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(DirectCanvas, info);
    LOGE("class_impl->GetReInited() %p", class_impl);
    if(!DirectCanvas::s_pDcanvas)
    {
    	return v8::Boolean::New(false);
    }
    
    return v8::Boolean::New(class_impl->GetReInited());
}

void V8DirectCanvas::SetIsReInited(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
	
}

v8::Handle<v8::Value> V8DirectCanvas::SetUsed(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::HandleScope scope;
    
    DirectCanvas* dc = ObjectWrap::Unwrap<DirectCanvas>(args.This());
    dc->SetUsed();
    
    return v8::Undefined();
}
    
/*
bool V8DirectCanvas::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("DirectCanvas::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}*/
}
