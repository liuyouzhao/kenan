#ifndef WEBGLCONTEXTATTRIBUTES_H
#define WEBGLCONTEXTATTRIBUTES_H

namespace DCanvas
{

class WebGLContextAttributes
{
public:
    // Context creation attributes.
    struct Attributes
    {
        Attributes()
            : alpha(true)
            , depth(true)
            , stencil(false)
            , antialias(true)
            , premultipliedAlpha(true)
            , canRecoverFromContextLoss(true)
            , preserveDrawingBuffer(false)
            , noExtensions(false)
            , shareResources(true)
        {
        }

        bool alpha;
        bool depth;
        bool stencil;
        bool antialias;
        bool premultipliedAlpha;
        bool canRecoverFromContextLoss;
        bool preserveDrawingBuffer;
        bool noExtensions;
        bool shareResources;
    };
      
    virtual ~WebGLContextAttributes();

    static WebGLContextAttributes* create();
    
    static WebGLContextAttributes* create(Attributes attributes);
    
    static WebGLContextAttributes* create(WebGLContextAttributes attributes);
    // Whether or not the drawing buffer has an alpha channel; default=true
    bool alpha() const;
    void setAlpha(bool alpha);

    // Whether or not the drawing buffer has a depth buffer; default=true
    bool depth() const;
    void setDepth(bool depth);

    // Whether or not the drawing buffer has a stencil buffer; default=true
    bool stencil() const;
    void setStencil(bool stencil);

    // Whether or not the drawing buffer is antialiased; default=true
    bool antialias() const;
    void setAntialias(bool antialias);

    // Whether or not to treat the values in the drawing buffer as
    // though their alpha channel has already been multiplied into the
    // color channels; default=true
    bool premultipliedAlpha() const;
    void setPremultipliedAlpha(bool premultipliedAlpha);

    // Whether or not to preserve the drawing buffer after presentation to the
    // screen; default=false
    bool preserveDrawingBuffer() const;
    void setPreserveDrawingBuffer(bool);

    // Fetches a copy of the attributes stored in this object in a
    // form that can be used to initialize a GraphicsContext3D.
    Attributes attributes() const;

protected:
    WebGLContextAttributes();
    WebGLContextAttributes(Attributes attributes);

private:
    Attributes m_attrs;
};

} // namespace DCanvas

#endif // WEBGLCONTEXTATTRIBUTES_H
