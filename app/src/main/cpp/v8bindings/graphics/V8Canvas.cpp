#include "defines.h"
#include "V8Canvas.h"
#include "V8GraphicsContext.h"
#include <v8.h>
#include "Canvas.h"
#include <android/log.h>
#include <string>

#undef LOG_TAG
#define  LOG_TAG    "V8Canvas"

namespace DCanvas
{
v8::Handle<v8::Integer>   V8Canvas::s_Width;
v8::Handle<v8::Integer>   V8Canvas::s_Height;

V8Canvas::V8Canvas()
{
}

V8Canvas::~V8Canvas()
{
}

v8::Handle<v8::Value> V8Canvas::SetLayer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    unsigned int numArgs = args.Length();
    if (numArgs != 1)
    {
		return THROW_EXCEPTION(TRangeError, "SetLayer : number of arguments exception");
    }
    if (!args[0]->IsNumber())
    {
        return THROW_EXCEPTION(TTypeError, "SetLayer : type of arguments Exception");
    }
    unsigned int lay =  args[0]->Uint32Value();
    ClassImpl* pImpl = ObjectWrap::Unwrap<ClassImpl>(args.This());
    pImpl->setLayer(lay);
    return v8::Undefined();
}

v8::Handle<v8::Value> V8Canvas::GetContext(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	unsigned int numArgs = args.Length();

	if(numArgs != 1 && numArgs != 2)
	{
		return THROW_EXCEPTION(TRangeError, "GetContext : number of arguments exception");
	}

	HandleScope scope;

	v8::String::Utf8Value value(args[0]);
	std::string type = *value;
	std::hash_map<std::string, bool> hash;

	if(numArgs == 2)
	{
		v8::Handle<v8::Object> jsAttrs = args[1]->ToObject();
		Local<Array> propertyNames = jsAttrs->GetPropertyNames();
		if(propertyNames->IsArray())
		{
			for(uint32_t i = 0; i < propertyNames->Length(); i ++)
			{
				v8::Handle<v8::Object> name = propertyNames->CloneElementAt(i);
				v8::Handle<v8::String> nameStr = name->ToString();
				v8::String::Utf8Value utf8Name(nameStr);
				if (jsAttrs->Has(nameStr))
				{
				   hash.insert(std::make_pair(*utf8Name, jsAttrs->Get(nameStr)->BooleanValue()));
				}
			}
		}
	}

    ClassImpl* pImpl = ObjectWrap::Unwrap<ClassImpl>(args.This());
    RenderingContext* ptr = NULL;
    if(numArgs == 1)
    {
    	LOGD("%s", type.c_str());
    	ptr = pImpl->getContext(type.c_str());
    }
    if(numArgs == 2)
    {
    	ptr = pImpl->getContext(type.c_str(), hash);
    }

    if (((RenderingContext*)ptr)->is2d())
    {
    	return V8GraphicsContext::Wrap(ptr);
    }
    else if(((RenderingContext*)ptr)->is3d())
    {
    	return pImpl->get_context(args);
    }
    return v8::Null();
}

v8::Handle<v8::Value> V8Canvas::Wrap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	/*
	 * (1) Prepare all params need to be wrapped ---------------------------------------------------
	 * ------depend on the map
	 * ------prepare the args number
	 * */
	unsigned int numArgs = args.Length();

	/*
	 * (2) Judge&wrap the params' format -------------------------------------------------
	 * ------length of the args be [0]
	 * */
	if (numArgs != 0)
	{
	       return v8::ThrowException(v8::Exception::Error(v8::String::New("Canvas Exception!!")));
	}

	/*
	 * (3) Really new the Impl pointer ----------------------------------------
	 * */
	ClassImpl* pImpl = new ClassImpl();

	/*
	 * (4) Mapping the Members ----------------------------------
	 */
	V8Canvas::s_Width = v8::Int32::New(pImpl->m_width);
	V8Canvas::s_Height = v8::Int32::New(pImpl->m_height);

	/*
	 * (5) Set Functions and Members
	 * --------------------------------------
	 * class_name: Canvas
	 * functions: getContext
	 * members: width, height
	 * --------------------------------------
	 * */
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>                  class_proto;
    v8::Handle<v8::Object>                          class_obj;
    v8::Handle<v8::FunctionTemplate>                class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("Canvas"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("getContext",FunctionTemplate::New(V8Canvas::GetContext));
    class_proto->Set("setLayer", FunctionTemplate::New(V8Canvas::SetLayer));
    class_proto->Set("width", V8Canvas::s_Width);
    class_proto->Set("height", V8Canvas::s_Height);

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins-> SetInternalFieldCount(1);
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(pImpl));

    return handle_scope.Close(class_obj);
}

v8::Handle<v8::Value> V8Canvas::Wrap(void* ptrIn)
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>                  class_proto;
    v8::Handle<v8::Object>                          class_obj;
    v8::Handle<v8::FunctionTemplate>                class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("Canvas"));
    class_proto = class_func_templ->PrototypeTemplate();

    ClassImpl* pImpl = (ClassImpl*)ptrIn;

	V8Canvas::s_Width = v8::Int32::New(pImpl->m_width);
	V8Canvas::s_Height = v8::Int32::New(pImpl->m_height);

    class_proto->Set("getContext",FunctionTemplate::New(V8Canvas::GetContext));
    class_proto->Set("setLayer", FunctionTemplate::New(V8Canvas::SetLayer));
    class_proto->Set("width", V8Canvas::s_Width);
    class_proto->Set("height", V8Canvas::s_Height);

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins-> SetInternalFieldCount(1);
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(pImpl));

    return handle_scope.Close(class_obj);
}

}// namepace DCanvas
