#ifndef  NATIVEENV_H
#define  NATIVEENV_H

#if NATIVE_SURFACE

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <stdlib.h>
#include <unistd.h>
#include <surfaceflinger/ISurface.h>
#include <surfaceflinger/SurfaceComposerClient.h>
#endif

class NativeEnv
{
public:
    static NativeEnv* create();
    ~NativeEnv();
#ifdef __ANDROID__
    bool setUp(int x, int y, int width, int height, int layer);
#endif

    int getX() const {return x_pos;}
    int getY() const {return y_pos;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    void setX(const int x) { x_pos = x;}
    void setY(const int y) { y_pos = y;}
    void setWidth(const int w) {width = w;}
    void setHeight(const int h) {height = h;}
    void setLayer(const int lay);
    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setAlpha(float a);

#if NATIVE_SURFACE
    void setClient(android::sp<android::SurfaceComposerClient> c) { m_client = c; }
    void setControl(android::sp<android::SurfaceControl> c) { m_surface_control = c; }
    void setSurface(android::sp<android::Surface> s) { m_surface_layer = s; }
#endif

#ifdef __ANDROID__
    EGLDisplay getDisplay(){return eglDisplay;}
    EGLConfig  getConfig(){return eglConfig;}
    EGLSurface getSurface(){return eglSurface;}
    EGLContext getContext(){return eglContext;}
#endif

private:
    NativeEnv();
    int x_pos;
    int y_pos;
    int width;
    int height;
    int layer;

    // the native surface used structure
#ifdef __ANDROID__
#if NATIVE_SURFACE
    android::sp<android::SurfaceComposerClient>       m_client;
    android::sp<android::SurfaceControl>              m_surface_control;
    android::sp<android::Surface>                     m_surface_layer;

	void openTransaction(android::sp<android::SurfaceComposerClient> client);
	void closeTransaction(android::sp<android::SurfaceComposerClient> client);
#endif
    bool hasSetup;

    // EGL variables
    EGLDisplay            eglDisplay;
    EGLConfig             eglConfig;
    EGLSurface            eglSurface;
    EGLContext            eglContext;
    //EGLNativeWindowType    eglWindow;


};

#endif // __ANDROID__

#endif //NATIVE_SURFACE

#endif // NATIVEENV_H
