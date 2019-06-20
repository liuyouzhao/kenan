#ifndef V8WEBVIEW_H
#define V8WEBVIEW_H

namespace aliyun
{
	class WebViewAliyun;
}

namespace DCanvas
{

class V8WebView : ObjectWrap
{
public:

	static v8::Handle<v8::Value> WebViewCreate(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void WebViewDestroy(Persistent<Object> self, aliyun::WebViewAliyun* parameter);
	static Handle<FunctionTemplate> CreateWebView();
	static bool HasInstance(v8::Handle<v8::Value>);

	static v8::Handle<v8::Value> LoadFromUrl(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> OnTouchStart(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> OnTouchMove(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> OnTouchEnd(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> NeedFireTimer(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> NeedRelayout(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> FireTimer(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> Relayout(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> RepaintPageStores(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Handle<v8::Value> RepaintBackingsCss3Cpu(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
	V8WebView();
	~V8WebView();

    //function
	static v8::Handle<v8::ObjectTemplate>               s_classProto;
	//property
	static v8::Handle<v8::ObjectTemplate>               s_objectTemplate;
	static Handle<FunctionTemplate>                     s_functionTemplate;
};
}
#endif
