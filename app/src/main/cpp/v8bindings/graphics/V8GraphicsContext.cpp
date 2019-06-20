/********************************
 *
 *implementation for graphics
 *used to render on the screen
 *
 *
 * ********************************/
#include "defines.h"
#include "V8GraphicsContext.h"
#include "DirectCanvas.h"
#include "Color.h"
#include "Image.h"
#include "ImageData.h"

#include "SkDevice.h"
#include "SkPaint.h"
#include "SkImageEncoder.h"
#include "SkImageDecoder.h"
#include "Path.h"
#include "SkPath.h"
#include "scan_path.h"
#include "SkMatrix.h"
#include "V8Image.h"
#include "V8ImageData.h"
//#include "V8WebView.h"
//#include "WebViewAliyun.h"

#undef LOG_TAG
#define LOG_TAG    "V8GraphicsContext"

namespace DCanvas
{
V8GraphicsContext::V8GraphicsContext()
{
}

V8GraphicsContext::~V8GraphicsContext()
{
}

/**
* for GraphicsContext can be used by js
**/
v8::Handle<v8::Value> V8GraphicsContext::Wrap(const v8::FunctionCallbackInfo<v8::Value>& args, int canvasId)
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>                    class_proto;
    v8::Handle<v8::Object>                            class_obj;
    v8::Handle<v8::FunctionTemplate>                  class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("GraphicsContext"));
    class_proto = class_func_templ->PrototypeTemplate();

    //all class functions
    class_proto->Set(v8::String::New("fillRect"),FunctionTemplate::New(V8GraphicsContext::FillRect));
    class_proto->Set(v8::String::New("fillText"),FunctionTemplate::New(V8GraphicsContext::FillText));
    class_proto->Set(v8::String::New("strokeText"),FunctionTemplate::New(V8GraphicsContext::StrokeText));
    class_proto->Set(v8::String::New("clearRect"),FunctionTemplate::New(V8GraphicsContext::ClearRect));
    class_proto->Set(v8::String::New("swapBuffer"),FunctionTemplate::New(V8GraphicsContext::SwapBuffer));
    class_proto->Set(v8::String::New("drawImage"),FunctionTemplate::New(V8GraphicsContext::DrawImage));
    class_proto->Set(v8::String::New("strokeRect"),FunctionTemplate::New(V8GraphicsContext::StrokeRect));
    class_proto->Set(v8::String::New("save"),FunctionTemplate::New(V8GraphicsContext::Save));
    class_proto->Set(v8::String::New("restore"),FunctionTemplate::New(V8GraphicsContext::Restore));
    class_proto->Set(v8::String::New("createImageData"),FunctionTemplate::New(V8GraphicsContext::CreateImageData));
    class_proto->Set(v8::String::New("putImageData"),FunctionTemplate::New(V8GraphicsContext::PutImageData));
    class_proto->Set(v8::String::New("getImageData"),FunctionTemplate::New(V8GraphicsContext::GetImageData));
    class_proto->Set(v8::String::New("clip"),FunctionTemplate::New(V8GraphicsContext::Clip));
    //path
    class_proto->Set(v8::String::New("fill"),FunctionTemplate::New(V8GraphicsContext::Fill));
    class_proto->Set(v8::String::New("stroke"),FunctionTemplate::New(V8GraphicsContext::Stroke));
    class_proto->Set(v8::String::New("beginPath"),FunctionTemplate::New(V8GraphicsContext::BeginPath));
    class_proto->Set(v8::String::New("closePath"),FunctionTemplate::New(V8GraphicsContext::ClosePath));
    class_proto->Set(v8::String::New("moveTo"),FunctionTemplate::New(V8GraphicsContext::MoveTo));
    class_proto->Set(v8::String::New("lineTo"),FunctionTemplate::New(V8GraphicsContext::LineTo));
    class_proto->Set(v8::String::New("arc"),FunctionTemplate::New(V8GraphicsContext::Arc));
    class_proto->Set(v8::String::New("quadraticCurveTo"),FunctionTemplate::New(V8GraphicsContext::QuadraticCurveTo));
    class_proto->Set(v8::String::New("bezierCurveTo"),FunctionTemplate::New(V8GraphicsContext::BezierCurveTo));
    class_proto->Set(v8::String::New("isPointInPath"),FunctionTemplate::New(V8GraphicsContext::IsPointInPath));

    class_proto->Set(v8::String::New("translate"),FunctionTemplate::New(V8GraphicsContext::Translate));
    class_proto->Set(v8::String::New("scale"),FunctionTemplate::New(V8GraphicsContext::Scale));
    class_proto->Set(v8::String::New("rotate"),FunctionTemplate::New(V8GraphicsContext::Rotate));
    class_proto->Set(v8::String::New("transform"),FunctionTemplate::New(V8GraphicsContext::Transform));
    class_proto->Set(v8::String::New("setTransform"),FunctionTemplate::New(V8GraphicsContext::SetTransform));

    class_proto->Set(v8::String::New("createPattern"),FunctionTemplate::New(V8GraphicsContext::CreatePattern));
    class_proto->Set(v8::String::New("createLinearGradient"),FunctionTemplate::New(V8GraphicsContext::CreateLinearGradient));
    class_proto->Set(v8::String::New("createRadialGradient"),FunctionTemplate::New(V8GraphicsContext::CreateRadialGradient));

    class_proto->Set(v8::String::New("drawCameraPreview"), FunctionTemplate::New(V8GraphicsContext::DrawCameraPreview));

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins->SetInternalFieldCount(1);
    //varables
    class_ins->SetAccessor(v8::String::New("fillStyle"), V8GraphicsContext::GetFillStyle, V8GraphicsContext::SetFillStyle);
    class_ins->SetAccessor(v8::String::New("strokeStyle"), V8GraphicsContext::GetStrokeStyle, V8GraphicsContext::SetStrokeStyle);
    class_ins->SetAccessor(v8::String::New("font"), V8GraphicsContext::GetFont, V8GraphicsContext::SetFont);
    class_ins->SetAccessor(v8::String::New("textBaseline"), V8GraphicsContext::GetTextBaseLine, V8GraphicsContext::SetTextBaseLine);//GetTextBaseLine
    class_ins->SetAccessor(v8::String::New("textAlign"), V8GraphicsContext::GetTextAlign, V8GraphicsContext::SetTextAlign);
    class_ins->SetAccessor(v8::String::New("strokeStyle"), V8GraphicsContext::GetStrokeStyle, V8GraphicsContext::SetStrokeStyle);
    class_ins->SetAccessor(v8::String::New("globalAlpha"), V8GraphicsContext::GetGlobalAlpha, V8GraphicsContext::SetGlobalAlpha);
    class_ins->SetAccessor(v8::String::New("lineWidth"), V8GraphicsContext::GetLineWidth, V8GraphicsContext::SetLineWidth);
    class_ins->SetAccessor(v8::String::New("lineCap"), V8GraphicsContext::GetLineCap, V8GraphicsContext::SetLineCap);
    class_ins->SetAccessor(v8::String::New("lineJoin"), V8GraphicsContext::GetLineJoin, V8GraphicsContext::SetLineJoin);
    class_ins->SetAccessor(v8::String::New("miterLimit"), V8GraphicsContext::GetMiterLimit, V8GraphicsContext::SetMiterLimit);
    class_ins->SetAccessor(v8::String::New("shadowOffsetX"), V8GraphicsContext::GetShadowOffsetX, V8GraphicsContext::SetShadowOffsetX);
    class_ins->SetAccessor(v8::String::New("shadowOffsetY"), V8GraphicsContext::GetShadowOffsetY, V8GraphicsContext::SetShadowOffsetY);
    class_ins->SetAccessor(v8::String::New("shadowBlur"), V8GraphicsContext::GetShadowBlur, V8GraphicsContext::SetShadowBlur);
    class_ins->SetAccessor(v8::String::New("shadowColor"), V8GraphicsContext::GetShadowColor, V8GraphicsContext::SetShadowColor);


    ClassImpl* pImpl = ClassImpl::create(canvasId);
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(pImpl));
    LOGD("GraphicsContext wrap end");
    return handle_scope.Close(class_obj);
}

