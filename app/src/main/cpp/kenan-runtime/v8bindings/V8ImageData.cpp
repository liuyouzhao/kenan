#include "V8ImageData.h"
#include "V8PixelArray.h"
#include "ImageData.h"
#include "Color.h"
//#include "MemoryManager.h"
#include "gl2d_impl.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "V8ImageData"

namespace DCanvas
{

bool V8ImageData::s_bInited  = false;

v8::Persistent<v8::FunctionTemplate>        V8ImageData::s_class_func_templ;
v8::Persistent<v8::ObjectTemplate>          V8ImageData::s_class_proto;
v8::Persistent<ObjectTemplate>              V8ImageData::s_class_ins;

V8ImageData::V8ImageData()
{
}


V8ImageData::~V8ImageData()
{
}


void V8ImageData::TemplateDestroy()
{
    if (s_bInited)
    {
        s_class_ins.Dispose();
        s_class_proto.Dispose();
        s_class_func_templ.Dispose();
    }
    s_bInited = false;
    V8PixelArray::TemplateDestroy();
}

////////////js wrap
bool V8ImageData::HasInstance(v8::Handle<v8::Value> value)
{
    LOGD("ImageData::HasInstance");
    if (!s_bInited)
    {
        return false;
    }
    return s_class_func_templ->HasInstance(value);
}

v8::Handle<v8::Value> V8ImageData::Wrap(const v8::FunctionCallbackInfo<v8::Value>& args, Gl2d_Impl* gc)
{
    HandleScope scope;

    ImageData* imgdata = NULL;
    if (1 == args.Length())
    {
        if (!V8ImageData::HasInstance(args[0]))
        {
            return THROW_EXCEPTION(TTypeError, "ImageData : type of arguments Exception");
        }

        ImageData* imgD = ObjectWrap::Unwrap<ImageData>(args[0]->ToObject());
        int w = imgD->Width();
        int h = imgD->Height();

        imgdata = new ImageData(w, h);
    }
    else if (2 == args.Length())
    {
        for (int i = 0; i < 2; ++i)
        {
            if (!args[i]->IsInt32())
            {
                return THROW_EXCEPTION(TTypeError, "ImageData : type of arguments Exception");
            }
        }

        int w = args[0]->Int32Value();
        int h = args[1]->Int32Value();

        imgdata = new ImageData(w, h);
    }
    else if (4 == args.Length())
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!args[i]->IsInt32())
            {
                return THROW_EXCEPTION(TTypeError,
                        "ImageData : type of arguments Exception");
            }
        }

        int x = args[0]->Int32Value();
        int y = args[1]->Int32Value();
        int w = args[2]->Int32Value();
        int h = args[3]->Int32Value();
        LOGD("has 4 param");
        imgdata = new ImageData(x, y, w, h, gc);
    }
    else
    {
        return THROW_EXCEPTION(TError,
                "ImageData : number of arguments Exception");
    }
    LOGD("init ok");

    if (imgdata->m_hasException)
    {
        return THROW_EXCEPTION(TError, "imagedata out of memory Exception");
    }

    if (!s_bInited)
    {
        s_class_func_templ = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

        //function
        s_class_func_templ->SetClassName(v8::String::New("ImageData"));
        s_class_proto = Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());

        s_class_ins = Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
        s_class_ins -> SetInternalFieldCount(1);
        //varables
        s_class_ins ->SetAccessor(v8::String::New("data"),   V8ImageData::GetData, V8ImageData::SetData);
        s_class_ins ->SetAccessor(v8::String::New("width"),  V8ImageData::GetWidth, V8ImageData::SetWidth);
        s_class_ins ->SetAccessor(v8::String::New("height"), V8ImageData::GetHeight, V8ImageData::SetHeight);
        s_bInited = true;
    }

    Persistent<Object> class_obj = Persistent<Object>::New(s_class_ins->NewInstance());
    class_obj.MakeWeak(imgdata, WeakReferenceCallback(V8ImageData::ImageDataDestroy));
    class_obj->SetInternalField(0, External::New(imgdata));

    imgdata->SetSelf(class_obj);
    return scope.Close(class_obj);
}

/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
void V8ImageData::ImageDataDestroy(Persistent<Object> self, ImageData* parameter)
{
    //  v8 will call destructor so no need to delete more we have own img memory manager so no need this one
    if (parameter)
    {
        //MemoryManager* mm = MemoryManager::getManager();
        //mm->DeleteImageData(parameter);
    }
    self.ClearWeak();
    self.Dispose();
    self.Clear();
    LOGD("ImageData destroy end!");
}

v8::Handle<v8::Value> V8ImageData::GetData(v8::Local<v8::String> property, const PropertyCallbackInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(ImageData, info);

    PixelArray* pix = class_impl->get_data();

    return V8PixelArray::Wrap(pix);
}

void V8ImageData::SetData(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo& info)
{
    HandleScope scope;

    LOGD("color setted! the color is  ???????????????????????????????????");
    LOGD("%s", *(v8::String::AsciiValue(value)));
}

v8::Handle<v8::Value> V8ImageData::GetWidth(v8::Local<v8::String> property, const PropertyCallbackInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(ImageData, info);

    return Integer::New(class_impl->Width());
}

void V8ImageData::SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo& info)
{
    HandleScope scope;
}

v8::Handle<v8::Value> V8ImageData::GetHeight(v8::Local<v8::String> property, const PropertyCallbackInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(ImageData, info);

    return Integer::New(class_impl->Height());
}

void V8ImageData::SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const PropertyCallbackInfo& info)
{
    HandleScope scope;
}

} // namespace DCanvas
