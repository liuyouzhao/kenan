#include "defines.h"

namespace DCanvas
{

v8::Handle<v8::Value> THROW_EXCEPTION(ExceptionType type, const char *str)
{
    switch(type){
        case TError:
            return v8::ThrowException(v8::Exception::Error(v8::String::New(str)));
        case TRangeError:
            return v8::ThrowException(v8::Exception::RangeError(v8::String::New(str)));
        case TSyntaxError:
            return v8::ThrowException(v8::Exception::SyntaxError(v8::String::New(str)));
        case TTypeError:
            return v8::ThrowException(v8::Exception::TypeError(v8::String::New(str)));
        case TReferenceError:
            return v8::ThrowException(v8::Exception::ReferenceError(v8::String::New(str)));
    }
    return v8::ThrowException(v8::Exception::Error(v8::String::New("exception Type not found")));
}

}
