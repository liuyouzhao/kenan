#include "defines.h"
#include "v8.h"
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"
#include "CallBackFuncs.h"

#include "V8GlobalFunctions.h"
#include "V8Image.h"
#include "V8Orc2d.h"
#include "V8Sprite.h"
#include "V8ResourceManager.h"


#undef LOG_TAG
#define  LOG_TAG    "mainjni_hujia"

Gl2d_Impl* g_pImpl = NULL;
extern "C"
{
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_init(JNIEnv* env, jobject obj, jint width, jint height, jstring jstr);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnFrame(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnResume(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchStart(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchEnd(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchMove(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnLoadResource(JNIEnv* env, jobject obj, jint resId);
};

Handle<Value> log_e(const v8::FunctionCallbackInfo<v8::Value>& args) {
	HandleScope  handle_scope;
	char buffer[4096];

	memset(buffer, 0, sizeof(buffer));
	Handle<String> str = args[0]->ToString();
	str->WriteUtf8(buffer);
	LOGE("log_e: %s\n", buffer);

	return Undefined();
}

WebGLGraphicsContext* glctx = 0;

v8::Persistent<v8::ObjectTemplate>             class_proto;
v8::Handle<v8::Object>                         class_obj;
v8::Persistent<v8::FunctionTemplate>           class_func_templ;

void wrap_all(v8::Handle<v8::Object> global, JNIEnv* env) {
	class_func_templ = v8::Persistent<v8::FunctionTemplate>::New(FunctionTemplate::New());
	class_func_templ->SetClassName(v8::String::New("CloudX"));
	class_proto = v8::Persistent<v8::ObjectTemplate>::New(class_func_templ->PrototypeTemplate());

	/** log object */
	class_proto->Set("log_e",             v8::FunctionTemplate::New(log_e));
	/** Init webgl needed structures */
	class_proto->Set("Image",           V8Image::CreateImage());

	if(!glctx) {
		glctx = WebGLGraphicsContext::Create(0, 0, 100, 100);
	}

	class_proto->Set("res", V8ResourceManager::Wrap(env));

	LOGE("Wrap V8Orc2d Now!");

	g_pImpl = new Gl2d_Impl();
	g_pImpl->_Init(480, 800);
	class_proto->Set("orc2d", V8Orc2d::Wrap(g_pImpl));

	class_proto->Set("Orc2dSprite",     V8Sprite::CreateV8SpriteTemplate());


	v8::Handle<v8::ObjectTemplate>    class_ins = class_func_templ->InstanceTemplate();
	class_obj = class_ins->NewInstance();

	global->Set(v8::String::New("x"), class_obj);
	LOGE("Wrap all webgl apis");
}


class V8ManagerGlobal {
public:
	Persistent<Context> _v8_exe_context;

	V8ManagerGlobal(JNIEnv * env, jstring code)
	{
	    LOGE("================> 1");
		HandleScope handleScope;
		_v8_exe_context = Context::New();
		LOGE("================> 1.5");
		Context::Scope context_scope(_v8_exe_context);
		LOGE("================> 2");
		wrap_all(_v8_exe_context->Global(), env);
		//update = function() { log_e('js_on_frame..');gl.clearColor(1.0, 0.0, 0.0, 1.0);gl.clear(gl.COLOR_BUFFER_BIT);gl.Flush();}
		char*   t;
		t   =   (char*)env->GetStringUTFChars(code, 0);
		//Handle<String> source = String::New("var gl = x.gl; this.update = function() {gl.clearColor(1.0, 0.0, 0.0, 1.0);gl.clear(gl.COLOR_BUFFER_BIT);gl.flush();}; x.log_e('code is ok'); ");
		Handle<String> source = String::New(t);
		Handle<Script> script = Script::Compile(source);
		LOGE("================> 3");
		if(script.IsEmpty()) {
			LOGE("Compile Failed!!");
			return;
		}
		LOGE("================> 4");
		TryCatch try_catch;
		Handle<Value> result = script->Run();
		LOGE("================> 5");
		// to see if have any exception
		if (try_catch.HasCaught())
		{
			v8::String::Utf8Value exception(try_catch.Exception());
			v8::Local<v8::Message> message = try_catch.Message();
			v8::String::Utf8Value smessage(message->Get());
			LOGE("has exception!");
			//LOGE(*exception);
			LOGE("line %d ", message->GetLineNumber());
		}
	}
};

V8ManagerGlobal* pV8ManagerGlobal;

/**init run for compile and run*/
JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_init(JNIEnv * env, jobject obj,jint width, jint height, jstring jstr)
{
    LOGE("======= Java_com_aliyun_cloudapp_directcanvas_JNILIB_init");
	pV8ManagerGlobal = new V8ManagerGlobal(env, jstr);
	LOGE("End Java_com_aliyun_cloudapp_directcanvas_JNILIB_init");
}

JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnFrame(JNIEnv* env, jobject obj) {
	HandleScope handleScope;
	TryCatch try_catch;
	Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
	v8::Local<v8::Value> js_value;
	Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
	js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("update"));
	v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
	if (js_function->IsFunction()) {
		js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 0, NULL);
		CallBackFuncs::getFuncQueue()->CallAllFunc();
	}
	// to see if have any exception
	if (try_catch.HasCaught())
	{
		v8::String::Utf8Value exception(try_catch.Exception());
		v8::Local<v8::Message> message = try_catch.Message();
		v8::String::Utf8Value smessage(message->Get());
		LOGE("has exception!");
		//LOGE(*exception);
		LOGE("line %d ", message->GetLineNumber());
	}
}


JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnResume(JNIEnv* env, jobject obj) {

	if(g_pImpl) {
		g_pImpl->_Init(480, 800);
	}

	HandleScope handleScope;
	TryCatch try_catch;
	Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
	v8::Local<v8::Value> js_value;
	Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
	js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("resume"));
	v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
	if (js_function->IsFunction()) {
		js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 0, NULL);
	}

	// to see if have any exception
	if (try_catch.HasCaught())
	{
		v8::String::Utf8Value exception(try_catch.Exception());
		v8::Local<v8::Message> message = try_catch.Message();
		v8::String::Utf8Value smessage(message->Get());
		LOGE("has exception!");
		//LOGE(*exception);
		LOGE("line %d ", message->GetLineNumber());
	}
}

JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchStart(JNIEnv* env, jobject obj, jint x, jint y) {

	HandleScope handleScope;
	TryCatch try_catch;
	Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
	v8::Local<v8::Value> js_value;
	Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
	js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("ontouchstart"));
	v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
	if (js_function->IsFunction()) {
		//js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 0, NULL);
		Handle<Value>  argv[2];
		argv[0] = Integer::New(x);
		argv[1] = Integer::New(y);
		js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 2, argv);
	}

	// to see if have any exception
	if (try_catch.HasCaught())
	{
		v8::String::Utf8Value exception(try_catch.Exception());
		v8::Local<v8::Message> message = try_catch.Message();
		v8::String::Utf8Value smessage(message->Get());
		LOGE("has exception!");
		//LOGE(*exception);
		LOGE("line %d ", message->GetLineNumber());
	}
}

JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchEnd(JNIEnv* env, jobject obj, jint x, jint y) {

	HandleScope handleScope;
	TryCatch try_catch;
	Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
	v8::Local<v8::Value> js_value;
	Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
	js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("ontouchend"));
	v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
	if (js_function->IsFunction()) {
		//js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 0, NULL);
		Handle<Value>  argv[2];
		argv[0] = Integer::New(x);
		argv[1] = Integer::New(y);
		js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 2, argv);
	}

	// to see if have any exception
	if (try_catch.HasCaught())
	{
		v8::String::Utf8Value exception(try_catch.Exception());
		v8::Local<v8::Message> message = try_catch.Message();
		v8::String::Utf8Value smessage(message->Get());
		LOGE("has exception!");
		//LOGE(*exception);
		LOGE("line %d ", message->GetLineNumber());
	}
}


JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_TouchMove(JNIEnv* env, jobject obj, jint x, jint y) {

	HandleScope handleScope;
	TryCatch try_catch;
	Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
	v8::Local<v8::Value> js_value;
	Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
	js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("ontouchmove"));
	v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
	if (js_function->IsFunction()) {
		Handle<Value>  argv[2];
		argv[0] = Integer::New(x);
		argv[1] = Integer::New(y);
		js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 2, argv);
	}

	// to see if have any exception
	if (try_catch.HasCaught())
	{
		v8::String::Utf8Value exception(try_catch.Exception());
		v8::Local<v8::Message> message = try_catch.Message();
		v8::String::Utf8Value smessage(message->Get());
		LOGE("has exception!");
		//LOGE(*exception);
		LOGE("line %d ", message->GetLineNumber());
	}
}

JNIEXPORT void JNICALL Java_com_aliyun_cloudapp_directcanvas_JNILIB_OnLoadResource(JNIEnv* env, jobject obj, jint resId) {
	HandleScope handleScope;
		TryCatch try_catch;
		Context::Scope context_scope(pV8ManagerGlobal->_v8_exe_context);
		v8::Local<v8::Value> js_value;
		Handle<Object> global = pV8ManagerGlobal->_v8_exe_context->Global();
		js_value = pV8ManagerGlobal->_v8_exe_context->Global()->Get(v8::String::New("onloadresource"));
		v8::Local<v8::Function> js_function = Local<Function>::Cast(js_value);
		if (js_function->IsFunction()) {
			Handle<Value> argv[1];
			argv[0] = Integer::New(resId);
			LOGE("onloadresource in C++ V8: %d", resId);
			js_function->Call(pV8ManagerGlobal->_v8_exe_context->Global(), 1, argv);
		}

		// to see if have any exception
		if (try_catch.HasCaught())
		{
			v8::String::Utf8Value exception(try_catch.Exception());
			v8::Local<v8::Message> message = try_catch.Message();
			v8::String::Utf8Value smessage(message->Get());
			LOGE("has exception!");
			//LOGE(*exception);
			LOGE("line %d ", message->GetLineNumber());
		}
}
