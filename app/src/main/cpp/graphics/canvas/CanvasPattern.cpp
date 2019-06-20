#include "defines.h"
#include "CanvasPattern.h"
#include "V8Image.h"


#undef LOG_TAG
#define  LOG_TAG    "CanvasPattern"

namespace DCanvas
{

bool CanvasPattern::s_bInited;
v8::Persistent<v8::ObjectTemplate>             CanvasPattern::s_class_ins;
v8::Persistent<v8::FunctionTemplate>           CanvasPattern::s_class_func_templ;

CanvasPattern::CanvasPattern(Image* image, const std::string& type, int canvasId, Gl2d_Impl* gc)
    : m_image(image)
    , m_htex(NO_TEXTURE)
    , m_gc(gc)
    , m_type(type)
    , m_repeatX(false)
    , m_repeatY(false)
    , m_canvasId(canvasId)
{
    this->m_name = Style::TClassCanvasPattern;
    this->parseRepetitionType(type);
    MemoryManager::getManager()->AddCanvasPattern(this);
}

CanvasPattern::~CanvasPattern()
{
    if (m_htex != NO_TEXTURE)
    {
        m_gc->Texture_Free(m_htex);
        m_gc->Target_Free(m_target);
    }
    //MemoryManager::getManager()->RemoveCanvasPattern(this);
}

void CanvasPattern::parseRepetitionType(const std::string& type)
{
    if (type.empty() || type == "repeat")
    {
        m_repeatX = true;
        m_repeatY = true;
        return;
    }

    if (type == "no-repeat")
    {
        m_repeatX = false;
        m_repeatY = false;
        return;
    }

    if (type == "repeat-x")
    {
        m_repeatX = true;
        m_repeatY = false;
        return;
    }

    if (type == "repeat-y")
    {
        m_repeatX = false;
        m_repeatY = true;
        return;
    }
}

void CanvasPattern::setTargetAndTexture(HTARGET htar, HTEXTURE htex)
{
    m_target = htar;
    m_htex = htex;
}

bool CanvasPattern::HasInstance(v8::Handle<v8::Value> value)
{
    if(!s_bInited)
        return false;

    return s_class_func_templ->HasInstance(value);
}

void CanvasPattern::destroy()
{
    if(s_bInited)
    {
        s_class_ins.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
}

v8::Handle<v8::Value> CanvasPattern::wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc, int canvasId)
{
    HandleScope scope;

    CanvasPattern* cp = NULL;
    if (2 == args.Length())
    {
        if (!V8Image::HasInstance(args[0]))
            return THROW_EXCEPTION(TTypeError,
                "CanvasPattern : argument 1 is not Image");

        Image* img = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
        v8::String::Utf8Value  value(args[1]);
        std::string repetition(*value);
        cp = MemoryManager::getManager()->FindCanvasPattern(img, repetition, canvasId);
        if (cp == NULL)
        {
            LOGD("new CanvasPattern!");
            cp = new CanvasPattern(img, repetition, canvasId, gc);
        }
    }
    else
    {
        return THROW_EXCEPTION(TTypeError,
                "CanvasPattern : number of arguments exception");
    }

    if(!s_bInited)
    {
        s_class_func_templ = v8::Persistent<FunctionTemplate>::New(FunctionTemplate::New());
        s_class_func_templ->SetClassName(v8::String::New("CanvasPattern"));
        s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        s_bInited = true;
    }

    v8::Handle<v8::Object> class_obj = s_class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(cp));

    return scope.Close(class_obj);
}

} // namepsace DCanvas
