#include "defines.h"
#include "WebGLTexture.h"
#include "Assert.h"

#undef LOG_TAG
#define  LOG_TAG    "WebGLTexture"

namespace DCanvas
{

v8::Persistent<v8::FunctionTemplate>        WebGLTexture::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          WebGLTexture::s_class_proto;
v8::Persistent<v8::ObjectTemplate>          WebGLTexture::s_class_ins;

bool WebGLTexture::s_bInited = false;

WebGLTexture* WebGLTexture::create(WebGLGraphicsContext* context)
{
    return new WebGLTexture(context);
}

WebGLTexture::WebGLTexture(WebGLGraphicsContext* context)
    : WebGLObject(context)
    , m_target(0)
    , m_minFilter(GL_NEAREST_MIPMAP_LINEAR)
    , m_magFilter(GL_LINEAR)
    , m_wrapS(GL_REPEAT)
    , m_wrapT(GL_REPEAT)
    , m_isNPOT(false)
    , m_isComplete(false)
    , m_needToUseBlackTexture(false)
{
    glGenTextures(1, &m_object);
}

WebGLTexture::~WebGLTexture()
{
    deleteObject();
}

bool WebGLTexture::setTarget(DCenum target)
{
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP)
    {
        LOGE("setTarget : bind texture with type error");
        return false;
    }
    m_target = target;

    return true;
}

void WebGLTexture::setTarget(DCenum target, DCint maxLevel)
{
    if (!getObject())
        return;
    // Target is finalized the first time bindTexture() is called.
    if (m_target)
        return;

    switch (target)
    {
        case GL_TEXTURE_2D:
            m_target = target;
            m_info.resize(1);
            m_info[0].resize(maxLevel);
            break;
        case GL_TEXTURE_CUBE_MAP:
            m_target = target;
            m_info.resize(6);
            for (int ii = 0; ii < 6; ++ii)
                m_info[ii].resize(maxLevel);
            break;
    }
}

DCenum WebGLTexture::getTarget()
{
    return m_target;
}

void WebGLTexture::deleteObjectImpl(DCPlatformObject &o)
{
    LOGD("delete texture %d", o);
    glDeleteTextures(1, &o);
}

void WebGLTexture::setLevelInfo(DCenum target, DCint level, DCenum internalFormat, DCsizei width, DCsizei height, DCenum type)
{
    if (!getObject() || !m_target)
        return;
    // We assume level, internalFormat, width, height, and type have all been
    // validated already.
    int index = mapTargetToIndex(target);

    if (index < 0)
        return;

    m_info[index][level].setInfo(internalFormat, width, height, type);
    update();
}

void WebGLTexture::generateMipmapLevelInfo()
{
    if (!getObject() || !m_target)
        return;
    if (!canGenerateMipmaps())
        return;

    if (!m_isComplete)
    {
        for (size_t ii = 0; ii < m_info.size(); ++ii)
        {
            const LevelInfo& info0 = m_info[ii][0];
            DCsizei width = info0.width;
            DCsizei height = info0.height;
            DCint levelCount = computeLevelCount(width, height);

            for (DCint level = 1; level < levelCount; ++level)
            {
                width = std::max(1, width >> 1);
                height = std::max(1, height >> 1);
                LevelInfo& info = m_info[ii][level];
                info.setInfo(info0.internalFormat, width, height, info0.type);
            }
        }
        m_isComplete = true;
    }
    m_needToUseBlackTexture = false;
}

DCenum WebGLTexture::getInternalFormat(DCenum target, DCint level) const
{
    const LevelInfo* info = getLevelInfo(target, level);
    if (!info)
        return 0;

    return info->internalFormat;
}

DCenum WebGLTexture::getType(DCenum target, DCint level) const
{
    const LevelInfo* info = getLevelInfo(target, level);
    if (!info)
        return 0;

    return info->type;
}

DCsizei WebGLTexture::getWidth(DCenum target, DCint level) const
{
    const LevelInfo* info = getLevelInfo(target, level);
    if (!info)
        return 0;

    return info->width;
}

DCsizei WebGLTexture::getHeight(DCenum target, DCint level) const
{
    const LevelInfo* info = getLevelInfo(target, level);
    if (!info)
        return 0;

    return info->height;
}

bool WebGLTexture::isNPOT(DCsizei width, DCsizei height)
{
    ASSERT(width >= 0 && height >= 0);

    if (!width || !height)
    {
        LOGE("isNPOT : width or height is invalid");
        return false;
    }

    if ((width & (width - 1)) || (height & (height - 1)))
    {
        return true;
    }

    return false;
}

