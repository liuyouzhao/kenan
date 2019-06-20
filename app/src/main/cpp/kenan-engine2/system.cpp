/*
** gl's 2d Engine 1.00
** Copyright (C) 2011-2014, aliyun cloud apps
**
** Core System functions
*/
#include "defines.h"
#include "gl2d_impl.h"

#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))

#if IN_ANDROID
#include <GLES2/gl2ext.h>
#if NATIVE_SURFACE
#include <ui/FramebufferNativeWindow.h>
#include <ui/EGLUtils.h>
#endif
#include <android/log.h>
#undef LOG_TAG
#undef LOGI
#undef LOGE
#define LOG_TAG    "aligl2_lib"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#if NATIVE_SURFACE
#include <surfaceflinger/Surface.h>

using namespace android;
#endif
#endif
#if IN_LINUXx86
#include <stdlib.h>
#endif

void CALL Gl2d_Impl::Release()
{
    //eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    //eglTerminate(eglDisplay);

    SAFE_FREE(_index_quad);
    SAFE_FREE(_vertex_data);
    SAFE_FREE(_texcoord_data);
    SAFE_FREE(_color_data);

    _index_quad = NULL;
    _vertex_data = NULL;
    _texcoord_data = NULL;
    _color_data = NULL;

    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_textureCoordBuffer);
    glDeleteBuffers(1, &_colorBuffer);
    glDeleteBuffers(1, &_indexBuffer);
    Target_FreeAll();
    Effect_Free_All();
}

#if IN_WINx86
bool CALL Gl2d_Impl::System_Initiate(HWND window, HDC H, int width, int height)
{
    nScreenWidth = width;
    nScreenHeight = height;

    eglWindow = window;
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)H);

    if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

    EGLint iMajorVersion, iMinorVersion;
    if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
    {
        return false;
    }

    const EGLint pi32ConfigAttribs[] =
    {
        EGL_LEVEL,               0,
        EGL_SURFACE_TYPE,        EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,     EGL_OPENGL_ES2_BIT,
        EGL_NATIVE_RENDERABLE,   EGL_FALSE,
        EGL_DEPTH_SIZE,          EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS,      1,
        EGL_SAMPLES,             4,
        EGL_NONE
    };

    int iConfigs;
    if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
    {
        return false;
    }

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    if(eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
    }

    if (!TestEGLError( "eglCreateWindowSurface"))
    {
        return false;
    }

    // Bind the API (It could be OpenGLES or OpenVG)
    eglBindAPI(EGL_OPENGL_ES_API);
    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

    if (!TestEGLError("eglCreateContext"))
    {
        return false;
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (!TestEGLError("eglMakeCurrent"))
    {
        return false;
    }

    //glViewport(0, 0, width, height);

    this->_Init();

    return true;
}

#elif IN_LINUXx86
bool CALL Gl2d_Impl::System_Initiate(Window x11Window, Display* x11Display, int width, int height)
{
    nScreenWidth = width;
    nScreenHeight = height;

    eglWindow = (void*)x11Window;
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)x11Display);

    EGLint iMajorVersion, iMinorVersion;
    if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
    {
        printf("Error: eglInitialize() failed.\n");
        return false;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    if (!TestEGLError("eglBindAPI"))
    {
        return false;
    }

    EGLint pi32ConfigAttribs[5];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;
    pi32ConfigAttribs[4] = EGL_NONE;

    EGLint iConfigs;
    if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
    {
        printf("Error: eglChooseConfig() failed.\n");
        return false;
    }

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)x11Window, NULL);

    if (!TestEGLError("eglCreateWindowSurface"))
    {
        return false;
    }

    /*
        Step 7 - Create a context.
    */
    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
    if (!TestEGLError("eglCreateContext"))
    {
        return false;
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (!TestEGLError("eglMakeCurrent"))
    {
        return false;
    }

    this->_Init();
    return true;
}
#elif IN_ANDROID

bool CALL Gl2d_Impl::System_Initiate(int x, int y, int width, int height, int layer)
{
	//LOGE("javas System_Initiate");
	m_isNative = false;
/*
#if NATIVE_SURFACE
    m_nativeEnv = NativeEnv::create();
    m_nativeEnv->setUp(x, y, width, height, layer);
#endif // NATIVE_SURFACE
*/
    return true;
}

#if NATIVE_SURFACE
bool CALL Gl2d_Impl::System_InitiateNative(int x, int y, int width, int height, int layer)
{
	 //LOGE("native System_InitiateNative");
    m_isNative = true;
    m_nativeEnv = NativeEnv::create();
    m_nativeEnv->setUp(x, y, width, height, layer);
    return true;
}
#endif
#endif

void CALL Gl2d_Impl::System_Shutdown()
{
    Release();
}

