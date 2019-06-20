#ifndef PIXELARRAY_H
#define PIXELARRAY_H
#include <v8.h>
#include "ObjectWrap.h"

#include "ByteArray.h"

namespace DCanvas
{

class PixelArray
{
	friend class V8PixelArray;
public:
    static PixelArray* create(unsigned length);
    ~PixelArray();

    ByteArray* data() { return m_data; }
    const ByteArray* data() const { return m_data; }
    unsigned length() const { return m_data->length(); }

    void set(unsigned index, double value)
    {
        m_data->set(index, value);
    }

    void set(unsigned index, unsigned char value)
    {
        m_data->set(index, value);
    }

    bool get(unsigned index, unsigned char& result) const
    {
        return m_data->get(index, result);
    }

    unsigned char get(unsigned index) const
    {
        return m_data->get(index);
    }
private:
    PixelArray(unsigned length);

    ByteArray*  m_data;
    bool        m_isset;
    v8::Persistent<v8::Object>                           m_js_self;
};

} // namespace DCanvas

#endif // PIXELARRAY_H
