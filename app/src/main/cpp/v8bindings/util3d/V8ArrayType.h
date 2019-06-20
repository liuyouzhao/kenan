#ifndef V8ARRAYTYPE_H
#define V8ARRAYTYPE_H

#include "defines.h"
#include "V8ArrayBufferView.h"
#include "V8ArrayBuffer.h"
#include <v8.h>

#undef 	LOG_TAG
#define	LOG_TAG "V8ArrayType"

namespace DCanvas
{

template <typename T>
class V8ArrayType: public ObjectWrap
{
public:
	static void DoNothing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info) {}
	static Handle<FunctionTemplate> CreateArray()
	{
		if (!s_bInited)
		{
			s_class_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New(ArrayConstructor));
			s_class_func_templ->SetClassName(v8::String::New(getClassName()));
			s_class_proto = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->PrototypeTemplate());
			s_class_proto->Set(v8::String::New("subarray"), FunctionTemplate::New(SubArray));

			s_class_ins = v8::Persistent<ObjectTemplate>::New(s_class_func_templ->InstanceTemplate());
			s_class_ins->Set("BYTE_PER_ELEMENT", v8::Uint32::New(sizeof(short)));
			s_class_ins->SetAccessor(v8::String::New("length"), GetLength, DoNothing);
			s_class_ins -> SetInternalFieldCount(1);

			s_class_func_templ->Inherit(V8ArrayBufferView::CreateArray());
			s_bInited = true;
		}
		return s_class_func_templ;
	}

	static v8::Handle<v8::Value> ArrayConstructor(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
	    // throw if called without `new'
	    if (!args.IsConstructCall())
	    {
	    	char exception[500] = {0};
	    	sprintf(exception, "%s : Cannot call constructor as function", getClassName());
	    	LOGE("%s", exception);
	        return THROW_EXCEPTION(TError, exception);
	    }

	    T * t = NULL;
	    unsigned int len = 0;
	    if (args.Length() == 1)
	    {
	        if (args[0]->IsArray())
	        {
	            v8::Array* array = v8::Array::Cast(*args[0]);
	            int len = array->Length();
	            t = T::create(len);
	            v8::Local<v8::Object> tmp = args[0]->ToObject();

	            for (int i = 0; i < len; ++i)
	            {
	                t->set(i, tmp->Get(v8::Int32::New(i))->NumberValue());
	            }
	        }
	        else if (args[0]->IsInt32())
	        {
	            int len = args[0]->Int32Value();
	            t = T::create(len);
	        }
	        else if (V8ArrayBuffer::HasInstance(args[0]))
	        {
	            //arraybuffer
	            ArrayBuffer *b = ObjectWrap::Unwrap<ArrayBuffer>(args[0]->ToObject());
	            len = b->byteLength() / sizeof(double);
	            t = T::create(b, 0, len);
	        }
	        else if (V8ArrayType<T>::HasInstance(args[0]))
	        {
	            T* ins = ObjectWrap::Unwrap<T>(args[0]->ToObject());
	            t = T::create(ins->buffer(), 0, ins->length());
	        }
	        else
	        {
	        	char exception[256] = {0};
				sprintf(exception, "%s : wrong type arguments", getClassName());
				LOGE("%s", exception);
				return THROW_EXCEPTION(TError, exception);
	        }
	    }
	    else if (args.Length() == 2)
	    {
	        if (!V8ArrayBuffer::HasInstance(args[0]) || !args[1]->IsInt32())
	        {
	        	char exception[256] = {0};
				sprintf(exception, "%s : wrong type arguments", getClassName());
				LOGE("%s", exception);
				return THROW_EXCEPTION(TError, exception);
	        }

	        ArrayBuffer *b = ObjectWrap::Unwrap<ArrayBuffer>(args[0]->ToObject());
	        unsigned int offset = args[1]->Uint32Value();
	        if (offset > b->byteLength())
	        {
	        	char exception[256] = {0};
				sprintf(exception, "%s : wrong type arguments", getClassName());
				LOGE("%s", exception);
				return THROW_EXCEPTION(TError, exception);
	        }
	        t = T::create(b, offset, (b->byteLength() - offset)/sizeof(double));
	    }
	    else if (args.Length() == 3)
	    {
	        if (!V8ArrayBuffer::HasInstance(args[0]) || !args[1]->IsInt32() || !args[2]->IsInt32())
	        {
	        	char exception[256] = {0};
				sprintf(exception, "%s : wrong type arguments", getClassName());
				LOGE("%s", exception);
				return THROW_EXCEPTION(TError, exception);
	        }

	        ArrayBuffer *b = ObjectWrap::Unwrap<ArrayBuffer>(args[0]->ToObject());
	        unsigned int offset = args[1]->Uint32Value();
	        len = args[2]->Uint32Value();

	        if (offset > b->byteLength() || (b->byteLength() - offset)/sizeof(double) < len)
	        {
	        	char exception[256] = {0};
				sprintf(exception, "%s : wrong type arguments", getClassName());
				LOGE("%s", exception);
				return THROW_EXCEPTION(TError, exception);
	        }

	        t = T::create(b, offset, len);
	    }
	    else
	    {
	    	char exception[256] = {0};
			sprintf(exception, "%s : wrong type arguments", getClassName());
			LOGE("%s", exception);
			return THROW_EXCEPTION(TError, exception);
	    }

	    // make a persistent handle for the instance and make it
	    // weak so we get a callback when it is garbage collected
	    Persistent<Object> self = Persistent<Object>::New(args.Holder());
	    self.MakeWeak(t, WeakReferenceCallback(ArrayDestructor));

	    // set internal field to the C++ point
	    self->SetInternalField(0, External::New(t));
	    self->SetIndexedPropertiesToExternalArrayData(t->data(),
	                                               getArrayType(t),
	                                               t->length());
	    return self;
	}

	static void ArrayDestructor(Persistent<Object> self, T* parameter)
	{
	    //v8 will call destructor so no need to delete more
	    if (parameter)
	        delete parameter;

	    self.ClearWeak();
	    self.Dispose();
	    self.Clear();
	}

	static v8::Handle<v8::Value> GetLength(v8::Local<v8::String> property,const AccessorInfo& info)
	{
	    HandleScope scope;
	    CASTTOCLASS(T, info);

	    unsigned int length = class_impl->length();
	    return v8::Uint32::New(length);
	}

	static v8::Handle<v8::Value> Wrap(T* t)
	{
	    HandleScope scope;
	    if (!s_bWrapInited)
	    {
	        s_wrap_func_templ = Persistent<FunctionTemplate>::New(v8::FunctionTemplate::New());
	        s_wrap_func_templ->SetClassName(v8::String::New(getClassName()));

	        s_wrap_ins = v8::Persistent<ObjectTemplate>::New(s_wrap_func_templ->InstanceTemplate());
	        s_wrap_ins->Set(v8::String::New("subarray"), FunctionTemplate::New(SubArray));
	        s_wrap_ins->Set("BYTE_PER_ELEMENT", v8::Uint32::New(getElementSize(t)));
	        s_wrap_ins->SetAccessor(v8::String::New("length"), GetLength, DoNothing);
	        s_wrap_ins -> SetInternalFieldCount(1);

	        s_wrap_func_templ->Inherit(V8ArrayBufferView::CreateArray());
	        s_bWrapInited = true;
	    }

	    v8::Handle<v8::Object> class_obj = s_wrap_ins->NewInstance();
	    class_obj->SetInternalField(0, External::New(t));

	    // what is this used to be?
	    //class_obj->Set(v8::String::New("Float32Array Object"),v8::String::New("Float32Array Object"));

	    class_obj->SetIndexedPropertiesToExternalArrayData(t->data(), getArrayType(t), t->length());

	    return scope.Close(class_obj);
	}

	static v8::Handle<v8::Value> SubArray(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
	    HandleScope scope;
	    T* t = ObjectWrap::Unwrap<T>(args.This());
	    T* res = NULL;

	    if (args.Length() == 1)
	    {
	        int start = args[0]->Int32Value();
	        res = t->subarray(start);
	    }
	    else if (args.Length() == 2)
	    {
	        int start = args[0]->Int32Value();
	        int end = args[1]->Int32Value();
	        res = t->subarray(start, end);
	    }
	    return Wrap(t);
	}

	static void TemplateDestroy()
	{
		if (s_bInited)
		{
			s_class_ins.Dispose();
			s_class_proto.Dispose();
			s_class_func_templ.Dispose();
		}
		if (s_bWrapInited)
		{
			s_wrap_ins.Dispose();
			s_wrap_func_templ.Dispose();
		}
		s_bInited = false;
		s_bWrapInited = false;
	}

	static bool HasInstance(v8::Handle<v8::Value> value)
	{
		return s_class_func_templ->HasInstance(value);
	}
