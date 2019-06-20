#include "Uint16Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Uint16Array"

namespace DCanvas
{
Uint16Array* Uint16Array::create(unsigned int length)
{
    return ArrayType<unsigned short>::create<Uint16Array>(length);
}

Uint16Array* Uint16Array::create(const unsigned short* array, unsigned int length)
{
    return ArrayType<unsigned short>::create<Uint16Array>(array, length);
}

Uint16Array* Uint16Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<unsigned short>::create<Uint16Array>(buffer, byteOffset, length);
}

Uint16Array::Uint16Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<unsigned short>(buffer, byteOffset, length)
{

}

void Uint16Array::set(unsigned int index, float value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = (unsigned short)value;
}

Uint16Array* Uint16Array::subarray(int start) const
{
    return subarray(start, length());
}

Uint16Array* Uint16Array::subarray(int start, int end) const
{
    Uint16Array* tmp = subarrayImpl<Uint16Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
