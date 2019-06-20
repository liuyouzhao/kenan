#ifndef WEBGLACTIVEINFO_H
#define WEBGLACTIVEINFO_H

#include "ObjectWrap.h"
#include "Types3D.h"

#include <string>
namespace DCanvas
{

class WebGLActiveInfo : public ObjectWrap
{
public:

    static WebGLActiveInfo* create(const std::string& name, DCenum type, DCint size)
    {
        return new WebGLActiveInfo(name, type, size);
    }

    std::string name() const { return m_name; }
    DCenum type() const { return m_type; }
    DCint size() const { return m_size; }

    static v8::Handle<v8::Value> GetName(v8::Local<v8::String> property,const AccessorInfo& info);
    static v8::Handle<v8::Value> GetType(v8::Local<v8::String> property,const AccessorInfo& info);
    static v8::Handle<v8::Value> GetSize(v8::Local<v8::String> property,const AccessorInfo& info);
    static void DoNothing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const AccessorInfo& info);

    v8::Handle<v8::Value> wrap(const v8::FunctionCallbackInfo<v8::Value>& args);

private:

    WebGLActiveInfo(const std::string& name, DCenum type, DCint size)
        : m_name(name)
        , m_type(type)
        , m_size(size)
    {
    }
    std::string m_name;
    DCenum      m_type;
    DCint       m_size;
};

} // namespace DCanvas

#endif // WEBGLACTIVEINFO_H
