#include <jni.h>
#include <string.h>
#include <stdio.h>

#include "RuntimeApi.h"
#include "RuntimeOptions.h"

#undef LOG_TAG
#define  LOG_TAG    "kenan_mainjni"

using namespace kenan_runtime;

JNIEnv *javaEnvironment;
JavaVM *jvm = NULL;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_init(JNIEnv* env, jobject obj, jint width, jint height, jstring jsCode, jstring dataDir);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_deinit(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnFrame(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnResume(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchStart(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchEnd(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchMove(JNIEnv* env, jobject obj, jint x, jint y);
    JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnLoadResource(JNIEnv* env, jobject obj, jint resId);
};

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_init(JNIEnv *env, jobject obj, jint _width, jint _height, jstring code, jstring dataDir) {

    javaEnvironment = env;
    env->GetJavaVM(&jvm);
    const char *dataDirectory = NULL;
    const char *javascriptCode = NULL;
    dataDirectory = (const char*)env->GetStringUTFChars(dataDir, 0);
    javascriptCode = (const char*)env->GetStringUTFChars(code, 0);

    RuntimeOptions opts;
    opts.setWorkingDirectory(dataDirectory);
    opts.setEntryScriptCode(std::string(javascriptCode));
    opts.setScreenSize(_width, _height);

    RuntimeApi::instance()->init(opts);
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_deinit(JNIEnv* env, jobject obj) {
    RuntimeApi::instance()->deinit();
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnFrame(JNIEnv* env, jobject obj) {
    RuntimeApi::instance()->onFrame();
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnResume(JNIEnv* env, jobject obj) {

}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchStart(JNIEnv* env, jobject obj, jint x, jint y) {
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchEnd(JNIEnv* env, jobject obj, jint x, jint y) {
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_TouchMove(JNIEnv* env, jobject obj, jint x, jint y) {
}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnLoadResource(JNIEnv* env, jobject obj, jint resId) {
}
