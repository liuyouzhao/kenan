#ifndef CANVAS_H
#define CANVAS_H

#include <v8.h>
#include <hash_map>
#include <string>
#include "NativeEnv.h"

using namespace v8;

namespace DCanvas
{
class RenderingContext;
class Canvas
{
friend class V8Canvas;
public:
    Canvas(int, int w, int h);
    Canvas();
    Canvas(int, int, int, int, int);
    Canvas(int x, int y, int width, int height);
    ~Canvas();
    void onFrame();
    void drawException(const char* s, int width, int height);
    void prepareToDrawException();
    int getId();
    RenderingContext* get_renderingContext() { return m_context; }

    v8::Handle<v8::Value> get_context(const v8::FunctionCallbackInfo<v8::Value>& args);

    RenderingContext* getContext(const char* type);
    RenderingContext* getContext(const char* type, hash_map<std::string, bool> hashAttri);
    
    void resume();
    void pause();
    void setLayer(unsigned int);
private:

    int m_id;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    RenderingContext*                       m_context;
    v8::Local<v8::Value>                    m_js_TouchStartValue;
    NativeEnv*                              m_nativeEnv;
    unsigned int                            m_currentLayer;
};

}// namespace DCanvas

#endif // CANVAS_H
