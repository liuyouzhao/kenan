#include "defines.h"
#include "V8WebView.h"
#include "WebViewAliyun.h"

#undef 	 LOG_TAG
#define  LOG_TAG    "V8WebView"

namespace DCanvas
{

v8::Handle<v8::ObjectTemplate>              V8WebView::s_classProto;
v8::Handle<v8::ObjectTemplate>              V8WebView::s_objectTemplate;
Handle<FunctionTemplate>                    V8WebView::s_functionTemplate;

V8WebView::V8WebView()
{
}

V8WebView::~V8WebView()
{
}

// js T constructor function, called when `new T(...)' in js
v8::Handle<v8::Value> V8WebView::WebViewCreate(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    // throw if called without `new'
    if (!args.IsConstructCall())
        return ThrowException(v8::String::New("Cannot call constructor as function"));

    // throw if we didn't get 0 args
    int width = 0;
    int height = 0;
    if (args.Length() == 0)
    {
        width = 480;
        height = 800;
    }
    else if(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())
    {
    	width = args[0]->Uint32Value();
    	height = args[1]->Uint32Value();
    }
    else
    {
    	return THROW_EXCEPTION(TError, "new WebView : number of arguments Exception");
    }

    // create the C++ Handle<T> to be wrapped
    aliyun::WebViewAliyun * t = aliyun::WebViewAliyun::create(width, height);

    // make a persistent handle for the instance and make it
    // weak so we get a callback when it is garbage collected
    Persistent<Object> self = Persistent<Object>::New(args.Holder());
    self.MakeWeak(t, WeakReferenceCallback(V8WebView::WebViewDestroy));

    // set internal field to the C++ point
    self->SetInternalField(0, External::New(t));


    LOGE("This is the Constructor of V8WebView!");
    return self;
}
/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void V8WebView::WebViewDestroy(Persistent<Object> self, aliyun::WebViewAliyun* parameter)
{
    //  v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    LOGE("V8WebView destroy");
    if (parameter)
    {
        delete parameter;
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
    LOGE("V8WebView destroy end!");
}

Handle<FunctionTemplate> V8WebView::CreateWebView()
{
    s_functionTemplate = FunctionTemplate::New(V8WebView::WebViewCreate);
    s_functionTemplate->SetClassName(v8::String::New("WebView"));
    s_classProto = s_functionTemplate ->PrototypeTemplate();

    s_objectTemplate = s_functionTemplate->InstanceTemplate();
    s_objectTemplate ->SetInternalFieldCount(1);

    s_classProto->Set(v8::String::New("loadFromUrl"), FunctionTemplate::New(V8WebView::LoadFromUrl));
    s_classProto->Set(v8::String::New("onTouchStart"), FunctionTemplate::New(V8WebView::OnTouchStart));
    s_classProto->Set(v8::String::New("onTouchMove"), FunctionTemplate::New(V8WebView::OnTouchMove));
    s_classProto->Set(v8::String::New("onTouchEnd"), FunctionTemplate::New(V8WebView::OnTouchEnd));
    s_classProto->Set(v8::String::New("needFireTimer"), FunctionTemplate::New(V8WebView::NeedFireTimer));
    s_classProto->Set(v8::String::New("needRelayout"), FunctionTemplate::New(V8WebView::NeedRelayout));
    s_classProto->Set(v8::String::New("fireTimer"), FunctionTemplate::New(V8WebView::FireTimer));
    s_classProto->Set(v8::String::New("relayout"), FunctionTemplate::New(V8WebView::Relayout));
    s_classProto->Set(v8::String::New("repaintPageStores"), FunctionTemplate::New(V8WebView::RepaintPageStores));
    s_classProto->Set(v8::String::New("repaintBackingsCss3Cpu"), FunctionTemplate::New(V8WebView::RepaintBackingsCss3Cpu));

    return s_functionTemplate;
}

bool V8WebView::HasInstance(v8::Handle<v8::Value> arg)
{
    if (s_functionTemplate.IsEmpty())
    {
        LOGE("V8WebView::HasInstance is Null");
        return false;
    }
    return s_functionTemplate->HasInstance(arg);
}

v8::Handle<v8::Value> V8WebView::LoadFromUrl(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (1 != args.Length())
        return THROW_EXCEPTION(TError, "closePath : number of arguments Exception");

    v8::Handle<v8::String> v8Url = args[0]->ToString();
    v8::String::Utf8Value  utfValue(v8Url);
    std::string url = *(utfValue);

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->loadFromUrl(url.c_str());

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::OnTouchStart(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (2 != args.Length())
        return THROW_EXCEPTION(TError, "onTouchStart : number of arguments Exception");

    int x = 0, y = 0;
    if(args[0]->IsNumber() && args[1]->IsNumber())
	{
		x = args[0]->Uint32Value();
		y = args[1]->Uint32Value();
	}
    else
    {
    	return THROW_EXCEPTION(TError, "onTouchStart : arguments wrong type");
    }

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->ontouchstart(x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::OnTouchMove(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (2 != args.Length())
        return THROW_EXCEPTION(TError, "onTouchMove : number of arguments Exception");

    int x = 0, y = 0;
    if(args[0]->IsNumber() && args[1]->IsNumber())
	{
		x = args[0]->Uint32Value();
		y = args[1]->Uint32Value();
	}
    else
    {
    	return THROW_EXCEPTION(TError, "onTouchMove : arguments wrong type");
    }

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->ontouchmove(x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::OnTouchEnd(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (2 != args.Length())
        return THROW_EXCEPTION(TError, "onTouchEnd : number of arguments Exception");

    int x = 0, y = 0;
    if(args[0]->IsNumber() && args[1]->IsNumber())
	{
		x = args[0]->Uint32Value();
		y = args[1]->Uint32Value();
	}
    else
    {
    	return THROW_EXCEPTION(TError, "onTouchEnd : arguments wrong type");
    }

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->ontouchend(x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::NeedFireTimer(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "needFireTimer : number of arguments Exception");

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    return v8::Boolean::New(wv->needFireTimer());
}

v8::Handle<v8::Value> V8WebView::NeedRelayout(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "needRelayout : number of arguments Exception");

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    return v8::Boolean::New(wv->needRelayout());
}

v8::Handle<v8::Value> V8WebView::FireTimer(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "fireTimer : number of arguments Exception");

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->fireTimer();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::Relayout(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "relayout : number of arguments Exception");

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->relayout();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::RepaintPageStores(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (4 != args.Length())
        return THROW_EXCEPTION(TError, "repaintPageStores : number of arguments Exception");

    for (int i = 0; i < 4; ++i)
	{
		if (!args[i]->IsNumber())
			return THROW_EXCEPTION(TTypeError,
			   "RepaintPageStores : type of arguments Exception");
	}
    int x, y, w, h;
	x = y = w = h = 0;
	x = args[0]->NumberValue();
	y = args[1]->NumberValue();
	w = args[2]->NumberValue();
	h = args[3]->NumberValue();

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->repaintPageStores(x, y, w, h);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8WebView::RepaintBackingsCss3Cpu(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "repaintBackingsCss3Cpu : number of arguments Exception");

    aliyun::WebViewAliyun* wv = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args.This());
    wv->repaintBackingsCss3Cpu();

    return v8::Undefined();
}

} // namespace DCanvas
