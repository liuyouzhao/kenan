#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>

//#ifndef UChar
//#define UChar unsigned char
//#endif


#define RADIAL_VERTEX_NUMBER 72
#define TEXT_BUFFER_LENGTH 512

#define APP_ROOT "com.kenan.main"

/*
** Safe delete&release
*/
#ifndef SAFE_DELETE
#define SAFE_DELETE(a) if(a) {	delete a; a = NULL;	}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(a) if(a) {	a->release(); a = NULL;	}
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(a) if(a) {	free(a); a = NULL;	}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a) if(a) {	delete[] a; a = NULL;	}
#endif

#define CLOUDX_SCREEN_WIDTH     480
#define CLOUDX_SCREEN_HEIGHT    800

#define TAG_SYS "[kenan-sys]"
#define TAG_JS  "[kenan-js]"
#define TAG_ASSERT "[kenan-assert]"

#ifdef __ANDROID__
#undef LOG_TAG
#undef LOGI
#undef LOGE
#undef LOGD
#include <android/log.h>
#define  __LOGI(tag,...)  __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#define  __LOGE(tag,...)  __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)
#define  __LOGD(tag,...)  __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

#endif // DEFINES_H
