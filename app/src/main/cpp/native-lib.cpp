#include <jni.h>
#include <string>
#include "libplatform/libplatform.h"
#include "v8.h"
#include "V8Main.h"
#include "V8Log.h"

/* the following references need to be around somewhere,
 * either as global (not recommended), or in some object,
 * otherwise they'll get garbage collected by C++
 * and cause a segmentation fault crash
 */
std::unique_ptr<v8::Platform> _platform;
v8::Isolate *isolate;
v8::Persistent<v8::Context> persistentContext;


extern "C" JNIEXPORT void
JNICALL
Java_org_cmdr2_jnitest_MyActivity_initV8(
        JNIEnv *env,
        jobject /* this */) {
#if 0
    v8::V8::InitializeICUDefaultLocation("./");
    v8::V8::InitializeExternalStartupData("./");
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);

    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = Context::New(isolate);
        v8::Context::Scope context_scope(context);

        V8Main::instance()->setupJavascriptEnvironment(isolate, context);

        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, "log.info('aaa');log.debug('bb');log.error('ccc');", v8::NewStringType::kNormal).ToLocalChecked();
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
#endif
    V8Main::instance()->initV8Environment();
    V8Main::instance()->firstRunJavascript("log.info('aaa');log.debug('bb');log.error('ccc');");
    V8Main::instance()->runJavascript("log.info('1');log.debug('2');log.error('3');");
    V8Main::instance()->destroyV8Environment();
}

extern "C" JNIEXPORT jstring
JNICALL
Java_org_cmdr2_jnitest_MyActivity_stringFromV8(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello v8 from C++!\n";

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    // Enter the context for compiling and running the hello world script.
    v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, persistentContext);
    v8::Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    v8::Local<v8::String> source = v8::String::NewFromUtf8(
            isolate, "debug('okokok')", v8::NewStringType::kNormal).ToLocalChecked();

    // Compile the source code.
    v8::Local<v8::Script> script =
            v8::Script::Compile(context, source).ToLocalChecked();

    // Run the script to get the result.
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    // Convert the result to an UTF8 string and print it.
    v8::String::Utf8Value utf8(result);
    printf("%s\n", *utf8);
    hello += *utf8;

    return env->NewStringUTF(hello.c_str());

}
