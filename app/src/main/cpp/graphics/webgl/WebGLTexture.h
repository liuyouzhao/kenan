#ifndef WEBGLTEXTURE_H
#define WEBGLTEXTURE_H

#include "ObjectWrap.h"
#include "WebGLObject.h"
#include "WebGLGraphicsContext.h"
#include "Types3D.h"

#include <vector>
namespace DCanvas
{

class WebGLTexture: public WebGLObject, public ObjectWrap
{
public:
    ~WebGLTexture();
    static WebGLTexture* create(WebGLGraphicsContext*);

    virtual bool isTexture() const { return true; }
    virtual void deleteObjectImpl(DCPlatformObject &o);
    bool setTarget(DCenum target);
    void setTarget(DCenum target, DCint maxLevel);
    DCenum getTarget();

    void setLevelInfo(DCenum target, DCint level, DCenum internalFormat, DCsizei width, DCsizei height, DCenum type);

    bool canGenerateMipmaps();
    // Generate all level information.
    void generateMipmapLevelInfo();

    DCenum getInternalFormat(DCenum target, DCint level) const;
    DCenum getType(DCenum target, DCint level) const;
    DCsizei getWidth(DCenum target, DCint level) const;
    DCsizei getHeight(DCenum target, DCint level) const;

    // Whether width/height is NotPowerOfTwo.
    static bool isNPOT(DCsizei, DCsizei);

    bool isNPOT() const;
    // Determine if texture sampling should always return [0, 0, 0, 1] (OpenGL ES 2.0 Sec 3.8.2).
    bool needToUseBlackTexture() const;
    bool hasEverBeenBound() const { return getObject() && m_target; }

    static DCint computeLevelCount(DCsizei width, DCsizei height);

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);

    static bool HasInstance(v8::Handle<v8::Value> value);
    static void destroy();

private:
    class LevelInfo
    {
    public:
        LevelInfo()
            : valid(false)
            , internalFormat(0)
            , width(0)
            , height(0)
            , type(0)
        {
        }

        void setInfo(DCenum internalFmt, DCsizei w, DCsizei h, DCenum tp)
        {
            valid = true;
            internalFormat = internalFmt;
            width = w;
            height = h;
            type = tp;
        }

        bool valid;
        DCenum internalFormat;
        DCsizei width;
        DCsizei height;
        DCenum type;
    };

    void update();

    int mapTargetToIndex(DCenum) const;

    WebGLTexture(WebGLGraphicsContext*);
    const LevelInfo* getLevelInfo(DCenum target, DCint level) const;

    DCenum m_target;

    DCenum m_minFilter;
    DCenum m_magFilter;
    DCenum m_wrapS;
    DCenum m_wrapT;

    std::vector<std::vector<LevelInfo> > m_info;

    bool m_isNPOT;
    bool m_isComplete;
    bool m_needToUseBlackTexture;
    //static int m_nFuncTmpCount;
    static v8::Persistent<v8::FunctionTemplate>                    s_class_func_templ;
    static v8::Persistent<v8::ObjectTemplate>                      s_class_proto;
    static v8::Persistent<v8::ObjectTemplate>                      s_class_ins;
    static bool s_bInited;
};

} // namespace DCanvas
#endif // WEBGLTEXTURE_H
