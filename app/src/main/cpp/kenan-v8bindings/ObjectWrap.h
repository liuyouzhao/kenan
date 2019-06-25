/**************
*this class is a base class for object
*
***************/

#ifndef OBJECTWRAP_H
#define OBJECTWRAP_H

#include <v8.h>
#include <assert.h>
#include "defines.h"

using namespace v8;

namespace kenan_v8bindings
{
#ifndef CASTTOCLASS
#define CASTTOCLASS(CLASSNAME, INFO)	\
	Local<Object> self = INFO.Holder();	\
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));	\
	void* ptr = wrap->Value();	\
    CLASSNAME *class_impl =  static_cast<CLASSNAME*>(ptr);
#endif
enum ExceptionType
{
    TError  = 0,
    TRangeError,
    TReferenceError,
    TSyntaxError,
    TTypeError
};

v8::Handle<v8::Value> THROW_EXCEPTION(Isolate *isolate, ExceptionType type, const char *str);

class ObjectWrap
{
 public:
    virtual bool isArrayBufferView(){return false;}
    virtual bool isArrayBuffer(){return false;}
    ObjectWrap ( )
    {
        refs_ = 0;
    }

    virtual ~ObjectWrap ( )
    {
        if (!handle_.IsEmpty())
        {
            assert(handle_.IsNearDeath());
            handle_.ClearWeak();
            //handle_->SetInternalField(0, v8::Undefined());
            //handle_.Dispose();
            //handle_.Clear();
        }
    }

    template <class T>
    static inline T* Unwrap (v8::Handle<v8::Object> handle)
    {
        if(handle.IsEmpty())
        {
            THROW_EXCEPTION(handle->GetIsolate(), TError, "unwrap : Object is not exist!");
        }
        Handle<External> field = Handle<External>::Cast(handle->GetInternalField(0));
        void* ptr = field->Value();

        //assert(!handle.IsEmpty());
        //assert(handle->InternalFieldCount() > 0);
        return static_cast<T*>(ptr);
    }

    v8::Persistent<v8::Object> handle_; // ro

protected:
    inline void Wrap (v8::Handle<v8::Object> handle)
    {
        assert(handle_.IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        //handle_ = v8::Persistent<v8::Object>::New(handle);
        //handle_->SetPointerInInternalField(0, this);
        //MakeWeak();
    }


    inline void MakeWeak (void)
    {
        handle_.SetWeak();
    }

    /* Ref() marks the object as being attached to an event loop.
    * Refed objects will not be garbage collected, even if
    * all references are lost.
    */
    virtual void Ref()
    {
        assert(!handle_.IsEmpty());
        refs_++;
        handle_.ClearWeak();
    }

    /* Unref() marks an object as detached from the event loop.  This is its
    * default state.  When an object with a "weak" reference changes from
    * attached to detached state it will be freed. Be careful not to access
    * the object after making this call as it might be gone!
    * (A "weak reference" means an object that only has a
    * persistant handle.)
    *
    * DO NOT CALL THIS FROM DESTRUCTOR
    */
    virtual void Unref()
    {
        assert(!handle_.IsEmpty());
        assert(!handle_.IsWeak());
        assert(refs_ > 0);
        if (--refs_ == 0) { MakeWeak(); }
    }

    int refs_; // ro

private:
    static void WeakCallback (v8::Persistent<v8::Value> value, void *data)
    {
        ObjectWrap *obj = static_cast<ObjectWrap*>(data);
        assert(value == obj->handle_);
        assert(!obj->refs_);
        assert(value.IsNearDeath());
        delete obj;
    }
};

} // namespace kenan_v8bindings

#endif // OBJECTWRAP_H