v8::Handle<v8::Value> V8GraphicsContext::Wrap(RenderingContext* ptr)
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>                    class_proto;
    v8::Handle<v8::Object>                            class_obj;
    v8::Handle<v8::FunctionTemplate>                  class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("GraphicsContext"));
    class_proto = class_func_templ->PrototypeTemplate();

    //all class functions
    class_proto->Set(v8::String::New("fillRect"),FunctionTemplate::New(V8GraphicsContext::FillRect));
    class_proto->Set(v8::String::New("fillText"),FunctionTemplate::New(V8GraphicsContext::FillText));
    class_proto->Set(v8::String::New("strokeText"),FunctionTemplate::New(V8GraphicsContext::StrokeText));
    class_proto->Set(v8::String::New("clearRect"),FunctionTemplate::New(V8GraphicsContext::ClearRect));
    class_proto->Set(v8::String::New("swapBuffer"),FunctionTemplate::New(V8GraphicsContext::SwapBuffer));
    class_proto->Set(v8::String::New("drawImage"),FunctionTemplate::New(V8GraphicsContext::DrawImage));
    class_proto->Set(v8::String::New("strokeRect"),FunctionTemplate::New(V8GraphicsContext::StrokeRect));
    class_proto->Set(v8::String::New("save"),FunctionTemplate::New(V8GraphicsContext::Save));
    class_proto->Set(v8::String::New("restore"),FunctionTemplate::New(V8GraphicsContext::Restore));
    class_proto->Set(v8::String::New("createImageData"),FunctionTemplate::New(V8GraphicsContext::CreateImageData));
    class_proto->Set(v8::String::New("putImageData"),FunctionTemplate::New(V8GraphicsContext::PutImageData));
    class_proto->Set(v8::String::New("getImageData"),FunctionTemplate::New(V8GraphicsContext::GetImageData));
    class_proto->Set(v8::String::New("clip"),FunctionTemplate::New(V8GraphicsContext::Clip));
    //path
    class_proto->Set(v8::String::New("fill"),FunctionTemplate::New(V8GraphicsContext::Fill));
    class_proto->Set(v8::String::New("stroke"),FunctionTemplate::New(V8GraphicsContext::Stroke));
    class_proto->Set(v8::String::New("beginPath"),FunctionTemplate::New(V8GraphicsContext::BeginPath));
    class_proto->Set(v8::String::New("closePath"),FunctionTemplate::New(V8GraphicsContext::ClosePath));
    class_proto->Set(v8::String::New("moveTo"),FunctionTemplate::New(V8GraphicsContext::MoveTo));
    class_proto->Set(v8::String::New("lineTo"),FunctionTemplate::New(V8GraphicsContext::LineTo));
    class_proto->Set(v8::String::New("arc"),FunctionTemplate::New(V8GraphicsContext::Arc));
    class_proto->Set(v8::String::New("quadraticCurveTo"),FunctionTemplate::New(V8GraphicsContext::QuadraticCurveTo));
    class_proto->Set(v8::String::New("bezierCurveTo"),FunctionTemplate::New(V8GraphicsContext::BezierCurveTo));
    class_proto->Set(v8::String::New("isPointInPath"),FunctionTemplate::New(V8GraphicsContext::IsPointInPath));

    class_proto->Set(v8::String::New("translate"),FunctionTemplate::New(V8GraphicsContext::Translate));
    class_proto->Set(v8::String::New("scale"),FunctionTemplate::New(V8GraphicsContext::Scale));
    class_proto->Set(v8::String::New("rotate"),FunctionTemplate::New(V8GraphicsContext::Rotate));
    class_proto->Set(v8::String::New("transform"),FunctionTemplate::New(V8GraphicsContext::Transform));
    class_proto->Set(v8::String::New("setTransform"),FunctionTemplate::New(V8GraphicsContext::SetTransform));

    class_proto->Set(v8::String::New("createPattern"),FunctionTemplate::New(V8GraphicsContext::CreatePattern));
    class_proto->Set(v8::String::New("createLinearGradient"),FunctionTemplate::New(V8GraphicsContext::CreateLinearGradient));
    class_proto->Set(v8::String::New("createRadialGradient"),FunctionTemplate::New(V8GraphicsContext::CreateRadialGradient));

    class_proto->Set(v8::String::New("drawCameraPreview"), FunctionTemplate::New(V8GraphicsContext::DrawCameraPreview));
    //class_proto->Set(v8::String::New("drawWebView"), FunctionTemplate::New(V8GraphicsContext::DrawWebView));
    //varables
    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins->SetInternalFieldCount(1);
    class_ins->SetAccessor(v8::String::New("fillStyle"), V8GraphicsContext::GetFillStyle, V8GraphicsContext::SetFillStyle);
    class_ins->SetAccessor(v8::String::New("strokeStyle"), V8GraphicsContext::GetStrokeStyle, V8GraphicsContext::SetStrokeStyle);
    class_ins->SetAccessor(v8::String::New("font"), V8GraphicsContext::GetFont, V8GraphicsContext::SetFont);
    class_ins->SetAccessor(v8::String::New("textBaseline"), V8GraphicsContext::GetTextBaseLine, V8GraphicsContext::SetTextBaseLine);//GetTextBaseLine
    class_ins->SetAccessor(v8::String::New("textAlign"), V8GraphicsContext::GetTextAlign, V8GraphicsContext::SetTextAlign);
    class_ins->SetAccessor(v8::String::New("strokeStyle"), V8GraphicsContext::GetStrokeStyle, V8GraphicsContext::SetStrokeStyle);
    class_ins->SetAccessor(v8::String::New("globalAlpha"), V8GraphicsContext::GetGlobalAlpha, V8GraphicsContext::SetGlobalAlpha);
    class_ins->SetAccessor(v8::String::New("lineWidth"), V8GraphicsContext::GetLineWidth, V8GraphicsContext::SetLineWidth);
    class_ins->SetAccessor(v8::String::New("lineCap"), V8GraphicsContext::GetLineCap, V8GraphicsContext::SetLineCap);
    class_ins->SetAccessor(v8::String::New("lineJoin"), V8GraphicsContext::GetLineJoin, V8GraphicsContext::SetLineJoin);
    class_ins->SetAccessor(v8::String::New("miterLimit"), V8GraphicsContext::GetMiterLimit, V8GraphicsContext::SetMiterLimit);
    class_ins->SetAccessor(v8::String::New("shadowOffsetX"), V8GraphicsContext::GetShadowOffsetX, V8GraphicsContext::SetShadowOffsetX);
    class_ins->SetAccessor(v8::String::New("shadowOffsetY"), V8GraphicsContext::GetShadowOffsetY, V8GraphicsContext::SetShadowOffsetY);
    class_ins->SetAccessor(v8::String::New("shadowBlur"), V8GraphicsContext::GetShadowBlur, V8GraphicsContext::SetShadowBlur);
    class_ins->SetAccessor(v8::String::New("shadowColor"), V8GraphicsContext::GetShadowColor, V8GraphicsContext::SetShadowColor);

    ClassImpl* pImpl = (ClassImpl*)ptr;
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(pImpl));
    return handle_scope.Close(class_obj);
}


