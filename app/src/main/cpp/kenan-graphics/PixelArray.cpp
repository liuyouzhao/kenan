#include "defines.h"
#include "PixelArray.h"
#include <v8.h>

#undef   LOG_TAG
#define  LOG_TAG    "PixelArray"

namespace DCanvas
{
PixelArray* PixelArray::create(unsigned length)
{
    return new PixelArray(length);
}

PixelArray::PixelArray(unsigned length)
    : m_data(ByteArray::create(length))
    , m_isset(false)
{
}

PixelArray::~PixelArray()
{
    delete m_data;

    //if(m_isset)
    //   m_js_self.Dispose();
}

} // namespace DCanvas
