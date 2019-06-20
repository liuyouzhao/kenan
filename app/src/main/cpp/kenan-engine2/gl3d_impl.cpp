#include "gl3d_impl.h"

#include <android/log.h>
#undef LOG_TAG
#undef LOGI
#undef LOGE
#define  LOG_TAG    "gl3d_impl"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

Gl3d_Impl* Gl3d_Impl::create()
{
    return new Gl3d_Impl();
}

void Gl3d_Impl::System_Initiate(int x, int y, int width, int height, int layer)
{
#if NATIVE_SURFACE
    m_nativeEnv = NativeEnv::create();
    m_nativeEnv->setUp(x, y, width, height, layer);
#endif
}

void Gl3d_Impl::_Init(int width, int height)
{
    m_width  = width;
    m_height = height;
}

Gl3d_Impl::Gl3d_Impl()
{
#if NATIVE_SURFACE
    m_nativeEnv = NULL;
#endif
}

void Gl3d_Impl::swapBuffer()
{
    //LOGE("gl3d_impl.cpp : %d, %d",m_nativeEnv->getDisplay(), m_nativeEnv->getSurface());
#if NATIVE_SURFACE
    eglSwapBuffers(m_nativeEnv->getDisplay(), m_nativeEnv->getSurface());
#endif
}

void Gl3d_Impl::setPosition(int x, int y)
{
#if NATIVE_SURFACE
    m_nativeEnv->setPosition(x, y);
#endif
}

void Gl3d_Impl::setSize(int w, int h)
{
    m_width  = w;
    m_height = h;
#if NATIVE_SURFACE
    m_nativeEnv->setSize(w, h);
#endif
    glViewport(0, 0, w, h);
}

void Gl3d_Impl::setAlpha(float a)
{
#if NATIVE_SURFACE
    m_nativeEnv->setAlpha(a);
#endif
}