/************************** javascript call functions *****************************/


/*------------------------------ for attribute -----------------------------------*/

//shadow Blur
v8::Handle<v8::Value> V8GraphicsContext::GetShadowBlur(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    return v8::Number::New(class_impl->getShadowBlur());
}

void V8GraphicsContext::SetShadowBlur(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetShadowBlur : type of arguments Exception");
        return;
    }

    float val = value->NumberValue();
    class_impl->setShadowBlur(val);
}

//shadowOffsetX
v8::Handle<v8::Value> V8GraphicsContext::GetShadowOffsetX(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    return v8::Number::New(class_impl->getShadowOffsetX());
}

void V8GraphicsContext::SetShadowOffsetX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetShadowOffsetX : type of arguments Exception");
        return;
    }
    float val = value->NumberValue();

    class_impl->setShadowOffsetX(val);
}

//shadowOffsetY
v8::Handle<v8::Value> V8GraphicsContext::GetShadowOffsetY(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    return v8::Number::New(class_impl->getShadowOffsetY());
}

void V8GraphicsContext::SetShadowOffsetY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetShadowOffsetY : type of arguments Exception");
        return;
    }

    float val = value->NumberValue();

    class_impl->setShadowOffsetY(val);
}

//shadow color
v8::Handle<v8::Value> V8GraphicsContext::GetShadowColor(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    return v8::String::New(class_impl->getShadowColor().name().c_str());
}

