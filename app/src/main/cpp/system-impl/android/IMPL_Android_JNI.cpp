#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

#include "RuntimeApi.h"
#include "RuntimeOptions.h"

#undef LOG_TAG
#define  LOG_TAG    "IMPL_Android_JNI"

using namespace kenan_runtime;

JNIEnv *javaEnvironment;
JavaVM *jvm = NULL;
void init_all_java_methods();

static void init(JNIEnv *env, jobject obj, jint _width, jint _height, jstring code, jstring dataDir);
static void deinit(JNIEnv* env, jobject obj);
static void onframe(JNIEnv* env, jobject obj);
static void onresume(JNIEnv* env, jobject obj);
static int taskloop(JNIEnv* env, jobject obj, jstring taskId, jstring file, jboolean rw) ;

static const char *classPathName = "com/kenan/jni/JNILIB";

static JNINativeMethod methods[] = {
  {"init", "(IILjava/lang/String;Ljava/lang/String;)V", (void*)init },
  {"deinit", "()V", (void*)deinit },
  {"OnFrame", "()V", (void*)onframe },
  {"OnResume", "()V", (void*)onresume },
  {"taskLoop", "(Ljava/lang/String;Ljava/lang/String;Z)I", (int*)taskloop }
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName, methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }
  return JNI_TRUE;
}
// ----------------------------------------------------------------------------
/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    jvm = vm;

    LOGI("JNI_OnLoad");
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
    javaEnvironment = env;

    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_6;
    LOGD("JNI_OnLoad Successfully %p", javaEnvironment);
bail:
    return result;
}


static void init(JNIEnv *env, jobject obj, jint _width, jint _height, jstring code, jstring dataDir) {

    LOGD("real env %p", env);
    char *dataDirectory = NULL;
    char *javascriptCode = NULL;
    dataDirectory = (char*)env->GetStringUTFChars(dataDir, 0);
    javascriptCode = (char*)env->GetStringUTFChars(code, 0);
    RuntimeOptions opts;
    opts.setWorkingDirectory(dataDirectory);
    opts.setEntryScriptCode(std::string(javascriptCode));
    opts.setScreenSize(_width, _height);

    RuntimeApi::instance()->init(opts);
}

static void deinit(JNIEnv* env, jobject obj) {
    RuntimeApi::instance()->deinit();
}

static void onframe(JNIEnv* env, jobject obj) {
    RuntimeApi::instance()->onFrame();
}

static void onresume(JNIEnv* env, jobject obj) {

}

static int taskloop(JNIEnv* env, jobject obj, jstring taskId, jstring jfile, jboolean rw) {
    char *id = NULL;
    char *file = NULL;
    id = (char*)env->GetStringUTFChars(taskId, 0);
    file = (char*)env->GetStringUTFChars(jfile, 0);
    return RuntimeApi::instance()->runTask(std::string(id), std::string(file), rw);
}