#if IN_WINx86
void CALL Gl2d_Impl::System_SetStateBool(gl2dBoolState state, bool value)
{
    switch(state)
    {
        case gl2dBoolState::GL2D_DONTSUSPEND:
            bDontSuspend = value;
            break;
        case gl2dBoolState::GL2D_HIDEMOUSE:
            bHideMouse = value;
            break;
        case gl2dBoolState::GL2D_TEXTUREFILTER:
            bTextureFilter = value;
            break;
        case gl2dBoolState::GL2D_USESOUND:
            bUseSound = value;
            break;
        case gl2dBoolState::GL2D_WINDOWED:
            bWindowed = value;
            break;
        case gl2dBoolState::GL2D_ZBUFFER:
            bZBuffer = value;
            break;
        case gl2dBoolState::GL2DBOOLSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

void CALL Gl2d_Impl::System_SetStateInt(gl2dIntState state, int value)
{
    switch (state)
    {
        case gl2dIntState::GL2D_FPS:
            ngl2dFPS = value;
            break;
        case gl2dIntState::GL2D_FXVOLUME:
            nFXVolume = value;
            break;
        case gl2dIntState::GL2D_SAMPLERATE:
            nSampleRate = value;
            break;
        case gl2dIntState::GL2D_SCREENBPP:
            nScreenBPP = value;
            break;
        case gl2dIntState::GL2D_SCREENHEIGHT:
            nScreenHeight = value;
            break;
        case gl2dIntState::GL2D_SCREENWIDTH:
            nScreenWidth = value;
            break;
        case gl2dIntState::GL2D_MUSVOLUME:
            nMusVolume = value;
            break;
        case gl2dIntState::GL2DINTSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

void CALL Gl2d_Impl::System_SetStateString(gl2dStringState state, const char *value)
{
    switch(state)
    {
        case gl2dStringState::GL2D_ICON:szIcon = value;
            break;
        case gl2dStringState::GL2D_INIFILE:
            break;
        case gl2dStringState::GL2D_LOGFILE:
            break;
        case gl2dStringState::GL2D_TITLE:
            break;
        case gl2dStringState::GL2DSTRINGSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }

}

bool CALL Gl2d_Impl::System_GetStateBool(gl2dBoolState state)
{
    switch(state)
    {
        case gl2dBoolState::GL2D_DONTSUSPEND:
            return bDontSuspend;

        case gl2dBoolState::GL2D_HIDEMOUSE:
            return bHideMouse ;

        case gl2dBoolState::GL2D_TEXTUREFILTER:
            return bTextureFilter;

        case gl2dBoolState::GL2D_USESOUND:
            return bUseSound;

        case gl2dBoolState::GL2D_WINDOWED:
            return bWindowed ;

        case gl2dBoolState::GL2D_ZBUFFER:
            return bZBuffer;

        case gl2dBoolState::GL2DBOOLSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

#elif IN_LINUXx86
void CALL Gl2d_Impl::System_SetStateBool(gl2dBoolState state, bool value)
{
    switch(state)
    {

        case GL2D_DONTSUSPEND:
            bDontSuspend = value;
            break;
        case GL2D_HIDEMOUSE:
            bHideMouse = value;
            break;
        case GL2D_TEXTUREFILTER:
            bTextureFilter = value;
            break;
        case GL2D_USESOUND:
            bUseSound = value;
            break;
        case GL2D_WINDOWED:
            bWindowed = value;
            break;
        case GL2D_ZBUFFER:
            bZBuffer = value;
            break;
        case GL2DBOOLSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

void CALL Gl2d_Impl::System_SetStateInt(gl2dIntState state, int value)
{
    switch (state)
    {
        case GL2D_FPS:
            ngl2dFPS = value;
            break;
        case GL2D_FXVOLUME:
            nFXVolume = value;
            break;
        case GL2D_SAMPLERATE:
            nSampleRate = value;
            break;
        case GL2D_SCREENBPP:
            nScreenBPP = value;
            break;
        case GL2D_SCREENHEIGHT:
            nScreenHeight = value;
            break;
        case GL2D_SCREENWIDTH:
            nScreenWidth = value;
            break;
        case GL2D_MUSVOLUME:
            nMusVolume = value;
            break;
        case GL2DINTSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

void CALL Gl2d_Impl::System_SetStateString(gl2dStringState state, const char *value)
{
    switch(state)
    {
        case GL2D_ICON:szIcon = value;
            break;
        case GL2D_INIFILE:
            break;
        case GL2D_LOGFILE:
            break;
        case GL2D_TITLE:
            break;
        case GL2DSTRINGSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}

bool CALL Gl2d_Impl::System_GetStateBool(gl2dBoolState state)
{
    switch(state)
    {
        case GL2D_DONTSUSPEND:
            return bDontSuspend;

        case GL2D_HIDEMOUSE:
            return bHideMouse ;

        case GL2D_TEXTUREFILTER:
            return bTextureFilter;
        case GL2D_USESOUND:
            return bUseSound;

        case GL2D_WINDOWED:
            return bWindowed ;

        case GL2D_ZBUFFER:
            return bZBuffer;

        case GL2DBOOLSTATE_FORCE_DWORD:
            break;
        default:
            break;
    }
}
#endif

void CALL Gl2d_Impl::System_ChangeSize(int x, int y, int width, int height)
{
    nScreenWidth  =  width;
    nScreenHeight =  height;
    glViewport(x,y,width,height);
}

char* CALL Gl2d_Impl::System_GetErrorMessage()
{
    return NULL;
}

//////// Implementation ////////
Gl2d_Impl::Gl2d_Impl()
{
    fTime = 0.0f;
    fDeltaTime = 0.0f;
    nFPS = 0;

    szIcon = 0;
    nScreenWidth = 800;
    nScreenHeight = 600;
    nScreenBPP = 32;
    bWindowed = false;
    bZBuffer = false;
    bTextureFilter = true;

    bUseSound = true;
    nSampleRate = 44100;
    nFXVolume = 100;
    nMusVolume = 100;
    nFixedDelta = 0;
    bHideMouse = true;
    bDontSuspend = false;
    bUseTransformation = true;

    _vertex_data = NULL;
    _color_data = NULL;
    _texcoord_data = NULL;
}
#if NATIVE_SURFACE
void Gl2d_Impl::setPosition(int x, int y)
{
    m_nativeEnv->setPosition(x, y);
}

void Gl2d_Impl::setSize(int w, int h)
{
    nScreenWidth = w;
    nScreenHeight = h;
    m_nativeEnv->setSize(w, h);
    glViewport(0, 0, w, h);
}

void Gl2d_Impl::setAlpha(float a)
{
    m_nativeEnv->setAlpha(a);
}
#endif