void V8GraphicsContext::SetShadowColor(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    class_impl->setShadowColor(Color(*(v8::String::AsciiValue(value))));
}

void V8GraphicsContext::SetLineCap(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    class_impl->setLineCap(*(v8::String::AsciiValue(value)));
}

v8::Handle<v8::Value> V8GraphicsContext::GetLineCap(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    return v8::String::New(class_impl->getLineCap().c_str());
}

void V8GraphicsContext::SetLineJoin(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    class_impl->setLineJoin(*(v8::String::AsciiValue(value)));
}

v8::Handle<v8::Value> V8GraphicsContext::GetLineJoin(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    return v8::String::New(class_impl->getLineJoin().c_str());
}

void V8GraphicsContext::SetMiterLimit(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetMiterLimit : type of arguments Exception");
        return;
    }
    
    class_impl->setMiterLimit(value->NumberValue());
}

v8::Handle<v8::Value> V8GraphicsContext::GetMiterLimit(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    return v8::Number::New(class_impl->getMiterLimit());
}

void V8GraphicsContext::SetLineWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetLineWidth : type of arguments Exception");
        return;
    }

    float val = value->NumberValue();
    if (val< 0.0 || val > 2000.0)
    {
        val = 1.0;
    }

    class_impl->setLineWidth(val);
}

v8::Handle<v8::Value> V8GraphicsContext::GetLineWidth(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    return Number::New(class_impl->getLineWidth());
}

void V8GraphicsContext::SetGlobalAlpha(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (!value->IsNumber())
    {
        THROW_EXCEPTION(TTypeError, "SetGlobalAlpha : type of arguments Exception");
        return;
    }

    float val = value->NumberValue();
    //float val = static_cast<float>(value->NumberValue());
    if (val < 0.0 || val > 1.0)
    {
        val = 1.0;
    }

    class_impl->setGlobalAlphaState(val);
}

v8::Handle<v8::Value> V8GraphicsContext::GetGlobalAlpha(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    
    return v8::Number::New(class_impl->getGlobalAlphaState());
}

void V8GraphicsContext::SetTextBaseLine(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    //String font(*(v8::String::AsciiValue(value)));
    unsigned int len = value->ToString()->Length();
    char* res = new char[len+1];
    memcpy(res, *(v8::String::AsciiValue(value)),len);
    res[len] = 0;
    class_impl->setTextBaseLine(res);
    delete[] res;
}

v8::Handle<v8::Value> V8GraphicsContext::GetTextBaseLine(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8GraphicsContext::SetTextAlign(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);
    std::string align(*(v8::String::AsciiValue(value)));

    if (align == "left")
        class_impl->setTextAlign(TLEFT);
    else if (align == "center")
        class_impl->setTextAlign(TCENTER);
    else if (align == "right")
        class_impl->setTextAlign(TRIGHT);

}

v8::Handle<v8::Value> V8GraphicsContext::GetTextAlign(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8GraphicsContext::SetStrokeStyle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (value->IsString())
    {
        int length = value->ToString()->Length();
        //change value to lower case
        char *color_s = new char[length+2];
        value->ToString()->WriteAscii(color_s, 0, length+1);
        int j=0;
        for (int i=0;i<length;++i)
        {
            if (color_s[i]!=' ')
                color_s[j++] = ((color_s[i]<='Z' && color_s[i]>='A') ? color_s[i] + 32 : color_s[i]);
        }
        color_s[j]='\0';
        class_impl->setStrokeStyle(Color(color_s));

        if (color_s)
            delete[] color_s;
    }
    else
    {
        if (CanvasGradient::HasInstance(value))
        {
            class_impl->setStrokeStyle(ObjectWrap::Unwrap<CanvasGradient>(value->ToObject()));
        }
        else if (CanvasPattern::HasInstance(value))
        {
            class_impl->setStrokeStyle(ObjectWrap::Unwrap<CanvasPattern>(value->ToObject()));
        }
        else
        {
            LOGE("no match type!!");
        }
    }
}