protected:


	static v8::ExternalArrayType getArrayType(T* t)
	{
		if(t->isByteArray())						return v8::kExternalByteArray;
		if(t->isUnsignedByteArray())				return v8::kExternalUnsignedByteArray;
		if(t->isShortArray())						return v8::kExternalShortArray;
		if(t->isUnsignedShortArray())				return v8::kExternalUnsignedShortArray;
		if(t->isIntArray())							return v8::kExternalIntArray;
		if(t->isUnsignedIntArray())					return v8::kExternalUnsignedIntArray;
		if(t->isFloatArray() || t->isDoubleArray())	return v8::kExternalFloatArray;
		return v8::kExternalFloatArray;
	}

	static unsigned int getElementSize(T* t)
	{
		if(t->isByteArray())						return sizeof(char);
		if(t->isUnsignedByteArray())				return sizeof(char);
		if(t->isShortArray())						return sizeof(short);
		if(t->isUnsignedShortArray())				return sizeof(short);
		if(t->isIntArray())							return sizeof(int);
		if(t->isUnsignedIntArray())					return sizeof(int);
		if(t->isFloatArray())						return sizeof(float);
		if(t->isDoubleArray())						return sizeof(double);
		return sizeof(int);
	}

	static const char* getClassName()
	{
		return T::getClassName();
	}

	static v8::Persistent<v8::FunctionTemplate>                 s_class_func_templ;
	static v8::Persistent<v8::ObjectTemplate>                   s_class_proto;
	static v8::Persistent<v8::ObjectTemplate>                   s_class_ins;
	static bool s_bInited;

	static v8::Persistent<v8::FunctionTemplate>                 s_wrap_func_templ;
	static v8::Persistent<v8::ObjectTemplate>                   s_wrap_ins;
	static bool s_bWrapInited;
};

template<typename Type>
v8::Persistent<v8::FunctionTemplate>                 V8ArrayType<Type>::s_class_func_templ;
template<typename Type>
v8::Persistent<v8::ObjectTemplate>                   V8ArrayType<Type>::s_class_proto;
template<typename Type>
v8::Persistent<v8::ObjectTemplate>                   V8ArrayType<Type>::s_class_ins;
template<typename Type>
bool V8ArrayType<Type>::s_bInited;
template<typename Type>
bool V8ArrayType<Type>::s_bWrapInited;
template<typename Type>
v8::Persistent<v8::FunctionTemplate>                 V8ArrayType<Type>::s_wrap_func_templ;
template<typename Type>
v8::Persistent<v8::ObjectTemplate>                   V8ArrayType<Type>::s_wrap_ins;

} // namespace DCanvas

#endif // V8ARRAYTYPE_H
