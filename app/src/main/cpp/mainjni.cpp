#include "defines.h"
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "V8Main.h"
#include "PlatformConfig.h"
#include "CallBackFuncs.h"
#include "gl2d_impl.h"


#undef LOG_TAG
#define  LOG_TAG    "kenan_mainjni"

using namespace kenan_v8bindings;
using namespace kenan_system;

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

    /**
    (1) Init graphics
    */
    Gl2d_Impl::instance()->_Init(_width, _height);

    /**
    (2) Set platform configuration
    */
    char* dataDirectory = NULL;
    dataDirectory = (char*)env->GetStringUTFChars(dataDir, 0);
    LOGE("dataDirectory is %s", dataDirectory);
    PlatformConfig::instance()->setDataDir(std::string(dataDirectory));


    /**
    (3) Init V8
    */
    char* javascriptCode = NULL;
    javascriptCode = (char*)env->GetStringUTFChars(code, 0);
    V8Main::instance()->initV8Environment();
    V8Main::instance()->firstRunJavascript(std::string(javascriptCode));


}

JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_deinit(JNIEnv* env, jobject obj) {
    V8Main::instance()->destroyV8Environment();
}


JNIEXPORT void JNICALL Java_com_kenan_jni_JNILIB_OnFrame(JNIEnv* env, jobject obj) {
    V8Main::instance()->onFrameUpdateCallback();
    CallBackFuncs::getFuncQueue()->CallAllFunc();
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