v8::Handle<v8::Value> V8GraphicsContext::GetStrokeStyle(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8GraphicsContext::SetFillStyle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    if (value->IsString())
    {
        int length = value->ToString()->Length();
        //change value to lower case
        char *color_s = new char[length+2];
        value->ToString()->WriteAscii(color_s, 0, length+1);
        int j = 0;
        for (int i = 0; i < length; ++i)
        {
            if (color_s[i] != ' ')
                color_s[j++] = ((color_s[i] <= 'Z' && color_s[i] >= 'A') ? color_s[i] + 32 : color_s[i]);
        }

        color_s[j] = '\0';
        class_impl->setFillStyle(Color(color_s));
        if (color_s)
            delete[] color_s;
    }
    else
    {
        if (CanvasGradient::HasInstance(value))
        {
            class_impl->setFillStyle(ObjectWrap::Unwrap<CanvasGradient>(value->ToObject()));
        }
        else if (CanvasPattern::HasInstance(value))
        {
            class_impl->setFillStyle(ObjectWrap::Unwrap<CanvasPattern>(value->ToObject()));
        }
        else
        {
            LOGE("no match type!!");
        }
    }
}

v8::Handle<v8::Value> V8GraphicsContext::GetFillStyle(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

void V8GraphicsContext::SetFont(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info)
{
    HandleScope scope;
    CASTTOCLASS(GraphicsContext, info);

    class_impl->setFont(*(v8::String::AsciiValue(value)));
}

v8::Handle<v8::Value> V8GraphicsContext::GetFont(v8::Local<v8::String> property, const AccessorInfo& info)
{
    HandleScope scope;
    return v8::Undefined();
}

/*------------------------------ for functions -----------------------------------*/

v8::Handle<v8::Value> V8GraphicsContext::SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float r = args[0]->NumberValue();
    float g = args[1]->NumberValue();
    float b = args[2]->NumberValue();
    float a = args[3]->NumberValue();

    gc->setClearColor(r, g, b, a);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Clear(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "clear : number of args exception!");

    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->clear();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::DrawBoxEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float pos[4][2];
    v8::Local<v8::Object> obj0 = args[0]->ToObject();
    //v8::Local<v8::Array> arr0 = obj0;
    for (int i = 0; i < 4; i ++)
    {
        v8::Local<v8::Object> _arr = obj0->Get(v8::Integer::New(i))->ToObject();
        for (int j = 0; j < 2; j ++)
        {
            pos[i][j] = _arr->Get(v8::Integer::New(j))->NumberValue();
        }
    }

    gc->drawBoxEx(pos);

    return v8::Undefined();
}

