/*******************
*this class is for drawing graphics
*******************/

#ifndef V8GRAPHICSCONTEXT_H
#define V8GRAPHICSCONTEXT_H

#include <ObjectWrap.h>
#include <v8.h>
#include "GraphicsContext.h"

namespace DCanvas
{

class V8GraphicsContext : public ObjectWrap
{
public:
	typedef GraphicsContext ClassImpl;
	V8GraphicsContext();
    virtual ~V8GraphicsContext();

    static v8::Handle<v8::Value> Wrap(const v8::FunctionCallbackInfo<v8::Value>& args, int canvasId);
    static v8::Handle<v8::Value> Wrap(RenderingContext* ptr);

    //javascript functions
    static v8::Handle<v8::Value> SetClearColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> Clear(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> DrawBoxEx(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> SwapBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetColor(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ClearImage(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetColorEx(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FillText(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StrokeText(const v8::FunctionCallbackInfo<v8::Value>& args);

    //fillstyle getter setter
    static v8::Handle<v8::Value> GetFillStyle(v8::Local<v8::String> property,const AccessorInfo& info);

    static void    SetFillStyle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // add font get/set
    static v8::Handle<v8::Value> GetFont(v8::Local<v8::String> property,const AccessorInfo& info);
    static void    SetFont(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // add font-baseline get/set
    static v8::Handle<v8::Value> GetTextBaseLine(v8::Local<v8::String> property,const AccessorInfo& info);
    static void    SetTextBaseLine(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // add font-align get/set
    static v8::Handle<v8::Value> GetTextAlign(v8::Local<v8::String> property,const AccessorInfo& info);
    static void    SetTextAlign(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // stroke style
    static v8::Handle<v8::Value> GetStrokeStyle(v8::Local<v8::String> property,const AccessorInfo& info);
    static void    SetStrokeStyle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // globalAlpha
    static v8::Handle<v8::Value> GetGlobalAlpha(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetGlobalAlpha(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // line width
    static v8::Handle<v8::Value> GetLineWidth(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetLineWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // line cap
    static v8::Handle<v8::Value> GetLineCap(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetLineCap(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // line join
    static v8::Handle<v8::Value> GetLineJoin(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetLineJoin(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info); 
    // miter limit
    static v8::Handle<v8::Value> GetMiterLimit(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetMiterLimit(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info); 
    // shadow Blur
    static v8::Handle<v8::Value> GetShadowBlur(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetShadowBlur(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    // shadowOffsetX
    static v8::Handle<v8::Value> GetShadowOffsetX(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetShadowOffsetX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    //shadowOffsetY
    static v8::Handle<v8::Value> GetShadowOffsetY(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetShadowOffsetY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);
    //shadow color
    static v8::Handle<v8::Value> GetShadowColor(v8::Local<v8::String> property, const AccessorInfo& info);
    static void SetShadowColor(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

//rect
    static v8::Handle<v8::Value> FillRect(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> ClearRect(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> StrokeRect(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Save(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Restore(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Handle<v8::Value> DrawImage(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> PutImageData(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Fill(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Stroke(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Clip(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> BeginPath(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> ClosePath(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> MoveTo(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> LineTo(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Arc(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> QuadraticCurveTo(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> BezierCurveTo(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> IsPointInPath(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Handle<v8::Value> Translate(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Scale(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Rotate(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> Transform(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> SetTransform(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Handle<v8::Value> CreatePattern(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> CreateLinearGradient(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> CreateRadialGradient(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Handle<v8::Value> DrawCameraPreview(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::Handle<v8::Value> DrawWebView(const v8::FunctionCallbackInfo<v8::Value> &args);
private:
};

} // namespace DCanvas

#endif // V8GRAPHICSCONTEXT_H
