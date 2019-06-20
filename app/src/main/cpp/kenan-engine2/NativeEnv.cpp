#include "NativeEnv.h"

#if NATIVE_SURFACE

#include <ui/FramebufferNativeWindow.h>
#include <ui/EGLUtils.h>

#include <android/log.h>
#undef LOG_TAG
#undef LOGI
#undef LOGE
#define  LOG_TAG    "NativeEnv"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

bool TestEGLError(char* pszLocation)
{
    /*
    eglGetError returns the last error that has happened using egl,
    not the status of the last called function. The user has to
    check after every single egl call or at least once every frame.
    */
    EGLint iErr = eglGetError();
    if (iErr != EGL_SUCCESS)
    {
        return false;
    }

    return true;
}

NativeEnv* NativeEnv::create()
{
    return new NativeEnv();
}

NativeEnv::~NativeEnv()
{
//#if NATIVE_SURFACE
	m_surface_control->clear();
	m_client->dispose();
	m_surface_control = NULL;
	m_client = NULL;
	m_surface_layer = NULL;
//#endif
}

#ifdef __ANDROID__

#if NATIVE_SURFACE
void NativeEnv::openTransaction(android::sp<android::SurfaceComposerClient> client)
{
#if _ANDROID_4_0_0==400
	client->openGlobalTransaction();
#elif _ANDROID_2_3_0_==230
	client->openTransaction();
#endif
}

void NativeEnv::closeTransaction(android::sp<android::SurfaceComposerClient> client)
{
#if _ANDROID_4_0_0==400
	client->closeGlobalTransaction();
#elif _ANDROID_2_3_0_==230
	client->closeTransaction();
#endif
}
#endif

bool NativeEnv::setUp(int x, int y, int width, int height, int layer)
{
    this->width = width;
    this->height = height;
    if(hasSetup)
    {
        LOGE("has setup");
        printf("has set up ");
        return false ;
    }

#if NATIVE_SURFACE
    // (1) create a client to surfaceflinger
    m_client = new android::SurfaceComposerClient();
    //native->setClient(client);

    // create  surface
    m_surface_control = m_client->createSurface(getpid(), 0, width, height, android::PIXEL_FORMAT_RGBA_8888);
    //native->setControl(surface_control);

    this->layer = layer;
	 openTransaction(m_client);
//    m_client->openGlobalTransaction();
    m_surface_control->setLayer(layer);
    m_surface_control->setPosition(x, y);
    //m_surface_control->setAlpha(0.3);
    //m_client->closeGlobalTransaction();
	 closeTransaction(m_client);

    m_surface_layer = m_surface_control->getSurface();
    //native->setControl(surface_layer);

    //begin to create egl context
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);


    EGLint iMajorVersion, iMinorVersion;
    if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
    {
        printf("Error: eglInitialize() failed.\n");
        return false;
    }

//        eglBindAPI(EGL_OPENGL_ES_API);
//
//        if (!TestEGLError("eglBindAPI"))
//        {
//            return false;
//        }

    //EGLint pi32ConfigAttribs[5];
//    EGL_OPENGL_ES2_BIT 4
    const EGLint pi32ConfigAttribs[] =
    {
//      EGL_LEVEL, 0,
//      EGL_RED_SIZE, 5,
//      EGL_GREEN_SIZE, 6,
//      EGL_BLUE_SIZE, 5,
//      EGL_DEPTH_SIZE, 0,
//      EGL_NONE
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_DEPTH_SIZE, 4,
        EGL_STENCIL_SIZE, 1,
        EGL_RENDERABLE_TYPE, 4,
        EGL_NONE
    };

    //pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    //pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    //pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    //pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;
    //pi32ConfigAttribs[4] = EGL_NONE;

    LOGD("haha! let's select config for native window!");
    EGLConfig iConfigs;
    int err = android::EGLUtils::selectConfigForNativeWindow(eglDisplay,
                            pi32ConfigAttribs, m_surface_layer.get(), &iConfigs);
    if (err)
    {
        LOGD("Error: eglChooseConfig() failed.\n");
        printf("Error: eglChooseConfig() failed.\n");
        return false;
    }
    LOGD("haha! let call eglCreateWindowSurface!");
    eglSurface = eglCreateWindowSurface(eglDisplay, iConfigs, m_surface_layer.get(), NULL);

    if (!TestEGLError((char*)"eglCreateWindowSurface"))
    {
        LOGE("Error: eglCreateWindowSurface() failed.\n");
        return false;
    }

    /*
        Step 7 - Create a context.
    */
    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, iConfigs, NULL, ai32ContextAttribs);
    if (!TestEGLError((char*)"eglCreateContext"))
    {
        return false;
    }
    LOGD("haha! let call eglMakeCurrent!");
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &(this->width));
    eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &(this->height));
    if (!TestEGLError((char*)"eglMakeCurrent"))
    {
        return false;
    }
#endif
    hasSetup = 1;
    return true;
}
#endif

NativeEnv::NativeEnv()
{
    hasSetup = 0;
}

void NativeEnv::setPosition(int x, int y)
{
    x_pos = x;
    y_pos = y;
#if NATIVE_SURFACE
	 openTransaction(m_client);
//    m_client->openGlobalTransaction();
    m_surface_control->setLayer(layer);
    m_surface_control->setPosition(x, y);
    //m_client->closeGlobalTransaction();
	 closeTransaction(m_client);
#endif
}

void NativeEnv::setSize(int w, int h)
{
    width = w;
    height = h;

#if NATIVE_SURFACE
	 openTransaction(m_client);
//    m_client->openGlobalTransaction();
    m_surface_control->setLayer(layer);
    m_surface_control->setSize(w, h);
    //m_client->closeGlobalTransaction();
	 closeTransaction(m_client);

    //eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    //eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &(this->width));
    //eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &(this->height));
#endif
}

void NativeEnv::setLayer(int lay)
{
    layer = lay;
#if NATIVE_SURFACE
	 openTransaction(m_client);
//    m_client->openGlobalTransaction();
    m_surface_control->setLayer(layer);
    //m_client->closeGlobalTransaction();
	 closeTransaction(m_client);
#endif
}

void NativeEnv::setAlpha(float a)
{
#if NATIVE_SURFACE
	 openTransaction(m_client);
//    m_client->openGlobalTransaction();
    m_surface_control->setLayer(layer);
    m_surface_control->setAlpha(a);
    //m_client->closeGlobalTransaction();
	 closeTransaction(m_client);
#endif
}

#endif //#if NATIVE_SURFACE
