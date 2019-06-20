/***
*entrance of DCanvas.
*manage event media canvas (memory?)
*
****/

#ifndef V8CLOUDX_H
#define V8CLOUDX_H

#include "v8.h"
#include "ObjectWrap.h"
#include "Canvas.h"
#include "CameraIns.h"
#include "AudioIns.h"
#include "SoundPoolIns.h"
#include "Sqlite3DB.h"
#include "CloudX.h"
#include <string>

namespace DCanvas
{

class V8CloudX : ObjectWrap
{
public:
	typedef CloudX ClassImpl;
	V8CloudX();
    ~V8CloudX();

    static v8::Persistent<v8::Object> Wrap();
    
    /* Only core-framework class has the methods Regist and UnRegist.
     * This is written by ourselves, cannot be written by PC
     * */
    static bool Regist(Persistent<v8::Context> _exe_context);
    static bool UnRegist();

    static v8::Handle<v8::Value> GetCanvas(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateCanvas(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetCamera(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetAudio(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetArguments(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> SetArguments(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> CreateSoundPool(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Value> GetDataBase(const v8::FunctionCallbackInfo<v8::Value>& args);
private:

    static v8::Handle<v8::ObjectTemplate>                  class_proto;
    static v8::Handle<v8::Object>                          class_obj;
    static v8::Persistent<v8::FunctionTemplate>            class_func_templ;// = FunctionTemplate::New();
    
};

}
#endif