bool WebGLTexture::isNPOT() const
{
    if (!getObject())
    {
        LOGE("object is 0");
        return false;
    }
    return m_isNPOT;
}

int WebGLTexture::mapTargetToIndex(DCenum target) const
{
    if (m_target == GL_TEXTURE_2D)
    {
        if (target == GL_TEXTURE_2D)
            return 0;
    }
    else if (m_target == GL_TEXTURE_CUBE_MAP)
    {
        switch (target)
        {
            case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                return 0;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                return 1;
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                return 2;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                return 3;
            case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                return 4;
            case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                return 5;
        }
    }
    return -1;
}

bool WebGLTexture::canGenerateMipmaps()
{
    if (isNPOT())
        return false;
    const LevelInfo& first = m_info[0][0];

    for (size_t ii = 0; ii < m_info.size(); ++ii)
    {
        const LevelInfo& info = m_info[ii][0];
        if (!info.valid
            || info.width != first.width || info.height != first.height
            || info.internalFormat != first.internalFormat || info.type != first.type)
        {
            if (!info.valid)
                LOGE("canGenerateMipmaps : levelInfo is valid");
            return false;
        }
    }
    return true;
}

DCint WebGLTexture::computeLevelCount(DCsizei width, DCsizei height)
{
    // return 1 + log2Floor(std::max(width, height));
    DCsizei n = std::max(width, height);

    if (n <= 0)
        return 0;

    DCint log = 0;
    DCsizei value = n;
    for (int ii = 4; ii >= 0; --ii)
    {
        int shift = (1 << ii);
        DCsizei x = (value >> shift);
        if (x)
        {
            value = x;
            log += shift;
        }
    }
    ASSERT(value == 1);
    return log + 1;
}

void WebGLTexture::update()
{
    m_isNPOT = false;
    for (size_t ii = 0; ii < m_info.size(); ++ii)
    {
        if (isNPOT(m_info[ii][0].width, m_info[ii][0].height))
        {
            m_isNPOT = true;
            break;
        }
    }

    m_isComplete = true;
    const LevelInfo& first = m_info[0][0];
    DCint levelCount = computeLevelCount(first.width, first.height);

    if (levelCount < 1)
        m_isComplete = false;
    else
    {
        for (size_t ii = 0; ii < m_info.size() && m_isComplete; ++ii)
        {
            const LevelInfo& info0 = m_info[ii][0];
            if (!info0.valid
              || info0.width != first.width || info0.height != first.height
              || info0.internalFormat != first.internalFormat || info0.type != first.type)
            {
                m_isComplete = false;
                break;
            }
            DCsizei width = info0.width;
            DCsizei height = info0.height;

            for (DCint level = 1; level < levelCount; ++level)
            {
                width = std::max(1, width >> 1);
                height = std::max(1, height >> 1);
                const LevelInfo& info = m_info[ii][level];
                if (!info.valid
                  || info.width != width || info.height != height
                  || info.internalFormat != info0.internalFormat || info.type != info0.type)
                {
                    m_isComplete = false;
                    break;
                }

            }
        }
    }

    m_needToUseBlackTexture = false;
    // NPOT
    if (m_isNPOT && ((m_minFilter != GL_NEAREST && m_minFilter != GL_LINEAR)
                   || m_wrapS != GL_CLAMP_TO_EDGE || m_wrapT != GL_CLAMP_TO_EDGE))
    {
        m_needToUseBlackTexture = true;
    }
    // Completeness
    if (!m_isComplete && m_minFilter != GL_NEAREST && m_minFilter != GL_LINEAR)
        m_needToUseBlackTexture = true;
}

const WebGLTexture::LevelInfo* WebGLTexture::getLevelInfo(DCenum target, DCint level) const
{
    if (!getObject() || !m_target)
        return 0;

    int targetIndex = mapTargetToIndex(target);

    if (targetIndex < 0 || targetIndex >= static_cast<int>(m_info.size()))
        return 0;

    if (level < 0 || level >= static_cast<DCint>(m_info[targetIndex].size()))
        return 0;

    return &(m_info[targetIndex][level]);
}

void WebGLTexture::destroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

bool WebGLTexture::HasInstance(v8::Handle<v8::Value> value)
{
    if (!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> WebGLTexture::wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("WebGLTexture"));
        s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_class_func_templ->Inherit(WebGLObject::createObject());
        s_bInited = true;
    }

    v8::Local<v8::Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));
    class_obj->Set(v8::String::New("WebGLTexture Object"),v8::String::New("WebGLTexture Object"));

    s_class_func_templ->Inherit(WebGLObject::createObject());

    return scope.Close(class_obj);
}

} // namespace DCanvas
