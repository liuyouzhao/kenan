#ifndef RENDERINGCONTEXT_H
#define RENDERINGCONTEXT_H

namespace DCanvas
{

class RenderingContext
{
public:
    RenderingContext(){}
    virtual ~RenderingContext(){}
    virtual bool is2d() const { return false; }
    virtual bool is3d() const { return false; }
    virtual void onFrame() = 0;
    virtual void destroy() = 0;
#if NATIVE_SURFACE
    virtual void setPosition(int x, int y) = 0;
    virtual void setSize(int w, int h) = 0;
    virtual void setAlpha(float alpha) = 0;
#endif
};

} //namespace DCanvas

#endif //RENDERINGCONTEXT_H
