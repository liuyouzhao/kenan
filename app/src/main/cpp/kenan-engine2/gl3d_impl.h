#ifndef GL3D_IMPL_H
#define GL3D_IMPL_H
/*
HAS_FIXED_POINT
#if NATIVE_SURFACE
/// here need to add a defination
*/

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "aligl.h"

class Gl3d_Impl
#if NATIVE_SURFACE
: public aligl
#endif
{
public:
     static Gl3d_Impl* create();
     void _Init(int width, int height);
     void System_Initiate(int x, int y, int width, int height, int layer);
     void swapBuffer();
     void setPosition(int x, int y);
     void setSize(int w, int h);
     void setAlpha(float a);

#if NATIVE_SURFACE
     NativeEnv* getNativeEnv() const { return m_nativeEnv; }
     void setNativeEnv(NativeEnv* nativeEnv) { m_nativeEnv = nativeEnv; }
#endif

private:
    int m_width;
    int m_height;
    Gl3d_Impl();
};

#endif // GL3D_IMPL_H
