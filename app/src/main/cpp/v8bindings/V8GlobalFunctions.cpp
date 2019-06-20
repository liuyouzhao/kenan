#include "defines.h"
#include "V8GlobalFunctions.h"
#include <v8.h>
#include <vector>

#undef 	 LOG_TAG
#define  LOG_TAG    "JAVASCRIPT_LOG"

namespace DCanvas
{

std::vector<v8::Handle<v8::Value> > logs;

static void v8ValueToString(v8::Handle<v8::Value> value, int maxDepth)
{
    if (value.IsEmpty())
    {
        LOGE("empty!");
        return;
    }
    if (!maxDepth)
        return;
    maxDepth--;

    if (value->IsNull() || value->IsUndefined() ||value->IsString() ||
        value->IsBoolean() || value->IsNumber())
    {
        logs.push_back(value);
        return ;
    }
    else if (value->IsArray())
    {
        v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
        uint32_t length = array->Length();
        logs.push_back(v8::String::New("Array : ["));
        
        for (uint32_t i = 0; i < length; i++)
        {
            v8::Local<v8::Value> value = array->Get(v8::Int32::New(i));
            v8ValueToString(value, maxDepth);
        }
        logs.push_back(v8::String::New("]"));
        return ;
    }
    else if (value->IsFunction())
    {
        logs.push_back(v8::String::New("Function : "));
        logs.push_back(v8::Function::Cast(*value)->GetName());
    }
    else if (value->IsExternal())
    {
        LOGE("IsExternal");
    }
    else if(value->IsObject())
    {
        v8::Handle<v8::Object> object = v8::Handle<v8::Object>::Cast(value);
        v8::Local<v8::Array> propertyNames = object->GetPropertyNames();
        if (propertyNames.IsEmpty())
        {
        	LOGE("2.5");
        }
        uint32_t length = propertyNames->Length();
        for (uint32_t i = 0; i < length; ++i)
        {
            v8::Local<v8::Value> name = propertyNames->Get(v8::Int32::New(i));
            if(!name.IsEmpty() && name->IsString())
            {
            	logs.push_back(name);
                if (!object->HasRealNamedProperty(v8::Handle<v8::String>::Cast(name)))
                    continue;
                v8ValueToString(object->Get(name), maxDepth);
           }
        }
        return ;
    }
    else 
    {
    	LOGE("we will add this type of object later!");
    }
    return;
}

v8::Handle<v8::Value> log(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (1 > args.Length())
    {
        return THROW_EXCEPTION(TError, " log arguments Exception!");    
    }
    
    v8ValueToString(args[0], 5);
    int len = logs.size();
    for(int i = 0; i < len; ++i)
    {
        v8::String::Utf8Value value(logs[i]);
        LOGD("%s", *value);
    }
    
    logs.clear();
    return v8::Undefined();
}

v8::Handle<v8::Value> writeToFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::HandleScope scope;

    return v8::Undefined();
}

} //namespace DCanvas