/// draw Image
v8::Handle<v8::Value> V8GraphicsContext::DrawImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    float sx, sy, sw, sh, dx, dy, dw, dh;
    Image* img;

    if (!V8Image::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TTypeError,
            "drawImage : type of arguments Exception");
    }

    if (1 == args.Length())
    {
        //unwrap to image
        img = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
        sx = sy = dx = dy = 0;
        sw = dw = img->getWidth();
        sh = dh = img->getHeight();
    }
    else if (3 == args.Length())
    {
        if (!args[1]->IsNumber() || !args[2]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "drawImage : type of arguments Exception");
                
        //unwrap to image
        img = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
        sx = sy = 0;
        dx = args[1]->NumberValue();
        dy = args[2]->NumberValue();
        sw = dw = img->getWidth();
        sh = dh = img->getHeight();
    }
    else if (5 == args.Length())
    {
        for (int i = 1; i < 5; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "drawImage : type of arguments Exception");
        }
        //unwrap to image
        img = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
        sx = sy = 0;
        dx = args[1]->NumberValue();
        dy = args[2]->NumberValue();
        sw = img->getWidth();
        sh = img->getHeight();
        dw = args[3]->NumberValue();
        dh = args[4]->NumberValue();
    }
    else if (9 == args.Length())
    {
        //unwrap to image
        img = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
        for (int i = 1; i < 9; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "drawImage : type of arguments Exception");
        }

        sx = args[1]->NumberValue();
        sy = args[2]->NumberValue();
        sw = args[3]->NumberValue();
        sh = args[4]->NumberValue();
        dx = args[5]->NumberValue();
        dy = args[6]->NumberValue();
        dw = args[7]->NumberValue();
        dh = args[8]->NumberValue();

        sw = (sw + sx) > img->getWidth() ? (img->getWidth() - sx) : sw;
        sh = (sh + sy) > img->getHeight() ? (img->getHeight() - sy) : sh;
    }
    else
    {
        return THROW_EXCEPTION(TTypeError,
            "drawImage : number of arguments Exception");
    }

    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    gc->drawImage(img, sx, sy, sw, sh, dx, dy, dw, dh);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::SetColor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    float r = args[0]->NumberValue();
    float g = args[1]->NumberValue();
    float b = args[2]->NumberValue();
    float a = args[3]->NumberValue();

    gc->setColor(r, g, b, a);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::SetColorEx(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    float color[4][4];
    v8::Local<v8::Object> _arr = args[0]->ToObject();

    for (int i = 0; i < 4; i ++)
    {
        v8::Local<v8::Object> _s_arr = (v8::Local<v8::Object>)_arr->Get(v8::Integer::New(i))->ToObject();
        for (int j = 0; j < 4; j++)
        {
            color[i][j] = _s_arr->Get(v8::Integer::New(j))->NumberValue();
        }
    }

    gc->setColorEx(color);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::ClearImage(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->clearImage();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (args.Length() != 0)
    {
        return THROW_EXCEPTION(TError,
                "swapBuffer : number of arguments Exception");
    }

    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->swapBufferToScreen();
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::FillRect(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    if (args.Length() != 4)
    {
        return THROW_EXCEPTION(TError,
                "fillRect : number of arguments Exception");
    }

    for (int i = 0; i < 4; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "fillRect : type of arguments Exception");
    }

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    float width = args[2]->NumberValue();
    float height = args[3]->NumberValue();

    gc->fillRect(x, y, width, height);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::FillText(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (3 != args.Length())
        return THROW_EXCEPTION(TError,
            "fillText : number of arguments Exception");

    if (args[0]->IsUndefined() ||!args[1]->IsNumber() || !args[2]->IsNumber())
        return THROW_EXCEPTION(TTypeError,
            "fillText : type of arguments Exception");

    char set[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[0]->ToString();
    jstr_tmp->WriteUtf8(set);

    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->fillText(set, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::StrokeText(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (3 != args.Length())
        return THROW_EXCEPTION(TError,
            "strokeText : number of arguments Exception");

    if (args[0]->IsUndefined() || !args[1]->IsNumber() || !args[2]->IsNumber())
        return THROW_EXCEPTION(TTypeError,
            "strokeText : type of arguments Exception");

    char set[MAX_TEXT_LENGTH] = {0};
    v8::Local<v8::String> jstr_tmp = args[0]->ToString();
    jstr_tmp->WriteUtf8(set);


    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->strokeText(set, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::ClearRect(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext *gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    if (4 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "clearRect : number of arguments Exception");
    }
    for (int i = 0; i < 4; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "clearRect : type of arguments Exception");
    }
    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    float width = args[2]->NumberValue();
    float height = args[3]->NumberValue();
    gc->clearRect(x, y, width, height);
    return v8::Undefined();
}

v8::Handle<Value> V8GraphicsContext::StrokeRect(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    if (4 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "strokeRect : number of arguments Exception");
    }

    for (int i = 0; i < 4; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "strokeRect : type of arguments Exception");
    }

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    float width  = args[2]->NumberValue();
    float height = args[3]->NumberValue();

    gc->strokeRect(x, y, width, height);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Save(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "Save : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->save();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Restore(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError, "Restore : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->restore();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::CreateImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    return V8ImageData::Wrap(args, gc->GetContext());
}

v8::Handle<v8::Value> V8GraphicsContext::GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    return V8ImageData::Wrap(args, gc->GetContext());
}

v8::Handle<v8::Value> V8GraphicsContext::PutImageData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    ImageData* imgData = NULL;
    float sx, sy, sw, sh, dx, dy, dw, dh;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    if (!V8ImageData::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TTypeError,
            "putImageData : type of arguments Exception");
    }

    imgData = ObjectWrap::Unwrap<ImageData>(args[0]->ToObject());

    if (1 == args.Length())
    {
        //unwrap to image data
        sx = sy = dx = dy = 0;
        sw = dw = imgData->Width();
        sh = dh = imgData->Height();
    }
    else if (3 == args.Length())
    {
        //unwrap to image
        for (int i = 1; i < 3; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "putImageData : type of arguments Exception");
        }
        
        sx = sy = 0;
        dx = args[1]->NumberValue();
        dy = args[2]->NumberValue();
        sw = dw = imgData->Width();
        sh = dh = imgData->Height();
    }
    else if (7 == args.Length())
    {
        //unwrap to image
        for (int i = 1; i < 7; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "putImageData : type of arguments Exception");
        }

        sx = args[3]->NumberValue();
        sy = args[4]->NumberValue();
        sw = dw = args[5]->NumberValue();
        sh = dh = args[6]->NumberValue();
        dx = args[1]->NumberValue() + sx;
        dy = args[2]->NumberValue() + sy;
    }
    else
    {
        return THROW_EXCEPTION(TError,
            "putImageData : number of arguments Exception");
    }

    gc->drawImageData(imgData, sx, sy, sw, sh, dx, dy, dw, dh);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Fill(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError,
            "fill : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->fill();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Stroke(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError,
            "stroke : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->stroke();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Clip(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError,
            "clip : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->clip();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::BeginPath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError,
            "closePath : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->beginPath();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::ClosePath(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (0 != args.Length())
        return THROW_EXCEPTION(TError,
        "closePath : number of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->closePath();

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::MoveTo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    if (2 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "moveTo : number of arguments Exception");
    }

    for (int i = 0; i < 2; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "moveTo : type of arguments Exception");
    }

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    gc->moveTo(x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::LineTo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (2 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "lineTo : number of arguments Exception");
    }

    for (int i = 0; i < 2; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "lineTo : type of arguments Exception");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    gc->lineTo(x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Translate(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (2 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "Translate : number of arguments Exception");
    }

    for (int i = 0; i < 2; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "Translate : type of arguments Exception");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();
    gc->translate(x,y);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Scale(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;

    if (2 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "scale : number of arguments Exception");
    }

    for (int i = 0; i < 2; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "scale : type of arguments Exception");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();

    gc->scale(x, y);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Rotate(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (1 != args.Length())
    {
        return THROW_EXCEPTION(TError,
            "rotate : number of arguments Exception");
    }

    if (!args[0]->IsNumber())
        return THROW_EXCEPTION(TTypeError,
            "rotate : type of arguments Exception");
            
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float angle = args[0]->NumberValue();
    gc->rotate(angle);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Transform(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;

    float m11, m12, m21, m22, dx, dy;
    if (6 == args.Length())
    {
        for (int i = 0; i < 6; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "transform : type of arguments Exception");
        }

        m11 = args[0]->NumberValue();
        m12 = args[1]->NumberValue();
        m21 = args[2]->NumberValue();
        m22 = args[3]->NumberValue();
        dx  = args[4]->NumberValue();
        dy  = args[5]->NumberValue();
    }
    else if (12 == args.Length())
    {
        return THROW_EXCEPTION(TError, "transform : unimplement");
    }
    else
    {
        return THROW_EXCEPTION(TError,
            "transform : number of arguments Exception");
    }
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    gc->transform(m11, m12, m21, m22, dx, dy);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::SetTransform(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;

    float m11, m12, m21, m22, dx, dy;
    if (6 == args.Length())
    {
        for (int i = 0; i < 6; ++i)
        {
            if (!args[i]->IsNumber())
                return THROW_EXCEPTION(TTypeError,
                    "setTransform : type of arguments Exception");
        }

        m11 = args[0]->NumberValue();
        m12 = args[1]->NumberValue();
        m21 = args[2]->NumberValue();
        m22 = args[3]->NumberValue();
        dx  = args[4]->NumberValue();
        dy  = args[5]->NumberValue();
    }
    else if (12 == args.Length())
    {
        return THROW_EXCEPTION(TError, "setTransform : unimplement");
    }
    else
    {
        return THROW_EXCEPTION(TError,
            "setTransform : number of arguments Exception");
    }
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->setTransform(m11, m12, m21, m22, dx, dy);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::Arc(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    if (6 != args.Length())
        return THROW_EXCEPTION(TError,
            "arc : number of arguments Exception");

    for (int i = 0; i < 5; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "arc : type of arguments Exception");
    }

    if (!args[5]->IsBoolean())
        return THROW_EXCEPTION(TTypeError,
            "arc : type of arguments Exception");

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();

    float r = args[2]->NumberValue();

    float sa = args[3]->NumberValue();
    float ea = args[4]->NumberValue();

    float anticlockwise = args[5]->BooleanValue();

    if(-1 == gc->arc(x, y, r, sa, ea, anticlockwise))
        return THROW_EXCEPTION(TTypeError,
            "arc : arguments value Exception");

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::QuadraticCurveTo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;

    if (4 != args.Length())
        return THROW_EXCEPTION(TError,
            "quadraticCurveTo : number of arguments Exception");

    for (int i = 0; i < 4; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "quadraticCurveTo : type of arguments Exception");
    }
    
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    float cpx = args[0]->NumberValue();
    float cpy = args[1]->NumberValue();

    float x = args[2]->NumberValue();
    float y = args[3]->NumberValue();

    gc->quadraticCurveTo(cpx, cpy, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::BezierCurveTo(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (6 != args.Length())
        return THROW_EXCEPTION(TError,
            "bezierCurveTo : number of arguments Exception");

    for (int i = 0; i < 6; ++i)
    {
        if (!args[i]->IsNumber())
            return THROW_EXCEPTION(TTypeError,
                "bezierCurveTo : type of arguments Exception");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());

    float cpx = args[0]->NumberValue();
    float cpy = args[1]->NumberValue();

    float cp1x = args[2]->NumberValue();
    float cp1y = args[3]->NumberValue();

    float x = args[4]->NumberValue();
    float y = args[5]->NumberValue();

    gc->bezierCurveTo(cpx, cpy, cp1x, cp1y, x, y);

    return v8::Undefined();
}

v8::Handle<v8::Value> V8GraphicsContext::IsPointInPath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if (2 != args.Length())
        return THROW_EXCEPTION(TError,
            "isPointInPath : number of arguments Exception");

    if (!args[0]->IsNumber() || !args[1]->IsNumber())
    {
        return THROW_EXCEPTION(TTypeError,
            "isPointInPath : type of arguments Exception");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    float x = args[0]->NumberValue();
    float y = args[1]->NumberValue();

    bool res = gc->isPointInPath(x, y);

    return v8::Boolean::New(res);
}

v8::Handle<v8::Value> V8GraphicsContext::CreatePattern(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    return CanvasPattern::wrap(args, gc->GetContext(), gc->getCanvasId());
}

v8::Handle<v8::Value> V8GraphicsContext::CreateLinearGradient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    return CanvasGradient::wrap(args, gc->GetContext(), gc->getCanvasId());
}

v8::Handle<v8::Value> V8GraphicsContext::CreateRadialGradient(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    return CanvasGradient::wrap(args, gc->GetContext(), gc->getCanvasId());
}

v8::Handle<v8::Value> V8GraphicsContext::DrawCameraPreview(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    HandleScope scope;
    float x = 0, y = 0, w = 0, h = 0;

    if (args.Length() == 1)
    {
        x = y = 0;
        w = h = -1;
    }
    else if (args.Length() == 3 )
    {
        x = args[1]->NumberValue();
        y = args[2]->NumberValue();
    }
    else if (args.Length() == 5)
    {
        x = args[1]->NumberValue();
        y = args[2]->NumberValue();
        w = args[3]->NumberValue();
        h = args[4]->NumberValue();
    }
    else
    {
        return THROW_EXCEPTION(TTypeError,
            "drawCameraPreview : number og arguments Exception");
    }

    if (!CameraIns::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TTypeError,
            "drawCameraPreview : argument 1 is not CameraIns");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    CameraIns* camera = ObjectWrap::Unwrap<CameraIns>(args[0]->ToObject());
    gc->drawCameraPreview(camera, x, y, w, h);
    return v8::Undefined();
}

/*
v8::Handle<v8::Value> V8GraphicsContext::DrawWebView(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	aliyun::WebViewAliyun* webview = NULL;
	bool isChanged = false;

    HandleScope scope;
    float sx, sy, sw, sh, dx, dy, dw, dh;

    if (2 == args.Length())
    {
       webview = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args[0]->ToObject());
       if(webview == NULL)
       {
       		return THROW_EXCEPTION(TTypeError,
       		           "DrawWebView : NULL arguments Exception");
       }
       v8::Local<v8::Boolean> changedBoolean = args[1]->ToBoolean();
       isChanged = changedBoolean->Value();
	   //unwrap to image
	   sx = sy = dx = dy = 0;
	   sw = dw = webview->getWidth();
	   sh = dh = webview->getHeight();
    }
    else if (4 == args.Length())
    {

	    if (!args[2]->IsNumber() || !args[3]->IsNumber())
		    return THROW_EXCEPTION(TTypeError,
		 	   "DrawWebView : type of arguments Exception");

	    webview = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args[0]->ToObject());
	    if(webview == NULL)
	    {
			 return THROW_EXCEPTION(TTypeError,
					    "DrawWebView : NULL arguments Exception");
	    }
		v8::Local<v8::Boolean> changedBoolean = args[1]->ToBoolean();
		isChanged = changedBoolean->Value();

	    sx = sy = 0;
	    dx = args[2]->NumberValue();
	    dy = args[3]->NumberValue();
	    sw = dw = webview->getWidth();
	    sh = dh = webview->getHeight();
    }
    else if (6 == args.Length())
    {
	    for (int i = 2; i < 6; ++i)
	    {
	 	    if (!args[i]->IsNumber())
			    return THROW_EXCEPTION(TTypeError,
			 	   "DrawWebView : type of arguments Exception");
	    }

	    webview = ObjectWrap::Unwrap<aliyun::WebViewAliyun>(args[0]->ToObject());
	    if(webview == NULL)
		{
			 return THROW_EXCEPTION(TTypeError,
						"DrawWebView : NULL arguments Exception");
		}
		v8::Local<v8::Boolean> changedBoolean = args[1]->ToBoolean();
		isChanged = changedBoolean->Value();
	    sx = sy = 0;
	    dx = args[2]->NumberValue();
	    dy = args[3]->NumberValue();
	    sw = webview->getWidth();
	    sh = webview->getHeight();
	    dw = args[4]->NumberValue();
	    dh = args[5]->NumberValue();
    }
    else
    {
        return THROW_EXCEPTION(TTypeError,
            "DrawWebView : number og arguments Exception");
    }

    if (!V8WebView::HasInstance(args[0]))
    {
        return THROW_EXCEPTION(TTypeError,
            "DrawWebView : argument 1 is not V8WebView");
    }

    GraphicsContext* gc = ObjectWrap::Unwrap<GraphicsContext>(args.This());
    gc->drawWebView(webview, isChanged, sx, sy, sw, sh, dx, dy, dw, dh);
    return v8::Undefined();
}
*/
} // namespace DCanvas
