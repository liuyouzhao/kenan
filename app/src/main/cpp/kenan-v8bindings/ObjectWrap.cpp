//
// Created by hujia on 25/06/19.
//
#include "ObjectWrap.h"
namespace kenan_v8bindings
{
v8::Handle<v8::Value> THROW_EXCEPTION(Isolate *isolate, ExceptionType type, const char *str)
{
    switch(type){
        case TError:
            return isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, str)));
        case TRangeError:
            return isolate->ThrowException(v8::Exception::RangeError(v8::String::NewFromUtf8(isolate, str)));
        case TSyntaxError:
            return isolate->ThrowException(v8::Exception::SyntaxError(v8::String::NewFromUtf8(isolate, str)));
        case TTypeError:
            return isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, str)));
        case TReferenceError:
            return isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, str)));
    }
    return isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "exception Type not found")));
}
}